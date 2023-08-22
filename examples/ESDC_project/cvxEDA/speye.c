/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * speye.c
 *
 * Code generation for function 'speye'
 *
 */

/* Include files */
#include "speye.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "fillIn.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void speye(float m, i_sparse *y)
{
  int c;
  int i;
  int n;
  int ndiag;
  int numalloc;
  ndiag = (int)m;
  n = (int)m;
  y->m = (int)m;
  y->n = (int)m;
  if ((int)m >= 1) {
    numalloc = (int)m;
  } else {
    numalloc = 1;
  }

  i = y->d->size[0];
  y->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(y->d, i);
  for (i = 0; i < numalloc; i++) {
    y->d->data[i] = 0.0f;
  }

  i = y->colidx->size[0];
  y->colidx->size[0] = (int)m + 1;
  emxEnsureCapacity_int32_T(y->colidx, i);
  y->colidx->data[0] = 1;
  i = y->rowidx->size[0];
  y->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(y->rowidx, i);
  for (i = 0; i < numalloc; i++) {
    y->rowidx->data[i] = 0;
  }

  for (c = 0; c < n; c++) {
    y->colidx->data[c + 1] = 1;
  }

  b_sparse_fillIn(y);
  y->colidx->data[0] = 1;
  numalloc = y->d->size[0];
  for (i = 0; i < numalloc; i++) {
    y->d->data[i] = 1.0f;
  }

  for (c = 2; c <= ndiag; c++) {
    y->colidx->data[c - 1] = c;
  }

  i = (int)m + 1;
  numalloc = (int)m + 1;
  for (c = i; c <= numalloc; c++) {
    y->colidx->data[c - 1] = (int)m + 1;
  }

  i = y->colidx->data[y->colidx->size[0] - 1];
  for (numalloc = 0; numalloc <= i - 2; numalloc++) {
    y->rowidx->data[numalloc] = numalloc + 1;
  }
}

/* End of code generation (speye.c) */
