/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse1.h
 *
 * Code generation for function 'sparse1'
 *
 */

#ifndef SPARSE1_H
#define SPARSE1_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_sparse_ctranspose(const emxArray_real_T *this_d, const emxArray_int32_T
    *this_colidx, const emxArray_int32_T *this_rowidx, int this_m, int this_n,
    i_sparse *y);
  void b_sparse_parenReference(const emxArray_real_T *this_d, const
    emxArray_int32_T *this_colidx, const emxArray_int32_T *this_rowidx, int
    this_m, emxArray_real_T *s_d, emxArray_int32_T *s_colidx, emxArray_int32_T
    *s_rowidx, int *s_maxnz);
  void b_sparse_times(const emxArray_real_T *b_d, const emxArray_int32_T
                      *b_colidx, const emxArray_int32_T *b_rowidx, h_sparse *s);
  void c_sparse_ctranspose(const emxArray_real_T *this_d, const emxArray_int32_T
    *this_colidx, const emxArray_int32_T *this_rowidx, emxArray_real_T *y_d,
    emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx);
  void c_sparse_parenReference(const emxArray_real_T *this_d, const
    emxArray_int32_T *this_colidx, const emxArray_int32_T *this_rowidx, int
    this_m, emxArray_real_T *s_d, emxArray_int32_T *s_colidx, emxArray_int32_T
    *s_rowidx);
  void sparse_ctranspose(const emxArray_real_T *this_d, const emxArray_int32_T
    *this_colidx, const emxArray_int32_T *this_rowidx, h_sparse *y);
  void sparse_parenReference(const emxArray_real_T *this_d, const
    emxArray_int32_T *this_colidx, const emxArray_int32_T *this_rowidx, int
    this_m, const emxArray_real_T *varargin_1, emxArray_real_T *s_d,
    emxArray_int32_T *s_colidx, emxArray_int32_T *s_rowidx, int *s_m, int
    *s_maxnz);
  void sparse_plus(const emxArray_real_T *a_d, const emxArray_int32_T *a_colidx,
                   const emxArray_int32_T *a_rowidx, const emxArray_real_T *b_d,
                   const emxArray_int32_T *b_colidx, const emxArray_int32_T
                   *b_rowidx, int b_m, int b_n, i_sparse *s);
  void sparse_times(const emxArray_int32_T *b_colidx, const emxArray_int32_T
                    *b_rowidx, int b_m, int b_n, i_sparse *s);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (sparse1.h) */
