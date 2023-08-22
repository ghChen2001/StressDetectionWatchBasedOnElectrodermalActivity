/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeFval_ReuseHx.c
 *
 * Code generation for function 'computeFval_ReuseHx'
 *
 */

/* Include files */
#include "computeFval_ReuseHx.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
float computeFval_ReuseHx(const e_struct_T *obj, emxArray_real_T *workspace,
  const emxArray_real_T *f, const emxArray_real_T *x)
{
  float val;
  int ixlast;
  int k;
  int maxRegVar;
  switch (obj->objtype) {
   case 5:
    val = obj->gammaScalar * x->data[obj->nvar - 1];
    break;

   case 3:
    if (obj->hasLinear) {
      ixlast = obj->nvar;
      for (k = 0; k < ixlast; k++) {
        workspace->data[k] = 0.5 * obj->Hx->data[k] + f->data[k];
      }

      val = 0.0f;
      if (obj->nvar >= 1) {
        ixlast = obj->nvar;
        for (k = 0; k < ixlast; k++) {
          val += x->data[k] * workspace->data[k];
        }
      }
    } else {
      val = 0.0f;
      if (obj->nvar >= 1) {
        ixlast = obj->nvar;
        for (k = 0; k < ixlast; k++) {
          val += x->data[k] * obj->Hx->data[k];
        }
      }

      val *= 0.5f;
    }
    break;

   default:
    maxRegVar = obj->maxVar - 1;
    if (obj->hasLinear) {
      ixlast = obj->nvar;
      for (k = 0; k < ixlast; k++) {
        workspace->data[k] = f->data[k];
      }

      ixlast = obj->maxVar - obj->nvar;
      for (k = 0; k <= ixlast - 2; k++) {
        workspace->data[obj->nvar + k] = 0.0;
      }

      for (k = 0; k < maxRegVar; k++) {
        workspace->data[k] += 0.5 * obj->Hx->data[k];
      }

      val = 0.0f;
      if (obj->maxVar - 1 >= 1) {
        ixlast = obj->maxVar;
        for (k = 0; k <= ixlast - 2; k++) {
          val += x->data[k] * workspace->data[k];
        }
      }
    } else {
      val = 0.0f;
      if (obj->maxVar - 1 >= 1) {
        ixlast = obj->maxVar;
        for (k = 0; k <= ixlast - 2; k++) {
          val += x->data[k] * obj->Hx->data[k];
        }
      }

      val *= 0.5f;
      ixlast = obj->nvar + 1;
      for (k = ixlast; k <= maxRegVar; k++) {
        val += x->data[k - 1] * 0.0f;
      }
    }
    break;
  }

  return val;
}

/* End of code generation (computeFval_ReuseHx.c) */
