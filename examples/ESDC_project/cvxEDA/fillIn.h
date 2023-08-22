/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * fillIn.h
 *
 * Code generation for function 'fillIn'
 *
 */

#ifndef FILLIN_H
#define FILLIN_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void b_sparse_fillIn(i_sparse *this);
  void c_sparse_fillIn(h_sparse *this);
  void d_sparse_fillIn(i_sparse *this);
  void sparse_fillIn(h_sparse *this);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (fillIn.h) */
