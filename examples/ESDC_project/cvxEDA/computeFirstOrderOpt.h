/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeFirstOrderOpt.h
 *
 * Code generation for function 'computeFirstOrderOpt'
 *
 */

#ifndef COMPUTEFIRSTORDEROPT_H
#define COMPUTEFIRSTORDEROPT_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void computeFirstOrderOpt(c_struct_T *solution, const e_struct_T *objective,
    int workingset_nVar, int workingset_ldA, const emxArray_real_T
    *workingset_ATwset, int workingset_nActiveConstr, emxArray_real_T *workspace);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (computeFirstOrderOpt.h) */
