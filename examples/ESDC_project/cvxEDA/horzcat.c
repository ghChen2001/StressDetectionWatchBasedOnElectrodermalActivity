/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * horzcat.c
 *
 * Code generation for function 'horzcat'
 *
 */

/* Include files */
#include "horzcat.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "fillIn.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_sparse_horzcat(const emxArray_real_T *varargin_1_d, const
                      emxArray_int32_T *varargin_1_colidx, const
                      emxArray_int32_T *varargin_1_rowidx, int varargin_1_m,
                      const emxArray_real_T *varargin_2, const emxArray_real_T
                      *varargin_3_d, const emxArray_int32_T *varargin_3_colidx,
                      const emxArray_int32_T *varargin_3_rowidx, int
                      varargin_3_m, int varargin_3_n, i_sparse *c)
{
  float dk;
  int ccolidx;
  int cnvardim;
  int idx;
  int numalloc;
  int nzCount;
  boolean_T allEmpty;
  boolean_T b_isAcceptableEmpty_tmp;
  boolean_T foundSize;
  boolean_T isAcceptableEmpty_tmp;
  c->m = varargin_1_m;
  isAcceptableEmpty_tmp = (varargin_1_m == 0);
  foundSize = !isAcceptableEmpty_tmp;
  allEmpty = (varargin_2->size[0] == 0);
  if ((!allEmpty) && (!foundSize)) {
    foundSize = true;
    c->m = varargin_2->size[0];
  }

  if ((varargin_3_m == 0) || (varargin_3_n == 0)) {
    b_isAcceptableEmpty_tmp = true;
  } else {
    b_isAcceptableEmpty_tmp = false;
  }

  allEmpty = (isAcceptableEmpty_tmp && allEmpty && b_isAcceptableEmpty_tmp);
  if ((!b_isAcceptableEmpty_tmp) && (!foundSize)) {
    c->m = varargin_3_m;
  }

  numalloc = 0;
  cnvardim = 0;
  if (allEmpty || (!isAcceptableEmpty_tmp)) {
    numalloc = varargin_1_colidx->data[varargin_1_colidx->size[0] - 1] - 1;
    cnvardim = 120;
  }

  if (allEmpty || (varargin_2->size[0] != 0)) {
    nzCount = 0;
    idx = varargin_2->size[0] << 1;
    for (ccolidx = 0; ccolidx < idx; ccolidx++) {
      if (varargin_2->data[ccolidx] != 0.0f) {
        nzCount++;
      }
    }

    numalloc += nzCount;
    cnvardim += 2;
  }

  if (allEmpty || (!b_isAcceptableEmpty_tmp)) {
    numalloc = (numalloc + varargin_3_colidx->data[varargin_3_colidx->size[0] -
                1]) - 1;
    cnvardim += varargin_3_n;
  }

  c->n = cnvardim;
  if (numalloc < 1) {
    numalloc = 1;
  }

  idx = c->d->size[0];
  c->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c->d, idx);
  for (idx = 0; idx < numalloc; idx++) {
    c->d->data[idx] = 0.0f;
  }

  idx = c->colidx->size[0];
  c->colidx->size[0] = cnvardim + 1;
  emxEnsureCapacity_int32_T(c->colidx, idx);
  c->colidx->data[0] = 1;
  idx = c->rowidx->size[0];
  c->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c->rowidx, idx);
  for (idx = 0; idx < numalloc; idx++) {
    c->rowidx->data[idx] = 0;
  }

  for (numalloc = 0; numalloc < cnvardim; numalloc++) {
    c->colidx->data[numalloc + 1] = 1;
  }

  b_sparse_fillIn(c);
  nzCount = 1;
  ccolidx = 0;
  if (varargin_1_m != 0) {
    cnvardim = -1;
    numalloc = varargin_1_colidx->data[varargin_1_colidx->size[0] - 1];
    for (idx = 0; idx <= numalloc - 2; idx++) {
      cnvardim++;
      c->rowidx->data[cnvardim] = varargin_1_rowidx->data[idx];
      c->d->data[cnvardim] = varargin_1_d->data[idx];
    }

    for (numalloc = 0; numalloc < 120; numalloc++) {
      ccolidx++;
      c->colidx->data[ccolidx] = varargin_1_colidx->data[numalloc + 1];
    }

    nzCount = varargin_1_colidx->data[varargin_1_colidx->size[0] - 1];
  }

  if (varargin_2->size[0] != 0) {
    numalloc = varargin_2->size[0];
    cnvardim = nzCount;
    for (nzCount = 0; nzCount < numalloc; nzCount++) {
      dk = varargin_2->data[nzCount];
      if (dk != 0.0f) {
        c->rowidx->data[cnvardim - 1] = nzCount + 1;
        c->d->data[cnvardim - 1] = dk;
        cnvardim++;
      }
    }

    ccolidx++;
    c->colidx->data[ccolidx] = cnvardim;
    for (nzCount = 0; nzCount < numalloc; nzCount++) {
      dk = varargin_2->data[nzCount + varargin_2->size[0]];
      if (dk != 0.0f) {
        c->rowidx->data[cnvardim - 1] = nzCount + 1;
        c->d->data[cnvardim - 1] = dk;
        cnvardim++;
      }
    }

    ccolidx++;
    c->colidx->data[ccolidx] = cnvardim;
    nzCount = cnvardim;
  }

  if ((varargin_3_m != 0) && (varargin_3_n != 0)) {
    cnvardim = nzCount - 2;
    numalloc = varargin_3_colidx->data[varargin_3_colidx->size[0] - 1];
    for (idx = 0; idx <= numalloc - 2; idx++) {
      cnvardim++;
      c->rowidx->data[cnvardim] = varargin_3_rowidx->data[idx];
      c->d->data[cnvardim] = varargin_3_d->data[idx];
    }

    for (numalloc = 0; numalloc < varargin_3_n; numalloc++) {
      ccolidx++;
      c->colidx->data[ccolidx] = (varargin_3_colidx->data[numalloc + 1] +
        nzCount) - 1;
    }
  }
}

void sparse_horzcat(const emxArray_real_T *varargin_1_d, const emxArray_int32_T *
                    varargin_1_colidx, const emxArray_int32_T *varargin_1_rowidx,
                    const float varargin_2[240], const emxArray_real_T
                    *varargin_3_d, const emxArray_int32_T *varargin_3_colidx,
                    const emxArray_int32_T *varargin_3_rowidx, int varargin_3_n,
                    emxArray_real_T *c_d, emxArray_int32_T *c_colidx,
                    emxArray_int32_T *c_rowidx, int *c_n)
{
  float d;
  int ccolidx;
  int cidx;
  int cncols;
  int idx;
  int numalloc;
  int nzCount;
  numalloc = -1;
  for (nzCount = 0; nzCount < 240; nzCount++) {
    if (varargin_2[nzCount] != 0.0f) {
      numalloc++;
    }
  }

  numalloc += varargin_1_colidx->data[varargin_1_colidx->size[0] - 1];
  cncols = 121;
  if (varargin_3_n != 0) {
    numalloc = (numalloc + varargin_3_colidx->data[varargin_3_colidx->size[0] -
                1]) - 1;
    cncols = varargin_3_n + 121;
  }

  if (numalloc < 1) {
    numalloc = 1;
  }

  nzCount = c_d->size[0];
  c_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c_d, nzCount);
  for (nzCount = 0; nzCount < numalloc; nzCount++) {
    c_d->data[nzCount] = 0.0f;
  }

  nzCount = c_colidx->size[0];
  c_colidx->size[0] = cncols + 2;
  emxEnsureCapacity_int32_T(c_colidx, nzCount);
  c_colidx->data[0] = 1;
  nzCount = c_rowidx->size[0];
  c_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c_rowidx, nzCount);
  for (nzCount = 0; nzCount < numalloc; nzCount++) {
    c_rowidx->data[nzCount] = 0;
  }

  for (numalloc = 0; numalloc <= cncols; numalloc++) {
    c_colidx->data[numalloc + 1] = 1;
  }

  nzCount = c_colidx->size[0];
  for (numalloc = 0; numalloc <= nzCount - 2; numalloc++) {
    c_colidx->data[numalloc] = 1;
  }

  c_colidx->data[c_colidx->size[0] - 1] = 1;
  ccolidx = 0;
  cidx = -1;
  numalloc = varargin_1_colidx->data[varargin_1_colidx->size[0] - 1];
  for (idx = 0; idx <= numalloc - 2; idx++) {
    cidx++;
    c_rowidx->data[cidx] = varargin_1_rowidx->data[idx];
    c_d->data[cidx] = varargin_1_d->data[idx];
  }

  for (idx = 0; idx < 120; idx++) {
    ccolidx++;
    c_colidx->data[ccolidx] = varargin_1_colidx->data[idx + 1];
  }

  cidx = varargin_1_colidx->data[varargin_1_colidx->size[0] - 1] - 1;
  for (idx = 0; idx < 2; idx++) {
    for (numalloc = 0; numalloc < 120; numalloc++) {
      d = varargin_2[numalloc + 120 * idx];
      if (d != 0.0) {
        c_rowidx->data[cidx] = numalloc + 1;
        c_d->data[cidx] = d;
        cidx++;
      }
    }

    ccolidx++;
    c_colidx->data[ccolidx] = cidx + 1;
  }

  nzCount = cidx;
  if (varargin_3_n != 0) {
    cidx--;
    numalloc = varargin_3_colidx->data[varargin_3_colidx->size[0] - 1];
    for (idx = 0; idx <= numalloc - 2; idx++) {
      cidx++;
      c_rowidx->data[cidx] = varargin_3_rowidx->data[idx];
      c_d->data[cidx] = varargin_3_d->data[idx];
    }

    for (idx = 0; idx < varargin_3_n; idx++) {
      ccolidx++;
      c_colidx->data[ccolidx] = varargin_3_colidx->data[idx + 1] + nzCount;
    }
  }

  *c_n = cncols + 1;
}

/* End of code generation (horzcat.c) */
