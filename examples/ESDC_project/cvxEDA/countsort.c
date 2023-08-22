/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * countsort.c
 *
 * Code generation for function 'countsort'
 *
 */

/* Include files */
#include "countsort.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void countsort(emxArray_int32_T *x, int xLen, emxArray_int32_T *workspace, int
               xMin, int xMax)
{
  int idx;
  int idxEnd;
  int idxFill;
  int idxStart;
  int maxOffset;
  if ((xLen > 1) && (xMax > xMin)) {
    idxStart = xMax - xMin;
    for (idx = 0; idx <= idxStart; idx++) {
      workspace->data[idx] = 0;
    }

    maxOffset = idxStart - 1;
    for (idx = 0; idx < xLen; idx++) {
      idxStart = x->data[idx] - xMin;
      workspace->data[idxStart]++;
    }

    for (idx = 2; idx <= maxOffset + 2; idx++) {
      workspace->data[idx - 1] += workspace->data[idx - 2];
    }

    idxStart = 1;
    idxEnd = workspace->data[0];
    for (idx = 0; idx <= maxOffset; idx++) {
      for (idxFill = idxStart; idxFill <= idxEnd; idxFill++) {
        x->data[idxFill - 1] = idx + xMin;
      }

      idxStart = workspace->data[idx] + 1;
      idxEnd = workspace->data[idx + 1];
    }

    for (idx = idxStart; idx <= idxEnd; idx++) {
      x->data[idx - 1] = xMax;
    }
  }
}

/* End of code generation (countsort.c) */
