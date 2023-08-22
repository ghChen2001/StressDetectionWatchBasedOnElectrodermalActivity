/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * bsxfun.c
 *
 * Code generation for function 'bsxfun'
 *
 */

/* Include files */
#include "bsxfun.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void bsxfun(const emxArray_real_T *a, const emxArray_real_T *b, emxArray_real_T *
            c)
{
  int acoef;
  int b_k;
  int bcoef;
  int i;
  int i1;
  int k;
  int varargin_3;
  i = c->size[0] * c->size[1];
  c->size[0] = a->size[0];
  c->size[1] = b->size[1];
  emxEnsureCapacity_real_T(c, i);
  if ((a->size[0] != 0) && (b->size[1] != 0)) {
    bcoef = (b->size[1] != 1);
    i = b->size[1] - 1;
    for (k = 0; k <= i; k++) {
      varargin_3 = bcoef * k;
      acoef = (a->size[0] != 1);
      i1 = c->size[0] - 1;
      for (b_k = 0; b_k <= i1; b_k++) {
        c->data[b_k + c->size[0] * k] = a->data[acoef * b_k] + b->
          data[varargin_3];
      }
    }
  }
}

/* End of code generation (bsxfun.c) */
