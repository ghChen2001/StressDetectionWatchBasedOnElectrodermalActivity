/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * linearForm_.c
 *
 * Code generation for function 'linearForm_'
 *
 */

/* Include files */
#include "linearForm_.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void linearForm_(boolean_T obj_hasLinear, int obj_nvar, emxArray_real_T
                 *workspace, const emxArray_real_T *H, const emxArray_real_T *f,
                 const emxArray_real_T *x)
{
  float c;
  int i;
  int i1;
  int ia;
  int iac;
  int ix;
  int iy;
  ix = 0;
  if (obj_hasLinear) {
    for (ix = 0; ix < obj_nvar; ix++) {
      workspace->data[ix] = f->data[ix];
    }

    ix = 1;
  }

  if (obj_nvar != 0) {
    if (ix != 1) {
      for (iy = 0; iy < obj_nvar; iy++) {
        workspace->data[iy] = 0.0f;
      }
    }

    ix = 0;
    i = obj_nvar * (obj_nvar - 1) + 1;
    for (iac = 1; obj_nvar < 0 ? iac >= i : iac <= i; iac += obj_nvar) {
      c = 0.5f * x->data[ix];
      iy = 0;
      i1 = (iac + obj_nvar) - 1;
      for (ia = iac; ia <= i1; ia++) {
        workspace->data[iy] += H->data[ia - 1] * c;
        iy++;
      }

      ix++;
    }
  }
}

/* End of code generation (linearForm_.c) */
