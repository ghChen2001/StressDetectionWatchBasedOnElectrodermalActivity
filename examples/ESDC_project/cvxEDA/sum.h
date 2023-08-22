/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.h
 *
 * Code generation for function 'sum'
 *
 */

#ifndef SUM_H
#define SUM_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void sum(const emxArray_real_T *x_d, const emxArray_int32_T *x_colidx,
           emxArray_real_T *y_d, emxArray_int32_T *y_colidx, emxArray_int32_T
           *y_rowidx);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (sum.h) */
