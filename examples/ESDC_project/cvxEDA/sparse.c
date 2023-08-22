/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse.c
 *
 * Code generation for function 'sparse'
 *
 */

/* Include files */
#include "sparse.h"
#include "cvxEDA_data.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "fillIn.h"
#include "introsort.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_sparse(h_sparse *y)
{
  static const signed char b_iv[354] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1 };

  int sortedIndices[354];
  int cptr;
  int k;
  signed char cidxInt[354];
  signed char ridxInt[354];
  for (k = 0; k < 354; k++) {
    sortedIndices[k] = k + 1;
  }

  introsort(sortedIndices);
  cptr = y->d->size[0];
  y->d->size[0] = 354;
  emxEnsureCapacity_real_T(y->d, cptr);
  cptr = y->colidx->size[0];
  y->colidx->size[0] = 121;
  emxEnsureCapacity_int32_T(y->colidx, cptr);
  y->colidx->data[0] = 1;
  cptr = y->rowidx->size[0];
  y->rowidx->size[0] = 354;
  emxEnsureCapacity_int32_T(y->rowidx, cptr);
  for (k = 0; k < 354; k++) {
    cptr = sortedIndices[k];
    cidxInt[k] = iv[cptr - 1];
    ridxInt[k] = iv1[cptr - 1];
    y->d->data[k] = 0.0;
    y->rowidx->data[k] = 0;
  }

  cptr = 0;
  for (k = 0; k < 120; k++) {
    while ((cptr + 1 <= 354) && (cidxInt[cptr] == k + 1)) {
      y->rowidx->data[cptr] = ridxInt[cptr];
      cptr++;
    }

    y->colidx->data[k + 1] = cptr + 1;
  }

  for (k = 0; k < 354; k++) {
    y->d->data[k] = b_iv[sortedIndices[k] - 1];
  }

  sparse_fillIn(y);
}

void c_sparse(const emxArray_real_T *varargin_1, const emxArray_real_T
              *varargin_2, const emxArray_real_T *varargin_3, i_sparse *y)
{
  cell_wrap_3 this_tunableEnvironment[2];
  emxArray_int32_T *sortedIndices;
  emxArray_int32_T *t;
  int i;
  int k;
  int nc;
  int ns;
  int ny;
  emxInitMatrix_cell_wrap_3(this_tunableEnvironment);
  nc = varargin_2->size[0];
  ns = varargin_1->size[0];
  i = this_tunableEnvironment[1].f1->size[0];
  this_tunableEnvironment[1].f1->size[0] = varargin_1->size[0];
  emxEnsureCapacity_int32_T(this_tunableEnvironment[1].f1, i);
  for (k = 0; k < ns; k++) {
    this_tunableEnvironment[1].f1->data[k] = (int)varargin_1->data[k];
  }

  ns = varargin_2->size[0];
  i = this_tunableEnvironment[0].f1->size[0];
  this_tunableEnvironment[0].f1->size[0] = varargin_2->size[0];
  emxEnsureCapacity_int32_T(this_tunableEnvironment[0].f1, i);
  for (k = 0; k < ns; k++) {
    this_tunableEnvironment[0].f1->data[k] = (int)varargin_2->data[k];
  }

  emxInit_int32_T(&sortedIndices, 1);
  i = sortedIndices->size[0];
  sortedIndices->size[0] = varargin_2->size[0];
  emxEnsureCapacity_int32_T(sortedIndices, i);
  for (k = 0; k < nc; k++) {
    sortedIndices->data[k] = k + 1;
  }

  emxInit_int32_T(&t, 1);
  b_introsort(sortedIndices, this_tunableEnvironment[0].f1->size[0],
              this_tunableEnvironment);
  ny = this_tunableEnvironment[0].f1->size[0];
  i = t->size[0];
  t->size[0] = this_tunableEnvironment[0].f1->size[0];
  emxEnsureCapacity_int32_T(t, i);
  ns = this_tunableEnvironment[0].f1->size[0];
  for (i = 0; i < ns; i++) {
    t->data[i] = this_tunableEnvironment[0].f1->data[i];
  }

  for (k = 0; k < ny; k++) {
    this_tunableEnvironment[0].f1->data[k] = t->data[sortedIndices->data[k] - 1];
  }

  ny = this_tunableEnvironment[1].f1->size[0];
  i = t->size[0];
  t->size[0] = this_tunableEnvironment[1].f1->size[0];
  emxEnsureCapacity_int32_T(t, i);
  ns = this_tunableEnvironment[1].f1->size[0];
  for (i = 0; i < ns; i++) {
    t->data[i] = this_tunableEnvironment[1].f1->data[i];
  }

  for (k = 0; k < ny; k++) {
    this_tunableEnvironment[1].f1->data[k] = t->data[sortedIndices->data[k] - 1];
  }

  emxFree_int32_T(&t);
  if ((this_tunableEnvironment[1].f1->size[0] == 0) || (this_tunableEnvironment
       [0].f1->size[0] == 0)) {
    ny = 0;
    y->n = 0;
  } else {
    ns = this_tunableEnvironment[1].f1->size[0];
    ny = this_tunableEnvironment[1].f1->data[0];
    for (k = 2; k <= ns; k++) {
      i = this_tunableEnvironment[1].f1->data[k - 1];
      if (ny < i) {
        ny = i;
      }
    }

    y->n = this_tunableEnvironment[0].f1->data[this_tunableEnvironment[0]
      .f1->size[0] - 1];
  }

  y->m = ny;
  if (varargin_2->size[0] >= 1) {
    ns = varargin_2->size[0];
  } else {
    ns = 1;
  }

  i = y->d->size[0];
  y->d->size[0] = ns;
  emxEnsureCapacity_real_T(y->d, i);
  for (i = 0; i < ns; i++) {
    y->d->data[i] = 0.0;
  }

  i = y->colidx->size[0];
  y->colidx->size[0] = y->n + 1;
  emxEnsureCapacity_int32_T(y->colidx, i);
  y->colidx->data[0] = 1;
  i = y->rowidx->size[0];
  y->rowidx->size[0] = ns;
  emxEnsureCapacity_int32_T(y->rowidx, i);
  for (i = 0; i < ns; i++) {
    y->rowidx->data[i] = 0;
  }

  ns = 0;
  i = y->n;
  for (ny = 0; ny < i; ny++) {
    while ((ns + 1 <= nc) && (this_tunableEnvironment[0].f1->data[ns] == ny + 1))
    {
      y->rowidx->data[ns] = this_tunableEnvironment[1].f1->data[ns];
      ns++;
    }

    y->colidx->data[ny + 1] = ns + 1;
  }

  emxFreeMatrix_cell_wrap_3(this_tunableEnvironment);
  for (k = 0; k < nc; k++) {
    y->d->data[k] = varargin_3->data[sortedIndices->data[k] - 1];
  }

  emxFree_int32_T(&sortedIndices);
  b_sparse_fillIn(y);
}

void d_sparse(float varargin_1, emxArray_real_T *y_d, emxArray_int32_T
              *y_colidx, emxArray_int32_T *y_rowidx)
{
  int i;
  i = y_d->size[0];
  y_d->size[0] = 1;
  emxEnsureCapacity_real_T(y_d, i);
  y_d->data[0] = 0.0;
  i = y_colidx->size[0];
  y_colidx->size[0] = 2;
  emxEnsureCapacity_int32_T(y_colidx, i);
  y_colidx->data[0] = 1;
  i = y_rowidx->size[0];
  y_rowidx->size[0] = 1;
  emxEnsureCapacity_int32_T(y_rowidx, i);
  y_rowidx->data[0] = 1;
  y_colidx->data[1] = 1;
  if (varargin_1 != 0.0) {
    y_rowidx->data[0] = 1;
    y_d->data[0] = varargin_1;
    y_colidx->data[1] = 2;
  }
}

void e_sparse(const emxArray_real_T *varargin_1, emxArray_real_T *y_d,
              emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx, int *y_m,
              int *y_maxnz)
{
  int ctr;
  int mInt;
  int numalloc;
  int row;
  mInt = varargin_1->size[0];
  ctr = 0;
  row = varargin_1->size[0];
  for (numalloc = 0; numalloc < row; numalloc++) {
    if (varargin_1->data[numalloc] != 0.0f) {
      ctr++;
    }
  }

  *y_m = varargin_1->size[0];
  if (ctr >= 1) {
    numalloc = ctr;
  } else {
    numalloc = 1;
  }

  row = y_d->size[0];
  y_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(y_d, row);
  for (row = 0; row < numalloc; row++) {
    y_d->data[row] = 0.0f;
  }

  row = y_colidx->size[0];
  y_colidx->size[0] = 2;
  emxEnsureCapacity_int32_T(y_colidx, row);
  y_colidx->data[0] = 1;
  row = y_rowidx->size[0];
  y_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(y_rowidx, row);
  for (row = 0; row < numalloc; row++) {
    y_rowidx->data[row] = 0;
  }

  y_rowidx->data[0] = 1;
  ctr = 0;
  for (row = 0; row < mInt; row++) {
    if (varargin_1->data[row] != 0.0) {
      y_rowidx->data[ctr] = row + 1;
      y_d->data[ctr] = varargin_1->data[row];
      ctr++;
    }
  }

  y_colidx->data[1] = ctr + 1;
  *y_maxnz = numalloc;
}

void sparse(const float varargin_3[354], h_sparse *y)
{
  int sortedIndices[354];
  int cptr;
  int k;
  signed char cidxInt[354];
  signed char ridxInt[354];
  for (k = 0; k < 354; k++) {
    sortedIndices[k] = k + 1;
  }

  introsort(sortedIndices);
  cptr = y->d->size[0];
  y->d->size[0] = 354;
  emxEnsureCapacity_real_T(y->d, cptr);
  cptr = y->colidx->size[0];
  y->colidx->size[0] = 121;
  emxEnsureCapacity_int32_T(y->colidx, cptr);
  y->colidx->data[0] = 1;
  cptr = y->rowidx->size[0];
  y->rowidx->size[0] = 354;
  emxEnsureCapacity_int32_T(y->rowidx, cptr);
  for (k = 0; k < 354; k++) {
    cptr = sortedIndices[k];
    cidxInt[k] = iv[cptr - 1];
    ridxInt[k] = iv1[cptr - 1];
    y->d->data[k] = 0.0f;
    y->rowidx->data[k] = 0;
  }

  cptr = 0;
  for (k = 0; k < 120; k++) {
    while ((cptr + 1 <= 354) && (cidxInt[cptr] == k + 1)) {
      y->rowidx->data[cptr] = ridxInt[cptr];
      cptr++;
    }

    y->colidx->data[k + 1] = cptr + 1;
  }

  for (k = 0; k < 354; k++) {
    y->d->data[k] = varargin_3[sortedIndices[k] - 1];
  }

  sparse_fillIn(y);
}

/* End of code generation (sparse.c) */
