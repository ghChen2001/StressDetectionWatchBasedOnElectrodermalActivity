/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * fullColLDL2_.c
 *
 * Code generation for function 'fullColLDL2_'
 *
 */

/* Include files */
#include "fullColLDL2_.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void fullColLDL2_(g_struct_T *obj, int LD_offset, int NColsRemain, float
                  REG_PRIMAL)
{
  float alpha1;
  float temp;
  int LD_diagOffset;
  int LDimSizeP1;
  int i;
  int i1;
  int ijA;
  int ix;
  int j;
  int jA;
  int jy;
  int k;
  int offset1;
  int subMatrixDim;
  LDimSizeP1 = obj->ldm;
  for (k = 0; k < NColsRemain; k++) {
    LD_diagOffset = (LD_offset + (LDimSizeP1 + 1) * k) - 1;
    if (fabsf(obj->FMat->data[LD_diagOffset]) <= obj->regTol_) {
      obj->FMat->data[LD_diagOffset] += REG_PRIMAL;
    }

    alpha1 = -1.0f / obj->FMat->data[LD_diagOffset];
    subMatrixDim = NColsRemain - k;
    offset1 = LD_diagOffset + 2;
    for (jA = 0; jA <= subMatrixDim - 2; jA++) {
      obj->workspace_->data[jA] = obj->FMat->data[(LD_diagOffset + jA) + 1];
    }

    if (!(alpha1 == 0.0f)) {
      jA = LD_diagOffset + LDimSizeP1;
      jy = 0;
      for (j = 0; j <= subMatrixDim - 2; j++) {
        if (obj->workspace_->data[jy] != 0.0f) {
          temp = obj->workspace_->data[jy] * alpha1;
          ix = 0;
          i = jA + 2;
          i1 = subMatrixDim + jA;
          for (ijA = i; ijA <= i1; ijA++) {
            obj->FMat->data[ijA - 1] += obj->workspace_->data[ix] * temp;
            ix++;
          }
        }

        jy++;
        jA += obj->ldm;
      }
    }

    alpha1 = 1.0f / obj->FMat->data[LD_diagOffset];
    i = LD_diagOffset + subMatrixDim;
    for (jA = offset1; jA <= i; jA++) {
      obj->FMat->data[jA - 1] *= alpha1;
    }
  }

  jA = (LD_offset + (obj->ldm + 1) * (NColsRemain - 1)) - 1;
  if (fabsf(obj->FMat->data[jA]) <= obj->regTol_) {
    obj->FMat->data[jA] += REG_PRIMAL;
  }
}

/* End of code generation (fullColLDL2_.c) */
