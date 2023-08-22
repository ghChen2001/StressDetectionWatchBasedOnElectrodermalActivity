/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xnrm2.c
 *
 * Code generation for function 'xnrm2'
 *
 */

/* Include files */
#include "xnrm2.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
float b_xnrm2(int n, const emxArray_real_T *x)
{
  float absxk;
  float scale;
  float t;
  float y;
  int k;
  y = 0.0f;
  if (n >= 1) {
    if (n == 1) {
      y = fabsf(x->data[0]);
    } else {
      scale = 3.3121686421112381E-10f;
      for (k = 0; k < n; k++) {
        absxk = fabsf(x->data[k]);
        if (absxk > scale) {
          t = scale / absxk;
          y = y * t * t + 1.0f;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrtf(y);
    }
  }

  return y;
}

float xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  float absxk;
  float scale;
  float t;
  float y;
  int k;
  int kend;
  y = 0.0f;
  if (n >= 1) {
    if (n == 1) {
      y = fabsf(x->data[ix0 - 1]);
    } else {
      scale = 3.3121686421112381E-45f;
      kend = (ix0 + n) - 1;
      for (k = ix0; k <= kend; k++) {
        absxk = fabsf(x->data[k - 1]);
        if (absxk > scale) {
          t = scale / absxk;
          y = y * t * t + 1.0f;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrtf(y);
    }
  }

  return y;
}

/* End of code generation (xnrm2.c) */
