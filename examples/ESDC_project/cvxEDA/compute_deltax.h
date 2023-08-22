/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * compute_deltax.h
 *
 * Code generation for function 'compute_deltax'
 *
 */

#ifndef COMPUTE_DELTAX_H
#define COMPUTE_DELTAX_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void compute_deltax(const emxArray_real_T *H, c_struct_T *solution, d_struct_T
                      *memspace, const f_struct_T *qrmanager, g_struct_T
                      *cholmanager, const e_struct_T *objective);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (compute_deltax.h) */
