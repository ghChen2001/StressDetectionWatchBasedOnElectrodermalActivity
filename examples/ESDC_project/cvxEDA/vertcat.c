/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * vertcat.c
 *
 * Code generation for function 'vertcat'
 *
 */

/* Include files */
#include "vertcat.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "fillIn.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void sparse_vertcat(const emxArray_real_T *varargin_1_d, const emxArray_int32_T *
                    varargin_1_colidx, const emxArray_int32_T *varargin_1_rowidx,
                    int varargin_1_n, const emxArray_real_T *varargin_2, const
                    emxArray_real_T *varargin_3_d, const emxArray_int32_T
                    *varargin_3_colidx, const emxArray_int32_T
                    *varargin_3_rowidx, int varargin_3_m, int varargin_3_n,
                    i_sparse *c)
{
  float dk;
  int ccol;
  int cidx;
  int cnfixeddim;
  int cnvardim;
  int i;
  int k;
  int kp;
  int kpend;
  int numalloc;
  boolean_T emptyflag_idx_0;
  boolean_T emptyflag_idx_2;
  cnfixeddim = varargin_1_n;
  emptyflag_idx_0 = (varargin_1_n == 0);
  if (emptyflag_idx_0) {
    cnfixeddim = varargin_2->size[1];
  }

  if ((varargin_3_m == 0) || (varargin_3_n == 0)) {
    emptyflag_idx_2 = true;
  } else {
    emptyflag_idx_2 = false;
  }

  numalloc = 0;
  cnvardim = 2;
  if (!emptyflag_idx_0) {
    numalloc = varargin_1_colidx->data[varargin_1_colidx->size[0] - 1] - 1;
    cnvardim = 122;
  }

  cidx = 0;
  i = varargin_2->size[1] << 1;
  for (k = 0; k < i; k++) {
    if (varargin_2->data[k] != 0.0) {
      cidx++;
    }
  }

  numalloc += cidx;
  if (!emptyflag_idx_2) {
    numalloc = (numalloc + varargin_3_colidx->data[varargin_3_colidx->size[0] -
                1]) - 1;
    cnvardim += varargin_3_m;
  }

  c->m = cnvardim;
  c->n = cnfixeddim;
  if (numalloc < 1) {
    numalloc = 1;
  }

  i = c->d->size[0];
  c->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c->d, i);
  for (i = 0; i < numalloc; i++) {
    c->d->data[i] = 0.0;
  }

  i = c->colidx->size[0];
  c->colidx->size[0] = cnfixeddim + 1;
  emxEnsureCapacity_int32_T(c->colidx, i);
  c->colidx->data[0] = 1;
  i = c->rowidx->size[0];
  c->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c->rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c->rowidx->data[i] = 0;
  }

  for (numalloc = 0; numalloc < cnfixeddim; numalloc++) {
    c->colidx->data[numalloc + 1] = 1;
  }

  b_sparse_fillIn(c);
  numalloc = -1;
  emptyflag_idx_0 = (varargin_1_n == 0);
  if ((varargin_3_m == 0) || (varargin_3_n == 0)) {
    emptyflag_idx_2 = true;
  } else {
    emptyflag_idx_2 = false;
  }

  i = c->n;
  for (ccol = 0; ccol < i; ccol++) {
    cnvardim = 1;
    if (!emptyflag_idx_0) {
      cidx = numalloc;
      cnfixeddim = varargin_1_colidx->data[ccol];
      k = varargin_1_colidx->data[ccol + 1];
      kpend = k - 1;
      for (kp = cnfixeddim; kp <= kpend; kp++) {
        cidx++;
        c->rowidx->data[cidx] = varargin_1_rowidx->data[kp - 1];
        c->d->data[cidx] = varargin_1_d->data[kp - 1];
      }

      numalloc = (numalloc + k) - varargin_1_colidx->data[ccol];
      cnvardim = 121;
    }

    cidx = numalloc;
    dk = varargin_2->data[2 * ccol];
    if (dk != 0.0f) {
      cidx = numalloc + 1;
      c->rowidx->data[numalloc + 1] = cnvardim;
      c->d->data[numalloc + 1] = dk;
    }

    dk = varargin_2->data[2 * ccol + 1];
    if (dk != 0.0f) {
      cidx++;
      c->rowidx->data[cidx] = cnvardim + 1;
      c->d->data[cidx] = dk;
    }

    numalloc = cidx;
    if (!emptyflag_idx_2) {
      cnfixeddim = varargin_3_colidx->data[ccol];
      k = varargin_3_colidx->data[ccol + 1];
      kpend = k - 1;
      for (kp = cnfixeddim; kp <= kpend; kp++) {
        cidx++;
        c->rowidx->data[cidx] = (varargin_3_rowidx->data[kp - 1] + cnvardim) + 1;
        c->d->data[cidx] = varargin_3_d->data[kp - 1];
      }

      numalloc = (numalloc + k) - varargin_3_colidx->data[ccol];
    }

    c->colidx->data[ccol + 1] = numalloc + 2;
  }
}

/* End of code generation (vertcat.c) */
