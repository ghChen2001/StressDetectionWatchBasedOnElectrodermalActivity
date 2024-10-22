/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xgemv.h
 *
 * Code generation for function 'xgemv'
 *
 */

#ifndef XGEMV_H
#define XGEMV_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_xgemv(int m, const emxArray_real_T *A, int lda, const emxArray_real_T
               *x, emxArray_real_T *y);
  void xgemv(int m, const emxArray_real_T *A, int lda, const emxArray_real_T *x,
             float y[121]);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (xgemv.h) */
