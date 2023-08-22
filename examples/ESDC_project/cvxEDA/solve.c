/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * solve.c
 *
 * Code generation for function 'solve'
 *
 */

/* Include files */
#include "solve.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void solve(const g_struct_T *obj, emxArray_real_T *rhs)
{
  float temp;
  int b_i;
  int i;
  int ix;
  int j;
  int jjA;
  int n;
  n = obj->ndims - 2;
  if (obj->ndims != 0) {
    for (j = 0; j <= n + 1; j++) {
      jjA = j + j * obj->ldm;
      i = n - j;
      for (b_i = 0; b_i <= i; b_i++) {
        ix = (j + b_i) + 1;
        rhs->data[ix] -= rhs->data[j] * obj->FMat->data[(jjA + b_i) + 1];
      }
    }
  }

  i = obj->ndims;
  for (jjA = 0; jjA < i; jjA++) {
    rhs->data[jjA] /= obj->FMat->data[jjA + obj->FMat->size[0] * jjA];
  }

  n = obj->ndims;
  if (obj->ndims != 0) {
    for (j = n; j >= 1; j--) {
      jjA = (j - 1) * obj->ldm;
      temp = rhs->data[j - 1];
      i = j + 1;
      for (b_i = n; b_i >= i; b_i--) {
        temp -= obj->FMat->data[(jjA + b_i) - 1] * rhs->data[b_i - 1];
      }

      rhs->data[j - 1] = temp;
    }
  }
}

/* End of code generation (solve.c) */
