/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * RemoveDependentIneq_.c
 *
 * Code generation for function 'RemoveDependentIneq_'
 *
 */

/* Include files */
#include "RemoveDependentIneq_.h"
#include "countsort.h"
#include "cvxEDA_types.h"
#include "factorQRE.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void RemoveDependentIneq_(h_struct_T *workingset, f_struct_T *qrmanager,
  d_struct_T *memspace)
{
  float tol;
  int b_idx;
  int i;
  int idx;
  int nActiveConstr;
  int nDepIneq;
  int nFixedConstr;
  nActiveConstr = workingset->nActiveConstr;
  nFixedConstr = workingset->nWConstr[1] + workingset->nWConstr[0];
  if ((workingset->nWConstr[2] + workingset->nWConstr[3]) + workingset->
      nWConstr[4] > 0) {
    tol = 100.0f * (float)workingset->nVar * 2.2204460492503131E-16f;
    for (idx = 0; idx < nFixedConstr; idx++) {
      qrmanager->jpvt->data[idx] = 1;
    }

    i = nFixedConstr + 1;
    for (idx = i; idx <= nActiveConstr; idx++) {
      qrmanager->jpvt->data[idx - 1] = 0;
    }

    factorQRE(qrmanager, workingset->ATwset, workingset->nVar,
              workingset->nActiveConstr);
    nDepIneq = 0;
    for (idx = workingset->nActiveConstr; idx > workingset->nVar; idx--) {
      nDepIneq++;
      memspace->workspace_int->data[nDepIneq - 1] = qrmanager->jpvt->data[idx -
        1];
    }

    if (idx <= workingset->nVar) {
      while ((idx > nFixedConstr) && (fabsf(qrmanager->QR->data[(idx +
                qrmanager->QR->size[0] * (idx - 1)) - 1]) < tol)) {
        nDepIneq++;
        memspace->workspace_int->data[nDepIneq - 1] = qrmanager->jpvt->data[idx
          - 1];
        idx--;
      }
    }

    countsort(memspace->workspace_int, nDepIneq, memspace->workspace_sort,
              nFixedConstr + 1, workingset->nActiveConstr);
    for (idx = nDepIneq; idx >= 1; idx--) {
      nActiveConstr = memspace->workspace_int->data[idx - 1] - 1;
      nFixedConstr = workingset->Wid[nActiveConstr] - 1;
      workingset->isActiveConstr[(workingset->isActiveIdx[nFixedConstr] +
        workingset->Wlocalidx[nActiveConstr]) - 2] = false;
      workingset->Wid[nActiveConstr] = workingset->Wid[workingset->nActiveConstr
        - 1];
      workingset->Wlocalidx[nActiveConstr] = workingset->Wlocalidx
        [workingset->nActiveConstr - 1];
      i = workingset->nVar;
      for (b_idx = 0; b_idx < i; b_idx++) {
        workingset->ATwset->data[b_idx + workingset->ATwset->size[0] *
          nActiveConstr] = workingset->ATwset->data[b_idx + workingset->
          ATwset->size[0] * (workingset->nActiveConstr - 1)];
      }

      workingset->bwset[nActiveConstr] = workingset->bwset
        [workingset->nActiveConstr - 1];
      workingset->nActiveConstr--;
      workingset->nWConstr[nFixedConstr]--;
    }
  }
}

void b_RemoveDependentIneq_(h_struct_T *workingset, f_struct_T *qrmanager,
  d_struct_T *memspace)
{
  float tol;
  int b_idx;
  int i;
  int idx;
  int nActiveConstr;
  int nDepIneq;
  int nFixedConstr;
  nActiveConstr = workingset->nActiveConstr;
  nFixedConstr = workingset->nWConstr[1] + workingset->nWConstr[0];
  if ((workingset->nWConstr[2] + workingset->nWConstr[3]) + workingset->
      nWConstr[4] > 0) {
    tol = 1000.0f * (float)workingset->nVar * 2.2204460492503131E-16f;
    for (idx = 0; idx < nFixedConstr; idx++) {
      qrmanager->jpvt->data[idx] = 1;
    }

    i = nFixedConstr + 1;
    for (idx = i; idx <= nActiveConstr; idx++) {
      qrmanager->jpvt->data[idx - 1] = 0;
    }

    factorQRE(qrmanager, workingset->ATwset, workingset->nVar,
              workingset->nActiveConstr);
    nDepIneq = 0;
    for (idx = workingset->nActiveConstr; idx > workingset->nVar; idx--) {
      nDepIneq++;
      memspace->workspace_int->data[nDepIneq - 1] = qrmanager->jpvt->data[idx -
        1];
    }

    if (idx <= workingset->nVar) {
      while ((idx > nFixedConstr) && (fabsf(qrmanager->QR->data[(idx +
                qrmanager->QR->size[0] * (idx - 1)) - 1]) < tol)) {
        nDepIneq++;
        memspace->workspace_int->data[nDepIneq - 1] = qrmanager->jpvt->data[idx
          - 1];
        idx--;
      }
    }

    countsort(memspace->workspace_int, nDepIneq, memspace->workspace_sort,
              nFixedConstr + 1, workingset->nActiveConstr);
    for (idx = nDepIneq; idx >= 1; idx--) {
      nActiveConstr = memspace->workspace_int->data[idx - 1] - 1;
      nFixedConstr = workingset->Wid[nActiveConstr] - 1;
      workingset->isActiveConstr[(workingset->isActiveIdx[nFixedConstr] +
        workingset->Wlocalidx[nActiveConstr]) - 2] = false;
      workingset->Wid[nActiveConstr] = workingset->Wid[workingset->nActiveConstr
        - 1];
      workingset->Wlocalidx[nActiveConstr] = workingset->Wlocalidx
        [workingset->nActiveConstr - 1];
      i = workingset->nVar;
      for (b_idx = 0; b_idx < i; b_idx++) {
        workingset->ATwset->data[b_idx + workingset->ATwset->size[0] *
          nActiveConstr] = workingset->ATwset->data[b_idx + workingset->
          ATwset->size[0] * (workingset->nActiveConstr - 1)];
      }

      workingset->bwset[nActiveConstr] = workingset->bwset
        [workingset->nActiveConstr - 1];
      workingset->nActiveConstr--;
      workingset->nWConstr[nFixedConstr]--;
    }
  }
}

/* End of code generation (RemoveDependentIneq_.c) */
