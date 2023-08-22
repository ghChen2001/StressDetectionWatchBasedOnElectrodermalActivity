/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * repmat.c
 *
 * Code generation for function 'repmat'
 *
 */

/* Include files */
#include "repmat.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void repmat(const emxArray_real_T *a, float varargin_2, emxArray_real_T *b)
{
  int ibtile;
  int jtilecol;
  int k;
  int nrows;
  int ntilecols;
  nrows = b->size[0] * b->size[1];
  b->size[0] = a->size[0];
  b->size[1] = (int)varargin_2;
  emxEnsureCapacity_real_T(b, nrows);
  nrows = a->size[0];
  ntilecols = (int)varargin_2;
  for (jtilecol = 0; jtilecol < ntilecols; jtilecol++) {
    ibtile = jtilecol * nrows;
    for (k = 0; k < nrows; k++) {
      b->data[ibtile + k] = a->data[k];
    }
  }
}

/* End of code generation (repmat.c) */
