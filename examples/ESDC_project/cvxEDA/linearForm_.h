/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * linearForm_.h
 *
 * Code generation for function 'linearForm_'
 *
 */

#ifndef LINEARFORM__H
#define LINEARFORM__H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void linearForm_(boolean_T obj_hasLinear, int obj_nvar, emxArray_real_T
                   *workspace, const emxArray_real_T *H, const emxArray_real_T
                   *f, const emxArray_real_T *x);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (linearForm_.h) */
