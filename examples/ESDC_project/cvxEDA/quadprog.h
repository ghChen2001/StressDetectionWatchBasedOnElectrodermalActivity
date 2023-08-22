/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * quadprog.h
 *
 * Code generation for function 'quadprog'
 *
 */

#ifndef QUADPROG_H
#define QUADPROG_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void quadprog(const emxArray_real_T *H, const emxArray_real_T *f, const
                emxArray_real_T *Aineq, const emxArray_real_T *x0,
                emxArray_real_T *x, float *fval);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (quadprog.h) */
