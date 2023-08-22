/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * insertionsort.c
 *
 * Code generation for function 'insertionsort'
 *
 */

/* Include files */
#include "insertionsort.h"
#include "cvxEDA_data.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_insertionsort(emxArray_int32_T *x, int xstart, int xend, const
                     cell_wrap_3 cmp_tunableEnvironment[2])
{
  int i;
  int idx;
  int k;
  int xc;
  boolean_T exitg1;
  boolean_T varargout_1;
  i = xstart + 1;
  for (k = i; k <= xend; k++) {
    xc = x->data[k - 1] - 1;
    idx = k - 2;
    exitg1 = false;
    while ((!exitg1) && (idx + 1 >= xstart)) {
      if (cmp_tunableEnvironment[0].f1->data[xc] < cmp_tunableEnvironment[0].
          f1->data[x->data[idx] - 1]) {
        varargout_1 = true;
      } else if (cmp_tunableEnvironment[0].f1->data[xc] ==
                 cmp_tunableEnvironment[0].f1->data[x->data[idx] - 1]) {
        varargout_1 = (cmp_tunableEnvironment[1].f1->data[xc] <
                       cmp_tunableEnvironment[1].f1->data[x->data[idx] - 1]);
      } else {
        varargout_1 = false;
      }

      if (varargout_1) {
        x->data[idx + 1] = x->data[idx];
        idx--;
      } else {
        exitg1 = true;
      }
    }

    x->data[idx + 1] = xc + 1;
  }
}

void c_insertionsort(emxArray_int32_T *x, int xstart, int xend)
{
  int i;
  int idx;
  int k;
  int xc;
  i = xstart + 1;
  for (k = i; k <= xend; k++) {
    xc = x->data[k - 1];
    idx = k - 1;
    while ((idx >= xstart) && (xc < x->data[idx - 1])) {
      x->data[idx] = x->data[idx - 1];
      idx--;
    }

    x->data[idx] = xc;
  }
}

void insertionsort(int x[354], int xstart, int xend)
{
  int aj;
  int i;
  int i1;
  int idx;
  int k;
  int xc;
  boolean_T exitg1;
  boolean_T varargout_1;
  i = xstart + 1;
  for (k = i; k <= xend; k++) {
    xc = x[k - 1];
    idx = k - 1;
    exitg1 = false;
    while ((!exitg1) && (idx >= xstart)) {
      aj = iv[x[idx - 1] - 1];
      i1 = iv[xc - 1];
      if (i1 < aj) {
        varargout_1 = true;
      } else if (i1 == aj) {
        varargout_1 = (iv1[xc - 1] < iv1[x[idx - 1] - 1]);
      } else {
        varargout_1 = false;
      }

      if (varargout_1) {
        x[idx] = x[idx - 1];
        idx--;
      } else {
        exitg1 = true;
      }
    }

    x[idx] = xc;
  }
}

/* End of code generation (insertionsort.c) */
