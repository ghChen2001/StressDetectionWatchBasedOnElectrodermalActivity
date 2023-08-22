/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * heapsort.c
 *
 * Code generation for function 'heapsort'
 *
 */

/* Include files */
#include "heapsort.h"
#include "cvxEDA_data.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void b_heapify(emxArray_int32_T *x, int idx, int xstart, int xend, const
                      cell_wrap_3 cmp_tunableEnvironment[2]);
static void c_heapify(emxArray_int32_T *x, int idx, int xstart, int xend);
static void heapify(int x[354], int idx, int xstart, int xend);

/* Function Definitions */
static void b_heapify(emxArray_int32_T *x, int idx, int xstart, int xend, const
                      cell_wrap_3 cmp_tunableEnvironment[2])
{
  int cmpIdx;
  int extremum;
  int extremumIdx;
  int i;
  int i1;
  int i2;
  int leftIdx;
  int xcmp;
  boolean_T changed;
  boolean_T exitg1;
  boolean_T varargout_1;
  changed = true;
  extremumIdx = (idx + xstart) - 2;
  leftIdx = ((idx << 1) + xstart) - 2;
  exitg1 = false;
  while ((!exitg1) && (leftIdx + 1 < xend)) {
    changed = false;
    extremum = x->data[extremumIdx];
    cmpIdx = leftIdx;
    xcmp = x->data[leftIdx];
    i = x->data[leftIdx + 1] - 1;
    i1 = cmp_tunableEnvironment[0].f1->data[x->data[leftIdx] - 1];
    i2 = cmp_tunableEnvironment[0].f1->data[i];
    if (i1 < i2) {
      varargout_1 = true;
    } else if (i1 == i2) {
      varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[leftIdx] - 1] <
                     cmp_tunableEnvironment[1].f1->data[i]);
    } else {
      varargout_1 = false;
    }

    if (varargout_1) {
      cmpIdx = leftIdx + 1;
      xcmp = x->data[leftIdx + 1];
    }

    i = cmp_tunableEnvironment[0].f1->data[x->data[extremumIdx] - 1];
    i1 = cmp_tunableEnvironment[0].f1->data[xcmp - 1];
    if (i < i1) {
      varargout_1 = true;
    } else if (i == i1) {
      varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[extremumIdx] - 1]
                     < cmp_tunableEnvironment[1].f1->data[xcmp - 1]);
    } else {
      varargout_1 = false;
    }

    if (varargout_1) {
      x->data[extremumIdx] = xcmp;
      x->data[cmpIdx] = extremum;
      extremumIdx = cmpIdx;
      leftIdx = ((((cmpIdx - xstart) + 2) << 1) + xstart) - 2;
      changed = true;
    } else {
      exitg1 = true;
    }
  }

  if (changed && (leftIdx + 1 <= xend)) {
    extremum = x->data[extremumIdx];
    i = cmp_tunableEnvironment[0].f1->data[x->data[extremumIdx] - 1];
    i1 = cmp_tunableEnvironment[0].f1->data[x->data[leftIdx] - 1];
    if (i < i1) {
      varargout_1 = true;
    } else if (i == i1) {
      varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[extremumIdx] - 1]
                     < cmp_tunableEnvironment[1].f1->data[x->data[leftIdx] - 1]);
    } else {
      varargout_1 = false;
    }

    if (varargout_1) {
      x->data[extremumIdx] = x->data[leftIdx];
      x->data[leftIdx] = extremum;
    }
  }
}

static void c_heapify(emxArray_int32_T *x, int idx, int xstart, int xend)
{
  int cmpIdx;
  int extremum;
  int extremumIdx;
  int leftIdx;
  int xcmp;
  boolean_T changed;
  boolean_T exitg1;
  changed = true;
  extremumIdx = (idx + xstart) - 2;
  leftIdx = ((idx << 1) + xstart) - 1;
  exitg1 = false;
  while ((!exitg1) && (leftIdx < xend)) {
    changed = false;
    extremum = x->data[extremumIdx];
    cmpIdx = leftIdx - 1;
    xcmp = x->data[leftIdx - 1];
    if (x->data[leftIdx - 1] < x->data[leftIdx]) {
      cmpIdx = leftIdx;
      xcmp = x->data[leftIdx];
    }

    if (x->data[extremumIdx] < xcmp) {
      x->data[extremumIdx] = xcmp;
      x->data[cmpIdx] = extremum;
      extremumIdx = cmpIdx;
      leftIdx = ((((cmpIdx - xstart) + 2) << 1) + xstart) - 1;
      changed = true;
    } else {
      exitg1 = true;
    }
  }

  if (changed && (leftIdx <= xend)) {
    extremum = x->data[extremumIdx];
    cmpIdx = x->data[leftIdx - 1];
    if (x->data[extremumIdx] < cmpIdx) {
      x->data[extremumIdx] = cmpIdx;
      x->data[leftIdx - 1] = extremum;
    }
  }
}

static void heapify(int x[354], int idx, int xstart, int xend)
{
  int ai;
  int aj;
  int aj_tmp;
  int cmpIdx;
  int extremum;
  int extremumIdx;
  int leftIdx;
  int xcmp;
  boolean_T changed;
  boolean_T exitg1;
  boolean_T varargout_1;
  changed = true;
  extremumIdx = (idx + xstart) - 2;
  leftIdx = ((idx << 1) + xstart) - 2;
  exitg1 = false;
  while ((!exitg1) && (leftIdx + 1 < xend)) {
    changed = false;
    extremum = x[extremumIdx];
    cmpIdx = leftIdx;
    xcmp = x[leftIdx];
    ai = iv[x[leftIdx] - 1];
    aj_tmp = x[leftIdx + 1] - 1;
    aj = iv[aj_tmp];
    if (ai < aj) {
      varargout_1 = true;
    } else if (ai == aj) {
      varargout_1 = (iv1[x[leftIdx] - 1] < iv1[aj_tmp]);
    } else {
      varargout_1 = false;
    }

    if (varargout_1) {
      cmpIdx = leftIdx + 1;
      xcmp = x[leftIdx + 1];
    }

    ai = iv[x[extremumIdx] - 1];
    aj_tmp = iv[xcmp - 1];
    if (ai < aj_tmp) {
      varargout_1 = true;
    } else if (ai == aj_tmp) {
      varargout_1 = (iv1[x[extremumIdx] - 1] < iv1[xcmp - 1]);
    } else {
      varargout_1 = false;
    }

    if (varargout_1) {
      x[extremumIdx] = xcmp;
      x[cmpIdx] = extremum;
      extremumIdx = cmpIdx;
      leftIdx = ((((cmpIdx - xstart) + 2) << 1) + xstart) - 2;
      changed = true;
    } else {
      exitg1 = true;
    }
  }

  if (changed && (leftIdx + 1 <= xend)) {
    extremum = x[extremumIdx];
    ai = iv[x[extremumIdx] - 1];
    aj = iv[x[leftIdx] - 1];
    if (ai < aj) {
      varargout_1 = true;
    } else if (ai == aj) {
      varargout_1 = (iv1[x[extremumIdx] - 1] < iv1[x[leftIdx] - 1]);
    } else {
      varargout_1 = false;
    }

    if (varargout_1) {
      x[extremumIdx] = x[leftIdx];
      x[leftIdx] = extremum;
    }
  }
}

void b_heapsort(int x[354], int xstart, int xend)
{
  int k;
  int n;
  int t;
  n = (xend - xstart) - 1;
  for (t = n + 2; t >= 1; t--) {
    heapify(x, t, xstart, xend);
  }

  for (k = 0; k <= n; k++) {
    t = x[xend - 1];
    x[xend - 1] = x[xstart - 1];
    x[xstart - 1] = t;
    xend--;
    heapify(x, 1, xstart, xend);
  }
}

void c_heapsort(emxArray_int32_T *x, int xstart, int xend, const cell_wrap_3
                cmp_tunableEnvironment[2])
{
  int k;
  int n;
  int t;
  n = (xend - xstart) - 1;
  for (t = n + 2; t >= 1; t--) {
    b_heapify(x, t, xstart, xend, cmp_tunableEnvironment);
  }

  for (k = 0; k <= n; k++) {
    t = x->data[xend - 1];
    x->data[xend - 1] = x->data[xstart - 1];
    x->data[xstart - 1] = t;
    xend--;
    b_heapify(x, 1, xstart, xend, cmp_tunableEnvironment);
  }
}

void d_heapsort(emxArray_int32_T *x, int xstart, int xend)
{
  int k;
  int n;
  int t;
  n = (xend - xstart) - 1;
  for (t = n + 2; t >= 1; t--) {
    c_heapify(x, t, xstart, xend);
  }

  for (k = 0; k <= n; k++) {
    t = x->data[xend - 1];
    x->data[xend - 1] = x->data[xstart - 1];
    x->data[xstart - 1] = t;
    xend--;
    c_heapify(x, 1, xstart, xend);
  }
}

/* End of code generation (heapsort.c) */
