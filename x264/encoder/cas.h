#ifndef CAS_H
#define CAS_H

#include "common/common.h"
#include <stdint.h>

#define EXTRA_POINTS 2

#define ORI_AX (2 * EXTRA_POINTS + 4)
#define ORI_AY (2 * EXTRA_POINTS + 4)
#define HOR_AX (2 * EXTRA_POINTS - 1)
#define HOR_AY (2 * EXTRA_POINTS + 4)
#define VER_AX (2 * EXTRA_POINTS)
#define VER_AY (2 * EXTRA_POINTS - 1)
#define DIA_AX (2 * EXTRA_POINTS - 1)
#define DIA_AY (2 * EXTRA_POINTS - 1)

typedef struct {
    const uint32_t *prog_mem;
    const uint16_t *point_mem;
    int feu, foptl, qeu, qoptl;
    int hadamard;

    const uint8_t *ref, *fcur, *qcur;
    int ref_stride, fcur_stride, qcur_stride;
    int f_pixel, q_pixel;
    x264_pixel_function_t *f_pixf, *q_pixf;
    int (*mvc)[2];
    int i_mvc;
    int x_min, x_max, y_min, y_max;
    const int16_t *p_cost_mvx, *p_cost_mvy;
	
    int fpts_done[256 * 2];
    int fpts_done_off, fpts_done_n, fpts_done_cap;
    int qpts_done[256 * 2];
    int qpts_done_off, qpts_done_n, qpts_done_cap;

    int fcost, fx, fy;
    uint8_t fcounter, ffirst, fjmpflag;
    int fwinner;

    int qcost, qfx, qfy, qx, qy;
    uint8_t qcounter, qjmpflag, qstarted;
    int qwinner;

    int last_qcycles; /* cycles used by last quarter pel search operation */

    int fidle, qidle, ready;

    uint8_t ori[(16 + ORI_AX) * (16 + ORI_AY)];
    uint8_t hor[(16 + HOR_AX) * (16 + HOR_AY)];
    uint8_t ver[(16 + VER_AX) * (16 + VER_AY)];
    uint8_t dia[(16 + DIA_AX) * (16 + DIA_AY)];
    int interp_done;

    int64_t energy_pJ;
    int64_t cycles, fcycles, qcycles;
    int64_t could_save_cycles; /* cycles that could be saved if full and quarter pel are done in parallel */
} cas_t;

cas_t *cas_new(const uint32_t *prog_mem, const uint16_t *point_mem, int feu, int foptl, int que, int qoptl, int hadamard);
void cas_del(cas_t *cas);

void cas_process(cas_t *cas,
                 uint8_t *ref, int ref_stride, uint8_t *cur, int cur_stride,
                 int i_pixel, x264_pixel_function_t *pixf,
                 int pmx, int pmy, int mvc[][2], int i_mvc,
                 int x_min, int x_max, int y_min, int y_max,
                 const int16_t *p_cost_mvx, const int16_t *p_cost_mvy);
void cas_retrieve(cas_t *cas, int *x, int *y, int *cost);

#endif /* !CAS_H */
