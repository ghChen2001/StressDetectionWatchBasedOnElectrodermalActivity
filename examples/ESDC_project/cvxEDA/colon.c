/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * colon.c
 *
 * Code generation for function 'colon'
 *
 */

/* Include files */
#include "colon.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void b_eml_float_colon(float d, emxArray_real_T *y)
{
  float apnd;
  float cdiff;
  float ndbl;
  int k;
  int n;
  int nm1d2;
  ndbl = floor(119.0 / d + 0.5);
  apnd = ndbl * d + 1.0;
  if (d > 0.0) {
    cdiff = apnd - 120.0;
  } else {
    cdiff = 120.0 - apnd;
  }

  if (fabs(cdiff) < 5.3290705182007514E-14) {
    ndbl++;
    apnd = 120.0;
  } else if (cdiff > 0.0) {
    apnd = (ndbl - 1.0) * d + 1.0;
  } else {
    ndbl++;
  }

  if (ndbl >= 0.0) {
    n = (int)ndbl;
  } else {
    n = 0;
  }

  nm1d2 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity_real_T(y, nm1d2);
  if (n > 0) {
    y->data[0] = 1.0;
    if (n > 1) {
      y->data[n - 1] = apnd;
      nm1d2 = (n - 1) / 2;
      for (k = 0; k <= nm1d2 - 2; k++) {
        ndbl = ((float)k + 1.0) * d;
        y->data[k + 1] = ndbl + 1.0;
        y->data[(n - k) - 2] = apnd - ndbl;
      }

      if (nm1d2 << 1 == n - 1) {
        y->data[nm1d2] = (apnd + 1.0) / 2.0;
      } else {
        ndbl = (float)nm1d2 * d;
        y->data[nm1d2] = ndbl + 1.0;
        y->data[nm1d2 + 1] = apnd - ndbl;
      }
    }
  }
}

void c_eml_float_colon(float a, float b, emxArray_real_T *y)
{
  float apnd;
  float cdiff;
  float ndbl;
  int k;
  int n;
  int nm1d2;
  ndbl = floorf((b - a) + 0.5);
  apnd = a + ndbl;
  cdiff = apnd - b;
  if (fabs(cdiff) < 4.4408920985006262E-16 * fmax(fabs(a), fabs(b))) {
    ndbl++;
    apnd = b;
  } else if (cdiff > 0.0) {
    apnd = a + (ndbl - 1.0);
  } else {
    ndbl++;
  }

  if (ndbl >= 0.0) {
    n = (int)ndbl;
  } else {
    n = 0;
  }

  nm1d2 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity_real_T(y, nm1d2);
  if (n > 0) {
    y->data[0] = a;
    if (n > 1) {
      y->data[n - 1] = apnd;
      nm1d2 = (n - 1) / 2;
      for (k = 0; k <= nm1d2 - 2; k++) {
        y->data[k + 1] = a + ((float)k + 1.0);
        y->data[(n - k) - 2] = apnd - ((float)k + 1.0);
      }

      if (nm1d2 << 1 == n - 1) {
        y->data[nm1d2] = (a + apnd) / 2.0;
      } else {
        y->data[nm1d2] = a + (float)nm1d2;
        y->data[nm1d2 + 1] = apnd - (float)nm1d2;
      }
    }
  }
}

void eml_float_colon(float a, emxArray_real_T *y)
{
  float apnd;
  float ndbl;
  int k;
  int n;
  int nm1d2;
  ndbl = floorf((1.0 - a) / -1.0 + 0.5);
  apnd = a + -ndbl;
  if (fabs(1.0 - apnd) < 4.4408920985006262E-16 * fmax(fabs(a), 1.0)) {
    ndbl++;
    apnd = 1.0;
  } else if (1.0 - apnd > 0.0) {
    apnd = a + -(ndbl - 1.0);
  } else {
    ndbl++;
  }

  if (ndbl >= 0.0) {
    n = (int)ndbl;
  } else {
    n = 0;
  }

  nm1d2 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity_real_T(y, nm1d2);
  if (n > 0) {
    y->data[0] = a;
    if (n > 1) {
      y->data[n - 1] = apnd;
      nm1d2 = (n - 1) / 2;
      for (k = 0; k <= nm1d2 - 2; k++) {
        y->data[k + 1] = a + -((float)k + 1.0);
        y->data[(n - k) - 2] = apnd - (-((float)k + 1.0));
      }

      if (nm1d2 << 1 == n - 1) {
        y->data[nm1d2] = (a + apnd) / 2.0;
      } else {
        y->data[nm1d2] = a + -(float)nm1d2;
        y->data[nm1d2 + 1] = apnd - (-(float)nm1d2);
      }
    }
  }
}

/* End of code generation (colon.c) */
