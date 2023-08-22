/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * RemoveDependentEq_.c
 *
 * Code generation for function 'RemoveDependentEq_'
 *
 */

/* Include files */
#include "RemoveDependentEq_.h"
#include "computeQ_.h"
#include "countsort.h"
#include "cvxEDA_types.h"
#include "factorQRE.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include <math.h>

/* Function Definitions */
int RemoveDependentEq_(d_struct_T *memspace, h_struct_T *workingset, f_struct_T *
  qrmanager)
{
  float qtb;
  float tol;
  int idx;
  int idx_col;
  int iy;
  int mTotalWorkingEq_tmp_tmp;
  int mWorkingFixed;
  int nDepInd;
  int nVar;
  boolean_T exitg1;
  nVar = workingset->nVar - 1;
  mWorkingFixed = workingset->nWConstr[0];
  mTotalWorkingEq_tmp_tmp = workingset->nWConstr[1] + workingset->nWConstr[0];
  nDepInd = 0;
  if (mTotalWorkingEq_tmp_tmp > 0) {
    for (iy = 0; iy < mTotalWorkingEq_tmp_tmp; iy++) {
      for (idx_col = 0; idx_col <= nVar; idx_col++) {
        qrmanager->QR->data[iy + qrmanager->QR->size[0] * idx_col] =
          workingset->ATwset->data[idx_col + workingset->ATwset->size[0] * iy];
      }
    }

    nDepInd = mTotalWorkingEq_tmp_tmp - workingset->nVar;
    if (0 > nDepInd) {
      nDepInd = 0;
    }

    for (idx = 0; idx <= nVar; idx++) {
      qrmanager->jpvt->data[idx] = 0;
    }

    qrmanager->usedPivoting = true;
    qrmanager->mrows = mTotalWorkingEq_tmp_tmp;
    qrmanager->ncols = workingset->nVar;
    nVar = workingset->nVar;
    if (mTotalWorkingEq_tmp_tmp < nVar) {
      nVar = mTotalWorkingEq_tmp_tmp;
    }

    qrmanager->minRowCol = nVar;
    xgeqp3(qrmanager->QR, mTotalWorkingEq_tmp_tmp, workingset->nVar,
           qrmanager->jpvt, qrmanager->tau);
    tol = 100.0f * (float)workingset->nVar * 2.2204460492503131E-16f;
    nVar = workingset->nVar;
    if (nVar >= mTotalWorkingEq_tmp_tmp) {
      nVar = mTotalWorkingEq_tmp_tmp;
    }

    while ((nVar > 0) && (fabsf(qrmanager->QR->data[(nVar + qrmanager->QR->size[0]
              * (nVar - 1)) - 1]) < tol)) {
      nVar--;
      nDepInd++;
    }

    if (nDepInd > 0) {
      computeQ_(qrmanager, mTotalWorkingEq_tmp_tmp);
      idx = 0;
      exitg1 = false;
      while ((!exitg1) && (idx <= nDepInd - 1)) {
        nVar = qrmanager->ldq * ((mTotalWorkingEq_tmp_tmp - idx) - 1);
        qtb = 0.0f;
        iy = 0;
        for (idx_col = 0; idx_col < mTotalWorkingEq_tmp_tmp; idx_col++) {
          qtb += qrmanager->Q->data[nVar] * workingset->bwset[iy];
          nVar++;
          iy++;
        }

        if (fabsf(qtb) >= tol) {
          nDepInd = -1;
          exitg1 = true;
        } else {
          idx++;
        }
      }
    }

    if (nDepInd > 0) {
      for (idx = 0; idx < mWorkingFixed; idx++) {
        qrmanager->jpvt->data[idx] = 1;
      }

      mWorkingFixed = workingset->nWConstr[0] + 1;
      for (idx = mWorkingFixed; idx <= mTotalWorkingEq_tmp_tmp; idx++) {
        qrmanager->jpvt->data[idx - 1] = 0;
      }

      factorQRE(qrmanager, workingset->ATwset, workingset->nVar,
                mTotalWorkingEq_tmp_tmp);
      for (idx = 0; idx < nDepInd; idx++) {
        memspace->workspace_int->data[idx] = qrmanager->jpvt->data
          [(mTotalWorkingEq_tmp_tmp - nDepInd) + idx];
      }

      countsort(memspace->workspace_int, nDepInd, memspace->workspace_sort, 1,
                mTotalWorkingEq_tmp_tmp);
      for (idx = nDepInd; idx >= 1; idx--) {
        mWorkingFixed = workingset->nWConstr[0] + workingset->nWConstr[1];
        if (mWorkingFixed != 0) {
          idx_col = memspace->workspace_int->data[idx - 1];
          if (idx_col <= mWorkingFixed) {
            if ((workingset->nActiveConstr == mWorkingFixed) || (idx_col ==
                 mWorkingFixed)) {
              workingset->mEqRemoved++;
              mWorkingFixed = memspace->workspace_int->data[idx - 1] - 1;
              nVar = workingset->Wid[mWorkingFixed] - 1;
              workingset->isActiveConstr[(workingset->isActiveIdx[nVar] +
                workingset->Wlocalidx[mWorkingFixed]) - 2] = false;
              workingset->Wid[mWorkingFixed] = workingset->Wid
                [workingset->nActiveConstr - 1];
              workingset->Wlocalidx[mWorkingFixed] = workingset->
                Wlocalidx[workingset->nActiveConstr - 1];
              idx_col = workingset->nVar;
              for (mTotalWorkingEq_tmp_tmp = 0; mTotalWorkingEq_tmp_tmp <
                   idx_col; mTotalWorkingEq_tmp_tmp++) {
                workingset->ATwset->data[mTotalWorkingEq_tmp_tmp +
                  workingset->ATwset->size[0] * mWorkingFixed] =
                  workingset->ATwset->data[mTotalWorkingEq_tmp_tmp +
                  workingset->ATwset->size[0] * (workingset->nActiveConstr - 1)];
              }

              workingset->bwset[mWorkingFixed] = workingset->bwset
                [workingset->nActiveConstr - 1];
              workingset->nActiveConstr--;
              workingset->nWConstr[nVar]--;
            } else {
              workingset->mEqRemoved++;
              nVar = workingset->Wid[idx_col - 1] - 1;
              workingset->isActiveConstr[(workingset->isActiveIdx[nVar] +
                workingset->Wlocalidx[idx_col - 1]) - 2] = false;
              workingset->Wid[idx_col - 1] = workingset->Wid[mWorkingFixed - 1];
              workingset->Wlocalidx[idx_col - 1] = workingset->
                Wlocalidx[mWorkingFixed - 1];
              iy = workingset->nVar;
              for (mTotalWorkingEq_tmp_tmp = 0; mTotalWorkingEq_tmp_tmp < iy;
                   mTotalWorkingEq_tmp_tmp++) {
                workingset->ATwset->data[mTotalWorkingEq_tmp_tmp +
                  workingset->ATwset->size[0] * (idx_col - 1)] =
                  workingset->ATwset->data[mTotalWorkingEq_tmp_tmp +
                  workingset->ATwset->size[0] * (mWorkingFixed - 1)];
              }

              workingset->bwset[idx_col - 1] = workingset->bwset[mWorkingFixed -
                1];
              workingset->Wid[mWorkingFixed - 1] = workingset->Wid
                [workingset->nActiveConstr - 1];
              workingset->Wlocalidx[mWorkingFixed - 1] = workingset->
                Wlocalidx[workingset->nActiveConstr - 1];
              idx_col = workingset->nVar;
              for (mTotalWorkingEq_tmp_tmp = 0; mTotalWorkingEq_tmp_tmp <
                   idx_col; mTotalWorkingEq_tmp_tmp++) {
                workingset->ATwset->data[mTotalWorkingEq_tmp_tmp +
                  workingset->ATwset->size[0] * (mWorkingFixed - 1)] =
                  workingset->ATwset->data[mTotalWorkingEq_tmp_tmp +
                  workingset->ATwset->size[0] * (workingset->nActiveConstr - 1)];
              }

              workingset->bwset[mWorkingFixed - 1] = workingset->
                bwset[workingset->nActiveConstr - 1];
              workingset->nActiveConstr--;
              workingset->nWConstr[nVar]--;
            }
          }
        }
      }
    }
  }

  return nDepInd;
}

/* End of code generation (RemoveDependentEq_.c) */
