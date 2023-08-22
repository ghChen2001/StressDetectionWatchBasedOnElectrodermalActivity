/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * setProblemType.c
 *
 * Code generation for function 'setProblemType'
 *
 */

/* Include files */
#include "setProblemType.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void setProblemType(h_struct_T *obj, int PROBLEM_TYPE)
{
  int i;
  int i1;
  int idxStartIneq;
  int idx_col;
  int idx_lb;
  int idx_row;
  int offsetIneq;
  switch (PROBLEM_TYPE) {
   case 3:
    obj->nVar = obj->nVarOrig;
    obj->mConstr = 120;
    for (i = 0; i < 5; i++) {
      obj->sizes[i] = obj->sizesNormal[i];
    }

    for (i = 0; i < 6; i++) {
      obj->isActiveIdx[i] = obj->isActiveIdxNormal[i];
    }
    break;

   case 1:
    obj->nVar = obj->nVarOrig + 1;
    obj->mConstr = 121;
    for (i = 0; i < 5; i++) {
      obj->sizes[i] = obj->sizesPhaseOne[i];
    }

    for (i = 0; i < 6; i++) {
      obj->isActiveIdx[i] = obj->isActiveIdxPhaseOne[i];
    }

    for (idx_lb = 0; idx_lb < 120; idx_lb++) {
      obj->Aineq->data[(obj->nVar + obj->Aineq->size[0] * idx_lb) - 1] = -1.0;
    }

    obj->indexLB->data[obj->sizes[3] - 1] = obj->nVar;
    obj->lb->data[obj->nVar - 1] = obj->SLACK0;
    idxStartIneq = obj->isActiveIdx[2];
    i = obj->nActiveConstr;
    for (idx_lb = idxStartIneq; idx_lb <= i; idx_lb++) {
      obj->ATwset->data[(obj->nVar + obj->ATwset->size[0] * (idx_lb - 1)) - 1] =
        -1.0;
    }
    break;

   case 2:
    obj->nVar = obj->nVarMax - 1;
    obj->mConstr = 120;
    for (i = 0; i < 5; i++) {
      obj->sizes[i] = obj->sizesRegularized[i];
    }

    for (i = 0; i < 6; i++) {
      obj->isActiveIdx[i] = obj->isActiveIdxRegularized[i];
    }

    if (obj->probType != 4) {
      offsetIneq = obj->nVarOrig + 1;
      idx_lb = obj->nVarOrig;
      for (idx_col = 0; idx_col < 120; idx_col++) {
        i = offsetIneq + idx_col;
        i1 = i - 1;
        for (idx_row = offsetIneq; idx_row <= i1; idx_row++) {
          obj->Aineq->data[(idx_row + obj->Aineq->size[0] * idx_col) - 1] = 0.0;
        }

        obj->Aineq->data[(i + obj->Aineq->size[0] * idx_col) - 1] = -1.0;
        i++;
        i1 = obj->nVar;
        for (idx_row = i; idx_row <= i1; idx_row++) {
          obj->Aineq->data[(idx_row + obj->Aineq->size[0] * idx_col) - 1] = 0.0;
        }

        idx_lb++;
        obj->indexLB->data[idx_col] = idx_lb;
      }

      i = obj->nVarOrig + 1;
      i1 = obj->nVarOrig + 120;
      for (idx_lb = i; idx_lb <= i1; idx_lb++) {
        obj->lb->data[idx_lb - 1] = 0.0f;
      }

      idxStartIneq = obj->isActiveIdx[2];
      i = obj->nActiveConstr;
      for (idx_col = idxStartIneq; idx_col <= i; idx_col++) {
        switch (obj->Wid[idx_col - 1]) {
         case 3:
          i1 = (offsetIneq + obj->Wlocalidx[idx_col - 1]) - 2;
          for (idx_row = offsetIneq; idx_row <= i1; idx_row++) {
            obj->ATwset->data[(idx_row + obj->ATwset->size[0] * (idx_col - 1)) -
              1] = 0.0f;
          }

          obj->ATwset->data[((offsetIneq + obj->Wlocalidx[idx_col - 1]) +
                             obj->ATwset->size[0] * (idx_col - 1)) - 2] = -1.0f;
          i1 = offsetIneq + obj->Wlocalidx[idx_col - 1];
          idx_lb = obj->nVar;
          for (idx_row = i1; idx_row <= idx_lb; idx_row++) {
            obj->ATwset->data[(idx_row + obj->ATwset->size[0] * (idx_col - 1)) -
              1] = 0.0f;
          }
          break;

         default:
          i1 = obj->nVar;
          for (idx_row = offsetIneq; idx_row <= i1; idx_row++) {
            obj->ATwset->data[(idx_row + obj->ATwset->size[0] * (idx_col - 1)) -
              1] = 0.0f;
          }
          break;
        }
      }
    }
    break;

   default:
    obj->nVar = obj->nVarMax;
    obj->mConstr = 121;
    for (i = 0; i < 5; i++) {
      obj->sizes[i] = obj->sizesRegPhaseOne[i];
    }

    for (i = 0; i < 6; i++) {
      obj->isActiveIdx[i] = obj->isActiveIdxRegPhaseOne[i];
    }

    for (idx_lb = 0; idx_lb < 120; idx_lb++) {
      obj->Aineq->data[(obj->nVar + obj->Aineq->size[0] * idx_lb) - 1] = -1.0f;
    }

    obj->indexLB->data[obj->sizes[3] - 1] = obj->nVar;
    obj->lb->data[obj->nVar - 1] = obj->SLACK0;
    idxStartIneq = obj->isActiveIdx[2];
    i = obj->nActiveConstr;
    for (idx_lb = idxStartIneq; idx_lb <= i; idx_lb++) {
      obj->ATwset->data[(obj->nVar + obj->ATwset->size[0] * (idx_lb - 1)) - 1] =
        -1.0f;
    }
    break;
  }

  obj->probType = PROBLEM_TYPE;
}

/* End of code generation (setProblemType.c) */
