/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * addBoundToActiveSetMatrix_.c
 *
 * Code generation for function 'addBoundToActiveSetMatrix_'
 *
 */

/* Include files */
#include "addBoundToActiveSetMatrix_.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void addBoundToActiveSetMatrix_(h_struct_T *obj, int TYPE, int idx_local)
{
  int i;
  int i1;
  int idx;
  int idx_bnd_local;
  obj->nWConstr[TYPE - 1]++;
  obj->isActiveConstr[(obj->isActiveIdx[TYPE - 1] + idx_local) - 2] = true;
  obj->nActiveConstr++;
  obj->Wid[obj->nActiveConstr - 1] = TYPE;
  obj->Wlocalidx[obj->nActiveConstr - 1] = idx_local;
  i = obj->nActiveConstr - 1;
  switch (TYPE) {
   case 5:
    idx_bnd_local = obj->indexUB->data[idx_local - 1];
    obj->bwset[obj->nActiveConstr - 1] = obj->ub->data[idx_bnd_local - 1];
    break;

   default:
    idx_bnd_local = obj->indexLB->data[idx_local - 1];
    obj->bwset[obj->nActiveConstr - 1] = obj->lb->data[idx_bnd_local - 1];
    break;
  }

  for (idx = 0; idx <= idx_bnd_local - 2; idx++) {
    obj->ATwset->data[idx + obj->ATwset->size[0] * i] = 0.0;
  }

  obj->ATwset->data[(idx_bnd_local + obj->ATwset->size[0] * (obj->nActiveConstr
    - 1)) - 1] = 2.0 * (float)(TYPE == 5) - 1.0;
  idx_bnd_local++;
  i1 = obj->nVar;
  for (idx = idx_bnd_local; idx <= i1; idx++) {
    obj->ATwset->data[(idx + obj->ATwset->size[0] * i) - 1] = 0.0;
  }

  switch (obj->probType) {
   case 3:
   case 2:
    break;

   default:
    obj->ATwset->data[(obj->nVar + obj->ATwset->size[0] * (obj->nActiveConstr -
      1)) - 1] = -1.0;
    break;
  }
}

/* End of code generation (addBoundToActiveSetMatrix_.c) */
