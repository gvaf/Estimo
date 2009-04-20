#include "cas.h"
#include "common/clip1.h"
#include "common/common.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_D 8

/* x264_tapfilter and x264_tapfilter1 copied from mc.c */
static inline int x264_tapfilter( const uint8_t *pix, int i_pix_next )
{
    return pix[-2*i_pix_next] - 5*pix[-1*i_pix_next] + 20*(pix[0] + pix[1*i_pix_next]) - 5*pix[ 2*i_pix_next] + pix[ 3*i_pix_next];
}
static inline int x264_tapfilter1( const uint8_t *pix )
{
    return pix[-2] - 5*pix[-1] + 20*(pix[0] + pix[1]) - 5*pix[ 2] + pix[ 3];
}

static void cas_execute(cas_t *cas);
static void full_pel_first(cas_t *cas, uint16_t pt);
static void full_pel(cas_t *cas, uint8_t point_n, uint8_t point_addr);
static void frac_pel(cas_t *cas, uint8_t point_n, uint8_t point_addr);
static int points_include(int *pts, int off, int n, int cap, int x, int y);
static void points_add(int *pts, int *off, int *n, int cap, int x, int y);
static void interp_ori(cas_t *cas);
static void interp_hor(cas_t *cas);
static void interp_ver(cas_t *cas);
static void interp_dia(cas_t *cas);

typedef int (*pixel_qsad_t)(const uint8_t *pix1, int stride1, const uint8_t *pixr1, const uint8_t *pixr2, int strider1, int strider2);

static int mb_w[] = {
    16, /* 16x16 */
    16, /* 16x8 */
    8,  /* 8x16 */
    8,  /* 8x8 */
    8,  /* 8x4 */
    4,  /* 4x8 */
    4   /* 4x4 */
};
static int mb_h[] = {
    16, /* 16x16 */
    8,  /* 16x8 */
    16, /* 8x16 */
    8,  /* 8x8 */
    4,  /* 8x4 */
    8,  /* 4x8 */
    4   /* 4x4 */
};
static int cycles_iter[] = {
    33, /* 16x16 */
    17, /* 16x8 */
    17, /* 8x16 */
    9,  /* 8x8 */
    5,  /* 8x4 */
    5,  /* 4x8 */
    3   /* 4x4 */
};
static int qcycles_iter[] = {
    33, /* 16x16 */
    17, /* 16x8 */
    17, /* 8x16 */
    9,  /* 8x8 */
    5,  /* 8x4 */
    5,  /* 4x8 */
    3   /* 4x4 */
};


#define PIXEL_QSAD_C(lx, ly) /* lx and ly must be 4, 8 or 16 */         \
    static int                                                          \
    pixel_qsad_##lx##x##ly(const uint8_t *pix1, int stride1,            \
                           const uint8_t *pixr1, const uint8_t *pixr2,  \
                           int strider1, int strider2)                  \
    {                                                                   \
        int sum = 0;                                                    \
        int x, y;                                                       \
        /* memory read/write and vector motion vector decision */       \
        for (y = 0; y < ly; ++y) {                                      \
            for (x = 0; x < lx; ++x)                                    \
                sum += abs(pix1[x] - ((pixr1[x] + pixr2[x]) >> 1));     \
            pix1 += stride1;                                            \
            pixr1 += strider1;                                          \
            pixr2 += strider2;                                          \
        }                                                               \
        return sum;                                                     \
    }

PIXEL_QSAD_C(16, 16)
PIXEL_QSAD_C(16, 8)
PIXEL_QSAD_C(8, 16)
PIXEL_QSAD_C(8, 8)
PIXEL_QSAD_C(8, 4)
PIXEL_QSAD_C(4, 8)
PIXEL_QSAD_C(4, 4)
//#undef PIXEL_QSAD_C

static pixel_qsad_t qsad[] = {
    pixel_qsad_16x16,
    pixel_qsad_16x8,
    pixel_qsad_8x16,
    pixel_qsad_8x8,
    pixel_qsad_8x4,
    pixel_qsad_4x8,
    pixel_qsad_4x4
};



cas_t *
cas_new(const uint32_t *prog_mem, const uint16_t *point_mem, int feu, int foptl, int qeu, int qoptl)
{
    cas_t *cas = (cas_t *) malloc(sizeof(cas_t));
    if (!cas)
        return NULL;

    cas->prog_mem = prog_mem;
    cas->point_mem = point_mem;
    cas->feu = feu;
    cas->foptl = foptl;
    cas->qeu = qeu;
    cas->qoptl = qoptl;

    cas->fpts_done_off = cas->fpts_done_n = 0;
    cas->fpts_done_cap = sizeof(cas->fpts_done) / sizeof(cas->fpts_done[0]) / 2;
    cas->qpts_done_off = cas->qpts_done_n = 0;
    cas->qpts_done_cap = sizeof(cas->qpts_done) / sizeof(cas->qpts_done[0]) / 2;

    cas->fidle = cas->qidle = 1;
    cas->ready = 0;

    cas->last_qcycles = 0;

    cas->cycles = cas->fcycles = cas->qcycles = cas->could_save_cycles = 0;
    cas->energy_pJ = 0;

    return cas;
}

void
cas_del(cas_t *cas)
{
    free(cas);
}

void
cas_process(cas_t *cas,
	    uint8_t *ref, int ref_stride, uint8_t *cur, int cur_stride,
	    int i_pixel, x264_pixel_function_t *pixf,
	    int pmx, int pmy, int mvc[][2], int i_mvc,
	    int x_min, int x_max, int y_min, int y_max,
	    const int16_t *p_cost_mvx, const int16_t *p_cost_mvy)
{
    assert(cas->fidle || cas->qidle);

    while (!cas->fidle)
        cas_execute(cas);

    cas->fidle = 0;

    cas->ref = ref;
    cas->ref_stride = ref_stride;
    cas->fcur = cur;
    cas->fcur_stride = cur_stride;
    cas->f_pixel = i_pixel;
    cas->f_pixf = pixf;
    cas->mvc = mvc;
    cas->i_mvc = i_mvc;
    cas->x_min = x_min;
    cas->x_max = x_max;
    cas->y_min = y_min;
    cas->y_max = y_max;
    cas->p_cost_mvx = p_cost_mvx;
    cas->p_cost_mvy = p_cost_mvy;
    cas->fpts_done_off = cas->fpts_done_n = 0;

    cas->fx = pmx;
    cas->fy = pmy;
    cas->freg[RCOST] = 1 << 28;
    cas->fcounter = 0;
    cas->fjmpflag = 0;
    cas->fwinner = 0;
}

void
cas_retrieve(cas_t *cas, int *x, int *y, int *cost)
{
    assert(!cas->fidle || !cas->qidle);

    while (!cas->ready)
        cas_execute(cas);

    *x = (cas->qfx << 2) + cas->qx;
    *y = (cas->qfy << 2) + cas->qy;
    *cost = cas->qreg[RCOST];

    cas->ready = 0;
    cas->qidle = 1;
}

static int cas_execute_full(cas_t *cas);
static int inst_full_pel(cas_t *cas, uint32_t inst);
static int cas_execute_quarter(cas_t *cas);
static int inst_quarter_pel(cas_t *cas, uint32_t inst);

static void
cas_execute(cas_t *cas)
{
    uint32_t inst;
    int fcycles, qcycles;
    uint8_t op;

    fcycles = 0;
    if (!cas->fidle)
        fcycles = cas_execute_full(cas);
    cas->fcycles += fcycles;

    qcycles = 0;
    if (!cas->qidle && !cas->ready)
        qcycles = cas_execute_quarter(cas);
    cas->qcycles += qcycles;

    cas->cycles += fcycles > qcycles ? fcycles : qcycles;

    if (fcycles > 0 && cas->qidle)
        cas->could_save_cycles += fcycles > cas->last_qcycles ? cas->last_qcycles : fcycles;

    if (!cas->fidle && cas->qidle) {
        cas->qidle = 0;

        cas->qcur = cas->fcur;
        cas->qcur_stride = cas->fcur_stride;
        cas->qpts_done_off = cas->qpts_done_n = 0;
        cas->q_pixel = cas->f_pixel;
        cas->qfx = cas->fx;
        cas->qfy = cas->fy;
        cas->qx = cas->qy = 0;
        cas->qreg[RCOST] = cas->freg[RCOST];
        cas->qcounter = cas->fcounter;
	cas->qjmpflag = cas->fjmpflag;
        cas->qwinner = 0;

        cas->interp_done = 0;
        inst = cas->prog_mem[cas->fcounter];
        op = (inst & 0xf0000) >> 16;
        if (cas->qeu > 0 && op == 1) { /* frac pel */
            cas->cycles += 159; // 309;
            interp_ori(cas);
            interp_hor(cas);
        }

        cas->fidle = 1;
    }
}

/* This is an exhaustive search command added
 * to the model to enable a fair comparison
 * between full search and other algorithms.
 */
static void
esa(cas_t *cas, int n)
{
    int ox = cas->fx;
    int oy = cas->fy;
    int x0 = ox - n, x1 = ox + n, y0 = oy - n, y1 = oy + n;
    int x, y;
    if (x0 > cas->x_max || x1 < cas->x_min || y0 > cas->y_max || y1 < cas->y_min)
	    return;
    if (x0 < cas->x_min)
	    x0 = cas->x_min;
    if (x1 > cas->x_max)
	    x1 = cas->x_max;
    if (y0 < cas->y_min)
	    y0 = cas->y_min;
    if (y1 > cas->y_max)
	    y1 = cas->y_max;
    for (y = y0; y <= y1; ++y) {
        for (x = x0; x <= x1; ++x) {
            x264_pixel_cmp_t sad = cas->f_pixf->sad[cas->f_pixel];
            int cost = sad((uint8_t *)cas->fcur, cas->fcur_stride,
                           (uint8_t *)cas->ref + x + y * cas->ref_stride, cas->ref_stride);
            cost += cas->p_cost_mvx[x << 2] + cas->p_cost_mvy[y << 2];
            if (cost < cas->freg[RCOST]) {
                cas->freg[RCOST] = cost;
                cas->fx = x;
                cas->fy = y;
            }
        }
    }
}

static int
cas_execute_full(cas_t *cas)
{
    uint32_t inst;
    int cycles = 0;
    int done = 0;

    cas->ffirst = 1;
    while (!done) {
        inst = cas->prog_mem[cas->fcounter++];
        if (inst == 0) /* halt */
            break;
        switch ((inst & 0xf0000) >> 16) {
        case 0: /* full pel */
            cycles += inst_full_pel(cas, inst);
            break;
        case 1: /* frac pel */
            done = 1;
            break;
        case 2: /* early termination */
            ++cycles;
            if (cas->fwinner == (inst & 0xff00) >> 8)
                cas->fcounter = inst & 0xff;
            break;
        case 3: /* branch */
            ++cycles;
            cas->fcounter = inst & 0xff;
            break;
        case 4: /* conditional branch */
            ++cycles;
            if (cas->fjmpflag)
                cas->fcounter = inst & 0xff;
            break;
        case 5: /* less than */
            ++cycles;
            cas->fjmpflag = cas->freg[inst & 0xf000] < (inst & 0x0fff);
            break;
        case 6: /* greater than */
            ++cycles;
            cas->fjmpflag = cas->freg[inst & 0xf000] > (inst & 0x0fff);
            break;
        case 7: /* equal to */
            ++cycles;
            cas->fjmpflag = cas->freg[inst & 0xf000] == (inst & 0x0fff);
            break;
	case 15: /* exhaustive */
            esa(cas, inst & 0xff);
            break;
        }
    }
    --cas->fcounter;
    return cycles;
}

static void
add_energy(cas_t *cas, int cycles, int full)
{
    int power;
    if (full)
        power = 1763 + 675 * cas->feu;
    else
        power = 440 * cas->qeu;
    cas->energy_pJ += cycles * power / 200; // nJ = mW / 200e6 Hz * (1e6 nJ / mJ)
}

static int
inst_full_pel(cas_t *cas, uint32_t inst)
{
    uint8_t point_addr;
    int point_n, points_on_eu;
    int i, opt_points, ox, oy;
    int cycles = 0;

    point_addr = inst & 0xff;
    if (cas->ffirst && cas->i_mvc > 0) {
        cas->ffirst = 0;
        opt_points = cas->i_mvc + 1;
        points_on_eu = (opt_points + cas->feu - 1) / cas->feu;
        cycles += cycles_iter[cas->f_pixel] * points_on_eu + 11;
        full_pel_first(cas, cas->point_mem[point_addr]);
    } else {
        point_n = (inst & 0xff00) >> 8;
        opt_points = point_n;
        ox = cas->fx;
        oy = cas->fy;
        if (cas->foptl) {
            for (i = 0; i < point_n; ++i) {
                if (points_include(cas->fpts_done, cas->fpts_done_off, cas->fpts_done_n, cas->fpts_done_cap,
                                   ox + (int8_t) (cas->point_mem[point_addr + i] >> 8),
                                   oy + (int8_t) (cas->point_mem[point_addr + i]))) {
                    --opt_points;
                }
            }
        }
        points_on_eu = (opt_points + cas->feu - 1) / cas->feu;
        cycles += cycles_iter[cas->f_pixel] * points_on_eu + 11;
        full_pel(cas, point_n, point_addr);
        if (cas->foptl) {
            if (cas->foptl == 1)
                cas->fpts_done_off = cas->fpts_done_n = 0;
            for (i = 0; i < point_n; ++i) {
                points_add(cas->fpts_done, &cas->fpts_done_off, &cas->fpts_done_n, cas->fpts_done_cap,
                           ox + (int8_t) (cas->point_mem[point_addr + i] >> 8),
                           oy + (int8_t) (cas->point_mem[point_addr + i]));
            }
            points_add(cas->fpts_done, &cas->fpts_done_off, &cas->fpts_done_n, cas->fpts_done_cap, ox, oy);
        }
    }

    add_energy(cas, cycles, 1);
    return cycles;
}

static int
cas_execute_quarter(cas_t *cas)
{
    uint32_t inst;
    uint8_t op;
    int cycles = 0;
    int done = 0;

    while (!done) {
        inst = cas->prog_mem[cas->qcounter++];
        op = (inst & 0xf0000) >> 16;
        if (cas->qeu < 1 || inst == 0 || op == 0) { /* halt */
            cas->ready = 1;
            cas->last_qcycles = cycles;
            break;
        }
        switch (op) {
        case 1: /* frac pel */
            cycles += inst_quarter_pel(cas, inst);
            break;
        case 2: /* early termination */
            ++cycles;
            if (cas->qwinner == (inst & 0xff00) >> 8)
                cas->qcounter = inst & 0xff;
            break;
        case 3: /* branch */
            ++cycles;
            cas->qcounter = inst & 0xff;
            break;
        case 4: /* conditional branch */
            ++cycles;
            if (cas->qjmpflag)
                cas->qcounter = inst & 0xff;
            break;
        case 5: /* less than */
            ++cycles;
            cas->qjmpflag = cas->qreg[inst & 0xf000] < (inst & 0x0fff);
            break;
        case 6: /* greater than */
            ++cycles;
            cas->qjmpflag = cas->qreg[inst & 0xf000] > (inst & 0x0fff);
            break;
        case 7: /* equal to */
            ++cycles;
            cas->qjmpflag = cas->qreg[inst & 0xf000] == (inst & 0x0fff);
            break;
        }
    }
    return cycles;
}

static int
inst_quarter_pel(cas_t *cas, uint32_t inst)
{
    uint8_t point_addr;
    int point_n, points_on_eu;
    int i, opt_points, oqx, oqy;
    int cycles = 0;

    point_addr = inst & 0xff;
    if (!cas->interp_done) {
        cas->interp_done = 1;
        cycles += 150;//309;
        interp_ver(cas);
        interp_dia(cas);
    }
    point_n = (inst & 0xff00) >> 8;
    opt_points = point_n;
    oqx = cas->qx;
    oqy = cas->qy;
    if (cas->qoptl) {
        for (i = 0; i < point_n; ++i) {
            if (points_include(cas->qpts_done, cas->qpts_done_off, cas->qpts_done_n, cas->qpts_done_cap,
                               oqx + (int8_t) (cas->point_mem[point_addr + i] >> 8),
                               oqy + (int8_t) (cas->point_mem[point_addr + i]))) {
                --opt_points;
            }
        }
    }
    points_on_eu = (opt_points + cas->qeu - 1) / cas->qeu;
    cycles += qcycles_iter[cas->q_pixel] * points_on_eu + 11;
    frac_pel(cas, point_n, point_addr);
    if (cas->qoptl) {
        if (cas->qoptl == 1)
            cas->qpts_done_off = cas->qpts_done_n = 0;
        for (i = 0; i < point_n; ++i) {
            points_add(cas->qpts_done, &cas->qpts_done_off, &cas->qpts_done_n, cas->qpts_done_cap,
                       oqx + (int8_t) (cas->point_mem[point_addr + i] >> 8),
                       oqy + (int8_t) (cas->point_mem[point_addr + i]));
        }
        points_add(cas->qpts_done, &cas->qpts_done_off, &cas->qpts_done_n, cas->qpts_done_cap, oqx, oqy);
    }

    add_energy(cas, cycles, 0);
    return cycles;
}

static void
full_pel_first(cas_t *cas, uint16_t pt)
{
    int i;
    int ptx = (int8_t) (pt >> 8);
    int pty = (int8_t) (pt & 0xff);
    int x_min = cas->x_min, x_max = cas->x_max, y_min = cas->y_min, y_max = cas->y_max;

    for (i = -1; i < cas->i_mvc; ++i) {
        int x = ptx, y = pty, cost;
        x264_pixel_cmp_t sad = cas->f_pixf->sad[cas->f_pixel];
        if (i == -1) {
            /* this is the first iteration, so fx and fy contain predicted vector */
            x += cas->fx;
            y += cas->fy;
        } else {
            x += (cas->mvc[i][0] + 2) >> 2;
            y += (cas->mvc[i][1] + 2) >> 2;
        }
        if (x < x_min || x > x_max || y < y_min || y > y_max)
            continue;

	cost = sad((uint8_t *)cas->fcur, cas->fcur_stride,
                   (uint8_t *)cas->ref + x + y * cas->ref_stride, cas->ref_stride);
	cost += cas->p_cost_mvx[x << 2] + cas->p_cost_mvy[y << 2];
        if (i == -1 || cost < cas->freg[RCOST]) {
            cas->freg[RCOST] = cost;
            cas->fx = x;
            cas->fy = y;
        }
    }
    cas->fwinner = 0;
}

static void
full_pel(cas_t *cas, uint8_t point_n, uint8_t point_addr)
{
    int i;
    int ox = cas->fx;
    int oy = cas->fy;
    int x_min = cas->x_min, x_max = cas->x_max, y_min = cas->y_min, y_max = cas->y_max;

    cas->fwinner = 0;
    for (i = 0; i < point_n; ++i) {
        int x = ox + (int8_t) (cas->point_mem[point_addr + i] >> 8);
        int y = oy + (int8_t) (cas->point_mem[point_addr + i]);
	int cost;
        x264_pixel_cmp_t sad = cas->f_pixf->sad[cas->f_pixel];

        if (x < x_min || x > x_max || y < y_min || y > y_max)
            continue;
        if (points_include(cas->fpts_done, cas->fpts_done_off, cas->fpts_done_n, cas->fpts_done_cap, x, y))
            continue;

	cost = sad((uint8_t *)cas->fcur, cas->fcur_stride,
                   (uint8_t *)cas->ref + x + y * cas->ref_stride, cas->ref_stride);
	cost += cas->p_cost_mvx[x << 2] + cas->p_cost_mvy[y << 2];
        if (cost < cas->freg[RCOST]) {
            cas->freg[RCOST] = cost;
            cas->fwinner = i + 1;
            cas->fx = x;
            cas->fy = y;
        }
    }
}


static void
frac_pel(cas_t *cas, uint8_t point_n, uint8_t point_addr)
{
    const int ori_w = mb_w[cas->f_pixel] + ORI_AX;
    const int hor_w = mb_w[cas->f_pixel] + HOR_AX;
    const int ver_w = mb_w[cas->f_pixel] + VER_AX;
    const int dia_w = mb_w[cas->f_pixel] + DIA_AX;
    const int range = 4 * EXTRA_POINTS;

    int i;
    uint8_t *ori_o = cas->ori + EXTRA_POINTS + 2 + ori_w * (EXTRA_POINTS + 2);
    uint8_t *hor_o = cas->hor + EXTRA_POINTS + hor_w * (EXTRA_POINTS + 2);
    uint8_t *ver_o = cas->ver + EXTRA_POINTS + ver_w * EXTRA_POINTS;
    uint8_t *dia_o = cas->dia + EXTRA_POINTS + dia_w * EXTRA_POINTS;
    int oqx = cas->qx;
    int oqy = cas->qy;
    int qfx = cas->qfx << 2;
    int qfy = cas->qfy << 2;

    cas->qwinner = 0;
    for (i = 0; i < point_n; ++i) {
        int qx = oqx + (int8_t) (cas->point_mem[point_addr + i] >> 8);
        int qy = oqy + (int8_t) (cas->point_mem[point_addr + i]);
        int x_int = qx >> 2, y_int = qy >> 2;
        pixel_qsad_t q = qsad[cas->q_pixel];
        int cost = 0;

        if (qx < -range || qx > range || qy < -range || qy > range)
            continue;
        if (points_include(cas->qpts_done, cas->qpts_done_off, cas->qpts_done_n, cas->qpts_done_cap, qx, qy))
            continue;

        switch (((qx & 3) << 2) | (qy & 3)) {

#define CASE_XY(fracx, fracy, ref1, ref2, r2x, r2y)             \
            case ((fracx << 2) | fracy): {                      \
                uint8_t *o1 = ref1 ## _o, *o2 = ref2 ## _o;     \
                int w1 = ref1 ## _w, w2 = ref2 ## _w;           \
                cost = q(cas->qcur, cas->qcur_stride,           \
                         o1 + x_int + y_int * w1,               \
                         o2 + x_int + r2x + (y_int + r2y) * w2, \
                         w1, w2);                               \
            } break

            CASE_XY(0, 0, ori, ori, 0, 0);
            CASE_XY(0, 1, ori, ver, 0, 0);
            CASE_XY(1, 0, ori, hor, 0, 0);
            CASE_XY(1, 1, ori, dia, 0, 0);

            CASE_XY(0, 2, ver, ver, 0, 0);
            CASE_XY(0, 3, ver, ori, 0, 1);
            CASE_XY(1, 2, ver, dia, 0, 0);
            CASE_XY(1, 3, ver, hor, 0, 1);

            CASE_XY(2, 0, hor, hor, 0, 0);
            CASE_XY(2, 1, hor, dia, 0, 0);
            CASE_XY(3, 0, hor, ori, 1, 0);
            /* CASE_XY(3, 1, hor, ver, 1, 0); */
            CASE_XY(3, 1, dia, ori, 1, 0);

            CASE_XY(2, 2, dia, dia, 0, 0);
            CASE_XY(2, 3, dia, hor, 0, 1);
            CASE_XY(3, 2, dia, ver, 1, 0);
            CASE_XY(3, 3, dia, ori, 1, 1);

#undef COST_MV_QP

        }

        cost += cas->p_cost_mvx[qfx + qx] + cas->p_cost_mvy[qfy + qy];
        if (cost < cas->qreg[RCOST]) {
            cas->qreg[RCOST] = cost;
            cas->qwinner = i + 1;
            cas->qx = qx;
            cas->qy = qy;
        }
    }
}

static int
points_include(int *pts, int off, int n, int cap, int x, int y)
{
    int i;
    for (i = 0; i < n; ++i) {
        int j = (i + off) % cap;
        if (pts[j * 2] == x && pts[j * 2 + 1] == y)
            return 1;
    }
    return 0;
}

static void
points_add(int *pts, int *off, int *n, int cap, int x, int y)
{
    int voff = *off, vn = *n, i;

    if (points_include(pts, voff, vn, cap, x, y))
        return;

    if (vn == cap)
        *off = voff = (voff + 1) % cap;
    else
        *n = ++vn;
    i = (vn + voff) % cap;
    pts[i * 2] = x;
    pts[i * 2 + 1] = y;
}

static void
interp_ori(cas_t *cas)
{
    int x0 = cas->fx - EXTRA_POINTS - 2;
    int y0 = cas->fy - EXTRA_POINTS - 2;
    int w = mb_w[cas->f_pixel] + ORI_AX;
    int h = mb_h[cas->f_pixel] + ORI_AY;
    int src_stride = cas->ref_stride;
    int dst_stride = w;

    const uint8_t *src = cas->ref + x0 + y0 * src_stride;
    uint8_t *dst = cas->ori;

    int mtop = 0, mbottom = 0, mleft = 0, mright = 0;
    int y;

    mtop = x264_clip3(cas->y_min - y0, 0, h);
    mbottom = x264_clip3(y0 + h - 1 - cas->y_max, 0, h - mtop);
    mleft = x264_clip3(cas->x_min - x0, 0, w);
    mright = x264_clip3(x0 + w - 1 - cas->x_max, 0, w - mleft);

    if (mtop) {
        memset(dst, 0, mtop * w);
        dst += mtop * dst_stride;
        src += mtop * src_stride;
        y0 += mtop;
        h -= mtop;
    }
    if (mbottom) {
        memset(dst + w * (h - mbottom), 0, w * mbottom);
        h -= mbottom;
    }
    if (mleft) {
        uint8_t *t = dst;
        for (y = 0; y < h; ++y) {
            memset(t, 0, mleft);
            t += dst_stride;
        }
        dst += mleft;
        src += mleft;
        x0 += mleft;
        w -= mleft;
    }
    if (mright) {
        uint8_t *t = dst + w - mright;
        for (y = 0; y < h; ++y) {
            memset(t, 0, mright);
            t += dst_stride;
        }
        w -= mright;
    }

    for (y = 0; y < h; ++y) {
        memcpy(dst, src, w);
        dst += dst_stride;
        src += src_stride;
    }
}

static void
interp_hor(cas_t *cas)
{
    const int src_stride = mb_w[cas->f_pixel] + ORI_AX;
    const uint8_t *src = cas->ori + 2;

    const int w = mb_w[cas->f_pixel] + HOR_AX;
    const int h = mb_h[cas->f_pixel] + HOR_AY;
    uint8_t *dst = cas->hor;

    int x, y;
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x)
            dst[x] = x264_mc_clip1((x264_tapfilter1(src + x) + 16) >> 5);
        dst += w;
        src += src_stride;
    }
}

static void
interp_ver(cas_t *cas)
{
    const int src_stride = mb_w[cas->f_pixel] + ORI_AX;
    const uint8_t *src = cas->ori + 2 * src_stride + 2;

    const int w = mb_w[cas->f_pixel] + VER_AX;
    const int h = mb_h[cas->f_pixel] + VER_AY;
    uint8_t *dst = cas->ver;

    int x, y;
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x)
            dst[x] = x264_mc_clip1((x264_tapfilter(src + x, src_stride) + 16) >> 5);
        dst += w;
        src += src_stride;
    }
}

static void
interp_dia(cas_t *cas)
{
    const int src_stride = mb_w[cas->f_pixel] + HOR_AX;
    const uint8_t *src = cas->hor + 2 * src_stride;

    const int w = mb_w[cas->f_pixel] + DIA_AX;
    const int h = mb_h[cas->f_pixel] + DIA_AY;
    uint8_t *dst = cas->dia;

    int x, y;
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x)
            dst[x] = x264_mc_clip1((x264_tapfilter(src + x, src_stride) + 16) >> 5);
        dst += w;
        src += src_stride;
    }
}
