/*****************************************************************************
 * common.c: h264 library
 *****************************************************************************
 * Copyright (C) 2003 Laurent Aimar
 * $Id: common.c,v 1.1 2004/06/03 19:27:06 fenrir Exp $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

#include <ctype.h>
#include <stdarg.h>

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

#include "common.h"
#include "cpu.h"

static void x264_log_default( void *, int, const char *, va_list );

/****************************************************************************
 * x264_param_default:
 ****************************************************************************/
void    x264_param_default( x264_param_t *param )
{
    /* */
    memset( param, 0, sizeof( x264_param_t ) );

    /* CPU autodetect */
    param->cpu = x264_cpu_detect();
    param->i_threads = 1;
    param->b_deterministic = 1;

    /* Video properties */
    param->i_csp           = X264_CSP_I420;
    param->i_width         = 0;
    param->i_height        = 0;
    param->vui.i_sar_width = 0;
    param->vui.i_sar_height= 0;
    param->vui.i_overscan  = 0;  /* undef */
    param->vui.i_vidformat = 5;  /* undef */
    param->vui.b_fullrange = 0;  /* off */
    param->vui.i_colorprim = 2;  /* undef */
    param->vui.i_transfer  = 2;  /* undef */
    param->vui.i_colmatrix = 2;  /* undef */
    param->vui.i_chroma_loc= 0;  /* left center */
    param->i_fps_num       = 25;
    param->i_fps_den       = 1;
    param->i_level_idc     = 51; /* as close to "unrestricted" as we can get */

    /* Encoder parameters */
    param->i_frame_reference = 1;
    param->i_keyint_max = 250;
    param->i_keyint_min = 25;
    param->i_bframe = 0;
    param->i_scenecut_threshold = 40;
    param->b_bframe_adaptive = 1;
    param->i_bframe_bias = 0;
    param->b_bframe_pyramid = 0;

    param->b_deblocking_filter = 1;
    param->i_deblocking_filter_alphac0 = 0;
    param->i_deblocking_filter_beta = 0;

    param->b_cabac = 1;
    param->i_cabac_init_idc = 0;

    param->rc.i_rc_method = X264_RC_NONE;
    param->rc.i_bitrate = 0;
    param->rc.f_rate_tolerance = 1.0;
    param->rc.i_vbv_max_bitrate = 0;
    param->rc.i_vbv_buffer_size = 0;
    param->rc.f_vbv_buffer_init = 0.9;
    param->rc.i_qp_constant = 26;
    param->rc.f_rf_constant = 0;
    param->rc.i_qp_min = 10;
    param->rc.i_qp_max = 51;
    param->rc.i_qp_step = 4;
    param->rc.f_ip_factor = 1.4;
    param->rc.f_pb_factor = 1.3;

    param->rc.b_stat_write = 0;
    param->rc.psz_stat_out = "x264_2pass.log";
    param->rc.b_stat_read = 0;
    param->rc.psz_stat_in = "x264_2pass.log";
    param->rc.psz_rc_eq = "blurCplx^(1-qComp)";
    param->rc.f_qcompress = 0.6;
    param->rc.f_qblur = 0.5;
    param->rc.f_complexity_blur = 20;
    param->rc.i_zones = 0;

    /* Log */
    param->pf_log = x264_log_default;
    param->p_log_private = NULL;
    param->i_log_level = X264_LOG_INFO;

    /* */
    param->analyse.intra = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8;
    param->analyse.inter = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8
                         | X264_ANALYSE_PSUB16x16 | X264_ANALYSE_BSUB16x16;
    param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_SPATIAL;
    param->analyse.i_me_method = X264_ME_HEX;
    param->analyse.i_cas_feu = 1;
    param->analyse.i_cas_foptl = 0;
    param->analyse.i_cas_qeu = 1;
    param->analyse.i_cas_qoptl = 0;
    param->analyse.p_cas_prog = NULL;
    param->analyse.p_cas_point = NULL;
    param->analyse.b_cas_hadamard = 0;
    param->analyse.b_cas_mvcost = 1;
    param->analyse.b_cas_mvcand = 1;
    param->analyse.i_me_range = 16;
    param->analyse.i_hard_range = -1;
    param->analyse.i_subpel_refine = 5;
    param->analyse.b_chroma_me = 1;
    param->analyse.i_mv_range_thread = -1;
    param->analyse.i_mv_range = -1; // set from level_idc
    param->analyse.i_direct_8x8_inference = -1; // set from level_idc
    param->analyse.i_chroma_qp_offset = 0;
    param->analyse.b_fast_pskip = 1;
    param->analyse.b_dct_decimate = 1;
    param->analyse.i_luma_deadzone[0] = 21;
    param->analyse.i_luma_deadzone[1] = 11;
    param->analyse.b_psnr = 1;
    param->analyse.b_ssim = 1;

    param->i_cqm_preset = X264_CQM_FLAT;
    memset( param->cqm_4iy, 16, 16 );
    memset( param->cqm_4ic, 16, 16 );
    memset( param->cqm_4py, 16, 16 );
    memset( param->cqm_4pc, 16, 16 );
    memset( param->cqm_8iy, 16, 64 );
    memset( param->cqm_8py, 16, 64 );

    param->b_repeat_headers = 1;
    param->b_aud = 0;
}

static int parse_enum( const char *arg, const char * const *names, int *dst )
{
    int i;
    for( i = 0; names[i]; i++ )
        if( !strcmp( arg, names[i] ) )
        {
            *dst = i;
            return 0;
        }
    return -1;
}

static int parse_memory_32( const char *arg, int words, int bits, uint32_t **mem)
{
    FILE *f;
    int i, j, c;
    f = fopen( arg, "r" );
    if( !f )
        goto err;
    *mem = (uint32_t *) x264_malloc( words * sizeof( uint32_t ));
    if( !mem )
        goto err_file;
    for( i = 0; i < words; ++i )
    {
        uint32_t word = 0;
        while( isspace( c = getc( f )))
            ;
        ungetc( c, f );
        for( j = 0; j < bits; ++j )
        {
            c = getc( f );
            word <<= 1;
            if( c == '1' )
                word |= 1;
            else if( c != '0' )
                goto err_mem;
        }
        (*mem)[i] = word;
        if( i < words - 1 )
        {
            c = getc( f );
            if( !isspace( c ))
                goto err_mem;
        }
    }
    return 0;

 err_mem:
    x264_free( *mem );
    *mem = NULL;
 err_file:
    fclose( f );
 err:
    return -1;
}

static int parse_memory_16( const char *arg, int words, int bits, uint16_t **mem)
{
    FILE *f;
    int i, j, c;
    f = fopen( arg, "r" );
    if( !f )
        goto err;
    *mem = (uint16_t *) x264_malloc( words * sizeof( uint16_t ));
    if( !mem )
        goto err_file;
    for( i = 0; i < words; ++i )
    {
        uint16_t word = 0;
        while( isspace( c = getc( f )))
            ;
        ungetc( c, f );
        for( j = 0; j < bits; ++j )
        {
            c = getc( f );
            word <<= 1;
            if( c == '1' )
                word |= 1;
            else if( c != '0' )
                goto err_mem;
        }
        (*mem)[i] = word;
        if( i < words - 1 )
        {
            c = getc( f );
            if( !isspace( c ))
                goto err_mem;
        }
    }
    return 0;

 err_mem:
    x264_free( *mem );
    *mem = NULL;
 err_file:
    fclose( f );
 err:
    return -1;
}

static int parse_cqm( const char *str, uint8_t *cqm, int length )
{
    int i = 0;
    do {
        int coef;
        if( !sscanf( str, "%d", &coef ) || coef < 1 || coef > 255 )
            return -1;
        cqm[i++] = coef;
    } while( i < length && (str = strchr( str, ',' )) && str++ );
    return (i == length) ? 0 : -1;
}

static int x264_atobool( const char *str, int *b_error )
{
    if( !strcmp(str, "1") ||
        !strcmp(str, "true") ||
        !strcmp(str, "yes") )
        return 1;
    if( !strcmp(str, "0") ||
        !strcmp(str, "false") ||
        !strcmp(str, "no") )
        return 0;
    *b_error = 1;
    return 0;
}

static int x264_atoi( const char *str, int *b_error )
{
    char *end;
    int v = strtol( str, &end, 0 );
    if( end == str || *end != '\0' )
        *b_error = 1;
    return v;
}

static double x264_atof( const char *str, int *b_error )
{
    char *end;
    double v = strtod( str, &end );
    if( end == str || *end != '\0' )
        *b_error = 1;
    return v;
}

#define atobool(str) ( name_was_bool = 1, x264_atobool( str, &b_error ) )
#define atoi(str) x264_atoi( str, &b_error )
#define atof(str) x264_atof( str, &b_error )

int x264_param_parse( x264_param_t *p, const char *name, const char *value )
{
    char *name_buf = NULL;
    int b_error = 0;
    int name_was_bool;
    int value_was_null = !value;
    int i;

    if( !name )
        return X264_PARAM_BAD_NAME;
    if( !value )
        value = "true";

    if( value[0] == '=' )
        value++;

    if( strchr( name, '_' ) ) // s/_/-/g
    {
        char *p;
        name_buf = strdup(name);
        while( (p = strchr( name_buf, '_' )) )
            *p = '-';
        name = name_buf;
    }

    if( (!strncmp( name, "no-", 3 ) && (i = 3)) ||
        (!strncmp( name, "no", 2 ) && (i = 2)) )
    {
        name += i;
        value = atobool(value) ? "false" : "true";
    }
    name_was_bool = 0;

#define OPT(STR) else if( !strcmp( name, STR ) )
#define OPT2(STR0, STR1) else if( !strcmp( name, STR0 ) || !strcmp( name, STR1 ) )
    if(0);
    OPT("asm")
        p->cpu = atobool(value) ? x264_cpu_detect() : 0;
    OPT("threads")
    {
        if( !strcmp(value, "auto") )
            p->i_threads = 0;
        else
            p->i_threads = atoi(value);
    }
    OPT2("deterministic", "n-deterministic")
        p->b_deterministic = atobool(value);
    OPT2("level", "level-idc")
    {
        if( atof(value) < 6 )
            p->i_level_idc = (int)(10*atof(value)+.5);
        else
            p->i_level_idc = atoi(value);
    }
    OPT("sar")
    {
        b_error = ( 2 != sscanf( value, "%d:%d", &p->vui.i_sar_width, &p->vui.i_sar_height ) &&
                    2 != sscanf( value, "%d/%d", &p->vui.i_sar_width, &p->vui.i_sar_height ) );
    }
    OPT("overscan")
        b_error |= parse_enum( value, x264_overscan_names, &p->vui.i_overscan );
    OPT("videoformat")
        b_error |= parse_enum( value, x264_vidformat_names, &p->vui.i_vidformat );
    OPT("fullrange")
        b_error |= parse_enum( value, x264_fullrange_names, &p->vui.b_fullrange );
    OPT("colorprim")
        b_error |= parse_enum( value, x264_colorprim_names, &p->vui.i_colorprim );
    OPT("transfer")
        b_error |= parse_enum( value, x264_transfer_names, &p->vui.i_transfer );
    OPT("colormatrix")
        b_error |= parse_enum( value, x264_colmatrix_names, &p->vui.i_colmatrix );
    OPT("chromaloc")
    {
        p->vui.i_chroma_loc = atoi(value);
        b_error = ( p->vui.i_chroma_loc < 0 || p->vui.i_chroma_loc > 5 );
    }
    OPT("fps")
    {
        if( sscanf( value, "%d/%d", &p->i_fps_num, &p->i_fps_den ) == 2 )
            ;
        else
        {
            float fps = atof(value);
            p->i_fps_num = (int)(fps * 1000 + .5);
            p->i_fps_den = 1000;
        }
    }
    OPT2("ref", "frameref")
        p->i_frame_reference = atoi(value);
    OPT("keyint")
    {
        p->i_keyint_max = atoi(value);
        if( p->i_keyint_min > p->i_keyint_max )
            p->i_keyint_min = p->i_keyint_max;
    }
    OPT2("min-keyint", "keyint-min")
    {
        p->i_keyint_min = atoi(value);
        if( p->i_keyint_max < p->i_keyint_min )
            p->i_keyint_max = p->i_keyint_min;
    }
    OPT("scenecut")
        p->i_scenecut_threshold = atoi(value);
    OPT("pre-scenecut")
        p->b_pre_scenecut = atobool(value);
    OPT("bframes")
        p->i_bframe = atoi(value);
    OPT("b-adapt")
        p->b_bframe_adaptive = atobool(value);
    OPT("b-bias")
        p->i_bframe_bias = atoi(value);
    OPT("b-pyramid")
        p->b_bframe_pyramid = atobool(value);
    OPT("nf")
        p->b_deblocking_filter = !atobool(value);
    OPT2("filter", "deblock")
    {
        if( 2 == sscanf( value, "%d:%d", &p->i_deblocking_filter_alphac0, &p->i_deblocking_filter_beta ) ||
            2 == sscanf( value, "%d,%d", &p->i_deblocking_filter_alphac0, &p->i_deblocking_filter_beta ) )
        {
            p->b_deblocking_filter = 1;
        }
        else if( sscanf( value, "%d", &p->i_deblocking_filter_alphac0 ) )
        {
            p->b_deblocking_filter = 1;
            p->i_deblocking_filter_beta = p->i_deblocking_filter_alphac0;
        }
        else
            p->b_deblocking_filter = atobool(value);
    }
    OPT("cabac")
        p->b_cabac = atobool(value);
    OPT("cabac-idc")
        p->i_cabac_init_idc = atoi(value);
    OPT("interlaced")
        p->b_interlaced = atobool(value);
    OPT("cqm")
    {
        if( strstr( value, "flat" ) )
            p->i_cqm_preset = X264_CQM_FLAT;
        else if( strstr( value, "jvt" ) )
            p->i_cqm_preset = X264_CQM_JVT;
        else
            p->psz_cqm_file = strdup(value);
    }
    OPT("cqmfile")
        p->psz_cqm_file = strdup(value);
    OPT("cqm4")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4iy, 16 );
        b_error |= parse_cqm( value, p->cqm_4ic, 16 );
        b_error |= parse_cqm( value, p->cqm_4py, 16 );
        b_error |= parse_cqm( value, p->cqm_4pc, 16 );
    }
    OPT("cqm8")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_8iy, 64 );
        b_error |= parse_cqm( value, p->cqm_8py, 64 );
    }
    OPT("cqm4i")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4iy, 16 );
        b_error |= parse_cqm( value, p->cqm_4ic, 16 );
    }
    OPT("cqm4p")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4py, 16 );
        b_error |= parse_cqm( value, p->cqm_4pc, 16 );
    }
    OPT("cqm4iy")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4iy, 16 );
    }
    OPT("cqm4ic")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4ic, 16 );
    }
    OPT("cqm4py")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4py, 16 );
    }
    OPT("cqm4pc")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_4pc, 16 );
    }
    OPT("cqm8i")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_8iy, 64 );
    }
    OPT("cqm8p")
    {
        p->i_cqm_preset = X264_CQM_CUSTOM;
        b_error |= parse_cqm( value, p->cqm_8py, 64 );
    }
    OPT("log")
        p->i_log_level = atoi(value);
#ifdef VISUALIZE
    OPT("visualize")
        p->b_visualize = atobool(value);
#endif
    OPT2("analyse", "partitions")
    {
        p->analyse.inter = 0;
        if( strstr( value, "none" ) )  p->analyse.inter =  0;
        if( strstr( value, "all" ) )   p->analyse.inter = ~0;

        if( strstr( value, "i4x4" ) )  p->analyse.inter |= X264_ANALYSE_I4x4;
        if( strstr( value, "i8x8" ) )  p->analyse.inter |= X264_ANALYSE_I8x8;
        if( strstr( value, "p8x8" ) )  p->analyse.inter |= X264_ANALYSE_PSUB16x16;
        if( strstr( value, "p4x4" ) )  p->analyse.inter |= X264_ANALYSE_PSUB8x8;
        if( strstr( value, "b8x8" ) )  p->analyse.inter |= X264_ANALYSE_BSUB16x16;
    }
    OPT("8x8dct")
        p->analyse.b_transform_8x8 = atobool(value);
    OPT2("weightb", "weight-b")
        p->analyse.b_weighted_bipred = atobool(value);
    OPT2("direct", "direct-pred")
        b_error |= parse_enum( value, x264_direct_pred_names, &p->analyse.i_direct_mv_pred );
    OPT("direct-8x8")
        p->analyse.i_direct_8x8_inference = atoi(value);
    OPT("chroma-qp-offset")
        p->analyse.i_chroma_qp_offset = atoi(value);
    OPT("me")
        b_error |= parse_enum( value, x264_motion_est_names, &p->analyse.i_me_method );
    OPT("casfeu")
        p->analyse.i_cas_feu = atoi(value);
    OPT("casfoptl")
        p->analyse.i_cas_foptl = atoi(value);
    OPT("casqeu")
        p->analyse.i_cas_qeu = atoi(value);
    OPT("casqoptl")
        p->analyse.i_cas_qoptl = atoi(value);
    OPT("casprog")
        b_error |= parse_memory_32( value, 256, 20, &p->analyse.p_cas_prog );
    OPT("caspoint")
        b_error |= parse_memory_16( value, 256, 16, &p->analyse.p_cas_point );
    OPT("cashadamard")
        p->analyse.b_cas_hadamard = atobool(value);
    OPT("casmvcost")
        p->analyse.b_cas_mvcost = atobool(value);
    OPT("casmvcand")
        p->analyse.b_cas_mvcand = atobool(value);
    OPT2("merange", "me-range")
        p->analyse.i_me_range = atoi(value);
    OPT2("hardrange", "hard-range")
        p->analyse.i_hard_range = atoi(value);
    OPT2("mvrange", "mv-range")
        p->analyse.i_mv_range = atoi(value);
    OPT2("mvrange-thread", "mv-range-thread")
        p->analyse.i_mv_range_thread = atoi(value);
    OPT2("subme", "subq")
        p->analyse.i_subpel_refine = atoi(value);
    OPT("bime")
        p->analyse.b_bidir_me = atobool(value);
    OPT("chroma-me")
        p->analyse.b_chroma_me = atobool(value);
    OPT2("b-rdo", "brdo")
        p->analyse.b_bframe_rdo = atobool(value);
    OPT("mixed-refs")
        p->analyse.b_mixed_references = atobool(value);
    OPT("trellis")
        p->analyse.i_trellis = atoi(value);
    OPT("fast-pskip")
        p->analyse.b_fast_pskip = atobool(value);
    OPT("dct-decimate")
        p->analyse.b_dct_decimate = atobool(value);
    OPT("deadzone-inter")
        p->analyse.i_luma_deadzone[0] = atoi(value);
    OPT("deadzone-intra")
        p->analyse.i_luma_deadzone[1] = atoi(value);
    OPT("nr")
        p->analyse.i_noise_reduction = atoi(value);
    OPT("bitrate")
    {
        p->rc.i_bitrate = atoi(value);
        p->rc.i_rc_method = X264_RC_ABR;
    }
    OPT2("qp", "qp_constant")
    {
        p->rc.i_qp_constant = atoi(value);
        p->rc.i_rc_method = X264_RC_CQP;
    }
    OPT("crf")
    {
        p->rc.f_rf_constant = atof(value);
        p->rc.i_rc_method = X264_RC_CRF;
    }
    OPT2("qpmin", "qp-min")
        p->rc.i_qp_min = atoi(value);
    OPT2("qpmax", "qp-max")
        p->rc.i_qp_max = atoi(value);
    OPT2("qpstep", "qp-step")
        p->rc.i_qp_step = atoi(value);
    OPT("ratetol")
        p->rc.f_rate_tolerance = !strncmp("inf", value, 3) ? 1e9 : atof(value);
    OPT("vbv-maxrate")
        p->rc.i_vbv_max_bitrate = atoi(value);
    OPT("vbv-bufsize")
        p->rc.i_vbv_buffer_size = atoi(value);
    OPT("vbv-init")
        p->rc.f_vbv_buffer_init = atof(value);
    OPT2("ipratio", "ip-factor")
        p->rc.f_ip_factor = atof(value);
    OPT2("pbratio", "pb-factor")
        p->rc.f_pb_factor = atof(value);
    OPT("pass")
    {
        int i = x264_clip3( atoi(value), 0, 3 );
        p->rc.b_stat_write = i & 1;
        p->rc.b_stat_read = i & 2;
    }
    OPT("stats")
    {
        p->rc.psz_stat_in = strdup(value);
        p->rc.psz_stat_out = strdup(value);
    }
    OPT("rceq")
        p->rc.psz_rc_eq = strdup(value);
    OPT("qcomp")
        p->rc.f_qcompress = atof(value);
    OPT("qblur")
        p->rc.f_qblur = atof(value);
    OPT2("cplxblur", "cplx-blur")
        p->rc.f_complexity_blur = atof(value);
    OPT("zones")
        p->rc.psz_zones = strdup(value);
    OPT("psnr")
        p->analyse.b_psnr = atobool(value);
    OPT("ssim")
        p->analyse.b_ssim = atobool(value);
    OPT("aud")
        p->b_aud = atobool(value);
    OPT("sps-id")
        p->i_sps_id = atoi(value);
    OPT("global-header")
        p->b_repeat_headers = !atobool(value);
    OPT("repeat-headers")
        p->b_repeat_headers = atobool(value);
    else
        return X264_PARAM_BAD_NAME;
#undef OPT
#undef OPT2
#undef atobool
#undef atoi
#undef atof

    if( name_buf )
        free( name_buf );

    b_error |= value_was_null && !name_was_bool;
    return b_error ? X264_PARAM_BAD_VALUE : 0;
}

/****************************************************************************
 * x264_log:
 ****************************************************************************/
void x264_log( x264_t *h, int i_level, const char *psz_fmt, ... )
{
    if( i_level <= h->param.i_log_level )
    {
        va_list arg;
        va_start( arg, psz_fmt );
        h->param.pf_log( h->param.p_log_private, i_level, psz_fmt, arg );
        va_end( arg );
    }
}

static void x264_log_default( void *p_unused, int i_level, const char *psz_fmt, va_list arg )
{
    char *psz_prefix;
    switch( i_level )
    {
        case X264_LOG_ERROR:
            psz_prefix = "error";
            break;
        case X264_LOG_WARNING:
            psz_prefix = "warning";
            break;
        case X264_LOG_INFO:
            psz_prefix = "info";
            break;
        case X264_LOG_DEBUG:
            psz_prefix = "debug";
            break;
        default:
            psz_prefix = "unknown";
            break;
    }
    fprintf( stderr, "x264 [%s]: ", psz_prefix );
    vfprintf( stderr, psz_fmt, arg );
}

/****************************************************************************
 * x264_picture_alloc:
 ****************************************************************************/
void x264_picture_alloc( x264_picture_t *pic, int i_csp, int i_width, int i_height )
{
    pic->i_type = X264_TYPE_AUTO;
    pic->i_qpplus1 = 0;
    pic->img.i_csp = i_csp;
    switch( i_csp & X264_CSP_MASK )
    {
        case X264_CSP_I420:
        case X264_CSP_YV12:
            pic->img.i_plane = 3;
            pic->img.plane[0] = x264_malloc( 3 * i_width * i_height / 2 );
            pic->img.plane[1] = pic->img.plane[0] + i_width * i_height;
            pic->img.plane[2] = pic->img.plane[1] + i_width * i_height / 4;
            pic->img.i_stride[0] = i_width;
            pic->img.i_stride[1] = i_width / 2;
            pic->img.i_stride[2] = i_width / 2;
            break;

        case X264_CSP_I422:
            pic->img.i_plane = 3;
            pic->img.plane[0] = x264_malloc( 2 * i_width * i_height );
            pic->img.plane[1] = pic->img.plane[0] + i_width * i_height;
            pic->img.plane[2] = pic->img.plane[1] + i_width * i_height / 2;
            pic->img.i_stride[0] = i_width;
            pic->img.i_stride[1] = i_width / 2;
            pic->img.i_stride[2] = i_width / 2;
            break;

        case X264_CSP_I444:
            pic->img.i_plane = 3;
            pic->img.plane[0] = x264_malloc( 3 * i_width * i_height );
            pic->img.plane[1] = pic->img.plane[0] + i_width * i_height;
            pic->img.plane[2] = pic->img.plane[1] + i_width * i_height;
            pic->img.i_stride[0] = i_width;
            pic->img.i_stride[1] = i_width;
            pic->img.i_stride[2] = i_width;
            break;

        case X264_CSP_YUYV:
            pic->img.i_plane = 1;
            pic->img.plane[0] = x264_malloc( 2 * i_width * i_height );
            pic->img.i_stride[0] = 2 * i_width;
            break;

        case X264_CSP_RGB:
        case X264_CSP_BGR:
            pic->img.i_plane = 1;
            pic->img.plane[0] = x264_malloc( 3 * i_width * i_height );
            pic->img.i_stride[0] = 3 * i_width;
            break;

        case X264_CSP_BGRA:
            pic->img.i_plane = 1;
            pic->img.plane[0] = x264_malloc( 4 * i_width * i_height );
            pic->img.i_stride[0] = 4 * i_width;
            break;

        default:
            fprintf( stderr, "invalid CSP\n" );
            pic->img.i_plane = 0;
            break;
    }
}

/****************************************************************************
 * x264_picture_clean:
 ****************************************************************************/
void x264_picture_clean( x264_picture_t *pic )
{
    x264_free( pic->img.plane[0] );

    /* just to be safe */
    memset( pic, 0, sizeof( x264_picture_t ) );
}

/****************************************************************************
 * x264_nal_encode:
 ****************************************************************************/
int x264_nal_encode( void *p_data, int *pi_data, int b_annexeb, x264_nal_t *nal )
{
    uint8_t *dst = p_data;
    uint8_t *src = nal->p_payload;
    uint8_t *end = &nal->p_payload[nal->i_payload];

    int i_count = 0;

    /* FIXME this code doesn't check overflow */

    if( b_annexeb )
    {
        /* long nal start code (we always use long ones)*/
        *dst++ = 0x00;
        *dst++ = 0x00;
        *dst++ = 0x00;
        *dst++ = 0x01;
    }

    /* nal header */
    *dst++ = ( 0x00 << 7 ) | ( nal->i_ref_idc << 5 ) | nal->i_type;

    while( src < end )
    {
        if( i_count == 2 && *src <= 0x03 )
        {
            *dst++ = 0x03;
            i_count = 0;
        }
        if( *src == 0 )
        {
            i_count++;
        }
        else
        {
            i_count = 0;
        }
        *dst++ = *src++;
    }
    *pi_data = dst - (uint8_t*)p_data;

    return *pi_data;
}

/****************************************************************************
 * x264_nal_decode:
 ****************************************************************************/
int x264_nal_decode( x264_nal_t *nal, void *p_data, int i_data )
{
    uint8_t *src = p_data;
    uint8_t *end = &src[i_data];
    uint8_t *dst = nal->p_payload;

    nal->i_type    = src[0]&0x1f;
    nal->i_ref_idc = (src[0] >> 5)&0x03;

    src++;

    while( src < end )
    {
        if( src < end - 3 && src[0] == 0x00 && src[1] == 0x00  && src[2] == 0x03 )
        {
            *dst++ = 0x00;
            *dst++ = 0x00;

            src += 3;
            continue;
        }
        *dst++ = *src++;
    }

    nal->i_payload = dst - (uint8_t*)p_data;
    return 0;
}



/****************************************************************************
 * x264_malloc:
 ****************************************************************************/
void *x264_malloc( int i_size )
{
#ifdef SYS_MACOSX
    /* Mac OS X always returns 16 bytes aligned memory */
    return malloc( i_size );
#elif defined( HAVE_MALLOC_H )
    return memalign( 16, i_size );
#else
    uint8_t * buf;
    uint8_t * align_buf;
    buf = (uint8_t *) malloc( i_size + 15 + sizeof( void ** ) +
              sizeof( int ) );
    align_buf = buf + 15 + sizeof( void ** ) + sizeof( int );
    align_buf -= (long) align_buf & 15;
    *( (void **) ( align_buf - sizeof( void ** ) ) ) = buf;
    *( (int *) ( align_buf - sizeof( void ** ) - sizeof( int ) ) ) = i_size;
    return align_buf;
#endif
}

/****************************************************************************
 * x264_free:
 ****************************************************************************/
void x264_free( void *p )
{
    if( p )
    {
#if defined( HAVE_MALLOC_H ) || defined( SYS_MACOSX )
        free( p );
#else
        free( *( ( ( void **) p ) - 1 ) );
#endif
    }
}

/****************************************************************************
 * x264_realloc:
 ****************************************************************************/
void *x264_realloc( void *p, int i_size )
{
#ifdef HAVE_MALLOC_H
    return realloc( p, i_size );
#else
    int       i_old_size = 0;
    uint8_t * p_new;
    if( p )
    {
        i_old_size = *( (int*) ( (uint8_t*) p ) - sizeof( void ** ) -
                         sizeof( int ) );
    }
    p_new = x264_malloc( i_size );
    if( i_old_size > 0 && i_size > 0 )
    {
        memcpy( p_new, p, ( i_old_size < i_size ) ? i_old_size : i_size );
    }
    x264_free( p );
    return p_new;
#endif
}

/****************************************************************************
 * x264_reduce_fraction:
 ****************************************************************************/
void x264_reduce_fraction( int *n, int *d )
{
    int a = *n;
    int b = *d;
    int c;
    if( !a || !b )
        return;
    c = a % b;
    while(c)
    {
	a = b;
	b = c;
	c = a % b;
    }
    *n /= b;
    *d /= b;
}

/****************************************************************************
 * x264_slurp_file:
 ****************************************************************************/
char *x264_slurp_file( const char *filename )
{
    int b_error = 0;
    int i_size;
    char *buf;
    FILE *fh = fopen( filename, "rb" );
    if( !fh )
        return NULL;
    b_error |= fseek( fh, 0, SEEK_END ) < 0;
    b_error |= ( i_size = ftell( fh ) ) <= 0;
    b_error |= fseek( fh, 0, SEEK_SET ) < 0;
    if( b_error )
        return NULL;
    buf = x264_malloc( i_size+2 );
    if( buf == NULL )
        return NULL;
    b_error |= fread( buf, 1, i_size, fh ) != i_size;
    if( buf[i_size-1] != '\n' )
        buf[i_size++] = '\n';
    buf[i_size] = 0;
    fclose( fh );
    if( b_error )
    {
        x264_free( buf );
        return NULL;
    }
    return buf;
}

/****************************************************************************
 * x264_param2string:
 ****************************************************************************/
char *x264_param2string( x264_param_t *p, int b_res )
{
    int len = 1000;
    char *buf, *s;
    if( p->rc.psz_zones )
        len += strlen(p->rc.psz_zones);
    buf = s = x264_malloc( len );

    if( b_res )
    {
        s += sprintf( s, "%dx%d ", p->i_width, p->i_height );
        s += sprintf( s, "fps=%d/%d ", p->i_fps_num, p->i_fps_den );
    }

    s += sprintf( s, "cabac=%d", p->b_cabac );
    s += sprintf( s, " ref=%d", p->i_frame_reference );
    s += sprintf( s, " deblock=%d:%d:%d", p->b_deblocking_filter,
                  p->i_deblocking_filter_alphac0, p->i_deblocking_filter_beta );
    s += sprintf( s, " analyse=%#x:%#x", p->analyse.intra, p->analyse.inter );
    s += sprintf( s, " me=%s", x264_motion_est_names[ p->analyse.i_me_method ] );
    s += sprintf( s, " subme=%d", p->analyse.i_subpel_refine );
    s += sprintf( s, " brdo=%d", p->analyse.b_bframe_rdo );
    s += sprintf( s, " mixed_ref=%d", p->analyse.b_mixed_references );
    s += sprintf( s, " me_range=%d", p->analyse.i_me_range );
    s += sprintf( s, " hard_range=%d", p->analyse.i_hard_range );
    s += sprintf( s, " chroma_me=%d", p->analyse.b_chroma_me );
    s += sprintf( s, " trellis=%d", p->analyse.i_trellis );
    s += sprintf( s, " 8x8dct=%d", p->analyse.b_transform_8x8 );
    s += sprintf( s, " cqm=%d", p->i_cqm_preset );
    s += sprintf( s, " deadzone=%d,%d", p->analyse.i_luma_deadzone[0], p->analyse.i_luma_deadzone[1] );
    s += sprintf( s, " chroma_qp_offset=%d", p->analyse.i_chroma_qp_offset );
    s += sprintf( s, " threads=%d", p->i_threads );
    s += sprintf( s, " nr=%d", p->analyse.i_noise_reduction );
    s += sprintf( s, " decimate=%d", p->analyse.b_dct_decimate );
    s += sprintf( s, " mbaff=%d", p->b_interlaced );

    s += sprintf( s, " bframes=%d", p->i_bframe );
    if( p->i_bframe )
    {
        s += sprintf( s, " b_pyramid=%d b_adapt=%d b_bias=%d direct=%d wpredb=%d bime=%d",
                      p->b_bframe_pyramid, p->b_bframe_adaptive, p->i_bframe_bias,
                      p->analyse.i_direct_mv_pred, p->analyse.b_weighted_bipred,
                      p->analyse.b_bidir_me );
    }

    s += sprintf( s, " keyint=%d keyint_min=%d scenecut=%d%s",
                  p->i_keyint_max, p->i_keyint_min, p->i_scenecut_threshold,
                  p->b_pre_scenecut ? "(pre)" : "" );

    s += sprintf( s, " rc=%s", p->rc.i_rc_method == X264_RC_ABR ?
                               ( p->rc.b_stat_read ? "2pass" : p->rc.i_vbv_buffer_size ? "cbr" : "abr" )
                               : p->rc.i_rc_method == X264_RC_CRF ? "crf" : "cqp" );
    if( p->rc.i_rc_method == X264_RC_ABR || p->rc.i_rc_method == X264_RC_CRF )
    {
        if( p->rc.i_rc_method == X264_RC_CRF )
            s += sprintf( s, " crf=%.1f", p->rc.f_rf_constant );
        else
            s += sprintf( s, " bitrate=%d ratetol=%.1f",
                          p->rc.i_bitrate, p->rc.f_rate_tolerance );
        s += sprintf( s, " rceq='%s' qcomp=%.2f qpmin=%d qpmax=%d qpstep=%d",
                      p->rc.psz_rc_eq, p->rc.f_qcompress,
                      p->rc.i_qp_min, p->rc.i_qp_max, p->rc.i_qp_step );
        if( p->rc.b_stat_read )
            s += sprintf( s, " cplxblur=%.1f qblur=%.1f",
                          p->rc.f_complexity_blur, p->rc.f_qblur );
        if( p->rc.i_vbv_buffer_size )
            s += sprintf( s, " vbv_maxrate=%d vbv_bufsize=%d",
                          p->rc.i_vbv_max_bitrate, p->rc.i_vbv_buffer_size );
    }
    else if( p->rc.i_rc_method == X264_RC_CQP )
        s += sprintf( s, " qp=%d", p->rc.i_qp_constant );
    if( !(p->rc.i_rc_method == X264_RC_CQP && p->rc.i_qp_constant == 0) )
    {
        s += sprintf( s, " ip_ratio=%.2f", p->rc.f_ip_factor );
        if( p->i_bframe )
            s += sprintf( s, " pb_ratio=%.2f", p->rc.f_pb_factor );
        if( p->rc.psz_zones )
            s += sprintf( s, " zones=%s", p->rc.psz_zones );
        else if( p->rc.i_zones )
            s += sprintf( s, " zones" );
    }

    return buf;
}

