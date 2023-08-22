/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * partialColLDL3_.c
 *
 * Code generation for function 'partialColLDL3_'
 *
 */

/* Include files */
#include "partialColLDL3_.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void partialColLDL3_(g_struct_T *obj, int LD_offset, int NColsRemain, float
                     REG_PRIMAL)
{
  float a;
  int LD_diagOffset;
  int LDimSizeP1;
  int b_k;
  int i;
  int i1;
  int i2;
  int i3;
  int ia;
  int ia0;
  int iac;
  int ic0;
  int ix;
  int iy;
  int j;
  int k;
  int lastColC;
  int m;
  int offsetColK;
  int subBlockSize;
  int subRows;
  LDimSizeP1 = obj->ldm + 1;
  for (k = 0; k < 48; k++) {
    subRows = (NColsRemain - k) - 1;
    lastColC = LDimSizeP1 * k;
    LD_diagOffset = (LD_offset + lastColC) - 1;
    for (b_k = 0; b_k <= subRows; b_k++) {
      obj->workspace_->data[lastColC + b_k] = obj->FMat->data[LD_diagOffset +
        b_k];
    }

    offsetColK = obj->ldm * k;
    for (b_k = 0; b_k < NColsRemain; b_k++) {
      obj->workspace2_->data[b_k] = obj->workspace_->data[offsetColK + b_k];
    }

    b_k = obj->ldm;
    if ((NColsRemain != 0) && (k != 0)) {
      ix = LD_offset + k;
      i = obj->ldm * (k - 1) + 1;
      for (iac = 1; b_k < 0 ? iac >= i : iac <= i; iac += b_k) {
        iy = 0;
        i1 = (iac + NColsRemain) - 1;
        for (ia = iac; ia <= i1; ia++) {
          obj->workspace2_->data[iy] += obj->workspace_->data[ia - 1] *
            -obj->FMat->data[ix - 1];
          iy++;
        }

        ix += obj->ldm;
      }
    }

    for (b_k = 0; b_k < NColsRemain; b_k++) {
      obj->workspace_->data[offsetColK + b_k] = obj->workspace2_->data[b_k];
    }

    for (b_k = 0; b_k <= subRows; b_k++) {
      obj->FMat->data[LD_diagOffset + b_k] = obj->workspace_->data[lastColC +
        b_k];
    }

    if (fabsf(obj->FMat->data[LD_diagOffset]) <= obj->regTol_) {
      obj->FMat->data[LD_diagOffset] += REG_PRIMAL;
    }

    a = 1.0 / obj->FMat->data[LD_diagOffset];
    ix = LD_diagOffset + 2;
    i = (LD_diagOffset + subRows) + 1;
    for (b_k = ix; b_k <= i; b_k++) {
      obj->FMat->data[b_k - 1] *= a;
    }
  }

  i = NColsRemain - 1;
  for (j = 48; j <= i; j += 48) {
    offsetColK = NColsRemain - j;
    if (48 < offsetColK) {
      subBlockSize = 48;
    } else {
      subBlockSize = offsetColK;
    }

    ia0 = j + subBlockSize;
    i1 = ia0 - 1;
    for (k = j; k <= i1; k++) {
      m = ia0 - k;
      subRows = (LD_offset + LDimSizeP1 * k) - 1;
      ix = LD_offset + k;
      for (b_k = 0; b_k < 48; b_k++) {
        obj->workspace2_->data[b_k] = obj->FMat->data[(ix + b_k * obj->ldm) - 1];
      }

      lastColC = k + 1;
      b_k = obj->ldm;
      if (m != 0) {
        ix = 0;
        i2 = (k + obj->ldm * 47) + 1;
        for (iac = lastColC; b_k < 0 ? iac >= i2 : iac <= i2; iac += b_k) {
          iy = subRows;
          i3 = (iac + m) - 1;
          for (ia = iac; ia <= i3; ia++) {
            obj->FMat->data[iy] += obj->workspace_->data[ia - 1] *
              -obj->workspace2_->data[ix];
            iy++;
          }

          ix++;
        }
      }
    }

    if (ia0 < NColsRemain) {
      m = offsetColK - subBlockSize;
      ic0 = ((LD_offset + subBlockSize) + LDimSizeP1 * j) - 1;
      for (offsetColK = 0; offsetColK < 48; offsetColK++) {
        lastColC = offsetColK * obj->ldm;
        ix = (LD_offset + j) + lastColC;
        for (k = 0; k < subBlockSize; k++) {
          obj->workspace2_->data[lastColC + k] = obj->FMat->data[(ix + k) - 1];
        }
      }

      LD_diagOffset = obj->ldm;
      b_k = obj->ldm;
      if ((m != 0) && (subBlockSize != 0)) {
        lastColC = ic0 + obj->ldm * (subBlockSize - 1);
        offsetColK = 0;
        for (ix = ic0; b_k < 0 ? ix >= lastColC : ix <= lastColC; ix += b_k) {
          subRows = ia0 - 1;
          offsetColK++;
          i1 = offsetColK + LD_diagOffset * 47;
          for (iy = offsetColK; LD_diagOffset < 0 ? iy >= i1 : iy <= i1; iy +=
               LD_diagOffset) {
            ia = subRows;
            i2 = ix + 1;
            i3 = ix + m;
            for (iac = i2; iac <= i3; iac++) {
              ia++;
              obj->FMat->data[iac - 1] += -obj->workspace2_->data[iy - 1] *
                obj->workspace_->data[ia];
            }

            subRows += obj->ldm;
          }
        }
      }
    }
  }
}

/* End of code generation (partialColLDL3_.c) */
