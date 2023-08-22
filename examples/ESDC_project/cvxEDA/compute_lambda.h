/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * compute_lambda.h
 *
 * Code generation for function 'compute_lambda'
 *
 */

#ifndef COMPUTE_LAMBDA_H
#define COMPUTE_LAMBDA_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void compute_lambda(emxArray_real_T *workspace, c_struct_T *solution, const
                      e_struct_T *objective, const f_struct_T *qrmanager);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (compute_lambda.h) */
