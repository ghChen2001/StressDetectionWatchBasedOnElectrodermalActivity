/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeGrad_StoreHx.c
 *
 * Code generation for function 'computeGrad_StoreHx'
 *
 */

/* Include files */
#include "computeGrad_StoreHx.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void computeGrad_StoreHx(e_struct_T *obj, const emxArray_real_T *H, const
  emxArray_real_T *f, const emxArray_real_T *x)
{
  int i;
  int i1;
  int ia;
  int iac;
  int ix;
  int ixlast;
  int iy;
  int lda;
  int maxRegVar;
  switch (obj->objtype) {
   case 5:
    i = obj->nvar;
    for (ixlast = 0; ixlast <= i - 2; ixlast++) {
      obj->grad->data[ixlast] = 0.0f;
    }

    obj->grad->data[obj->nvar - 1] = obj->gammaScalar;
    break;

   case 3:
    ixlast = obj->nvar - 1;
    lda = obj->nvar;
    if (obj->nvar != 0) {
      for (iy = 0; iy <= ixlast; iy++) {
        obj->Hx->data[iy] = 0.0f;
      }

      ix = 0;
      i = obj->nvar * (obj->nvar - 1) + 1;
      for (iac = 1; lda < 0 ? iac >= i : iac <= i; iac += lda) {
        iy = 0;
        i1 = iac + ixlast;
        for (ia = iac; ia <= i1; ia++) {
          obj->Hx->data[iy] += H->data[ia - 1] * x->data[ix];
          iy++;
        }

        ix++;
      }
    }

    i = obj->nvar;
    for (ixlast = 0; ixlast < i; ixlast++) {
      obj->grad->data[ixlast] = obj->Hx->data[ixlast];
    }

    if (obj->hasLinear && (obj->nvar >= 1)) {
      ixlast = obj->nvar - 1;
      for (lda = 0; lda <= ixlast; lda++) {
        obj->grad->data[lda] += f->data[lda];
      }
    }
    break;

   default:
    maxRegVar = obj->maxVar - 1;
    ixlast = obj->nvar - 1;
    lda = obj->nvar;
    if (obj->nvar != 0) {
      for (iy = 0; iy <= ixlast; iy++) {
        obj->Hx->data[iy] = 0.0f;
      }

      ix = 0;
      i = obj->nvar * (obj->nvar - 1) + 1;
      for (iac = 1; lda < 0 ? iac >= i : iac <= i; iac += lda) {
        iy = 0;
        i1 = iac + ixlast;
        for (ia = iac; ia <= i1; ia++) {
          obj->Hx->data[iy] += H->data[ia - 1] * x->data[ix];
          iy++;
        }

        ix++;
      }
    }

    i = obj->nvar + 1;
    for (ixlast = i; ixlast <= maxRegVar; ixlast++) {
      obj->Hx->data[ixlast - 1] = 0.0f * x->data[ixlast - 1];
    }

    for (ixlast = 0; ixlast < maxRegVar; ixlast++) {
      obj->grad->data[ixlast] = obj->Hx->data[ixlast];
    }

    if (obj->hasLinear && (obj->nvar >= 1)) {
      ixlast = obj->nvar - 1;
      for (lda = 0; lda <= ixlast; lda++) {
        obj->grad->data[lda] += f->data[lda];
      }
    }
    break;
  }
}

/* End of code generation (computeGrad_StoreHx.c) */
