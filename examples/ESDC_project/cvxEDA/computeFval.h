/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeFval.h
 *
 * Code generation for function 'computeFval'
 *
 */

#ifndef COMPUTEFVAL_H
#define COMPUTEFVAL_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  float computeFval(const e_struct_T *obj, emxArray_real_T *workspace, const
                     emxArray_real_T *H, const emxArray_real_T *f, const
                     emxArray_real_T *x);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (computeFval.h) */
