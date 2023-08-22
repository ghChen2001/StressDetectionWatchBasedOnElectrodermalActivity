/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mtimes.h
 *
 * Code generation for function 'mtimes'
 *
 */

#ifndef MTIMES_H
#define MTIMES_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, const float
                       b[240], float c[240]);
  void c_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, const
                       emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                       const emxArray_int32_T *b_rowidx, int b_n,
                       emxArray_real_T *c_d, emxArray_int32_T *c_colidx,
                       emxArray_int32_T *c_rowidx, int *c_n);
  void d_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, int a_m,
                       const emxArray_real_T *b_d, const emxArray_int32_T
                       *b_colidx, const emxArray_int32_T *b_rowidx,
                       emxArray_real_T *c_d, emxArray_int32_T *c_colidx,
                       emxArray_int32_T *c_rowidx, int *c_m);
  void e_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, int
                       a_n, emxArray_real_T *c);
  void f_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, int a_m,
                       const emxArray_real_T *b_d, const emxArray_int32_T
                       *b_colidx, const emxArray_int32_T *b_rowidx, int b_n,
                       i_sparse *c);
  void g_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, const float
                       b[120], float c[120]);
  void h_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, int
                       a_n, const float b[120], emxArray_real_T *c);
  void i_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, int a_m,
                       const emxArray_real_T *b_d, const emxArray_int32_T
                       *b_colidx, const emxArray_int32_T *b_rowidx,
                       emxArray_real_T *c_d, emxArray_int32_T *c_colidx,
                       emxArray_int32_T *c_rowidx, int *c_m, int *c_maxnz);
  void j_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                       *a_colidx, const emxArray_int32_T *a_rowidx, const
                       emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                       const emxArray_int32_T *b_rowidx, h_sparse *c);
  void sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, const
                     emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                     const emxArray_int32_T *b_rowidx, h_sparse *c);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (mtimes.h) */
