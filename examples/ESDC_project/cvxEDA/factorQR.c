/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * factorQR.c
 *
 * Code generation for function 'factorQR'
 *
 */

/* Include files */
#include "factorQR.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xzgeqp3.h"

/* Function Definitions */
void factorQR(f_struct_T *obj, const emxArray_real_T *A, int mrows, int ncols)
{
  int ix0;
  int iy0;
  int k;
  int minmana;
  boolean_T guard1 = false;
  iy0 = mrows * ncols;
  guard1 = false;
  if (iy0 > 0) {
    for (minmana = 0; minmana < ncols; minmana++) {
      ix0 = A->size[0] * minmana;
      iy0 = obj->ldq * minmana;
      for (k = 0; k < mrows; k++) {
        obj->QR->data[iy0 + k] = A->data[ix0 + k];
      }
    }

    guard1 = true;
  } else if (iy0 == 0) {
    obj->mrows = mrows;
    obj->ncols = ncols;
    obj->minRowCol = 0;
  } else {
    guard1 = true;
  }

  if (guard1) {
    obj->usedPivoting = false;
    obj->mrows = mrows;
    obj->ncols = ncols;
    for (minmana = 0; minmana < ncols; minmana++) {
      obj->jpvt->data[minmana] = minmana + 1;
    }

    if (mrows < ncols) {
      iy0 = mrows;
    } else {
      iy0 = ncols;
    }

    obj->minRowCol = iy0;
    ix0 = obj->QR->size[0];
    minmana = obj->QR->size[1];
    if (ix0 < minmana) {
      minmana = ix0;
    }

    ix0 = obj->tau->size[0];
    obj->tau->size[0] = minmana;
    emxEnsureCapacity_real_T(obj->tau, ix0);
    for (ix0 = 0; ix0 < minmana; ix0++) {
      obj->tau->data[ix0] = 0.0f;
    }

    if (iy0 >= 1) {
      qrf(obj->QR, mrows, ncols, iy0, obj->tau);
    }
  }
}

/* End of code generation (factorQR.c) */
