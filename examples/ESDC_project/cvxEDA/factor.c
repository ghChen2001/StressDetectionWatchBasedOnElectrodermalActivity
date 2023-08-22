/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * factor.c
 *
 * Code generation for function 'factor'
 *
 */

/* Include files */
#include "factor.h"
#include "cvxEDA_types.h"
#include "fullColLDL2_.h"
#include "partialColLDL3_.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void factor(g_struct_T *obj, const emxArray_real_T *A, int ndims, int ldA)
{
  float SCALED_REG_PRIMAL;
  float s;
  float smax;
  int A_maxDiag_idx;
  int LDimSizeP1;
  int exitg2;
  int idx;
  int ix;
  int k;
  boolean_T exitg1;
  SCALED_REG_PRIMAL = 1.4901161193847656E-6f * (float)ndims;
  LDimSizeP1 = obj->ldm + 1;
  obj->ndims = ndims;
  if (A->size[1] != 0) {
    for (idx = 0; idx < ndims; idx++) {
      A_maxDiag_idx = ldA * idx;
      ix = obj->ldm * idx;
      for (k = 0; k < ndims; k++) {
        obj->FMat->data[ix + k] = A->data[A_maxDiag_idx + k];
      }
    }
  }

  if (ndims < 1) {
    A_maxDiag_idx = 0;
  } else {
    A_maxDiag_idx = 1;
    if (ndims > 1) {
      ix = 0;
      smax = fabsf(obj->FMat->data[0]);
      for (k = 2; k <= ndims; k++) {
        ix += LDimSizeP1;
        s = fabsf(obj->FMat->data[ix]);
        if (s > smax) {
          A_maxDiag_idx = k;
          smax = s;
        }
      }
    }
  }

  obj->regTol_ = fmaxf(fabsf(obj->FMat->data[A_maxDiag_idx * (obj->ldm + 1) - 1]) *
                      2.2204460492503131E-8f, fabsf(SCALED_REG_PRIMAL));
  if (ndims > 128) {
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < ndims)) {
      A_maxDiag_idx = LDimSizeP1 * k + 1;
      ix = ndims - k;
      if (k + 48 <= ndims) {
        partialColLDL3_(obj, A_maxDiag_idx, ix, SCALED_REG_PRIMAL);
        k += 48;
      } else {
        fullColLDL2_(obj, A_maxDiag_idx, ix, SCALED_REG_PRIMAL);
        exitg1 = true;
      }
    }
  } else {
    fullColLDL2_(obj, 1, ndims, SCALED_REG_PRIMAL);
  }

  if (obj->ConvexCheck) {
    idx = 0;
    do {
      exitg2 = 0;
      if (idx <= ndims - 1) {
        if (obj->FMat->data[idx + obj->FMat->size[0] * idx] <= 0.0) {
          obj->info = -idx - 1;
          exitg2 = 1;
        } else {
          idx++;
        }
      } else {
        obj->ConvexCheck = false;
        exitg2 = 1;
      }
    } while (exitg2 == 0);
  }
}

/* End of code generation (factor.c) */
