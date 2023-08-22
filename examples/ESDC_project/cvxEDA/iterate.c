/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * iterate.c
 *
 * Code generation for function 'iterate'
 *
 */

/* Include files */
#include "iterate.h"
#include "addBoundToActiveSetMatrix_.h"
#include "computeFval_ReuseHx.h"
#include "computeGrad_StoreHx.h"
#include "computeQ_.h"
#include "compute_deltax.h"
#include "compute_lambda.h"
#include "cvxEDA_types.h"
#include "deleteColMoveEnd.h"
#include "factorQR.h"
#include "feasibleX0ForWorkingSet.h"
#include "feasibleratiotest.h"
#include "maxConstraintViolation.h"
#include "ratiotest.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "xrotg.h"
#include <string.h>

/* Function Definitions */
void iterate(const emxArray_real_T *H, const emxArray_real_T *f, c_struct_T
             *solution, d_struct_T *memspace, h_struct_T *workingset, f_struct_T
             *qrmanager, g_struct_T *cholmanager, e_struct_T *objective, float
             options_ObjectiveLimit, float options_StepTolerance, int
             runTimeOptions_MaxIterations, float
             c_runTimeOptions_ConstrRelTolFa, float
             runTimeOptions_ProbRelTolFactor, boolean_T
             runTimeOptions_RemainFeasible)
{
  float c;
  float d;
  float s;
  float temp;
  float tolDelta;
  int Qk0;
  int TYPE;
  int activeSetChangeID;
  int b_iy;
  int exitg1;
  int globalActiveConstrIdx;
  int ia;
  int iac;
  int idx;
  int iy;
  int iyend;
  int k;
  int nVar;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  boolean_T newBlocking;
  boolean_T subProblemChanged;
  boolean_T updateFval;
  subProblemChanged = true;
  updateFval = true;
  activeSetChangeID = 0;
  TYPE = objective->objtype;
  tolDelta = 6.7434957617430445E-7;
  nVar = workingset->nVar;
  globalActiveConstrIdx = 0;
  computeGrad_StoreHx(objective, H, f, solution->xstar);
  solution->fstar = computeFval_ReuseHx(objective, memspace->workspace_double, f,
    solution->xstar);
  if (solution->iterations < runTimeOptions_MaxIterations) {
    solution->state = -5;
  } else {
    solution->state = 0;
  }
  printf("iterate 1\r\n");

  memset(&solution->lambda[0], 0, 121U * sizeof(float));
  do {
    printf("iterate do\r\n");
    exitg1 = 0;
    if (solution->state == -5) {
      guard1 = false;
      guard2 = false;
      if (subProblemChanged) {
        switch (activeSetChangeID) {
         case 1:
          idx = workingset->ldA * (workingset->nActiveConstr - 1);
          iyend = qrmanager->mrows;
          Qk0 = qrmanager->ncols + 1;
          if (iyend < Qk0) {
            Qk0 = iyend;
          }

          qrmanager->minRowCol = Qk0;
          iy = qrmanager->ldq * qrmanager->ncols;
          Qk0 = qrmanager->ldq;
          if (qrmanager->mrows != 0) {
            iyend = iy + qrmanager->mrows;
            for (b_iy = iy + 1; b_iy <= iyend; b_iy++) {
              qrmanager->QR->data[b_iy - 1] = 0.0;
            }

            b_iy = qrmanager->ldq * (qrmanager->mrows - 1) + 1;
            for (iac = 1; Qk0 < 0 ? iac >= b_iy : iac <= b_iy; iac += Qk0) {
              iyend = idx;
              c = 0.0;
              k = (iac + qrmanager->mrows) - 1;
              for (ia = iac; ia <= k; ia++) {
                c += qrmanager->Q->data[ia - 1] * workingset->ATwset->data[iyend];
                iyend++;
              }

              qrmanager->QR->data[iy] += c;
              iy++;
            }
          }

          qrmanager->ncols++;
          qrmanager->jpvt->data[qrmanager->ncols - 1] = qrmanager->ncols;
          for (idx = qrmanager->mrows - 1; idx + 1 > qrmanager->ncols; idx--) {
            d = qrmanager->QR->data[idx + qrmanager->QR->size[0] *
              (qrmanager->ncols - 1)];
            xrotg(&qrmanager->QR->data[(idx + qrmanager->QR->size[0] *
                   (qrmanager->ncols - 1)) - 1], &d, &c, &s);
            qrmanager->QR->data[idx + qrmanager->QR->size[0] * (qrmanager->ncols
              - 1)] = d;
            Qk0 = qrmanager->ldq * (idx - 1);
            iy = qrmanager->mrows;
            if (qrmanager->mrows >= 1) {
              b_iy = qrmanager->ldq + Qk0;
              for (k = 0; k < iy; k++) {
                temp = c * qrmanager->Q->data[Qk0] + s * qrmanager->Q->data[b_iy];
                qrmanager->Q->data[b_iy] = c * qrmanager->Q->data[b_iy] - s *
                  qrmanager->Q->data[Qk0];
                qrmanager->Q->data[Qk0] = temp;
                b_iy++;
                Qk0++;
              }
            }
          }
          break;

         case -1:
          deleteColMoveEnd(qrmanager, globalActiveConstrIdx);
          break;

         default:
          factorQR(qrmanager, workingset->ATwset, nVar,
                   workingset->nActiveConstr);
          computeQ_(qrmanager, qrmanager->mrows);
          break;
        }

        compute_deltax(H, solution, memspace, qrmanager, cholmanager, objective);
        if (solution->state != -5) {
          exitg1 = 1;
        } else {
          temp = b_xnrm2(nVar, solution->searchDir);
          if ((temp < options_StepTolerance) || (workingset->nActiveConstr >=
               nVar)) {
            guard2 = true;
          } else {
            updateFval = (TYPE == 5);
            if (updateFval || runTimeOptions_RemainFeasible) {
              feasibleratiotest(solution->xstar, solution->searchDir,
                                memspace->workspace_double, workingset->nVar,
                                workingset->ldA, workingset->Aineq,
                                workingset->lb, workingset->indexLB,
                                workingset->sizes, workingset->isActiveIdx,
                                workingset->isActiveConstr, workingset->nWConstr,
                                updateFval, &temp, &newBlocking, &iyend, &Qk0);
            } else {
              ratiotest(solution->xstar, solution->searchDir,
                        memspace->workspace_double, workingset->nVar,
                        workingset->ldA, workingset->Aineq, workingset->lb,
                        workingset->indexLB, workingset->sizes,
                        workingset->isActiveIdx, workingset->isActiveConstr,
                        workingset->nWConstr, &tolDelta, &temp, &newBlocking,
                        &iyend, &Qk0);
            }

            if (newBlocking) {
              switch (iyend) {
               case 3:
                workingset->nWConstr[2]++;
                workingset->isActiveConstr[(workingset->isActiveIdx[2] + Qk0) -
                  2] = true;
                workingset->nActiveConstr++;
                workingset->Wid[workingset->nActiveConstr - 1] = 3;
                workingset->Wlocalidx[workingset->nActiveConstr - 1] = Qk0;
                idx = workingset->ldA * (Qk0 - 1);
                iyend = workingset->ldA * (workingset->nActiveConstr - 1);
                iy = workingset->nVar;
                for (k = 0; k < iy; k++) {
                  workingset->ATwset->data[iyend + k] = workingset->Aineq->
                    data[idx + k];
                }

                workingset->bwset[workingset->nActiveConstr - 1] = 0.0;
                break;

               case 4:
                addBoundToActiveSetMatrix_(workingset, 4, Qk0);
                break;

               default:
                addBoundToActiveSetMatrix_(workingset, 5, Qk0);
                break;
              }

              activeSetChangeID = 1;
            } else {
              if (objective->objtype == 5) {
                if (b_xnrm2(objective->nvar, solution->searchDir) > 100.0f *
                    (float)objective->nvar * 1.4901161193847656E-8f) {
                  solution->state = 3;
                } else {
                  solution->state = 4;
                }
              }

              subProblemChanged = false;
              if (workingset->nActiveConstr == 0) {
                solution->state = 1;
              }
            }

            if ((nVar >= 1) && (!(temp == 0.0f))) {
              iyend = nVar - 1;
              for (k = 0; k <= iyend; k++) {
                solution->xstar->data[k] += temp * solution->searchDir->data[k];
              }
            }

            computeGrad_StoreHx(objective, H, f, solution->xstar);
            updateFval = true;
            guard1 = true;
          }
        }
      } else {
        for (k = 0; k < nVar; k++) {
          solution->searchDir->data[k] = 0.0f;
        }

        guard2 = true;
      }

      if (guard2) {
        compute_lambda(memspace->workspace_double, solution, objective,
                       qrmanager);
        if (solution->state != -7) {
          iy = -1;
          temp = 0.0f * runTimeOptions_ProbRelTolFactor * (float)(TYPE != 5);
          b_iy = (workingset->nWConstr[0] + workingset->nWConstr[1]) + 1;
          k = workingset->nActiveConstr;
          for (idx = b_iy; idx <= k; idx++) {
            d = solution->lambda[idx - 1];
            if (d < temp) {
              temp = d;
              iy = idx - 1;
            }
          }

          if (iy + 1 == 0) {
            solution->state = 1;
          } else {
            activeSetChangeID = -1;
            globalActiveConstrIdx = iy + 1;
            subProblemChanged = true;
            iyend = workingset->Wid[iy] - 1;
            workingset->isActiveConstr[(workingset->isActiveIdx[workingset->
              Wid[iy] - 1] + workingset->Wlocalidx[iy]) - 2] = false;
            workingset->Wid[iy] = workingset->Wid[workingset->nActiveConstr - 1];
            workingset->Wlocalidx[iy] = workingset->Wlocalidx
              [workingset->nActiveConstr - 1];
            b_iy = workingset->nVar;
            for (idx = 0; idx < b_iy; idx++) {
              workingset->ATwset->data[idx + workingset->ATwset->size[0] * iy] =
                workingset->ATwset->data[idx + workingset->ATwset->size[0] *
                (workingset->nActiveConstr - 1)];
            }

            workingset->bwset[iy] = workingset->bwset[workingset->nActiveConstr
              - 1];
            workingset->nActiveConstr--;
            workingset->nWConstr[iyend]--;
            solution->lambda[iy] = 0.0f;
          }
        } else {
          iy = workingset->nActiveConstr;
          activeSetChangeID = 0;
          globalActiveConstrIdx = workingset->nActiveConstr;
          subProblemChanged = true;
          iyend = workingset->nActiveConstr - 1;
          Qk0 = workingset->Wid[iyend] - 1;
          workingset->isActiveConstr[(workingset->isActiveIdx[Qk0] +
            workingset->Wlocalidx[iyend]) - 2] = false;
          workingset->nActiveConstr--;
          workingset->nWConstr[Qk0]--;
          solution->lambda[iy - 1] = 0.0f;
        }

        updateFval = false;
        guard1 = true;
      }

      if (guard1) {
        solution->iterations++;
        iyend = objective->nvar - 1;
        if ((solution->iterations >= runTimeOptions_MaxIterations) &&
            ((solution->state != 1) || (objective->objtype == 5))) {
          solution->state = 0;
        }

        if (solution->iterations - solution->iterations / 50 * 50 == 0) {
          solution->maxConstr = maxConstraintViolation(workingset,
            solution->xstar);
          if (solution->maxConstr > 1.0E-8f * c_runTimeOptions_ConstrRelTolFa) {
            for (k = 0; k <= iyend; k++) {
              solution->searchDir->data[k] = solution->xstar->data[k];
            }

            newBlocking = feasibleX0ForWorkingSet(memspace->workspace_double,
              solution->searchDir, workingset, qrmanager);
            if ((!newBlocking) && (solution->state != 0)) {
              solution->state = -2;
            }

            activeSetChangeID = 0;
            temp = maxConstraintViolation(workingset, solution->searchDir);
            if (temp < solution->maxConstr) {
              for (idx = 0; idx <= iyend; idx++) {
                solution->xstar->data[idx] = solution->searchDir->data[idx];
              }

              solution->maxConstr = temp;
            }
          }
        }

        if ((options_ObjectiveLimit > rtMinusInf) && updateFval) {
          solution->fstar = computeFval_ReuseHx(objective,
            memspace->workspace_double, f, solution->xstar);
          if ((solution->fstar < options_ObjectiveLimit) && ((solution->state !=
                0) || (objective->objtype != 5))) {
            solution->state = 2;
          }
        }
      }
    } else {
      if (!updateFval) {
        solution->fstar = computeFval_ReuseHx(objective,
          memspace->workspace_double, f, solution->xstar);
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* End of code generation (iterate.c) */
