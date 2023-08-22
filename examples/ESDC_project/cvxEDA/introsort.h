/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * introsort.h
 *
 * Code generation for function 'introsort'
 *
 */

#ifndef INTROSORT_H
#define INTROSORT_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_introsort(emxArray_int32_T *x, int xend, const cell_wrap_3
                   cmp_tunableEnvironment[2]);
  void c_introsort(emxArray_int32_T *x, int xstart, int xend);
  void introsort(int x[354]);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (introsort.h) */
