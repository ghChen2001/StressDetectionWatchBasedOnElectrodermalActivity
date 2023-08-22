/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xgemm.c
 *
 * Code generation for function 'xgemm'
 *
 */

/* Include files */
#include "xgemm.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_xgemm(int m, int n, int k, const emxArray_real_T *A, int ia0, int lda,
             const emxArray_real_T *B, int ldb, emxArray_real_T *C, int ldc)
{
  float temp;
  int ar;
  int br;
  int cr;
  int i;
  int i1;
  int ic;
  int lastColC;
  int w;
  if ((m != 0) && (n != 0)) {
    lastColC = ldc * (n - 1);
    for (cr = 0; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      i = cr + 1;
      i1 = cr + m;
      for (ic = i; ic <= i1; ic++) {
        C->data[ic - 1] = 0.0f;
      }
    }

    br = -1;
    for (cr = 0; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      ar = ia0;
      i = cr + 1;
      i1 = cr + m;
      for (ic = i; ic <= i1; ic++) {
        temp = 0.0f;
        for (w = 0; w < k; w++) {
          temp += A->data[(w + ar) - 1] * B->data[(w + br) + 1];
        }

        C->data[ic - 1] += temp;
        ar += lda;
      }

      br += ldb;
    }
  }
}

void xgemm(int m, int n, int k, const emxArray_real_T *A, int lda, const
           emxArray_real_T *B, int ib0, int ldb, emxArray_real_T *C, int ldc)
{
  int ar;
  int br;
  int cr;
  int i;
  int i1;
  int i2;
  int ia;
  int ib;
  int ic;
  int lastColC;
  if ((m != 0) && (n != 0)) {
    br = ib0;
    lastColC = ldc * (n - 1);
    for (cr = 0; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      i = cr + 1;
      i1 = cr + m;
      for (ic = i; ic <= i1; ic++) {
        C->data[ic - 1] = 0.0f;
      }
    }

    for (cr = 0; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      ar = -1;
      i = br + k;
      for (ib = br; ib < i; ib++) {
        ia = ar;
        i1 = cr + 1;
        i2 = cr + m;
        for (ic = i1; ic <= i2; ic++) {
          ia++;
          C->data[ic - 1] += B->data[ib - 1] * A->data[ia];
        }

        ar += lda;
      }

      br += ldb;
    }
  }
}

/* End of code generation (xgemm.c) */
