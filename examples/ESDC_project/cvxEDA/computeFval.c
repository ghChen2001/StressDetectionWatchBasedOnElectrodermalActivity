/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeFval.c
 *
 * Code generation for function 'computeFval'
 *
 */

/* Include files */
#include "computeFval.h"
#include "cvxEDA_types.h"
#include "linearForm_.h"
#include "rt_nonfinite.h"

/* Function Definitions */
float computeFval(const e_struct_T *obj, emxArray_real_T *workspace, const
                   emxArray_real_T *H, const emxArray_real_T *f, const
                   emxArray_real_T *x)
{
  float val;
  int idx;
  int ixlast;
  int k;
  switch (obj->objtype) {
   case 5:
    val = obj->gammaScalar * x->data[obj->nvar - 1];
    break;

   case 3:
    linearForm_(obj->hasLinear, obj->nvar, workspace, H, f, x);
    val = 0.0f;
    if (obj->nvar >= 1) {
      ixlast = obj->nvar;
      for (k = 0; k < ixlast; k++) {
        val += x->data[k] * workspace->data[k];
      }
    }
    break;

   default:
    linearForm_(obj->hasLinear, obj->nvar, workspace, H, f, x);
    ixlast = obj->nvar + 1;
    k = obj->maxVar - 1;
    for (idx = ixlast; idx <= k; idx++) {
      workspace->data[idx - 1] = 0.0f * x->data[idx - 1];
    }

    val = 0.0f;
    if (obj->maxVar - 1 >= 1) {
      ixlast = obj->maxVar;
      for (k = 0; k <= ixlast - 2; k++) {
        val += x->data[k] * workspace->data[k];
      }
    }
    break;
  }

  return val;
}

/* End of code generation (computeFval.c) */
