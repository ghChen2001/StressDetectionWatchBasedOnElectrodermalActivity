/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * vertcat.h
 *
 * Code generation for function 'vertcat'
 *
 */

#ifndef VERTCAT_H
#define VERTCAT_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void sparse_vertcat(const emxArray_real_T *varargin_1_d, const
                      emxArray_int32_T *varargin_1_colidx, const
                      emxArray_int32_T *varargin_1_rowidx, int varargin_1_n,
                      const emxArray_real_T *varargin_2, const emxArray_real_T
                      *varargin_3_d, const emxArray_int32_T *varargin_3_colidx,
                      const emxArray_int32_T *varargin_3_rowidx, int
                      varargin_3_m, int varargin_3_n, i_sparse *c);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (vertcat.h) */
