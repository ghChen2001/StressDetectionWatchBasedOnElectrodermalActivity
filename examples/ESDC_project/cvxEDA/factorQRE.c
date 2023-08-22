/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * factorQRE.c
 *
 * Code generation for function 'factorQRE'
 *
 */

/* Include files */
#include "factorQRE.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"

/* Function Definitions */
void factorQRE(f_struct_T *obj, const emxArray_real_T *A, int mrows, int ncols)
{
  int idx;
  int ix0;
  int iy0;
  int k;
  boolean_T guard1 = false;
  ix0 = mrows * ncols;
  guard1 = false;
  if (ix0 > 0) {
    for (idx = 0; idx < ncols; idx++) {
      ix0 = A->size[0] * idx;
      iy0 = obj->ldq * idx;
      for (k = 0; k < mrows; k++) {
        obj->QR->data[iy0 + k] = A->data[ix0 + k];
      }
    }

    guard1 = true;
  } else if (ix0 == 0) {
    obj->mrows = mrows;
    obj->ncols = ncols;
    obj->minRowCol = 0;
  } else {
    guard1 = true;
  }

  if (guard1) {
    obj->usedPivoting = true;
    obj->mrows = mrows;
    obj->ncols = ncols;
    if (mrows < ncols) {
      ix0 = mrows;
    } else {
      ix0 = ncols;
    }

    obj->minRowCol = ix0;
    xgeqp3(obj->QR, mrows, ncols, obj->jpvt, obj->tau);
  }
}

/* End of code generation (factorQRE.c) */
