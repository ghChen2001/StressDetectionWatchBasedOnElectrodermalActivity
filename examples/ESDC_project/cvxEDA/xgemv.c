/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xgemv.c
 *
 * Code generation for function 'xgemv'
 *
 */

/* Include files */
#include "xgemv.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_xgemv(int m, const emxArray_real_T *A, int lda, const emxArray_real_T *x,
             emxArray_real_T *y)
{
  float c;
  int i;
  int i1;
  int ia;
  int iac;
  int ix;
  int iy;
  for (iy = 0; iy < 120; iy++) {
    y->data[iy] = -y->data[iy];
  }

  iy = 0;
  i = lda * 119 + 1;
  for (iac = 1; lda < 0 ? iac >= i : iac <= i; iac += lda) {
    ix = 0;
    c = 0.0f;
    i1 = (iac + m) - 1;
    for (ia = iac; ia <= i1; ia++) {
      c += A->data[ia - 1] * x->data[ix];
      ix++;
    }

    y->data[iy] += c;
    iy++;
  }
}

void xgemv(int m, const emxArray_real_T *A, int lda, const emxArray_real_T *x,
           float y[121])
{
  float c;
  int i;
  int i1;
  int ia;
  int iac;
  int ix;
  int iy;
  for (iy = 0; iy < 120; iy++) {
    y[iy] = -y[iy];
  }

  iy = 0;
  i = lda * 119 + 1;
  for (iac = 1; lda < 0 ? iac >= i : iac <= i; iac += lda) {
    ix = 0;
    c = 0.0f;
    i1 = (iac + m) - 1;
    for (ia = iac; ia <= i1; ia++) {
      c += A->data[ia - 1] * x->data[ix];
      ix++;
    }

    y[iy] += c;
    iy++;
  }
}

/* End of code generation (xgemv.c) */
