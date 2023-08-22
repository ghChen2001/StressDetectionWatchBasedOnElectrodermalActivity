/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xzlarfg.c
 *
 * Code generation for function 'xzlarfg'
 *
 */

/* Include files */
#include "xzlarfg.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static float rt_hypotd_snf(float u0, float u1);

/* Function Definitions */
static float rt_hypotd_snf(float u0, float u1)
{
  float a;
  float y;
  a = fabsf(u0);
  y = fabsf(u1);
  if (a < y) {
    a /= y;
    y *= sqrtf(a * a + 1.0f);
  } else if (a > y) {
    y /= a;
    y = a * sqrtf(y * y + 1.0f);
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951f;
    }
  }

  return y;
}

float xzlarfg(int n, float *alpha1, emxArray_real_T *x, int ix0)
{
  float beta1;
  float tau;
  float xnorm;
  int i;
  int k;
  int knt;
  tau = 0.0f;
  if (n > 0) {
    xnorm = xnrm2(n - 1, x, ix0);
    if (xnorm != 0.0f) {
      beta1 = rt_hypotd_snf(*alpha1, xnorm);
      if (*alpha1 >= 0.0f) {
        beta1 = -beta1;
      }

      if (fabsf(beta1) < 1.0020841800044864E-30f) {
        knt = -1;
        i = (ix0 + n) - 2;
        do {
          knt++;
          for (k = ix0; k <= i; k++) {
            x->data[k - 1] *= 9.9792015476736E+30f;
          }

          beta1 *= 9.9792015476736E+30f;
          *alpha1 *= 9.9792015476736E+30f;
        } while (!(fabsf(beta1) >= 1.0020841800044864E-30f));

        beta1 = rt_hypotd_snf(*alpha1, xnrm2(n - 1, x, ix0));
        if (*alpha1 >= 0.0f) {
          beta1 = -beta1;
        }

        tau = (beta1 - *alpha1) / beta1;
        xnorm = 1.0f / (*alpha1 - beta1);
        for (k = ix0; k <= i; k++) {
          x->data[k - 1] *= xnorm;
        }

        for (k = 0; k <= knt; k++) {
          beta1 *= 1.0020841800044864E-30f;
        }

        *alpha1 = beta1;
      } else {
        tau = (beta1 - *alpha1) / beta1;
        xnorm = 1.0f / (*alpha1 - beta1);
        i = (ix0 + n) - 2;
        for (k = ix0; k <= i; k++) {
          x->data[k - 1] *= xnorm;
        }

        *alpha1 = beta1;
      }
    }
  }

  return tau;
}

/* End of code generation (xzlarfg.c) */
