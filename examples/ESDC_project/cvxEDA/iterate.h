/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * iterate.h
 *
 * Code generation for function 'iterate'
 *
 */

#ifndef ITERATE_H
#define ITERATE_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void iterate(const emxArray_real_T *H, const emxArray_real_T *f, c_struct_T
               *solution, d_struct_T *memspace, h_struct_T *workingset,
               f_struct_T *qrmanager, g_struct_T *cholmanager, e_struct_T
               *objective, float options_ObjectiveLimit, float
               options_StepTolerance, int runTimeOptions_MaxIterations, float
               c_runTimeOptions_ConstrRelTolFa, float
               runTimeOptions_ProbRelTolFactor, boolean_T
               runTimeOptions_RemainFeasible);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (iterate.h) */
