/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xgemm.h
 *
 * Code generation for function 'xgemm'
 *
 */

#ifndef XGEMM_H
#define XGEMM_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_xgemm(int m, int n, int k, const emxArray_real_T *A, int ia0, int lda,
               const emxArray_real_T *B, int ldb, emxArray_real_T *C, int ldc);
  void xgemm(int m, int n, int k, const emxArray_real_T *A, int lda, const
             emxArray_real_T *B, int ib0, int ldb, emxArray_real_T *C, int ldc);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (xgemm.h) */
