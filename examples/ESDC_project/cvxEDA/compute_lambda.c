/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * compute_lambda.c
 *
 * Code generation for function 'compute_lambda'
 *
 */

/* Include files */
#include "compute_lambda.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void compute_lambda(emxArray_real_T *workspace, c_struct_T *solution, const
                    e_struct_T *objective, const f_struct_T *qrmanager)
{
  float c;
  int ia;
  int iac;
  int idx;
  int ix;
  int iy;
  int j;
  int ldq;
  int nActiveConstr;
  boolean_T guard1 = false;
  boolean_T nonDegenerate;
  nActiveConstr = qrmanager->ncols;
  if (qrmanager->ncols > 0) {
    c = 100.0f * (float)qrmanager->mrows * 2.2204460492503131E-16f;
    if ((qrmanager->mrows > 0) && (qrmanager->ncols > 0)) {
      nonDegenerate = true;
    } else {
      nonDegenerate = false;
    }

    if (nonDegenerate) {
      idx = qrmanager->ncols;
      guard1 = false;
      if (qrmanager->mrows < qrmanager->ncols) {
        while ((idx > qrmanager->mrows) && (fabsf(qrmanager->QR->data
                 [(qrmanager->mrows + qrmanager->QR->size[0] * (idx - 1)) - 1]) >=
                c)) {
          idx--;
        }

        nonDegenerate = (idx == qrmanager->mrows);
        if (nonDegenerate) {
          guard1 = true;
        }
      } else {
        guard1 = true;
      }

      if (guard1) {
        while ((idx >= 1) && (fabsf(qrmanager->QR->data[(idx + qrmanager->
                  QR->size[0] * (idx - 1)) - 1]) >= c)) {
          idx--;
        }

        nonDegenerate = (idx == 0);
      }
    }

    if (!nonDegenerate) {
      solution->state = -7;
    } else {
      ldq = qrmanager->ldq;
      if (qrmanager->mrows != 0) {
        idx = qrmanager->ncols;
        for (iy = 0; iy < idx; iy++) {
          workspace->data[iy] = 0.0;
        }

        iy = 0;
        idx = qrmanager->ldq * (qrmanager->ncols - 1) + 1;
        for (iac = 1; ldq < 0 ? iac >= idx : iac <= idx; iac += ldq) {
          ix = 0;
          c = 0.0;
          j = (iac + qrmanager->mrows) - 1;
          for (ia = iac; ia <= j; ia++) {
            c += qrmanager->Q->data[ia - 1] * objective->grad->data[ix];
            ix++;
          }

          workspace->data[iy] += c;
          iy++;
        }
      }

      for (j = nActiveConstr; j >= 1; j--) {
        idx = (j + (j - 1) * ldq) - 1;
        workspace->data[j - 1] /= qrmanager->QR->data[idx];
        for (iac = 0; iac <= j - 2; iac++) {
          ix = (j - iac) - 2;
          workspace->data[ix] -= workspace->data[j - 1] * qrmanager->QR->data
            [(idx - iac) - 1];
        }
      }

      for (idx = 0; idx < nActiveConstr; idx++) {
        solution->lambda[idx] = -workspace->data[idx];
      }
    }
  }
}

/* End of code generation (compute_lambda.c) */
