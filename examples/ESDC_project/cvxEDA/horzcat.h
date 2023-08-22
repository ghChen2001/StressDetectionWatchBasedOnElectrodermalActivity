/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * horzcat.h
 *
 * Code generation for function 'horzcat'
 *
 */

#ifndef HORZCAT_H
#define HORZCAT_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_sparse_horzcat(const emxArray_real_T *varargin_1_d, const
                        emxArray_int32_T *varargin_1_colidx, const
                        emxArray_int32_T *varargin_1_rowidx, int varargin_1_m,
                        const emxArray_real_T *varargin_2, const emxArray_real_T
                        *varargin_3_d, const emxArray_int32_T *varargin_3_colidx,
                        const emxArray_int32_T *varargin_3_rowidx, int
                        varargin_3_m, int varargin_3_n, i_sparse *c);
  void sparse_horzcat(const emxArray_real_T *varargin_1_d, const
                      emxArray_int32_T *varargin_1_colidx, const
                      emxArray_int32_T *varargin_1_rowidx, const float
                      varargin_2[240], const emxArray_real_T *varargin_3_d,
                      const emxArray_int32_T *varargin_3_colidx, const
                      emxArray_int32_T *varargin_3_rowidx, int varargin_3_n,
                      emxArray_real_T *c_d, emxArray_int32_T *c_colidx,
                      emxArray_int32_T *c_rowidx, int *c_n);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (horzcat.h) */
