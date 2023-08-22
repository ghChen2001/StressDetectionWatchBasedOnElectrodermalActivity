/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * feasibleratiotest.c
 *
 * Code generation for function 'feasibleratiotest'
 *
 */

/* Include files */
#include "feasibleratiotest.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xnrm2.h"
#include <math.h>

/* Function Definitions */
void feasibleratiotest(const emxArray_real_T *solution_xstar, const
  emxArray_real_T *solution_searchDir, emxArray_real_T *workspace, int
  workingset_nVar, int workingset_ldA, const emxArray_real_T *workingset_Aineq,
  const emxArray_real_T *workingset_lb, const emxArray_int32_T
  *workingset_indexLB, const int workingset_sizes[5], const int
  workingset_isActiveIdx[6], const boolean_T workingset_isActiveConstr[121],
  const int workingset_nWConstr[5], boolean_T isPhaseOne, float *alpha,
  boolean_T *newBlocking, int *constrType, int *constrIdx)
{
  float c;
  float denomTol;
  float phaseOneCorrectionP;
  float phaseOneCorrectionX;
  float ratio;
  int b_iy;
  int i;
  int ia;
  int iac;
  int iy;
  int iyend;
  int ldw;
  *alpha = 1.0E+30f;
  *newBlocking = false;
  *constrType = 0;
  *constrIdx = 0;
  denomTol = 2.2204460492503131E-8f * b_xnrm2(workingset_nVar,
    solution_searchDir);
  if (workingset_nWConstr[2] < 120) {
    for (iyend = 0; iyend < 120; iyend++) {
      workspace->data[iyend] = 0.0f;
    }

    b_xgemv(workingset_nVar, workingset_Aineq, workingset_ldA, solution_xstar,
            workspace);
    ldw = workspace->size[0];
    iy = workspace->size[0];
    iyend = workspace->size[0] + 120;
    for (b_iy = iy + 1; b_iy <= iyend; b_iy++) {
      workspace->data[b_iy - 1] = 0.0f;
    }

    i = workingset_ldA * 119 + 1;
    for (iac = 1; workingset_ldA < 0 ? iac >= i : iac <= i; iac +=
         workingset_ldA) {
      iyend = 0;
      c = 0.0f;
      b_iy = (iac + workingset_nVar) - 1;
      for (ia = iac; ia <= b_iy; ia++) {
        c += workingset_Aineq->data[ia - 1] * solution_searchDir->data[iyend];
        iyend++;
      }

      workspace->data[iy] += c;
      iy++;
    }

    for (iyend = 0; iyend < 120; iyend++) {
      i = ldw + iyend;
      if ((workspace->data[i] > denomTol) && (!workingset_isActiveConstr
           [(workingset_isActiveIdx[2] + iyend) - 1])) {
        c = fminf(fabsf(workspace->data[iyend]), 1.0E-8f - workspace->data[iyend]) /
          workspace->data[i];
        if (c < *alpha) {
          *alpha = c;
          *constrType = 3;
          *constrIdx = iyend + 1;
          *newBlocking = true;
        }
      }
    }
  }

  if (workingset_nWConstr[3] < workingset_sizes[3]) {
    phaseOneCorrectionX = (float)isPhaseOne * solution_xstar->
      data[workingset_nVar - 1];
    phaseOneCorrectionP = (float)isPhaseOne * solution_searchDir->
      data[workingset_nVar - 1];
    i = workingset_sizes[3];
    for (iyend = 0; iyend <= i - 2; iyend++) {
      c = -solution_searchDir->data[workingset_indexLB->data[iyend] - 1] -
        phaseOneCorrectionP;
      if ((c > denomTol) && (!workingset_isActiveConstr[(workingset_isActiveIdx
            [3] + iyend) - 1])) {
        ratio = (-solution_xstar->data[workingset_indexLB->data[iyend] - 1] -
                 workingset_lb->data[workingset_indexLB->data[iyend] - 1]) -
          phaseOneCorrectionX;
        c = fminf(fabsf(ratio), 1.0E-8f - ratio) / c;
        if (c < *alpha) {
          *alpha = c;
          *constrType = 4;
          *constrIdx = iyend + 1;
          *newBlocking = true;
        }
      }
    }

    i = workingset_indexLB->data[workingset_sizes[3] - 1] - 1;
    c = -solution_searchDir->data[i];
    if ((c > denomTol) && (!workingset_isActiveConstr[(workingset_isActiveIdx[3]
          + workingset_sizes[3]) - 2])) {
      ratio = -solution_xstar->data[i] - workingset_lb->data[i];
      c = fminf(fabsf(ratio), 1.0E-8f - ratio) / c;
      if (c < *alpha) {
        *alpha = c;
        *constrType = 4;
        *constrIdx = workingset_sizes[3];
        *newBlocking = true;
      }
    }
  }

  if (!isPhaseOne) {
    if ((*newBlocking) && (*alpha > 1.0f)) {
      *newBlocking = false;
    }

    *alpha = fminf(*alpha, 1.0f);
  }
}

/* End of code generation (feasibleratiotest.c) */
