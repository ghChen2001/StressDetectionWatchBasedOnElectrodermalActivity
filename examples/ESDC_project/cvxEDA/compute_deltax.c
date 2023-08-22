/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * compute_deltax.c
 *
 * Code generation for function 'compute_deltax'
 *
 */

/* Include files */
#include "compute_deltax.h"
#include "cvxEDA_types.h"
#include "factor.h"
#include "fullColLDL2_.h"
#include "partialColLDL3_.h"
#include "rt_nonfinite.h"
#include "solve.h"
#include "xgemm.h"
#include <math.h>

/* Function Definitions */
void compute_deltax(const emxArray_real_T *H, c_struct_T *solution, d_struct_T
                    *memspace, const f_struct_T *qrmanager, g_struct_T
                    *cholmanager, const e_struct_T *objective)
{
  float SCALED_REG_PRIMAL;
  float s;
  float smax;
  int LDimSizeP1;
  int exitg2;
  int i;
  int ia;
  int iac;
  int idx_row;
  int ix;
  int ldw;
  int mNull_tmp;
  int nVar;
  int nVars;
  int nullStartIdx_tmp;
  boolean_T exitg1;
  nVar = qrmanager->mrows - 1;
  mNull_tmp = qrmanager->mrows - qrmanager->ncols;
  if (mNull_tmp <= 0) {
    for (nVars = 0; nVars <= nVar; nVars++) {
      solution->searchDir->data[nVars] = 0.0;
    }
  } else {
    for (nVars = 0; nVars <= nVar; nVars++) {
      solution->searchDir->data[nVars] = -objective->grad->data[nVars];
    }

    if (qrmanager->ncols <= 0) {
      switch (objective->objtype) {
       case 5:
        break;

       case 3:
        factor(cholmanager, H, qrmanager->mrows, qrmanager->mrows);
        if (cholmanager->info != 0) {
          solution->state = -6;
        } else {
          solve(cholmanager, solution->searchDir);
        }
        break;

       default:
        factor(cholmanager, H, objective->nvar, objective->nvar);
        if (cholmanager->info != 0) {
          solution->state = -6;
        } else {
          solve(cholmanager, solution->searchDir);
          nVars = objective->nvar + 1;
          i = qrmanager->mrows;
          for (idx_row = nVars; idx_row <= i; idx_row++) {
            solution->searchDir->data[idx_row - 1] *= rtInf;
          }
        }
        break;
      }
    } else {
      nullStartIdx_tmp = qrmanager->ldq * qrmanager->ncols + 1;
      switch (objective->objtype) {
       case 5:
        for (nVars = 0; nVars < mNull_tmp; nVars++) {
          memspace->workspace_double->data[nVars] = -qrmanager->Q->data[nVar +
            qrmanager->Q->size[0] * (qrmanager->ncols + nVars)];
        }

        nVars = qrmanager->mrows - 1;
        ldw = qrmanager->ldq;
        if (qrmanager->mrows != 0) {
          for (idx_row = 0; idx_row <= nVars; idx_row++) {
            solution->searchDir->data[idx_row] = 0.0;
          }

          ix = 0;
          i = nullStartIdx_tmp + qrmanager->ldq * (mNull_tmp - 1);
          for (iac = nullStartIdx_tmp; ldw < 0 ? iac >= i : iac <= i; iac += ldw)
          {
            idx_row = 0;
            LDimSizeP1 = iac + nVars;
            for (ia = iac; ia <= LDimSizeP1; ia++) {
              solution->searchDir->data[idx_row] += qrmanager->Q->data[ia - 1] *
                memspace->workspace_double->data[ix];
              idx_row++;
            }

            ix++;
          }
        }
        break;

       default:
        switch (objective->objtype) {
         case 3:
          ldw = memspace->workspace_double->size[0];
          xgemm(qrmanager->mrows, mNull_tmp, qrmanager->mrows, H,
                qrmanager->mrows, qrmanager->Q, nullStartIdx_tmp, qrmanager->ldq,
                memspace->workspace_double, memspace->workspace_double->size[0]);
          b_xgemm(mNull_tmp, mNull_tmp, qrmanager->mrows, qrmanager->Q,
                  nullStartIdx_tmp, qrmanager->ldq, memspace->workspace_double,
                  ldw, cholmanager->FMat, cholmanager->ldm);
          break;

         default:
          nVars = qrmanager->mrows;
          ldw = memspace->workspace_double->size[0];
          xgemm(objective->nvar, mNull_tmp, objective->nvar, H, objective->nvar,
                qrmanager->Q, nullStartIdx_tmp, qrmanager->ldq,
                memspace->workspace_double, memspace->workspace_double->size[0]);
          for (LDimSizeP1 = 0; LDimSizeP1 < mNull_tmp; LDimSizeP1++) {
            i = objective->nvar + 1;
            for (idx_row = i; idx_row <= nVars; idx_row++) {
              memspace->workspace_double->data[(idx_row +
                memspace->workspace_double->size[0] * LDimSizeP1) - 1] = 0.0 *
                qrmanager->Q->data[(idx_row + qrmanager->Q->size[0] *
                                    (LDimSizeP1 + qrmanager->ncols)) - 1];
            }
          }

          b_xgemm(mNull_tmp, mNull_tmp, qrmanager->mrows, qrmanager->Q,
                  nullStartIdx_tmp, qrmanager->ldq, memspace->workspace_double,
                  ldw, cholmanager->FMat, cholmanager->ldm);
          break;
        }

        SCALED_REG_PRIMAL = 1.4901161193847656E-6 * (float)mNull_tmp;
        LDimSizeP1 = cholmanager->ldm + 1;
        cholmanager->ndims = mNull_tmp;
        nVars = 1;
        if (mNull_tmp > 1) {
          ix = 0;
          smax = fabsf(cholmanager->FMat->data[0]);
          for (idx_row = 2; idx_row <= mNull_tmp; idx_row++) {
            ix += LDimSizeP1;
            s = fabsf(cholmanager->FMat->data[ix]);
            if (s > smax) {
              nVars = idx_row;
              smax = s;
            }
          }
        }

        cholmanager->regTol_ = fmaxf(fabsf(cholmanager->FMat->data[nVars *
          (cholmanager->ldm + 1) - 1]) * 2.2204460492503131e-8f,
          SCALED_REG_PRIMAL);
        if (mNull_tmp > 128) {
          idx_row = 0;
          exitg1 = false;
          while ((!exitg1) && (idx_row < mNull_tmp)) {
            nVars = LDimSizeP1 * idx_row + 1;
            ldw = mNull_tmp - idx_row;
            if (idx_row + 48 <= mNull_tmp) {
              partialColLDL3_(cholmanager, nVars, ldw, SCALED_REG_PRIMAL);
              idx_row += 48;
            } else {
              fullColLDL2_(cholmanager, nVars, ldw, SCALED_REG_PRIMAL);
              exitg1 = true;
            }
          }
        } else {
          fullColLDL2_(cholmanager, 1, mNull_tmp, SCALED_REG_PRIMAL);
        }

        if (cholmanager->ConvexCheck) {
          nVars = 0;
          do {
            exitg2 = 0;
            if (nVars <= mNull_tmp - 1) {
              if (cholmanager->FMat->data[nVars + cholmanager->FMat->size[0] *
                  nVars] <= 0.0) {
                cholmanager->info = -nVars - 1;
                exitg2 = 1;
              } else {
                nVars++;
              }
            } else {
              cholmanager->ConvexCheck = false;
              exitg2 = 1;
            }
          } while (exitg2 == 0);
        }

        if (cholmanager->info != 0) {
          solution->state = -6;
        } else {
          ldw = qrmanager->ldq;
          if (qrmanager->mrows != 0) {
            for (idx_row = 0; idx_row < mNull_tmp; idx_row++) {
              memspace->workspace_double->data[idx_row] = 0.0;
            }

            idx_row = 0;
            i = nullStartIdx_tmp + qrmanager->ldq * (mNull_tmp - 1);
            for (iac = nullStartIdx_tmp; ldw < 0 ? iac >= i : iac <= i; iac +=
                 ldw) {
              ix = 0;
              SCALED_REG_PRIMAL = 0.0;
              LDimSizeP1 = iac + nVar;
              for (ia = iac; ia <= LDimSizeP1; ia++) {
                SCALED_REG_PRIMAL += qrmanager->Q->data[ia - 1] *
                  objective->grad->data[ix];
                ix++;
              }

              memspace->workspace_double->data[idx_row] += -SCALED_REG_PRIMAL;
              idx_row++;
            }
          }

          ldw = cholmanager->ndims - 2;
          if (cholmanager->ndims != 0) {
            for (LDimSizeP1 = 0; LDimSizeP1 <= ldw + 1; LDimSizeP1++) {
              nVars = LDimSizeP1 + LDimSizeP1 * cholmanager->ldm;
              i = ldw - LDimSizeP1;
              for (idx_row = 0; idx_row <= i; idx_row++) {
                ix = (LDimSizeP1 + idx_row) + 1;
                memspace->workspace_double->data[ix] -=
                  memspace->workspace_double->data[LDimSizeP1] *
                  cholmanager->FMat->data[(nVars + idx_row) + 1];
              }
            }
          }

          i = cholmanager->ndims;
          for (nVars = 0; nVars < i; nVars++) {
            memspace->workspace_double->data[nVars] /= cholmanager->FMat->
              data[nVars + cholmanager->FMat->size[0] * nVars];
          }

          ldw = cholmanager->ndims;
          if (cholmanager->ndims != 0) {
            for (LDimSizeP1 = ldw; LDimSizeP1 >= 1; LDimSizeP1--) {
              nVars = (LDimSizeP1 - 1) * cholmanager->ldm;
              SCALED_REG_PRIMAL = memspace->workspace_double->data[LDimSizeP1 -
                1];
              i = LDimSizeP1 + 1;
              for (idx_row = ldw; idx_row >= i; idx_row--) {
                SCALED_REG_PRIMAL -= cholmanager->FMat->data[(nVars + idx_row) -
                  1] * memspace->workspace_double->data[idx_row - 1];
              }

              memspace->workspace_double->data[LDimSizeP1 - 1] =
                SCALED_REG_PRIMAL;
            }
          }

          nVars = qrmanager->mrows - 1;
          ldw = qrmanager->ldq;
          if (qrmanager->mrows != 0) {
            for (idx_row = 0; idx_row <= nVars; idx_row++) {
              solution->searchDir->data[idx_row] = 0.0;
            }

            ix = 0;
            i = nullStartIdx_tmp + qrmanager->ldq * (mNull_tmp - 1);
            for (iac = nullStartIdx_tmp; ldw < 0 ? iac >= i : iac <= i; iac +=
                 ldw) {
              idx_row = 0;
              LDimSizeP1 = iac + nVars;
              for (ia = iac; ia <= LDimSizeP1; ia++) {
                solution->searchDir->data[idx_row] += qrmanager->Q->data[ia - 1]
                  * memspace->workspace_double->data[ix];
                idx_row++;
              }

              ix++;
            }
          }
        }
        break;
      }
    }
  }
}

/* End of code generation (compute_deltax.c) */
