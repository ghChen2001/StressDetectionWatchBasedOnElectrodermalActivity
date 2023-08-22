/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse.h
 *
 * Code generation for function 'sparse'
 *
 */

#ifndef SPARSE_H
#define SPARSE_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_sparse(h_sparse *y);
  void c_sparse(const emxArray_real_T *varargin_1, const emxArray_real_T
                *varargin_2, const emxArray_real_T *varargin_3, i_sparse *y);
  void d_sparse(float varargin_1, emxArray_real_T *y_d, emxArray_int32_T
                *y_colidx, emxArray_int32_T *y_rowidx);
  void e_sparse(const emxArray_real_T *varargin_1, emxArray_real_T *y_d,
                emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx, int *y_m,
                int *y_maxnz);
  void sparse(const float varargin_3[354], h_sparse *y);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (sparse.h) */
