/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeFirstOrderOpt.c
 *
 * Code generation for function 'computeFirstOrderOpt'
 *
 */

/* Include files */
#include "computeFirstOrderOpt.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void computeFirstOrderOpt(c_struct_T *solution, const e_struct_T *objective, int
  workingset_nVar, int workingset_ldA, const emxArray_real_T *workingset_ATwset,
  int workingset_nActiveConstr, emxArray_real_T *workspace)
{
  float s;
  float smax;
  int i;
  int ia;
  int iac;
  int idxmax;
  int ix;
  int iy;
  for (iy = 0; iy < workingset_nVar; iy++) {
    workspace->data[iy] = objective->grad->data[iy];
  }

  if (workingset_nActiveConstr != 0) {
    ix = 0;
    idxmax = workingset_ldA * (workingset_nActiveConstr - 1) + 1;
    for (iac = 1; workingset_ldA < 0 ? iac >= idxmax : iac <= idxmax; iac +=
         workingset_ldA) {
      iy = 0;
      i = (iac + workingset_nVar) - 1;
      for (ia = iac; ia <= i; ia++) {
        workspace->data[iy] += workingset_ATwset->data[ia - 1] *
          solution->lambda[ix];
        iy++;
      }

      ix++;
    }
  }

  if (workingset_nVar < 1) {
    idxmax = 0;
  } else {
    idxmax = 1;
    if (workingset_nVar > 1) {
      ix = 0;
      smax = fabsf(workspace->data[0]);
      for (iy = 2; iy <= workingset_nVar; iy++) {
        ix++;
        s = fabsf(workspace->data[ix]);
        if (s > smax) {
          idxmax = iy;
          smax = s;
        }
      }
    }
  }

  solution->firstorderopt = fabsf(workspace->data[idxmax - 1]);
}

/* End of code generation (computeFirstOrderOpt.c) */
