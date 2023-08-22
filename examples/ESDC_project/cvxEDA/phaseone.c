/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * phaseone.c
 *
 * Code generation for function 'phaseone'
 *
 */

/* Include files */
#include "phaseone.h"
#include "computeFval.h"
#include "cvxEDA_internal_types.h"
#include "cvxEDA_types.h"
#include "iterate.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"

/* Function Definitions */
void phaseone(const emxArray_real_T *H, const emxArray_real_T *f, c_struct_T
              *solution, d_struct_T *memspace, h_struct_T *workingset,
              f_struct_T *qrmanager, g_struct_T *cholmanager, e_struct_T
              *objective, i_struct_T *options, const b_struct_T *runTimeOptions)
{
  int PHASEONE;
  int PROBTYPE_ORIG;
  int idxEndIneq;
  int idxStartIneq;
  int idx_global;
  int nVar;
  int nVarP1;
  boolean_T exitg1;
  PROBTYPE_ORIG = workingset->probType;
  nVar = workingset->nVar;
  nVarP1 = workingset->nVar;
  solution->xstar->data[workingset->nVar] = solution->maxConstr + 1.0;
  if (workingset->probType == 3) {
    PHASEONE = 1;
  } else {
    PHASEONE = 4;
  }

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
  setProblemType(workingset, PHASEONE);
  objective->prev_objtype = objective->objtype;
  objective->prev_nvar = objective->nvar;
  objective->prev_hasLinear = objective->hasLinear;
  objective->objtype = 5;
  objective->nvar = nVarP1 + 1;
  objective->gammaScalar = 1.0f;
  objective->hasLinear = true;
  options->ObjectiveLimit = 1.0E-8f * runTimeOptions->ConstrRelTolFactor;
  options->StepTolerance = 1.4901161193847657E-10f;
  solution->fstar = computeFval(objective, memspace->workspace_double, H, f,
    solution->xstar);
  solution->state = 5;
  iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
          objective, options->ObjectiveLimit, options->StepTolerance,
          runTimeOptions->MaxIterations, runTimeOptions->ConstrRelTolFactor,
          runTimeOptions->ProbRelTolFactor, runTimeOptions->RemainFeasible);
  if (workingset->isActiveConstr[(workingset->isActiveIdx[3] + workingset->
       sizes[3]) - 2]) {
    idx_global = 0;
    exitg1 = false;
    while ((!exitg1) && (idx_global + 1 <= workingset->nActiveConstr)) {
      if ((workingset->Wid[idx_global] == 4) && (workingset->
           Wlocalidx[idx_global] == workingset->sizes[3])) {
        idxStartIneq = workingset->Wid[idx_global] - 1;
        workingset->isActiveConstr[(workingset->isActiveIdx[workingset->
          Wid[idx_global] - 1] + workingset->Wlocalidx[idx_global]) - 2] = false;
        workingset->Wid[idx_global] = workingset->Wid[workingset->nActiveConstr
          - 1];
        workingset->Wlocalidx[idx_global] = workingset->Wlocalidx
          [workingset->nActiveConstr - 1];
        idxEndIneq = workingset->nVar;
        for (PHASEONE = 0; PHASEONE < idxEndIneq; PHASEONE++) {
          workingset->ATwset->data[PHASEONE + workingset->ATwset->size[0] *
            idx_global] = workingset->ATwset->data[PHASEONE + workingset->
            ATwset->size[0] * (workingset->nActiveConstr - 1)];
        }

        workingset->bwset[idx_global] = workingset->bwset
          [workingset->nActiveConstr - 1];
        workingset->nActiveConstr--;
        workingset->nWConstr[idxStartIneq]--;
        exitg1 = true;
      } else {
        idx_global++;
      }
    }
  }

  PHASEONE = workingset->nActiveConstr - 1;
  while ((PHASEONE + 1 > 0) && (PHASEONE + 1 > nVar)) {
    idxStartIneq = workingset->Wid[PHASEONE] - 1;
    workingset->isActiveConstr[(workingset->isActiveIdx[workingset->Wid[PHASEONE]
      - 1] + workingset->Wlocalidx[PHASEONE]) - 2] = false;
    workingset->Wid[PHASEONE] = workingset->Wid[workingset->nActiveConstr - 1];
    workingset->Wlocalidx[PHASEONE] = workingset->Wlocalidx
      [workingset->nActiveConstr - 1];
    idxEndIneq = workingset->nVar;
    for (idx_global = 0; idx_global < idxEndIneq; idx_global++) {
      workingset->ATwset->data[idx_global + workingset->ATwset->size[0] *
        PHASEONE] = workingset->ATwset->data[idx_global + workingset->
        ATwset->size[0] * (workingset->nActiveConstr - 1)];
    }

    workingset->bwset[PHASEONE] = workingset->bwset[workingset->nActiveConstr -
      1];
    workingset->nActiveConstr--;
    workingset->nWConstr[idxStartIneq]--;
    PHASEONE--;
  }

  solution->maxConstr = solution->xstar->data[nVarP1];
  setProblemType(workingset, PROBTYPE_ORIG);
  objective->objtype = objective->prev_objtype;
  objective->nvar = objective->prev_nvar;
  objective->hasLinear = objective->prev_hasLinear;
  options->ObjectiveLimit = -1.0E+20f;
  options->StepTolerance = 1.0E-13f;
}

/* End of code generation (phaseone.c) */
