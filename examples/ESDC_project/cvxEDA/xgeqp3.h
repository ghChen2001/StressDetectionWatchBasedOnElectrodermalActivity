/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xgeqp3.h
 *
 * Code generation for function 'xgeqp3'
 *
 */

#ifndef XGEQP3_H
#define XGEQP3_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void xgeqp3(emxArray_real_T *A, int m, int n, emxArray_int32_T *jpvt,
              emxArray_real_T *tau);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (xgeqp3.h) */
