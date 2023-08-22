/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * feasibleratiotest.h
 *
 * Code generation for function 'feasibleratiotest'
 *
 */

#ifndef FEASIBLERATIOTEST_H
#define FEASIBLERATIOTEST_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void feasibleratiotest(const emxArray_real_T *solution_xstar, const
    emxArray_real_T *solution_searchDir, emxArray_real_T *workspace, int
    workingset_nVar, int workingset_ldA, const emxArray_real_T *workingset_Aineq,
    const emxArray_real_T *workingset_lb, const emxArray_int32_T
    *workingset_indexLB, const int workingset_sizes[5], const int
    workingset_isActiveIdx[6], const boolean_T workingset_isActiveConstr[121],
    const int workingset_nWConstr[5], boolean_T isPhaseOne, float *alpha,
    boolean_T *newBlocking, int *constrType, int *constrIdx);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (feasibleratiotest.h) */
