/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.c
 *
 * Code generation for function 'sum'
 *
 */

/* Include files */
#include "sum.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void sum(const emxArray_real_T *x_d, const emxArray_int32_T *x_colidx,
         emxArray_real_T *y_d, emxArray_int32_T *y_colidx, emxArray_int32_T
         *y_rowidx)
{
  float r;
  int col;
  int numalloc;
  int xend;
  int xp;
  int xstart;
  if (120 < x_colidx->data[x_colidx->size[0] - 1] - 1) {
    numalloc = 120;
  } else {
    numalloc = x_colidx->data[x_colidx->size[0] - 1] - 1;
  }

  if (numalloc < 1) {
    numalloc = 1;
  }

  xstart = y_d->size[0];
  y_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(y_d, xstart);
  for (xstart = 0; xstart < numalloc; xstart++) {
    y_d->data[xstart] = 0.0f;
  }

  xstart = y_colidx->size[0];
  y_colidx->size[0] = 121;
  emxEnsureCapacity_int32_T(y_colidx, xstart);
  xstart = y_rowidx->size[0];
  y_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(y_rowidx, xstart);
  for (xstart = 0; xstart < numalloc; xstart++) {
    y_rowidx->data[xstart] = 0;
  }

  for (numalloc = 0; numalloc < 120; numalloc++) {
    y_colidx->data[numalloc + 1] = 1;
    y_colidx->data[numalloc] = 1;
  }

  y_colidx->data[120] = 1;
  y_colidx->data[0] = 1;
  numalloc = 1;
  for (col = 0; col < 120; col++) {
    xstart = x_colidx->data[col];
    xend = x_colidx->data[col + 1] - 1;
    r = 0.0f;
    for (xp = xstart; xp <= xend; xp++) {
      r += x_d->data[xp - 1];
    }

    if (r != 0.0f) {
      y_d->data[numalloc - 1] = r;
      numalloc++;
    }

    y_colidx->data[col + 1] = numalloc;
  }

  xstart = y_colidx->data[120];
  for (numalloc = 0; numalloc <= xstart - 2; numalloc++) {
    y_rowidx->data[numalloc] = 1;
  }
}

/* End of code generation (sum.c) */
