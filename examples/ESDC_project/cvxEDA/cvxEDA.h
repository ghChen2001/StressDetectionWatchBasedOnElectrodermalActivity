/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * cvxEDA.h
 *
 * Code generation for function 'cvxEDA'
 *
 */

#ifndef CVXEDA_H
#define CVXEDA_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  extern void cvxEDA(const float y[120], float delta, float r[120], float p
                     [120], float t[120], f_sparse *l, g_sparse *d, float e
                     [120], float *obj);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (cvxEDA.h) */
