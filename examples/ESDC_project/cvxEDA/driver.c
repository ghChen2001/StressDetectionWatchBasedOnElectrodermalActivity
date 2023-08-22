/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * driver.c
 *
 * Code generation for function 'driver'
 *
 */

/* Include files */
#include "driver.h"
#include "PresolveWorkingSet.h"
#include "RemoveDependentEq_.h"
#include "RemoveDependentIneq_.h"
#include "computeFirstOrderOpt.h"
#include "computeFval.h"
#include "cvxEDA_internal_types.h"
#include "cvxEDA_types.h"
#include "feasibleX0ForWorkingSet.h"
#include "iterate.h"
#include "maxConstraintViolation.h"
#include "phaseone.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void driver(const emxArray_real_T *H, const emxArray_real_T *f, c_struct_T
            *solution, d_struct_T *memspace, h_struct_T *workingset, f_struct_T *
            qrmanager, g_struct_T *cholmanager, e_struct_T *objective,
            b_struct_T runTimeOptions)
{
  static const char cv[128] = { '\x00', '\x01', '\x02', '\x03', '\x04', '\x05',
    '\x06', '\x07', '\x08', '	', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18',
    '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', ' ', '!', '\"', '#',
    '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'a',
    'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', '\\', ']', '^', '_',
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}',
    '~', '\x7f' };

  static const char t2_Algorithm[10] = { 'a', 'c', 't', 'i', 'v', 'e', '-', 's',
    'e', 't' };

  static const char cv1[8] = { 'q', 'u', 'a', 'd', 'p', 'r', 'o', 'g' };

  static const char t2_SolverName[8] = { 'q', 'u', 'a', 'd', 'p', 'r', 'o', 'g'
  };

  static const char t2_FiniteDifferenceType[7] = { 'f', 'o', 'r', 'w', 'a', 'r',
    'd' };

  static const char t2_Display[5] = { 'f', 'i', 'n', 'a', 'l' };

  i_struct_T options;
  float maxConstr_new;
  int exitg1;
  int idx;
  int k;
  int nVar;
  boolean_T guard1 = false;
  boolean_T okWorkingSet;
  solution->iterations = 0;
  runTimeOptions.RemainFeasible = true;
  nVar = workingset->nVar - 1;
  k = workingset->sizes[3];
  for (idx = 0; idx < k; idx++) {
    if (workingset->isActiveConstr[(workingset->isActiveIdx[3] + idx) - 1]) {
      solution->xstar->data[workingset->indexLB->data[idx] - 1] =
        -workingset->lb->data[workingset->indexLB->data[idx] - 1];
    }
  }

  solution->state = 82;
  RemoveDependentEq_(memspace, workingset, qrmanager);
  RemoveDependentIneq_(workingset, qrmanager, memspace);
  okWorkingSet = feasibleX0ForWorkingSet(memspace->workspace_double,
    solution->xstar, workingset, qrmanager);
  if (!okWorkingSet) {
    b_RemoveDependentIneq_(workingset, qrmanager, memspace);
    okWorkingSet = feasibleX0ForWorkingSet(memspace->workspace_double,
      solution->xstar, workingset, qrmanager);
    if (!okWorkingSet) {
      solution->state = -7;
    }
  }

  printf("driver 1\r\n");
  options.InitDamping = 0.01f;
  for (k = 0; k < 7; k++) {
    options.FiniteDifferenceType[k] = t2_FiniteDifferenceType[k];
  }

  options.SpecifyObjectiveGradient = false;
  options.ScaleProblem = false;
  options.SpecifyConstraintGradient = false;
  options.NonFiniteSupport = true;
  options.IterDisplaySQP = false;
  options.FiniteDifferenceStepSize = -1.0f;
  options.MaxFunctionEvaluations = -1.0f;
  options.IterDisplayQP = false;
  options.PricingTolerance = 0.0;
  for (k = 0; k < 10; k++) {
    options.Algorithm[k] = t2_Algorithm[k];
  }

  options.ObjectiveLimit = -1.0E+20f;
  options.ConstraintTolerance = 1.0E-4f;
  options.OptimalityTolerance = 1.0E-4f;
  options.StepTolerance = 1.0E-6f;
  options.MaxIterations = 20.0f;
  options.FunctionTolerance = rtInf;
  for (k = 0; k < 8; k++) {
    options.SolverName[k] = t2_SolverName[k];
  }

  options.CheckGradients = false;
  options.Diagnostics[0] = 'o';
  options.Diagnostics[1] = 'f';
  options.Diagnostics[2] = 'f';
  options.DiffMaxChange = rtInf;
  options.DiffMinChange = 0.0;
  for (k = 0; k < 5; k++) {
    options.Display[k] = t2_Display[k];
  }

  options.FunValCheck[0] = 'o';
  options.FunValCheck[1] = 'f';
  options.FunValCheck[2] = 'f';
  options.UseParallel = false;
  options.LinearSolver[0] = 'a';
  options.LinearSolver[1] = 'u';
  options.LinearSolver[2] = 't';
  options.LinearSolver[3] = 'o';
  options.SubproblemAlgorithm[0] = 'c';
  options.SubproblemAlgorithm[1] = 'g';
  if (solution->state >= 0) {
    solution->iterations = 0;
    solution->maxConstr = maxConstraintViolation(workingset, solution->xstar);
    maxConstr_new = 1.0E-4f * runTimeOptions.ConstrRelTolFactor;
    guard1 = false;
    if (solution->maxConstr > maxConstr_new) {
      phaseone(H, f, solution, memspace, workingset, qrmanager, cholmanager,
               objective, &options, &runTimeOptions);
      if (solution->state != 0) {
        solution->maxConstr = maxConstraintViolation(workingset, solution->xstar);
        if (solution->maxConstr > maxConstr_new) {
          memset(&solution->lambda[0], 0, 121U * sizeof(float));
          solution->fstar = computeFval(objective, memspace->workspace_double, H,
            f, solution->xstar);
          solution->state = -2;
        } else {
          if (solution->maxConstr > 0.0) {
            for (k = 0; k <= nVar; k++) {
              solution->searchDir->data[k] = solution->xstar->data[k];
            }

            PresolveWorkingSet(solution, memspace, workingset, qrmanager);
            maxConstr_new = maxConstraintViolation(workingset, solution->xstar);
            if (maxConstr_new >= solution->maxConstr) {
              solution->maxConstr = maxConstr_new;
              for (k = 0; k <= nVar; k++) {
                solution->xstar->data[k] = solution->searchDir->data[k];
              }
            }
          }

          guard1 = true;
        }
      }
    } else {
      guard1 = true;
    }

    printf("driver 2\r\n");

    if (guard1) {
      iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
              objective, options.ObjectiveLimit, options.StepTolerance,
              runTimeOptions.MaxIterations, runTimeOptions.ConstrRelTolFactor,
              runTimeOptions.ProbRelTolFactor, true);
      okWorkingSet = false;
      nVar = 0;
      do {
        exitg1 = 0;
        if (nVar < 8) {
          if (cv[(unsigned char)options.SolverName[nVar]] != cv[(int)cv1[nVar]])
          {
            exitg1 = 1;
          } else {
            nVar++;
          }
        } else {
          okWorkingSet = true;
          exitg1 = 1;
        }
      } while (exitg1 == 0);

      printf("driver 3\r\n");

      if (okWorkingSet && (solution->state != -6)) {
        solution->maxConstr = maxConstraintViolation(workingset, solution->xstar);
        computeFirstOrderOpt(solution, objective, workingset->nVar,
                             workingset->ldA, workingset->ATwset,
                             workingset->nActiveConstr,
                             memspace->workspace_double);
        runTimeOptions.RemainFeasible = false;
        while ((solution->iterations < runTimeOptions.MaxIterations) &&
               ((solution->state == -7) || ((solution->state == 1) &&
                 ((solution->maxConstr > 1.0E-4f *
                   runTimeOptions.ConstrRelTolFactor) ||
                  (solution->firstorderopt > 1.0E-4f *
                   runTimeOptions.ProbRelTolFactor))))) {
          feasibleX0ForWorkingSet(memspace->workspace_double, solution->xstar,
            workingset, qrmanager);
          PresolveWorkingSet(solution, memspace, workingset, qrmanager);
          phaseone(H, f, solution, memspace, workingset, qrmanager, cholmanager,
                   objective, &options, &runTimeOptions);
          iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
                  objective, options.ObjectiveLimit, options.StepTolerance,
                  runTimeOptions.MaxIterations,
                  runTimeOptions.ConstrRelTolFactor,
                  runTimeOptions.ProbRelTolFactor, false);
          solution->maxConstr = maxConstraintViolation(workingset,
            solution->xstar);
          computeFirstOrderOpt(solution, objective, workingset->nVar,
                               workingset->ldA, workingset->ATwset,
                               workingset->nActiveConstr,
                               memspace->workspace_double);
        }
      }
      printf("driver 4\r\n");
    }
  }
}

/* End of code generation (driver.c) */
