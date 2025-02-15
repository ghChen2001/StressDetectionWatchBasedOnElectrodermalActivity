/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xzlarf.c
 *
 * Code generation for function 'xzlarf'
 *
 */

/* Include files */
#include "xzlarf.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xgerc.h"

/* Function Definitions */
void xzlarf(int m, int n, int iv0, float tau, emxArray_real_T *C, int ic0, int
            ldc, emxArray_real_T *work)
{
  float c;
  int b_i;
  int exitg1;
  int i;
  int ia;
  int iac;
  int ix;
  int iy;
  int lastc;
  int lastv;
  boolean_T exitg2;
  if (tau != 0.0f) {
    lastv = m;
    i = iv0 + m;
    while ((lastv > 0) && (C->data[i - 2] == 0.0f)) {
      lastv--;
      i--;
    }

    lastc = n;
    exitg2 = false;
    while ((!exitg2) && (lastc > 0)) {
      i = ic0 + (lastc - 1) * ldc;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (C->data[ia - 1] != 0.0f) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          lastc--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    lastc = 0;
  }

  if (lastv > 0) {
    if (lastc != 0) {
      for (iy = 0; iy < lastc; iy++) {
        work->data[iy] = 0.0f;
      }

      iy = 0;
      i = ic0 + ldc * (lastc - 1);
      for (iac = ic0; ldc < 0 ? iac >= i : iac <= i; iac += ldc) {
        ix = iv0;
        c = 0.0f;
        b_i = (iac + lastv) - 1;
        for (ia = iac; ia <= b_i; ia++) {
          c += C->data[ia - 1] * C->data[ix - 1];
          ix++;
        }

        work->data[iy] += c;
        iy++;
      }
    }

    xgerc(lastv, lastc, -tau, iv0, work, C, ic0, ldc);
  }
}

/* End of code generation (xzlarf.c) */
