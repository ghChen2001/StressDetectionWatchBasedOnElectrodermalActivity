/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * PresolveWorkingSet.c
 *
 * Code generation for function 'PresolveWorkingSet'
 *
 */

/* Include files */
#include "PresolveWorkingSet.h"
#include "RemoveDependentEq_.h"
#include "RemoveDependentIneq_.h"
#include "cvxEDA_types.h"
#include "feasibleX0ForWorkingSet.h"
#include "maxConstraintViolation.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void PresolveWorkingSet(c_struct_T *solution, d_struct_T *memspace, h_struct_T
  *workingset, f_struct_T *qrmanager)
{
  float constrViolation;
  int idxEndIneq;
  int idxStartIneq;
  int idx_global;
  boolean_T guard1 = false;
  boolean_T okWorkingSet;
  solution->state = 82;
  idxStartIneq = RemoveDependentEq_(memspace, workingset, qrmanager);
  if (idxStartIneq != -1) {
    RemoveDependentIneq_(workingset, qrmanager, memspace);
    okWorkingSet = feasibleX0ForWorkingSet(memspace->workspace_double,
      solution->xstar, workingset, qrmanager);
    guard1 = false;
    if (!okWorkingSet) {
      b_RemoveDependentIneq_(workingset, qrmanager, memspace);
      okWorkingSet = feasibleX0ForWorkingSet(memspace->workspace_double,
        solution->xstar, workingset, qrmanager);
      if (!okWorkingSet) {
        solution->state = -7;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1 && (workingset->nWConstr[0] + workingset->nWConstr[1] ==
                   workingset->nVar)) {
      constrViolation = maxConstraintViolation(workingset, solution->xstar);
      if (constrViolation > 1.0E-8f) {
        solution->state = -2;
      }
    }
  } else {
    solution->state = -3;
    idxStartIneq = (workingset->nWConstr[0] + workingset->nWConstr[1]) + 1;
    idxEndIneq = workingset->nActiveConstr;
    for (idx_global = idxStartIneq; idx_global <= idxEndIneq; idx_global++) {
      workingset->isActiveConstr[(workingset->isActiveIdx[workingset->
        Wid[idx_global - 1] - 1] + workingset->Wlocalidx[idx_global - 1]) - 2] =
        false;
    }

    workingset->nWConstr[2] = 0;
    workingset->nWConstr[3] = 0;
    workingset->nWConstr[4] = 0;
    workingset->nActiveConstr = workingset->nWConstr[0] + workingset->nWConstr[1];
  }
}

/* End of code generation (PresolveWorkingSet.c) */
