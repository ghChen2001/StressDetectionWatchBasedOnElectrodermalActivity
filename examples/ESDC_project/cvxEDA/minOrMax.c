/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * minOrMax.c
 *
 * Code generation for function 'minOrMax'
 *
 */

/* Include files */
#include "minOrMax.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
float maximum(const emxArray_real_T *x)
{
  float d;
  float ex;
  int idx;
  int k;
  int n;
  boolean_T exitg1;
  n = x->size[0];
  if (x->size[0] <= 2) {
    if (x->size[0] == 1) {
      ex = x->data[0];
    } else if ((x->data[0] < x->data[1]) || (rtIsNaN(x->data[0]) && (!rtIsNaN
                 (x->data[1])))) {
      ex = x->data[1];
    } else {
      ex = x->data[0];
    }
  } else {
    if (!rtIsNaN(x->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= x->size[0])) {
        if (!rtIsNaN(x->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      ex = x->data[0];
    } else {
      ex = x->data[idx - 1];
      idx++;
      for (k = idx; k <= n; k++) {
        d = x->data[k - 1];
        if (ex < d) {
          ex = d;
        }
      }
    }
  }

  return ex;
}

/* End of code generation (minOrMax.c) */
