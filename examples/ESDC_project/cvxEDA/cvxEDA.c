/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * cvxEDA.c
 *
 * Code generation for function 'cvxEDA'
 *
 */

/* Include files */
#include "cvxEDA.h"
#include "bsxfun.h"
#include "colon.h"
#include "conv.h"
#include "cvxEDA_data.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "horzcat.h"
#include "minOrMax.h"
#include "mtimes.h"
#include "quadprog.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sparse.h"
#include "sparse1.h"
#include "speye.h"
#include "sum.h"
#include "vertcat.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>
#include "bflb_core.h"

/* Function Declarations */
static float rt_roundd_snf(float u);

/* Function Definitions */
static float rt_roundd_snf(float u)
{
    float y;
    if (fabsf(u) < 4.503599627370496E+15f) {
        if (u >= 0.5f) {
            y = floorf(u + 0.5f);
        } else if (u > -0.5f) {
            y = u * 0.0f;
        } else {
            y = ceilf(u - 0.5f);
        }
    } else {
        y = u;
    }

    return y;
}

float A_[14400] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
void cvxEDA(const float y[120], float delta, float r[120], float p[120],
            float t[120], f_sparse *l, g_sparse *d, float e[120], float *obj)
{
    static const float a[240] = { 1.0, 0.0083333333333333332, 1.0,
                                   0.016666666666666666, 1.0, 0.025, 1.0, 0.033333333333333333, 1.0,
                                   0.041666666666666664, 1.0, 0.05, 1.0, 0.058333333333333334, 1.0,
                                   0.066666666666666666, 1.0, 0.075, 1.0, 0.083333333333333329, 1.0,
                                   0.09166666666666666, 1.0, 0.1, 1.0, 0.10833333333333334, 1.0,
                                   0.11666666666666667, 1.0, 0.125, 1.0, 0.13333333333333333, 1.0,
                                   0.14166666666666666, 1.0, 0.15, 1.0, 0.15833333333333333, 1.0,
                                   0.16666666666666666, 1.0, 0.175, 1.0, 0.18333333333333332, 1.0,
                                   0.19166666666666668, 1.0, 0.2, 1.0, 0.20833333333333334, 1.0,
                                   0.21666666666666667, 1.0, 0.225, 1.0, 0.23333333333333334, 1.0,
                                   0.24166666666666667, 1.0, 0.25, 1.0, 0.25833333333333336, 1.0,
                                   0.26666666666666666, 1.0, 0.275, 1.0, 0.28333333333333333, 1.0,
                                   0.29166666666666669, 1.0, 0.3, 1.0, 0.30833333333333335, 1.0,
                                   0.31666666666666665, 1.0, 0.325, 1.0, 0.33333333333333331, 1.0,
                                   0.34166666666666667, 1.0, 0.35, 1.0, 0.35833333333333334, 1.0,
                                   0.36666666666666664, 1.0, 0.375, 1.0, 0.38333333333333336, 1.0,
                                   0.39166666666666666, 1.0, 0.4, 1.0, 0.40833333333333333, 1.0,
                                   0.41666666666666669, 1.0, 0.425, 1.0, 0.43333333333333335, 1.0,
                                   0.44166666666666665, 1.0, 0.45, 1.0, 0.45833333333333331, 1.0,
                                   0.46666666666666667, 1.0, 0.475, 1.0, 0.48333333333333334, 1.0,
                                   0.49166666666666664, 1.0, 0.5, 1.0, 0.5083333333333333, 1.0,
                                   0.51666666666666672, 1.0, 0.525, 1.0, 0.53333333333333333, 1.0,
                                   0.54166666666666663, 1.0, 0.55, 1.0, 0.55833333333333335, 1.0,
                                   0.56666666666666665, 1.0, 0.575, 1.0, 0.58333333333333337, 1.0,
                                   0.59166666666666667, 1.0, 0.6, 1.0, 0.60833333333333328, 1.0,
                                   0.6166666666666667, 1.0, 0.625, 1.0, 0.6333333333333333, 1.0,
                                   0.64166666666666672, 1.0, 0.65, 1.0, 0.65833333333333333, 1.0,
                                   0.66666666666666663, 1.0, 0.675, 1.0, 0.68333333333333335, 1.0,
                                   0.69166666666666665, 1.0, 0.7, 1.0, 0.70833333333333337, 1.0,
                                   0.71666666666666667, 1.0, 0.725, 1.0, 0.73333333333333328, 1.0,
                                   0.7416666666666667, 1.0, 0.75, 1.0, 0.7583333333333333, 1.0,
                                   0.76666666666666672, 1.0, 0.775, 1.0, 0.78333333333333333, 1.0,
                                   0.79166666666666663, 1.0, 0.8, 1.0, 0.80833333333333335, 1.0,
                                   0.81666666666666665, 1.0, 0.825, 1.0, 0.83333333333333337, 1.0,
                                   0.84166666666666667, 1.0, 0.85, 1.0, 0.85833333333333328, 1.0,
                                   0.8666666666666667, 1.0, 0.875, 1.0, 0.8833333333333333, 1.0,
                                   0.89166666666666672, 1.0, 0.9, 1.0, 0.90833333333333333, 1.0,
                                   0.91666666666666663, 1.0, 0.925, 1.0, 0.93333333333333335, 1.0,
                                   0.94166666666666665, 1.0, 0.95, 1.0, 0.95833333333333337, 1.0,
                                   0.96666666666666667, 1.0, 0.975, 1.0, 0.98333333333333328, 1.0,
                                   0.9916666666666667, 1.0, 1.0 };

    emxArray_boolean_T *b_r;
    emxArray_boolean_T *r1;
    emxArray_int32_T *t7_colidx;
    emxArray_int32_T *t7_rowidx;
    emxArray_int32_T *t8_colidx;
    emxArray_int32_T *t8_rowidx;
    emxArray_real_T *b_A_;
    emxArray_real_T *b_c;
    emxArray_real_T *b_i;
    emxArray_real_T *b_p;
    emxArray_real_T *b_spl;
    emxArray_real_T *b_y;
    emxArray_real_T *c_c;
    emxArray_real_T *c_y;
    emxArray_real_T *f;
    emxArray_real_T *j;
    emxArray_real_T *r2;
    emxArray_real_T *spl;
    h_sparse b_expl_temp;
    h_sparse expl_temp;
    h_sparse f_expl_temp;
    h_sparse g_expl_temp;
    h_sparse h_expl_temp;
    h_sparse l_expl_temp;
    i_sparse c_expl_temp;
    i_sparse d_expl_temp;
    i_sparse e_expl_temp;
    i_sparse i_expl_temp;
    i_sparse j_expl_temp;
    i_sparse k_expl_temp;

    float b[354];
    float b_dv[240];
    float c[240];
    float S[120];
    float b_b[120];
    float ar[3];
    float b_a[2];
    float d_y;
    float delta_knot_s;
    float maxval;
    float uniOp_tunableEnvironment_idx_0;
    int bp;
    int bpend;
    int i;
    int i1;
    int ibmat;
    int itilerow;
    int ncols;
    int ntilerows;

    printf("Init cvxEDA\r\n");
    /*  function [r, p, t, l, d, e, obj] = cvxEDA(y, delta, tau0, tau1, delta_knot, alpha, gamma, solver) */
    /* CVXEDA Convex optimization approach to electrodermal activity processing */
    /*    This function implements the cvxEDA algorithm described in "cvxEDA: a */
    /*    Convex Optimization Approach to Electrodermal Activity Processing" */
    /*    (http://dx.doi.org/10.1109/TBME.2015.2474131 also available from the */
    /*    authors' homepages). */
    /*  */
    /*    Syntax: */
    /*    [r, p, t, l, d, e, obj] = cvxEDA(y, delta, tau0, tau1, delta_knot, */
    /*                                     alpha, gamma, solver) */
    /*  */
    /*    where: */
    /*       y: observed EDA signal (we recommend normalizing it: y = zscore(y)) */
    /*       delta: sampling interval (in seconds) of y */
    /*       tau0: slow time constant of the Bateman function (default 2.0) */
    /*       tau1: fast time constant of the Bateman function (default 0.7) */
    /*       delta_knot: time between knots of the tonic spline function (default 10) */
    /*       alpha: penalization for the sparse SMNA driver (default 0.0008) */
    /*       gamma: penalization for the tonic spline coefficients (default 0.01) */
    /*       solver: sparse QP solver to be used, 'quadprog' (default) or 'sedumi' */
    /*  */
    /*    returns (see paper for details): */
    /*       r: phasic component */
    /*       p: sparse SMNA driver of phasic component */
    /*       t: tonic component */
    /*       l: coefficients of tonic spline */
    /*       d: offset and slope of the linear drift term */
    /*       e: model residuals */
    /*       obj: value of objective function being minimized (eq 15 of paper) */
    /*  ______________________________________________________________________________ */
    /*  */
    /*  File:                         cvxEDA.m */
    /*  Last revised:                 22 Oct 2015 r68 */
    /*  ______________________________________________________________________________ */
    /*  */
    /*  Copyright (C) 2014-2015 Luca Citi, Alberto Greco */
    /*  */
    /*  This program is free software; you can redistribute it and/or modify it under */
    /*  the terms of the GNU General Public License as published by the Free Software */
    /*  Foundation; either version 3 of the License, or (at your option) any later */
    /*  version. */
    /*  */
    /*  This program is distributed in the hope that it will be useful, but WITHOUT */
    /*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS */
    /*  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. */
    /*  */
    /*  You may contact the author by e-mail (lciti@ieee.org). */
    /*  ______________________________________________________________________________ */
    /*  */
    /*  This method was first proposed in: */
    /*  A Greco, G Valenza, A Lanata, EP Scilingo, and L Citi */
    /*  "cvxEDA: a Convex Optimization Approach to Electrodermal Activity Processing" */
    /*  IEEE Transactions on Biomedical Engineering, 2015 */
    /*  DOI: 10.1109/TBME.2015.2474131 */
    /*  */
    /*  If you use this program in support of published research, please include a */
    /*  citation of the reference above. If you use this code in a software package, */
    /*  please explicitly inform the end users of this copyright notice and ask them */
    /*  to cite the reference above in their published research. */
    /*  ______________________________________________________________________________ */
    /*  parse arguments */
    /*  i = ~cellfun(@isempty, varargin); */
    /*  params(i) = varargin(i); */
    /*  bateman ARMA model */
    /*  a1 > a0 */
    maxval = delta * delta;
    delta_knot_s = 0.9285714285714286f * maxval;
    ar[0] = (1.4285714285714286f * delta + 2.0f) * (0.5f * delta + 2.0f) /
            delta_knot_s;
    ar[1] = (1.4285714285714286f * maxval - 8.0f) / delta_knot_s;
    ar[2] = (1.4285714285714286f * delta - 2.0f) * (0.5f * delta - 2.0f) /
            delta_knot_s;

    /*  matrices for ARMA model */
    for (bpend = 0; bpend < 3; bpend++) {
        ibmat = bpend * 118;
        for (itilerow = 0; itilerow < 118; itilerow++) {
            b[ibmat + itilerow] = ar[bpend];
        }
    }

    emxInit_real_T(&b_y, 2);
    emxInitStruct_sparse(&expl_temp);
    emxInitStruct_sparse(&b_expl_temp);
    sparse(b, &expl_temp);
    b_sparse(&b_expl_temp);

    printf("matrices for ARMA model\r\n");

    /*  spline */
    delta_knot_s = rt_roundd_snf(10.0f / delta);
    if (rtIsNaN(delta_knot_s)) {
        i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        b_y->size[1] = 1;
        emxEnsureCapacity_real_T(b_y, i);
        b_y->data[0] = rtNaN;
    } else if (delta_knot_s < 1.0f) {
        b_y->size[0] = 1;
        b_y->size[1] = 0;
    } else if (rtIsInf(delta_knot_s) && (1.0f == delta_knot_s)) {
        i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        b_y->size[1] = 1;
        emxEnsureCapacity_real_T(b_y, i);
        b_y->data[0] = rtNaN;
    } else {
        i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        ntilerows = (int)(delta_knot_s - 1.0f);
        b_y->size[1] = (int)(delta_knot_s - 1.0f) + 1;
        emxEnsureCapacity_real_T(b_y, i);
        for (i = 0; i <= ntilerows; i++) {
            b_y->data[i] = (float)i + 1.0f;
        }
    }

    emxInit_real_T(&c_y, 2);
    if (rtIsNaN(delta_knot_s - 1.0f)) {
        i = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 1;
        c_y->size[1] = 1;
        emxEnsureCapacity_real_T(c_y, i);
        c_y->data[0] = rtNaN;
    } else if (delta_knot_s - 1.0f < 1.0f) {
        c_y->size[0] = 1;
        c_y->size[1] = 0;
    } else if (rtIsInf(delta_knot_s - 1.0f) && (delta_knot_s - 1.0f == 1.0f)) {
        i = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 1;
        c_y->size[1] = 1;
        emxEnsureCapacity_real_T(c_y, i);
        c_y->data[0] = rtNaN;
    } else if (delta_knot_s - 1.0f == delta_knot_s - 1.0f) {
        i = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 1;
        ntilerows = (int)-(1.0f - (delta_knot_s - 1.0f));
        c_y->size[1] = (int)-(1.0f - (delta_knot_s - 1.0f)) + 1;
        emxEnsureCapacity_real_T(c_y, i);
        for (i = 0; i <= ntilerows; i++) {
            c_y->data[i] = (delta_knot_s - 1.0f) - (float)i;
        }
    } else {
        eml_float_colon(delta_knot_s - 1.0f, c_y);
    }

    emxInit_real_T(&spl, 1);
    i = spl->size[0];
    spl->size[0] = b_y->size[1] + c_y->size[1];
    emxEnsureCapacity_real_T(spl, i);
    ntilerows = b_y->size[1];
    for (i = 0; i < ntilerows; i++) {
        spl->data[i] = b_y->data[i];
    }

    ntilerows = c_y->size[1];
    for (i = 0; i < ntilerows; i++) {
        spl->data[i + b_y->size[1]] = c_y->data[i];
    }

    emxInit_real_T(&f, 1);

    printf("spline\r\n");

    /*  order 1 */
    i = f->size[0];
    f->size[0] = spl->size[0];
    emxEnsureCapacity_real_T(f, i);
    ntilerows = spl->size[0] - 1;
    for (i = 0; i <= ntilerows; i++) {
        f->data[i] = spl->data[i];
    }

    emxInit_real_T(&b_spl, 1);
    i = b_spl->size[0];
    b_spl->size[0] = spl->size[0];
    emxEnsureCapacity_real_T(b_spl, i);
    ntilerows = spl->size[0] - 1;
    for (i = 0; i <= ntilerows; i++) {
        b_spl->data[i] = spl->data[i];
    }

    conv(f, b_spl, spl);
    maxval = maximum(spl);
    ntilerows = spl->size[0];
    for (i = 0; i < ntilerows; i++) {
        spl->data[i] /= maxval;
    }

    printf("order 1\r\n");

    /*  matrix of spline regressors */
    if (spl->size[0] - 1 < 0) {
        b_y->size[0] = 1;
        b_y->size[1] = 0;
    } else {
        i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        b_y->size[1] = (int)((float)spl->size[0] - 1.0f) + 1;
        emxEnsureCapacity_real_T(b_y, i);
        ntilerows = (int)((float)spl->size[0] - 1.0f);
        for (i = 0; i <= ntilerows; i++) {
            b_y->data[i] = i;
        }
    }

    if (rtIsNaN(delta_knot_s)) {
        i = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 1;
        c_y->size[1] = 1;
        emxEnsureCapacity_real_T(c_y, i);
        c_y->data[0] = rtNaN;
    } else if ((delta_knot_s == 0.0f) || (delta_knot_s < 0.0f)) {
        c_y->size[0] = 1;
        c_y->size[1] = 0;
    } else if (rtIsInf(delta_knot_s)) {
        i = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 1;
        c_y->size[1] = 1;
        emxEnsureCapacity_real_T(c_y, i);
        c_y->data[0] = 1.0;
    } else if (delta_knot_s == delta_knot_s) {
        i = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 1;
        ntilerows = (int)floorf(119.0f / delta_knot_s);
        c_y->size[1] = ntilerows + 1;
        emxEnsureCapacity_real_T(c_y, i);
        for (i = 0; i <= ntilerows; i++) {
            c_y->data[i] = delta_knot_s * (float)i + 1.0f;
        }
    } else {
        b_eml_float_colon(delta_knot_s, c_y);
    }

    ibmat = (int)floorf((float)spl->size[0] / 2.0f);
    i = f->size[0];
    f->size[0] = b_y->size[1];
    emxEnsureCapacity_real_T(f, i);
    ntilerows = b_y->size[1];
    for (i = 0; i < ntilerows; i++) {
        f->data[i] = b_y->data[i] - (float)ibmat;
    }

    emxInit_real_T(&b_i, 2);
    bsxfun(f, c_y, b_i);
    emxFree_real_T(&c_y);
    if (b_i->size[1] < 1) {
        b_y->size[0] = 1;
        b_y->size[1] = 0;
    } else {
        i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        b_y->size[1] = (int)((float)b_i->size[1] - 1.0f) + 1;
        emxEnsureCapacity_real_T(b_y, i);
        ntilerows = (int)((float)b_i->size[1] - 1.0f);
        for (i = 0; i <= ntilerows; i++) {
            b_y->data[i] = (float)i + 1.0f;
        }
    }

    emxInit_real_T(&j, 2);
    i = j->size[0] * j->size[1];
    j->size[0] = spl->size[0];
    j->size[1] = b_y->size[1];
    emxEnsureCapacity_real_T(j, i);
    ncols = b_y->size[1];
    ntilerows = spl->size[0];
    for (bpend = 0; bpend < ncols; bpend++) {
        ibmat = bpend * ntilerows;
        for (itilerow = 0; itilerow < ntilerows; itilerow++) {
            j->data[ibmat + itilerow] = b_y->data[bpend];
        }
    }

    emxInit_real_T(&b_p, 2);
    emxInit_boolean_T(&b_r, 2);
    repmat(spl, b_i->size[1], b_p);
    i = b_r->size[0] * b_r->size[1];
    b_r->size[0] = b_i->size[0];
    b_r->size[1] = b_i->size[1];
    emxEnsureCapacity_boolean_T(b_r, i);
    ntilerows = b_i->size[0] * b_i->size[1];
    for (i = 0; i < ntilerows; i++) {
        b_r->data[i] = (b_i->data[i] >= 1.0f);
    }

    emxInit_boolean_T(&r1, 2);
    i = r1->size[0] * r1->size[1];
    r1->size[0] = b_i->size[0];
    r1->size[1] = b_i->size[1];
    emxEnsureCapacity_boolean_T(r1, i);
    ntilerows = b_i->size[0] * b_i->size[1];
    for (i = 0; i < ntilerows; i++) {
        r1->data[i] = (b_i->data[i] <= 120.0f);
    }

    ncols = b_r->size[0] * b_r->size[1] - 1;
    ibmat = 0;
    for (ntilerows = 0; ntilerows <= ncols; ntilerows++) {
        if (b_r->data[ntilerows] && r1->data[ntilerows]) {
            ibmat++;
        }
    }

    emxInit_int32_T(&t7_colidx, 1);
    i = t7_colidx->size[0];
    t7_colidx->size[0] = ibmat;
    emxEnsureCapacity_int32_T(t7_colidx, i);
    ibmat = 0;
    for (ntilerows = 0; ntilerows <= ncols; ntilerows++) {
        if (b_r->data[ntilerows] && r1->data[ntilerows]) {
            t7_colidx->data[ibmat] = ntilerows + 1;
            ibmat++;
        }
    }

    emxFree_boolean_T(&r1);
    emxFree_boolean_T(&b_r);
    i = f->size[0];
    f->size[0] = t7_colidx->size[0];
    emxEnsureCapacity_real_T(f, i);
    ntilerows = t7_colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f->data[i] = b_i->data[t7_colidx->data[i] - 1];
    }

    i = b_spl->size[0];
    b_spl->size[0] = t7_colidx->size[0];
    emxEnsureCapacity_real_T(b_spl, i);
    ntilerows = t7_colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        b_spl->data[i] = j->data[t7_colidx->data[i] - 1];
    }

    i = spl->size[0];
    spl->size[0] = t7_colidx->size[0];
    emxEnsureCapacity_real_T(spl, i);
    ntilerows = t7_colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        spl->data[i] = b_p->data[t7_colidx->data[i] - 1];
    }

    emxFree_real_T(&b_p);
    emxInitStruct_sparse1(&c_expl_temp);
    emxInitStruct_sparse1(&d_expl_temp);
    emxInitStruct_sparse1(&e_expl_temp);
    c_sparse(f, b_spl, spl, &c_expl_temp);
    ncols = c_expl_temp.n;

    printf("matrix of spline regressors\r\n");

    /*  trend */
    /*  Solve the problem: */
    /*  .5*(M*q + B*l + C*d - y)^2 + alpha*sum(A,1)*p + .5*gamma*l'*l */
    /*  s.t. A*q >= 0 */
    /*  Use Matlab's quadprog */
    speye(b_i->size[1], &d_expl_temp);
    sparse_times(d_expl_temp.colidx, d_expl_temp.rowidx, d_expl_temp.m,
                 d_expl_temp.n, &e_expl_temp);
    memset(&c[0], 0, 240U * sizeof(float));
    if (b_expl_temp.colidx->data[b_expl_temp.colidx->size[0] - 1] - 1 != 0) {
        for (itilerow = 0; itilerow < 120; itilerow++) {
            bpend = b_expl_temp.colidx->data[itilerow + 1] - 1;
            i = b_expl_temp.colidx->data[itilerow];
            for (bp = i; bp <= bpend; bp++) {
                ntilerows = itilerow << 1;
                ibmat = (b_expl_temp.rowidx->data[bp - 1] - 1) << 1;
                delta_knot_s = b_expl_temp.d->data[bp - 1];
                c[ntilerows] += a[ibmat] * delta_knot_s;
                c[ntilerows + 1] += a[ibmat + 1] * delta_knot_s;
            }
        }
    }
    printf("trend 1\r\n");

    emxInit_real_T(&b_c, 2);
    i = b_c->size[0] * b_c->size[1];
    b_c->size[0] = 2;
    b_c->size[1] = c_expl_temp.n;
    emxEnsureCapacity_real_T(b_c, i);
    ntilerows = c_expl_temp.n << 1;
    for (i = 0; i < ntilerows; i++) {
        b_c->data[i] = 0.0f;
    }

    if ((c_expl_temp.n != 0) && (c_expl_temp.colidx->data[c_expl_temp.colidx->size[0] - 1] - 1 != 0)) {
        for (itilerow = 0; itilerow < ncols; itilerow++) {
            bpend = c_expl_temp.colidx->data[itilerow + 1] - 1;
            i = c_expl_temp.colidx->data[itilerow];
            for (bp = i; bp <= bpend; bp++) {
                ntilerows = (c_expl_temp.rowidx->data[bp - 1] - 1) << 1;
                delta_knot_s = c_expl_temp.d->data[bp - 1];
                b_c->data[2 * itilerow] += a[ntilerows] * delta_knot_s;
                i1 = 2 * itilerow + 1;
                b_c->data[i1] += a[ntilerows + 1] * delta_knot_s;
            }
        }
    }
    printf("trend 2\r\n");

    emxInitStruct_sparse(&f_expl_temp);
    emxInitStruct_sparse(&g_expl_temp);
    emxInitStruct_sparse(&h_expl_temp);
    sparse_ctranspose(b_expl_temp.d, b_expl_temp.colidx, b_expl_temp.rowidx,
                      &f_expl_temp);
    sparse_mtimes(f_expl_temp.d, f_expl_temp.colidx, f_expl_temp.rowidx,
                  b_expl_temp.d, b_expl_temp.colidx, b_expl_temp.rowidx,
                  &g_expl_temp);
    sparse_ctranspose(b_expl_temp.d, b_expl_temp.colidx, b_expl_temp.rowidx,
                      &h_expl_temp);
    i = f_expl_temp.d->size[0];
    f_expl_temp.d->size[0] = h_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(f_expl_temp.d, i);
    ntilerows = h_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.d->data[i] = h_expl_temp.d->data[i];
    }

    printf("trend 3\r\n");

    i = f_expl_temp.colidx->size[0];
    f_expl_temp.colidx->size[0] = h_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.colidx, i);
    ntilerows = h_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.colidx->data[i] = h_expl_temp.colidx->data[i];
    }

    i = f_expl_temp.rowidx->size[0];
    f_expl_temp.rowidx->size[0] = h_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.rowidx, i);
    ntilerows = h_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.rowidx->data[i] = h_expl_temp.rowidx->data[i];
    }

    printf("trend 4\r\n");

    b_sparse_mtimes(f_expl_temp.d, f_expl_temp.colidx, f_expl_temp.rowidx, dv,
                    b_dv);
    sparse_ctranspose(b_expl_temp.d, b_expl_temp.colidx, b_expl_temp.rowidx,
                      &h_expl_temp);
    i = f_expl_temp.d->size[0];
    f_expl_temp.d->size[0] = h_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(f_expl_temp.d, i);
    ntilerows = h_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.d->data[i] = h_expl_temp.d->data[i];
    }

    i = f_expl_temp.colidx->size[0];
    f_expl_temp.colidx->size[0] = h_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.colidx, i);
    ntilerows = h_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.colidx->data[i] = h_expl_temp.colidx->data[i];
    }

    i = f_expl_temp.rowidx->size[0];
    f_expl_temp.rowidx->size[0] = h_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.rowidx, i);
    ntilerows = h_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.rowidx->data[i] = h_expl_temp.rowidx->data[i];
    }

    printf("trend 5\r\n");
    emxInit_int32_T(&t7_rowidx, 1);
    emxInit_int32_T(&t8_colidx, 1);
    emxInit_int32_T(&t8_rowidx, 1);
    emxInitStruct_sparse1(&i_expl_temp);
    emxInitStruct_sparse1(&j_expl_temp);
    c_sparse_mtimes(f_expl_temp.d, f_expl_temp.colidx, f_expl_temp.rowidx,
                    c_expl_temp.d, c_expl_temp.colidx, c_expl_temp.rowidx,
                    c_expl_temp.n, f, t7_colidx, t7_rowidx, &ibmat);
    sparse_horzcat(g_expl_temp.d, g_expl_temp.colidx, g_expl_temp.rowidx, b_dv, f,
                   t7_colidx, t7_rowidx, ibmat, b_spl, t8_colidx, t8_rowidx,
                   &ncols);
    b_sparse_ctranspose(c_expl_temp.d, c_expl_temp.colidx, c_expl_temp.rowidx,
                        c_expl_temp.m, c_expl_temp.n, &i_expl_temp);
    d_sparse_mtimes(i_expl_temp.d, i_expl_temp.colidx, i_expl_temp.rowidx,
                    i_expl_temp.m, b_expl_temp.d, b_expl_temp.colidx,
                    b_expl_temp.rowidx, f, t7_colidx, t7_rowidx, &ibmat);
    b_sparse_ctranspose(c_expl_temp.d, c_expl_temp.colidx, c_expl_temp.rowidx,
                        c_expl_temp.m, c_expl_temp.n, &j_expl_temp);
    i = i_expl_temp.d->size[0];
    i_expl_temp.d->size[0] = j_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(i_expl_temp.d, i);
    ntilerows = j_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.d->data[i] = j_expl_temp.d->data[i];
    }

    printf("trend 6\r\n");
    i = i_expl_temp.colidx->size[0];
    i_expl_temp.colidx->size[0] = j_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.colidx, i);
    ntilerows = j_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.colidx->data[i] = j_expl_temp.colidx->data[i];
    }

    i = i_expl_temp.rowidx->size[0];
    i_expl_temp.rowidx->size[0] = j_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.rowidx, i);
    ntilerows = j_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.rowidx->data[i] = j_expl_temp.rowidx->data[i];
    }

    printf("trend 7\r\n");
    emxInit_real_T(&r2, 2);
    e_sparse_mtimes(i_expl_temp.d, i_expl_temp.colidx, i_expl_temp.rowidx,
                    j_expl_temp.m, j_expl_temp.n, r2);
    b_sparse_ctranspose(c_expl_temp.d, c_expl_temp.colidx, c_expl_temp.rowidx,
                        c_expl_temp.m, c_expl_temp.n, &j_expl_temp);
    i = i_expl_temp.d->size[0];
    i_expl_temp.d->size[0] = j_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(i_expl_temp.d, i);
    ntilerows = j_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.d->data[i] = j_expl_temp.d->data[i];
    }

    i = i_expl_temp.colidx->size[0];
    i_expl_temp.colidx->size[0] = j_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.colidx, i);
    ntilerows = j_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.colidx->data[i] = j_expl_temp.colidx->data[i];
    }

    i = i_expl_temp.rowidx->size[0];
    i_expl_temp.rowidx->size[0] = j_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.rowidx, i);
    ntilerows = j_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.rowidx->data[i] = j_expl_temp.rowidx->data[i];
    }

    printf("trend 8\r\n");
    emxInitStruct_sparse1(&k_expl_temp);
    f_sparse_mtimes(i_expl_temp.d, i_expl_temp.colidx, i_expl_temp.rowidx,
                    j_expl_temp.m, c_expl_temp.d, c_expl_temp.colidx,
                    c_expl_temp.rowidx, c_expl_temp.n, &j_expl_temp);
    sparse_plus(j_expl_temp.d, j_expl_temp.colidx, j_expl_temp.rowidx,
                e_expl_temp.d, e_expl_temp.colidx, e_expl_temp.rowidx,
                e_expl_temp.m, e_expl_temp.n, &k_expl_temp);
    i = i_expl_temp.d->size[0];
    i_expl_temp.d->size[0] = k_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(i_expl_temp.d, i);
    ntilerows = k_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.d->data[i] = k_expl_temp.d->data[i];
    }

    i = i_expl_temp.colidx->size[0];
    i_expl_temp.colidx->size[0] = k_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.colidx, i);
    ntilerows = k_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.colidx->data[i] = k_expl_temp.colidx->data[i];
    }

    i = i_expl_temp.rowidx->size[0];
    i_expl_temp.rowidx->size[0] = k_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.rowidx, i);
    ntilerows = k_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.rowidx->data[i] = k_expl_temp.rowidx->data[i];
    }

    b_sparse_horzcat(f, t7_colidx, t7_rowidx, ibmat, r2, i_expl_temp.d,
                     i_expl_temp.colidx, i_expl_temp.rowidx, k_expl_temp.m,
                     k_expl_temp.n, &e_expl_temp);
    i = j_expl_temp.d->size[0];
    j_expl_temp.d->size[0] = e_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(j_expl_temp.d, i);
    ntilerows = e_expl_temp.d->size[0];
    emxFreeStruct_sparse1(&k_expl_temp);
    emxFree_real_T(&r2);
    for (i = 0; i < ntilerows; i++) {
        j_expl_temp.d->data[i] = e_expl_temp.d->data[i];
    }

    printf("trend 9\r\n");

    i = j_expl_temp.colidx->size[0];
    j_expl_temp.colidx->size[0] = e_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(j_expl_temp.colidx, i);
    ntilerows = e_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        j_expl_temp.colidx->data[i] = e_expl_temp.colidx->data[i];
    }

    i = j_expl_temp.rowidx->size[0];
    j_expl_temp.rowidx->size[0] = e_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(j_expl_temp.rowidx, i);
    ntilerows = e_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        j_expl_temp.rowidx->data[i] = e_expl_temp.rowidx->data[i];
    }

    emxInit_real_T(&c_c, 2);
    i = c_c->size[0] * c_c->size[1];
    c_c->size[0] = 2;
    c_c->size[1] = b_c->size[1] + 122;
    emxEnsureCapacity_real_T(c_c, i);
    for (i = 0; i < 120; i++) {
        ntilerows = i << 1;
        c_c->data[2 * i] = c[ntilerows];
        c_c->data[2 * i + 1] = c[ntilerows + 1];
    }

    printf("trend 10\r\n");

    c_c->data[240] = 120.0f;
    c_c->data[241] = 60.5f;
    c_c->data[242] = 60.5f;
    c_c->data[243] = 40.50138888888889f;
    ntilerows = b_c->size[1];
    for (i = 0; i < ntilerows; i++) {
        i1 = 2 * (i + 122);
        c_c->data[i1] = b_c->data[2 * i];
        c_c->data[i1 + 1] = b_c->data[2 * i + 1];
    }

    emxFree_real_T(&b_c);
    sparse_vertcat(b_spl, t8_colidx, t8_rowidx, ncols, c_c, j_expl_temp.d,
                   j_expl_temp.colidx, j_expl_temp.rowidx, e_expl_temp.m,
                   e_expl_temp.n, &e_expl_temp);
    i = d_expl_temp.colidx->size[0];
    d_expl_temp.colidx->size[0] = e_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(d_expl_temp.colidx, i);
    ntilerows = e_expl_temp.colidx->size[0];
    emxFree_real_T(&c_c);
    for (i = 0; i < ntilerows; i++) {
        d_expl_temp.colidx->data[i] = e_expl_temp.colidx->data[i];
    }

    printf("trend 11\r\n");

    i = d_expl_temp.rowidx->size[0];
    d_expl_temp.rowidx->size[0] = e_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(d_expl_temp.rowidx, i);
    ntilerows = e_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        d_expl_temp.rowidx->data[i] = e_expl_temp.rowidx->data[i];
    }

    emxInitStruct_sparse(&l_expl_temp);
    ncols = e_expl_temp.n;
    sum(expl_temp.d, expl_temp.colidx, f_expl_temp.d, f_expl_temp.colidx,
        f_expl_temp.rowidx);
    c_sparse_ctranspose(f_expl_temp.d, f_expl_temp.colidx, f_expl_temp.rowidx,
                        g_expl_temp.d, g_expl_temp.colidx, g_expl_temp.rowidx);
    b_sparse_times(g_expl_temp.d, g_expl_temp.colidx, g_expl_temp.rowidx,
                   &h_expl_temp);
    sparse_ctranspose(b_expl_temp.d, b_expl_temp.colidx, b_expl_temp.rowidx,
                      &l_expl_temp);
    i = f_expl_temp.d->size[0];
    f_expl_temp.d->size[0] = l_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(f_expl_temp.d, i);
    ntilerows = l_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.d->data[i] = l_expl_temp.d->data[i];
    }

    printf("trend 12\r\n");

    i = f_expl_temp.colidx->size[0];
    f_expl_temp.colidx->size[0] = l_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.colidx, i);
    ntilerows = l_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.colidx->data[i] = l_expl_temp.colidx->data[i];
    }

    i = f_expl_temp.rowidx->size[0];
    f_expl_temp.rowidx->size[0] = l_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.rowidx, i);
    ntilerows = l_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.rowidx->data[i] = l_expl_temp.rowidx->data[i];
    }
    printf("trend 13\r\n");

    g_sparse_mtimes(f_expl_temp.d, f_expl_temp.colidx, f_expl_temp.rowidx, y, b_b);
    bpend = h_expl_temp.colidx->data[0];
    for (ibmat = 0; ibmat < 120; ibmat++) {
        if ((bpend < h_expl_temp.colidx->data[1]) && (ibmat + 1 ==
                                                      h_expl_temp.rowidx->data[bpend - 1])) {
            S[ibmat] = h_expl_temp.d->data[bpend - 1] - b_b[ibmat];
            bpend++;
        } else {
            S[ibmat] = 0.0 - b_b[ibmat];
        }
    }

    b_sparse_ctranspose(c_expl_temp.d, c_expl_temp.colidx, c_expl_temp.rowidx,
                        c_expl_temp.m, c_expl_temp.n, &j_expl_temp);
    i = i_expl_temp.d->size[0];
    i_expl_temp.d->size[0] = j_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(i_expl_temp.d, i);
    ntilerows = j_expl_temp.d->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.d->data[i] = j_expl_temp.d->data[i];
    }

    printf("trend 14\r\n");
    i = i_expl_temp.colidx->size[0];
    i_expl_temp.colidx->size[0] = j_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.colidx, i);
    ntilerows = j_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.colidx->data[i] = j_expl_temp.colidx->data[i];
    }

    i = i_expl_temp.rowidx->size[0];
    i_expl_temp.rowidx->size[0] = j_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(i_expl_temp.rowidx, i);
    ntilerows = j_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        i_expl_temp.rowidx->data[i] = j_expl_temp.rowidx->data[i];
    }

    h_sparse_mtimes(i_expl_temp.d, i_expl_temp.colidx, i_expl_temp.rowidx,
                    j_expl_temp.m, j_expl_temp.n, y, spl);
    emxFreeStruct_sparse1(&j_expl_temp);
    emxFreeStruct_sparse1(&i_expl_temp);
    for (i = 0; i < 2; i++) {
        maxval = 0.0f;
        for (i1 = 0; i1 < 120; i1++) {
            maxval += a[i + (i1 << 1)] * y[i1];
        }

        b_a[i] = -maxval;
    }

    printf("trend 15\r\n");
    i = f->size[0];
    f->size[0] = spl->size[0] + 122;
    emxEnsureCapacity_real_T(f, i);
    for (i = 0; i < 120; i++) {
        f->data[i] = S[i];
    }

    f->data[120] = b_a[0];
    f->data[121] = b_a[1];
    ntilerows = spl->size[0];
    for (i = 0; i < ntilerows; i++) {
        f->data[i + 122] = -spl->data[i];
    }

    i = j->size[0] * j->size[1];
    j->size[0] = e_expl_temp.m;
    j->size[1] = e_expl_temp.n;
    emxEnsureCapacity_real_T(j, i);
    ntilerows = e_expl_temp.m * e_expl_temp.n;
    for (i = 0; i < ntilerows; i++) {
        j->data[i] = 0.0f;
    }

    for (ntilerows = 0; ntilerows < ncols; ntilerows++) {
        ibmat = d_expl_temp.colidx->data[ntilerows + 1] - 1;
        i = d_expl_temp.colidx->data[ntilerows];
        for (bpend = i; bpend <= ibmat; bpend++) {
            j->data[(d_expl_temp.rowidx->data[bpend - 1] + j->size[0] * ntilerows) - 1] = e_expl_temp.d->data[bpend - 1];
        }
    }

    emxFreeStruct_sparse1(&d_expl_temp);
    memset(&A_[0], 0, 14400U * sizeof(float));
    for (ntilerows = 0; ntilerows < 120; ntilerows++) {
        ibmat = expl_temp.colidx->data[ntilerows + 1] - 1;
        i = expl_temp.colidx->data[ntilerows];
        for (bpend = i; bpend <= ibmat; bpend++) {
            A_[(expl_temp.rowidx->data[bpend - 1] + 120 * ntilerows) - 1] =
                expl_temp.d->data[bpend - 1];
        }
    }

    printf("trend 16\r\n");
    if ((e_expl_temp.m == 0) || (e_expl_temp.n == 0)) {
        ncols = 0;
    } else if (e_expl_temp.m >= e_expl_temp.n) {
        ncols = e_expl_temp.m;
    } else {
        ncols = e_expl_temp.n;
    }

    emxFreeStruct_sparse1(&e_expl_temp);
    emxInit_real_T(&b_A_, 2);
    i = b_A_->size[0] * b_A_->size[1];
    b_A_->size[0] = 120;
    b_A_->size[1] = f->size[0];
    emxEnsureCapacity_real_T(b_A_, i);
    for (i = 0; i < 120; i++) {
        for (i1 = 0; i1 < 120; i1++) {
            ibmat = i1 + 120 * i;
            b_A_->data[ibmat] = -A_[ibmat];
        }
    }

    ntilerows = f->size[0] - 120;
    for (i = 0; i < ntilerows; i++) {
        for (i1 = 0; i1 < 120; i1++) {
            b_A_->data[i1 + 120 * (i + 120)] = 0.0f;
        }
    }

    i = b_spl->size[0];
    b_spl->size[0] = ncols;
    emxEnsureCapacity_real_T(b_spl, i);
    for (i = 0; i < ncols; i++) {
        b_spl->data[i] = 0.0f;
    }

    printf("trend 17\r\n");
    quadprog(j, f, b_A_, b_spl, spl, obj);
    printf("quadprog done\r\n");
    d_sparse(*obj, g_expl_temp.d, g_expl_temp.colidx, g_expl_temp.rowidx);
    e_sparse(spl, b_spl, t7_colidx, t7_rowidx, &itilerow, &ibmat);

    /* z = qp([], H, f, [], [], [], [], zeros(n,1), [A zeros(n,length(f)-n)], []);  */
    emxFree_real_T(&b_A_);
    emxFree_real_T(&j);
    emxFree_real_T(&spl);
    if (g_expl_temp.colidx->data[g_expl_temp.colidx->size[0] - 1] - 1 > 0) {
        uniOp_tunableEnvironment_idx_0 = g_expl_temp.d->data[0];
    } else {
        uniOp_tunableEnvironment_idx_0 = 0.0f;
    }

    d_y = 0.0f;
    for (i = 0; i < 120; i++) {
        maxval = y[i];
        d_y += maxval * maxval;
    }

    printf("trend 18\r\n");
    delta_knot_s = ((float)itilerow - (float)b_i->size[1]) + 1.0f;
    emxFree_real_T(&b_i);
    if (itilerow < delta_knot_s) {
        b_y->size[0] = 1;
        b_y->size[1] = 0;
    } else if (delta_knot_s == delta_knot_s) {
        i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        ntilerows = (int)((float)itilerow - delta_knot_s);
        b_y->size[1] = ntilerows + 1;
        emxEnsureCapacity_real_T(b_y, i);
        for (i = 0; i <= ntilerows; i++) {
            b_y->data[i] = delta_knot_s + (float)i;
        }
    } else {
        c_eml_float_colon(delta_knot_s, itilerow, b_y);
    }

    sparse_parenReference(b_spl, t7_colidx, t7_rowidx, itilerow, b_y, l->d,
                          l->colidx, l->rowidx, &l->m, &l->maxnz);
    b_sparse_parenReference(b_spl, t7_colidx, t7_rowidx, itilerow, d->d, d->colidx,
                            d->rowidx, &d->maxnz);
    emxFree_real_T(&b_y);
    memset(&S[0], 0, 120U * sizeof(float));
    if (d->colidx->data[d->colidx->size[0] - 1] - 1 != 0) {
        bpend = d->colidx->data[1] - 1;
        i = d->colidx->data[0];
        for (bp = i; bp <= bpend; bp++) {
            ibmat = (d->rowidx->data[bp - 1] - 1) * 120;
            delta_knot_s = d->d->data[bp - 1];
            for (ntilerows = 0; ntilerows <= 119; ntilerows += 4) {
                ncols = ntilerows + ibmat;
                S[ntilerows] += dv[ncols] * delta_knot_s;
                S[ntilerows + 1] += dv[ncols + 1] * delta_knot_s;
                S[ntilerows + 2] += dv[ncols + 2] * delta_knot_s;
                S[ntilerows + 3] += dv[ncols + 3] * delta_knot_s;
            }
        }
    }

    printf("trend 19\r\n");

    i_sparse_mtimes(c_expl_temp.d, c_expl_temp.colidx, c_expl_temp.rowidx,
                    c_expl_temp.m, l->d, l->colidx, l->rowidx, f, t8_colidx,
                    t8_rowidx, &ibmat, &ncols);
    bpend = t8_colidx->data[0];
    emxFreeStruct_sparse1(&c_expl_temp);
    for (ibmat = 0; ibmat < 120; ibmat++) {
        if ((bpend < t8_colidx->data[1]) && (ibmat + 1 == t8_rowidx->data[bpend - 1])) {
            t[ibmat] = f->data[bpend - 1] + S[ibmat];
            bpend++;
        } else {
            t[ibmat] = S[ibmat];
        }
    }

    emxFree_int32_T(&t8_rowidx);
    emxFree_int32_T(&t8_colidx);
    emxFree_real_T(&f);
    c_sparse_parenReference(b_spl, t7_colidx, t7_rowidx, itilerow, h_expl_temp.d,
                            h_expl_temp.colidx, h_expl_temp.rowidx);
    j_sparse_mtimes(expl_temp.d, expl_temp.colidx, expl_temp.rowidx, h_expl_temp.d,
                    h_expl_temp.colidx, h_expl_temp.rowidx, &l_expl_temp);
    i = f_expl_temp.d->size[0];
    f_expl_temp.d->size[0] = l_expl_temp.d->size[0];
    emxEnsureCapacity_real_T(f_expl_temp.d, i);
    ntilerows = l_expl_temp.d->size[0];
    emxFree_real_T(&b_spl);
    emxFree_int32_T(&t7_rowidx);
    emxFree_int32_T(&t7_colidx);
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.d->data[i] = l_expl_temp.d->data[i];
    }

    printf("trend 20\r\n");
    i = f_expl_temp.colidx->size[0];
    f_expl_temp.colidx->size[0] = l_expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.colidx, i);
    ntilerows = l_expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.colidx->data[i] = l_expl_temp.colidx->data[i];
    }

    i = f_expl_temp.rowidx->size[0];
    f_expl_temp.rowidx->size[0] = l_expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(f_expl_temp.rowidx, i);
    ntilerows = l_expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        f_expl_temp.rowidx->data[i] = l_expl_temp.rowidx->data[i];
    }

    emxFreeStruct_sparse(&l_expl_temp);
    j_sparse_mtimes(b_expl_temp.d, b_expl_temp.colidx, b_expl_temp.rowidx,
                    h_expl_temp.d, h_expl_temp.colidx, h_expl_temp.rowidx,
                    &expl_temp);
    i = g_expl_temp.d->size[0];
    g_expl_temp.d->size[0] = expl_temp.d->size[0];
    emxEnsureCapacity_real_T(g_expl_temp.d, i);
    ntilerows = expl_temp.d->size[0];
    emxFreeStruct_sparse(&h_expl_temp);
    emxFreeStruct_sparse(&b_expl_temp);
    for (i = 0; i < ntilerows; i++) {
        g_expl_temp.d->data[i] = expl_temp.d->data[i];
    }

    i = g_expl_temp.colidx->size[0];
    g_expl_temp.colidx->size[0] = expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(g_expl_temp.colidx, i);
    ntilerows = expl_temp.colidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        g_expl_temp.colidx->data[i] = expl_temp.colidx->data[i];
    }

    i = g_expl_temp.rowidx->size[0];
    g_expl_temp.rowidx->size[0] = expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(g_expl_temp.rowidx, i);
    ntilerows = expl_temp.rowidx->size[0];
    for (i = 0; i < ntilerows; i++) {
        g_expl_temp.rowidx->data[i] = expl_temp.rowidx->data[i];
    }

    emxFreeStruct_sparse(&expl_temp);
    bpend = g_expl_temp.colidx->data[0];
    for (ibmat = 0; ibmat < 120; ibmat++) {
        if ((bpend < g_expl_temp.colidx->data[1]) && (ibmat + 1 ==
                                                      g_expl_temp.rowidx->data[bpend - 1])) {
            maxval = y[ibmat] - g_expl_temp.d->data[bpend - 1];
            bpend++;
        } else {
            maxval = y[ibmat];
        }

        maxval -= t[ibmat];
        e[ibmat] = maxval;
        r[ibmat] = 0.0f;
    }

    ibmat = g_expl_temp.colidx->data[1] - 1;
    i = g_expl_temp.colidx->data[0];
    for (bpend = i; bpend <= ibmat; bpend++) {
        r[g_expl_temp.rowidx->data[bpend - 1] - 1] = g_expl_temp.d->data[bpend - 1];
    }

    printf("trend 21\r\n");
    emxFreeStruct_sparse(&g_expl_temp);
    memset(&p[0], 0, 120U * sizeof(float));
    ibmat = f_expl_temp.colidx->data[1] - 1;
    i = f_expl_temp.colidx->data[0];
    for (bpend = i; bpend <= ibmat; bpend++) {
        p[f_expl_temp.rowidx->data[bpend - 1] - 1] = f_expl_temp.d->data[bpend - 1];
    }

    emxFreeStruct_sparse(&f_expl_temp);
    *obj = uniOp_tunableEnvironment_idx_0 + 0.5f * d_y;
    printf("END cvxEDA\r\n");
}

/* End of code generation (cvxEDA.c) */
