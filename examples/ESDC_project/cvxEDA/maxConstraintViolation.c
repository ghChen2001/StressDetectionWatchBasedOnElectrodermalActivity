/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * maxConstraintViolation.c
 *
 * Code generation for function 'maxConstraintViolation'
 *
 */

/* Include files */
#include "maxConstraintViolation.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
float maxConstraintViolation(h_struct_T *obj, const emxArray_real_T *x)
{
  float v;
  int idx;
  int idxLB;
  int mLB;
  mLB = obj->sizes[3];
  switch (obj->probType) {
   case 2:
    v = 0.0f;
    memset(&obj->maxConstrWorkspace[0], 0, 120U * sizeof(float));
    xgemv(obj->nVarOrig, obj->Aineq, obj->ldA, x, obj->maxConstrWorkspace);
    for (idx = 0; idx < 120; idx++) {
      obj->maxConstrWorkspace[idx] -= x->data[obj->nVarOrig + idx];
      v = fmaxf(v, obj->maxConstrWorkspace[idx]);
    }
    break;

   default:
    v = 0.0f;
    memset(&obj->maxConstrWorkspace[0], 0, 120U * sizeof(float));
    xgemv(obj->nVar, obj->Aineq, obj->ldA, x, obj->maxConstrWorkspace);
    for (idx = 0; idx < 120; idx++) {
      v = fmaxf(v, obj->maxConstrWorkspace[idx]);
    }
    break;
  }

  if (obj->sizes[3] > 0) {
    for (idx = 0; idx < mLB; idx++) {
      idxLB = obj->indexLB->data[idx] - 1;
      v = fmaxf(v, -x->data[idxLB] - obj->lb->data[idxLB]);
    }
  }

  return v;
}

/* End of code generation (maxConstraintViolation.c) */
