/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * cvxEDA_internal_types.h
 *
 * Code generation for function 'cvxEDA'
 *
 */

#ifndef CVXEDA_INTERNAL_TYPES_H
#define CVXEDA_INTERNAL_TYPES_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_b_struct_T
#define typedef_b_struct_T

typedef struct {
  int MaxIterations;
  float ConstrRelTolFactor;
  float ProbRelTolFactor;
  boolean_T RemainFeasible;
} b_struct_T;

#endif                                 /*typedef_b_struct_T*/

#ifndef typedef_i_struct_T
#define typedef_i_struct_T

typedef struct {
  float InitDamping;
  char FiniteDifferenceType[7];
  boolean_T SpecifyObjectiveGradient;
  boolean_T ScaleProblem;
  boolean_T SpecifyConstraintGradient;
  boolean_T NonFiniteSupport;
  boolean_T IterDisplaySQP;
  float FiniteDifferenceStepSize;
  float MaxFunctionEvaluations;
  boolean_T IterDisplayQP;
  float PricingTolerance;
  char Algorithm[10];
  float ObjectiveLimit;
  float ConstraintTolerance;
  float OptimalityTolerance;
  float StepTolerance;
  float MaxIterations;
  float FunctionTolerance;
  char SolverName[8];
  boolean_T CheckGradients;
  char Diagnostics[3];
  float DiffMaxChange;
  float DiffMinChange;
  char Display[5];
  char FunValCheck[3];
  boolean_T UseParallel;
  char LinearSolver[4];
  char SubproblemAlgorithm[2];
} i_struct_T;

#endif                                 /*typedef_i_struct_T*/
#endif

/* End of code generation (cvxEDA_internal_types.h) */
