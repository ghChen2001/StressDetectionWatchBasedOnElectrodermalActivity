/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * feasibleX0ForWorkingSet.c
 *
 * Code generation for function 'feasibleX0ForWorkingSet'
 *
 */

/* Include files */
#include "feasibleX0ForWorkingSet.h"
#include "computeQ_.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "factorQR.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xzgeqp3.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
boolean_T feasibleX0ForWorkingSet(emxArray_real_T *workspace, emxArray_real_T
  *xCurrent, h_struct_T *workingset, f_struct_T *qrmanager)
{
  emxArray_real_T *B;
  float b_v;
  float c;
  float v;
  int exitg1;
  int i;
  int i1;
  int ia;
  int iac;
  int ix;
  int ix0_2;
  int iy;
  int ldq;
  int ldw;
  int mLB;
  int mWConstr;
  int minmana;
  int nVar;
  boolean_T nonDegenerateWset;
  mWConstr = workingset->nActiveConstr - 1;
  nVar = workingset->nVar;
  ldw = workingset->ATwset->size[0];
  nonDegenerateWset = true;
  if (workingset->nActiveConstr != 0) {
    for (minmana = 0; minmana <= mWConstr; minmana++) {
      workspace->data[minmana] = workingset->bwset[minmana];
      workspace->data[minmana + workspace->size[0]] = workingset->bwset[minmana];
    }

    if (workingset->nActiveConstr != 0) {
      iy = 0;
      i = workingset->ATwset->size[0] * (workingset->nActiveConstr - 1) + 1;
      for (iac = 1; ldw < 0 ? iac >= i : iac <= i; iac += ldw) {
        ix = 0;
        c = 0.0f;
        i1 = (iac + nVar) - 1;
        for (ia = iac; ia <= i1; ia++) {
          c += workingset->ATwset->data[ia - 1] * xCurrent->data[ix];
          ix++;
        }

        workspace->data[iy] += -c;
        iy++;
      }
    }

    emxInit_real_T(&B, 2);
    if (workingset->nActiveConstr >= workingset->nVar) {
      for (minmana = 0; minmana < nVar; minmana++) {
        for (ix = 0; ix <= mWConstr; ix++) {
          qrmanager->QR->data[ix + qrmanager->QR->size[0] * minmana] =
            workingset->ATwset->data[minmana + workingset->ATwset->size[0] * ix];
        }
      }

      qrmanager->usedPivoting = false;
      qrmanager->mrows = workingset->nActiveConstr;
      qrmanager->ncols = workingset->nVar;
      i = workingset->nVar;
      for (minmana = 0; minmana < i; minmana++) {
        qrmanager->jpvt->data[minmana] = minmana + 1;
      }

      iy = workingset->nActiveConstr;
      ix = workingset->nVar;
      if (iy < ix) {
        ix = iy;
      }

      qrmanager->minRowCol = ix;
      i = B->size[0] * B->size[1];
      B->size[0] = qrmanager->QR->size[0];
      B->size[1] = qrmanager->QR->size[1];
      emxEnsureCapacity_real_T(B, i);
      iy = qrmanager->QR->size[0] * qrmanager->QR->size[1];
      for (i = 0; i < iy; i++) {
        B->data[i] = qrmanager->QR->data[i];
      }

      iy = qrmanager->QR->size[0];
      minmana = qrmanager->QR->size[1];
      if (iy < minmana) {
        minmana = iy;
      }

      i = qrmanager->tau->size[0];
      qrmanager->tau->size[0] = minmana;
      emxEnsureCapacity_real_T(qrmanager->tau, i);
      for (i = 0; i < minmana; i++) {
        qrmanager->tau->data[i] = 0.0f;
      }

      if (ix >= 1) {
        qrf(B, workingset->nActiveConstr, workingset->nVar, ix, qrmanager->tau);
      }

      i = qrmanager->QR->size[0] * qrmanager->QR->size[1];
      qrmanager->QR->size[0] = B->size[0];
      qrmanager->QR->size[1] = B->size[1];
      emxEnsureCapacity_real_T(qrmanager->QR, i);
      iy = B->size[0] * B->size[1];
      for (i = 0; i < iy; i++) {
        qrmanager->QR->data[i] = B->data[i];
      }

      computeQ_(qrmanager, workingset->nActiveConstr);
      ldq = qrmanager->ldq;
      ldw = workspace->size[0];
      i = B->size[0] * B->size[1];
      B->size[0] = workspace->size[0];
      B->size[1] = workspace->size[1];
      emxEnsureCapacity_real_T(B, i);
      iy = workspace->size[0] * workspace->size[1];
      for (i = 0; i < iy; i++) {
        B->data[i] = workspace->data[i];
      }

      for (mLB = 0; ldw < 0 ? mLB >= ldw : mLB <= ldw; mLB += ldw) {
        i = mLB + 1;
        i1 = mLB + nVar;
        for (iac = i; iac <= i1; iac++) {
          workspace->data[iac - 1] = 0.0f;
        }
      }

      iy = -1;
      for (mLB = 0; ldw < 0 ? mLB >= ldw : mLB <= ldw; mLB += ldw) {
        ix = -1;
        i = mLB + 1;
        i1 = mLB + nVar;
        for (iac = i; iac <= i1; iac++) {
          c = 0.0f;
          for (minmana = 0; minmana <= mWConstr; minmana++) {
            c += qrmanager->Q->data[(minmana + ix) + 1] * B->data[(minmana + iy)
              + 1];
          }

          workspace->data[iac - 1] += c;
          ix += ldq;
        }

        iy += ldw;
      }

      for (iac = nVar; iac >= 1; iac--) {
        minmana = ldq * (iac - 1) - 1;
        if (workspace->data[iac + -1] != 0.0f) {
          workspace->data[iac + -1] /= qrmanager->QR->data[iac + minmana];
          for (ix = 0; ix <= iac - 2; ix++) {
            workspace->data[ix] -= workspace->data[iac + -1] * qrmanager->
              QR->data[(ix + minmana) + 1];
          }
        }
      }

      for (iac = nVar; iac >= 1; iac--) {
        minmana = ldq * (iac - 1) - 1;
        i = (iac + ldw) - 1;
        if (workspace->data[i] != 0.0f) {
          workspace->data[i] /= qrmanager->QR->data[iac + minmana];
          for (ix = 0; ix <= iac - 2; ix++) {
            i1 = ix + ldw;
            workspace->data[i1] -= workspace->data[i] * qrmanager->QR->data[(ix
              + minmana) + 1];
          }
        }
      }
    } else {
      factorQR(qrmanager, workingset->ATwset, workingset->nVar,
               workingset->nActiveConstr);
      computeQ_(qrmanager, qrmanager->minRowCol);
      ldq = qrmanager->ldq;
      ldw = workspace->size[0];
      for (ix = 0; ix <= mWConstr; ix++) {
        iy = ldq * ix;
        c = workspace->data[ix];
        for (iac = 0; iac < ix; iac++) {
          c -= qrmanager->QR->data[iac + iy] * workspace->data[iac];
        }

        workspace->data[ix] = c / qrmanager->QR->data[ix + iy];
      }

      for (ix = 0; ix <= mWConstr; ix++) {
        iy = ldq * ix;
        minmana = ix + ldw;
        c = workspace->data[minmana];
        for (iac = 0; iac < ix; iac++) {
          c -= qrmanager->QR->data[iac + iy] * workspace->data[iac + ldw];
        }

        workspace->data[minmana] = c / qrmanager->QR->data[ix + iy];
      }

      i = B->size[0] * B->size[1];
      B->size[0] = workspace->size[0];
      B->size[1] = workspace->size[1];
      emxEnsureCapacity_real_T(B, i);
      iy = workspace->size[0] * workspace->size[1];
      for (i = 0; i < iy; i++) {
        B->data[i] = workspace->data[i];
      }

      for (mLB = 0; ldw < 0 ? mLB >= ldw : mLB <= ldw; mLB += ldw) {
        i = mLB + 1;
        i1 = mLB + nVar;
        for (iac = i; iac <= i1; iac++) {
          workspace->data[iac - 1] = 0.0;
        }
      }

      iy = 1;
      for (mLB = 0; ldw < 0 ? mLB >= ldw : mLB <= ldw; mLB += ldw) {
        ix = -1;
        i = iy + mWConstr;
        for (ix0_2 = iy; ix0_2 <= i; ix0_2++) {
          ia = ix;
          i1 = mLB + 1;
          minmana = mLB + nVar;
          for (iac = i1; iac <= minmana; iac++) {
            ia++;
            workspace->data[iac - 1] += B->data[ix0_2 - 1] * qrmanager->Q->
              data[ia];
          }

          ix += ldq;
        }

        iy += ldw;
      }
    }

    emxFree_real_T(&B);
    minmana = 0;
    do {
      exitg1 = 0;
      if (minmana <= nVar - 1) {
        c = workspace->data[minmana];
        if (rtIsInf(c) || rtIsNaN(c)) {
          nonDegenerateWset = false;
          exitg1 = 1;
        } else {
          c = workspace->data[minmana + workspace->size[0]];
          if (rtIsInf(c) || rtIsNaN(c)) {
            nonDegenerateWset = false;
            exitg1 = 1;
          } else {
            minmana++;
          }
        }
      } else {
        iy = nVar - 1;
        for (iac = 0; iac <= iy; iac++) {
          workspace->data[iac] += xCurrent->data[iac];
        }

        ix0_2 = workspace->size[0];
        mLB = workingset->sizes[3];
        switch (workingset->probType) {
         case 2:
          v = 0.0f;
          memset(&workingset->maxConstrWorkspace[0], 0, 120U * sizeof(float));
          xgemv(workingset->nVarOrig, workingset->Aineq, workingset->ldA,
                workspace, workingset->maxConstrWorkspace);
          for (minmana = 0; minmana < 120; minmana++) {
            workingset->maxConstrWorkspace[minmana] -= workspace->
              data[workingset->nVarOrig + minmana];
            v = fmax(v, workingset->maxConstrWorkspace[minmana]);
          }
          break;

         default:
          v = 0.0f;
          memset(&workingset->maxConstrWorkspace[0], 0, 120U * sizeof(float));
          xgemv(workingset->nVar, workingset->Aineq, workingset->ldA, workspace,
                workingset->maxConstrWorkspace);
          for (minmana = 0; minmana < 120; minmana++) {
            v = fmaxf(v, workingset->maxConstrWorkspace[minmana]);
          }
          break;
        }

        if (workingset->sizes[3] > 0) {
          for (minmana = 0; minmana < mLB; minmana++) {
            iy = workingset->indexLB->data[minmana] - 1;
            v = fmaxf(v, -workspace->data[iy] - workingset->lb->data[iy]);
          }
        }

        mLB = workingset->sizes[3];
        switch (workingset->probType) {
         case 2:
          b_v = 0.0f;
          minmana = workingset->ldA;
          for (iac = 0; iac < 120; iac++) {
            workingset->maxConstrWorkspace[iac] = 0.0f;
            workingset->maxConstrWorkspace[iac] =
              -workingset->maxConstrWorkspace[iac];
          }

          iy = 0;
          i = workingset->ldA * 119 + 1;
          for (iac = 1; minmana < 0 ? iac >= i : iac <= i; iac += minmana) {
            ix = workspace->size[0];
            c = 0.0f;
            i1 = (iac + workingset->nVarOrig) - 1;
            for (ia = iac; ia <= i1; ia++) {
              c += workingset->Aineq->data[ia - 1] * workspace->data[ix];
              ix++;
            }

            workingset->maxConstrWorkspace[iy] += c;
            iy++;
          }

          for (minmana = 0; minmana < 120; minmana++) {
            workingset->maxConstrWorkspace[minmana] -= workspace->data[(ix0_2 +
              workingset->nVarOrig) + minmana];
            b_v = fmaxf(b_v, workingset->maxConstrWorkspace[minmana]);
          }
          break;

         default:
          b_v = 0.0f;
          minmana = workingset->ldA;
          for (iac = 0; iac < 120; iac++) {
            workingset->maxConstrWorkspace[iac] = 0.0f;
            workingset->maxConstrWorkspace[iac] =
              -workingset->maxConstrWorkspace[iac];
          }

          iy = 0;
          i = workingset->ldA * 119 + 1;
          for (iac = 1; minmana < 0 ? iac >= i : iac <= i; iac += minmana) {
            ix = workspace->size[0];
            c = 0.0f;
            i1 = (iac + workingset->nVar) - 1;
            for (ia = iac; ia <= i1; ia++) {
              c += workingset->Aineq->data[ia - 1] * workspace->data[ix];
              ix++;
            }

            workingset->maxConstrWorkspace[iy] += c;
            iy++;
          }

          for (minmana = 0; minmana < 120; minmana++) {
            b_v = fmaxf(b_v, workingset->maxConstrWorkspace[minmana]);
          }
          break;
        }

        if (workingset->sizes[3] > 0) {
          for (minmana = 0; minmana < mLB; minmana++) {
            b_v = fmaxf(b_v, -workspace->data[(ix0_2 + workingset->indexLB->
                        data[minmana]) - 1] - workingset->lb->data
                       [workingset->indexLB->data[minmana] - 1]);
          }
        }

        if ((v <= 2.2204460492503131E-16f) || (v < b_v)) {
          for (iac = 0; iac < nVar; iac++) {
            xCurrent->data[iac] = workspace->data[iac];
          }
        } else {
          for (iac = 0; iac < nVar; iac++) {
            xCurrent->data[iac] = workspace->data[ix0_2 + iac];
          }
        }

        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  return nonDegenerateWset;
}

/* End of code generation (feasibleX0ForWorkingSet.c) */
