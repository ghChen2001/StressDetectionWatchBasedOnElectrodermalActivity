/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * locBsearch.h
 *
 * Code generation for function 'locBsearch'
 *
 */

#ifndef LOCBSEARCH_H
#define LOCBSEARCH_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void sparse_locBsearch(const emxArray_int32_T *x, float xi, int xstart, int
    xend, int *n, boolean_T *found);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (locBsearch.h) */
