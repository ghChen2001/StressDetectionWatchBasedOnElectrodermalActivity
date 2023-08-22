/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * heapsort.h
 *
 * Code generation for function 'heapsort'
 *
 */

#ifndef HEAPSORT_H
#define HEAPSORT_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_heapsort(int x[354], int xstart, int xend);
  void c_heapsort(emxArray_int32_T *x, int xstart, int xend, const cell_wrap_3
                  cmp_tunableEnvironment[2]);
  void d_heapsort(emxArray_int32_T *x, int xstart, int xend);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (heapsort.h) */
