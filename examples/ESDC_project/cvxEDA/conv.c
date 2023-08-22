/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * conv.c
 *
 * Code generation for function 'conv'
 *
 */

/* Include files */
#include "conv.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void conv(const emxArray_real_T *A, const emxArray_real_T *B, emxArray_real_T *C)
{
  int b_k;
  int k;
  int nA;
  int nApnB;
  int nB;
  nA = A->size[0] - 1;
  nB = B->size[0] - 1;
  nApnB = A->size[0] + B->size[0];
  if ((A->size[0] != 0) && (B->size[0] != 0)) {
    nApnB--;
  }

  k = C->size[0];
  C->size[0] = nApnB;
  emxEnsureCapacity_real_T(C, k);
  for (k = 0; k < nApnB; k++) {
    C->data[k] = 0.0f;
  }

  if ((A->size[0] > 0) && (B->size[0] > 0)) {
    if (B->size[0] > A->size[0]) {
      for (k = 0; k <= nA; k++) {
        for (b_k = 0; b_k <= nB; b_k++) {
          nApnB = k + b_k;
          C->data[nApnB] += A->data[k] * B->data[b_k];
        }
      }
    } else {
      for (k = 0; k <= nB; k++) {
        for (b_k = 0; b_k <= nA; b_k++) {
          nApnB = k + b_k;
          C->data[nApnB] += B->data[k] * A->data[b_k];
        }
      }
    }
  }
}

/* End of code generation (conv.c) */
