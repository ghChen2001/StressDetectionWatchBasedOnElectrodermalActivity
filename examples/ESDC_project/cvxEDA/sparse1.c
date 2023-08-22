/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse1.c
 *
 * Code generation for function 'sparse1'
 *
 */

/* Include files */
#include "sparse1.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "fillIn.h"
#include "locBsearch.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static int div_s32(int numerator, int denominator);

/* Function Definitions */
static int div_s32(int numerator, int denominator)
{
  unsigned int b_denominator;
  unsigned int b_numerator;
  int quotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      b_numerator = ~(unsigned int)numerator + 1U;
    } else {
      b_numerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      b_denominator = ~(unsigned int)denominator + 1U;
    } else {
      b_denominator = (unsigned int)denominator;
    }

    b_numerator /= b_denominator;
    if ((numerator < 0) != (denominator < 0)) {
      quotient = -(int)b_numerator;
    } else {
      quotient = (int)b_numerator;
    }
  }

  return quotient;
}

void b_sparse_ctranspose(const emxArray_real_T *this_d, const emxArray_int32_T
  *this_colidx, const emxArray_int32_T *this_rowidx, int this_m, int this_n,
  i_sparse *y)
{
  emxArray_int32_T *counts;
  int c;
  int numalloc;
  int outridx;
  int outridx_tmp;
  y->m = this_n;
  y->n = this_m;
  if (this_colidx->data[this_colidx->size[0] - 1] - 1 >= 1) {
    numalloc = this_colidx->data[this_colidx->size[0] - 1] - 2;
  } else {
    numalloc = 0;
  }

  outridx_tmp = y->d->size[0];
  y->d->size[0] = numalloc + 1;
  emxEnsureCapacity_real_T(y->d, outridx_tmp);
  for (outridx_tmp = 0; outridx_tmp <= numalloc; outridx_tmp++) {
    y->d->data[outridx_tmp] = 0.0;
  }

  outridx_tmp = y->colidx->size[0];
  y->colidx->size[0] = this_m + 1;
  emxEnsureCapacity_int32_T(y->colidx, outridx_tmp);
  y->colidx->data[0] = 1;
  outridx_tmp = y->rowidx->size[0];
  y->rowidx->size[0] = numalloc + 1;
  emxEnsureCapacity_int32_T(y->rowidx, outridx_tmp);
  for (outridx_tmp = 0; outridx_tmp <= numalloc; outridx_tmp++) {
    y->rowidx->data[outridx_tmp] = 0;
  }

  for (c = 0; c < this_m; c++) {
    y->colidx->data[c + 1] = 1;
  }

  b_sparse_fillIn(y);
  if ((this_m != 0) && (this_n != 0)) {
    numalloc = y->colidx->size[0];
    for (outridx_tmp = 0; outridx_tmp < numalloc; outridx_tmp++) {
      y->colidx->data[outridx_tmp] = 0;
    }

    outridx_tmp = this_colidx->data[this_colidx->size[0] - 1];
    for (numalloc = 0; numalloc <= outridx_tmp - 2; numalloc++) {
      y->colidx->data[this_rowidx->data[numalloc]]++;
    }

    y->colidx->data[0] = 1;
    outridx_tmp = this_m + 1;
    for (numalloc = 2; numalloc <= outridx_tmp; numalloc++) {
      y->colidx->data[numalloc - 1] += y->colidx->data[numalloc - 2];
    }

    emxInit_int32_T(&counts, 1);
    outridx_tmp = counts->size[0];
    counts->size[0] = this_m;
    emxEnsureCapacity_int32_T(counts, outridx_tmp);
    for (outridx_tmp = 0; outridx_tmp < this_m; outridx_tmp++) {
      counts->data[outridx_tmp] = 0;
    }

    for (c = 0; c < this_n; c++) {
      for (numalloc = this_colidx->data[c] - 1; numalloc + 1 < this_colidx->
           data[c + 1]; numalloc++) {
        outridx_tmp = counts->data[this_rowidx->data[numalloc] - 1];
        outridx = (outridx_tmp + y->colidx->data[this_rowidx->data[numalloc] - 1])
          - 1;
        y->d->data[outridx] = this_d->data[numalloc];
        y->rowidx->data[outridx] = c + 1;
        counts->data[this_rowidx->data[numalloc] - 1] = outridx_tmp + 1;
      }
    }

    emxFree_int32_T(&counts);
  }
}

void b_sparse_parenReference(const emxArray_real_T *this_d, const
  emxArray_int32_T *this_colidx, const emxArray_int32_T *this_rowidx, int this_m,
  emxArray_real_T *s_d, emxArray_int32_T *s_colidx, emxArray_int32_T *s_rowidx,
  int *s_maxnz)
{
  int colNnz;
  int i;
  int idx;
  int vk;
  boolean_T found;
  s_d->size[0] = 0;
  s_rowidx->size[0] = 0;
  vk = s_colidx->size[0];
  s_colidx->size[0] = 2;
  emxEnsureCapacity_int32_T(s_colidx, vk);
  colNnz = 1;
  vk = div_s32(120, this_m);
  sparse_locBsearch(this_rowidx, 121 - vk * this_m, this_colidx->data[vk],
                    this_colidx->data[vk + 1], &idx, &found);
  if (found) {
    vk = s_d->size[0];
    s_d->size[0] = 1;
    emxEnsureCapacity_real_T(s_d, vk);
    s_d->data[0] = this_d->data[idx - 1];
    vk = s_rowidx->size[0];
    s_rowidx->size[0] = 1;
    emxEnsureCapacity_int32_T(s_rowidx, vk);
    s_rowidx->data[0] = 1;
    colNnz = 2;
  }

  vk = div_s32(121, this_m);
  sparse_locBsearch(this_rowidx, 122 - vk * this_m, this_colidx->data[vk],
                    this_colidx->data[vk + 1], &idx, &found);
  if (found) {
    vk = s_d->size[0];
    i = s_d->size[0];
    s_d->size[0]++;
    emxEnsureCapacity_real_T(s_d, i);
    s_d->data[vk] = this_d->data[idx - 1];
    vk = s_rowidx->size[0];
    i = s_rowidx->size[0];
    s_rowidx->size[0]++;
    emxEnsureCapacity_int32_T(s_rowidx, i);
    s_rowidx->data[vk] = 2;
    colNnz++;
  }

  s_colidx->data[0] = 1;
  s_colidx->data[1] = colNnz;
  if (s_d->size[0] == 0) {
    vk = s_d->size[0];
    s_d->size[0] = 1;
    emxEnsureCapacity_real_T(s_d, vk);
    s_d->data[0] = 0.0;
    vk = s_rowidx->size[0];
    s_rowidx->size[0] = 1;
    emxEnsureCapacity_int32_T(s_rowidx, vk);
    s_rowidx->data[0] = 0;
  }

  if (s_colidx->data[1] - 1 >= 1) {
    *s_maxnz = s_colidx->data[1] - 1;
  } else {
    *s_maxnz = 1;
  }
}

void b_sparse_times(const emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                    const emxArray_int32_T *b_rowidx, h_sparse *s)
{
  emxArray_real_T *tmpd;
  int i;
  int numalloc;
  int nzs_tmp;
  nzs_tmp = b_colidx->data[b_colidx->size[0] - 1];
  if (1 > b_colidx->data[b_colidx->size[0] - 1] - 1) {
    numalloc = 0;
  } else {
    numalloc = b_colidx->data[b_colidx->size[0] - 1] - 1;
  }

  emxInit_real_T(&tmpd, 1);
  i = tmpd->size[0];
  tmpd->size[0] = numalloc;
  emxEnsureCapacity_real_T(tmpd, i);
  for (i = 0; i < numalloc; i++) {
    tmpd->data[i] = 0.0008f * b_d->data[i];
  }

  if (b_colidx->data[b_colidx->size[0] - 1] - 1 >= 1) {
    numalloc = b_colidx->data[b_colidx->size[0] - 1] - 2;
  } else {
    numalloc = 0;
  }

  i = s->d->size[0];
  s->d->size[0] = numalloc + 1;
  emxEnsureCapacity_real_T(s->d, i);
  for (i = 0; i <= numalloc; i++) {
    s->d->data[i] = 0.0;
  }

  i = s->rowidx->size[0];
  s->rowidx->size[0] = numalloc + 1;
  emxEnsureCapacity_int32_T(s->rowidx, i);
  for (i = 0; i <= numalloc; i++) {
    s->rowidx->data[i] = 0;
  }

  if (1 > b_colidx->data[b_colidx->size[0] - 1] - 1) {
    numalloc = 1;
  } else {
    numalloc = b_colidx->data[b_colidx->size[0] - 1];
  }

  for (i = 0; i <= numalloc - 2; i++) {
    s->rowidx->data[i] = b_rowidx->data[i];
  }

  i = s->colidx->size[0];
  s->colidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(s->colidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    s->colidx->data[i] = b_colidx->data[i];
  }

  for (numalloc = 0; numalloc <= nzs_tmp - 2; numalloc++) {
    s->d->data[numalloc] = tmpd->data[numalloc];
  }

  emxFree_real_T(&tmpd);
  c_sparse_fillIn(s);
}

void c_sparse_ctranspose(const emxArray_real_T *this_d, const emxArray_int32_T
  *this_colidx, const emxArray_int32_T *this_rowidx, emxArray_real_T *y_d,
  emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx)
{
  int c;
  int idx;
  int numalloc;
  int outridx;
  if (this_colidx->data[this_colidx->size[0] - 1] - 1 >= 1) {
    numalloc = this_colidx->data[this_colidx->size[0] - 1] - 2;
  } else {
    numalloc = 0;
  }

  idx = y_d->size[0];
  y_d->size[0] = numalloc + 1;
  emxEnsureCapacity_real_T(y_d, idx);
  for (idx = 0; idx <= numalloc; idx++) {
    y_d->data[idx] = 0.0f;
  }

  idx = y_rowidx->size[0];
  y_rowidx->size[0] = numalloc + 1;
  emxEnsureCapacity_int32_T(y_rowidx, idx);
  for (idx = 0; idx <= numalloc; idx++) {
    y_rowidx->data[idx] = 0;
  }

  idx = y_colidx->size[0];
  y_colidx->size[0] = 2;
  emxEnsureCapacity_int32_T(y_colidx, idx);
  for (idx = 0; idx < 2; idx++) {
    y_colidx->data[idx] = 0;
  }

  idx = this_colidx->data[this_colidx->size[0] - 1];
  for (numalloc = 0; numalloc <= idx - 2; numalloc++) {
    y_colidx->data[this_rowidx->data[numalloc]]++;
  }

  y_colidx->data[0] = 1;
  y_colidx->data[1] += y_colidx->data[0];
  numalloc = -1;
  for (c = 0; c < 120; c++) {
    for (idx = this_colidx->data[c]; idx < this_colidx->data[c + 1]; idx++) {
      outridx = numalloc + y_colidx->data[0];
      y_d->data[outridx] = this_d->data[idx - 1];
      y_rowidx->data[outridx] = c + 1;
      numalloc++;
    }
  }
}

void c_sparse_parenReference(const emxArray_real_T *this_d, const
  emxArray_int32_T *this_colidx, const emxArray_int32_T *this_rowidx, int this_m,
  emxArray_real_T *s_d, emxArray_int32_T *s_colidx, emxArray_int32_T *s_rowidx)
{
  int colNnz;
  int i;
  int idx;
  int k;
  int ridx;
  int vk;
  boolean_T found;
  s_d->size[0] = 0;
  s_rowidx->size[0] = 0;
  vk = s_colidx->size[0];
  s_colidx->size[0] = 2;
  emxEnsureCapacity_int32_T(s_colidx, vk);
  k = 1;
  s_colidx->data[0] = 1;
  colNnz = 1;
  for (ridx = 0; ridx < 120; ridx++) {
    vk = div_s32(k - 1, this_m);
    sparse_locBsearch(this_rowidx, k - vk * this_m, this_colidx->data[vk],
                      this_colidx->data[vk + 1], &idx, &found);
    if (found) {
      vk = s_d->size[0];
      i = s_d->size[0];
      s_d->size[0]++;
      emxEnsureCapacity_real_T(s_d, i);
      s_d->data[vk] = this_d->data[idx - 1];
      vk = s_rowidx->size[0];
      i = s_rowidx->size[0];
      s_rowidx->size[0]++;
      emxEnsureCapacity_int32_T(s_rowidx, i);
      s_rowidx->data[vk] = ridx + 1;
      colNnz++;
    }

    k++;
  }

  s_colidx->data[1] = colNnz;
  if (s_d->size[0] == 0) {
    vk = s_d->size[0];
    s_d->size[0] = 1;
    emxEnsureCapacity_real_T(s_d, vk);
    s_d->data[0] = 0.0f;
    vk = s_rowidx->size[0];
    s_rowidx->size[0] = 1;
    emxEnsureCapacity_int32_T(s_rowidx, vk);
    s_rowidx->data[0] = 0;
  }
}

void sparse_ctranspose(const emxArray_real_T *this_d, const emxArray_int32_T
  *this_colidx, const emxArray_int32_T *this_rowidx, h_sparse *y)
{
  int counts[120];
  int c;
  int numalloc;
  int outridx;
  int outridx_tmp;
  if (this_colidx->data[this_colidx->size[0] - 1] - 1 >= 1) {
    numalloc = this_colidx->data[this_colidx->size[0] - 1] - 2;
  } else {
    numalloc = 0;
  }

  outridx_tmp = y->d->size[0];
  y->d->size[0] = numalloc + 1;
  emxEnsureCapacity_real_T(y->d, outridx_tmp);
  for (outridx_tmp = 0; outridx_tmp <= numalloc; outridx_tmp++) {
    y->d->data[outridx_tmp] = 0.0f;
  }

  outridx_tmp = y->colidx->size[0];
  y->colidx->size[0] = 121;
  emxEnsureCapacity_int32_T(y->colidx, outridx_tmp);
  y->colidx->data[0] = 1;
  outridx_tmp = y->rowidx->size[0];
  y->rowidx->size[0] = numalloc + 1;
  emxEnsureCapacity_int32_T(y->rowidx, outridx_tmp);
  for (outridx_tmp = 0; outridx_tmp <= numalloc; outridx_tmp++) {
    y->rowidx->data[outridx_tmp] = 0;
  }

  for (c = 0; c < 120; c++) {
    y->colidx->data[c + 1] = 1;
  }

  sparse_fillIn(y);
  numalloc = y->colidx->size[0];
  for (outridx_tmp = 0; outridx_tmp < numalloc; outridx_tmp++) {
    y->colidx->data[outridx_tmp] = 0;
  }

  outridx_tmp = this_colidx->data[this_colidx->size[0] - 1];
  for (numalloc = 0; numalloc <= outridx_tmp - 2; numalloc++) {
    y->colidx->data[this_rowidx->data[numalloc]]++;
  }

  y->colidx->data[0] = 1;
  for (numalloc = 0; numalloc < 120; numalloc++) {
    y->colidx->data[numalloc + 1] += y->colidx->data[numalloc];
    counts[numalloc] = 0;
  }

  for (c = 0; c < 120; c++) {
    for (numalloc = this_colidx->data[c] - 1; numalloc + 1 < this_colidx->data[c
         + 1]; numalloc++) {
      outridx_tmp = counts[this_rowidx->data[numalloc] - 1];
      outridx = (outridx_tmp + y->colidx->data[this_rowidx->data[numalloc] - 1])
        - 1;
      y->d->data[outridx] = this_d->data[numalloc];
      y->rowidx->data[outridx] = c + 1;
      counts[this_rowidx->data[numalloc] - 1] = outridx_tmp + 1;
    }
  }
}

void sparse_parenReference(const emxArray_real_T *this_d, const emxArray_int32_T
  *this_colidx, const emxArray_int32_T *this_rowidx, int this_m, const
  emxArray_real_T *varargin_1, emxArray_real_T *s_d, emxArray_int32_T *s_colidx,
  emxArray_int32_T *s_rowidx, int *s_m, int *s_maxnz)
{
  int colNnz;
  int i;
  int idx;
  int k;
  int ndx;
  int ridx;
  int vk;
  boolean_T found;
  i = varargin_1->size[1] - 1;
  *s_m = varargin_1->size[1];
  s_d->size[0] = 0;
  s_rowidx->size[0] = 0;
  ndx = s_colidx->size[0];
  s_colidx->size[0] = 2;
  emxEnsureCapacity_int32_T(s_colidx, ndx);
  k = 0;
  s_colidx->data[0] = 1;
  colNnz = 1;
  for (ridx = 0; ridx <= i; ridx++) {
    ndx = (int)varargin_1->data[k];
    vk = div_s32(ndx - 1, this_m);
    sparse_locBsearch(this_rowidx, ndx - vk * this_m, this_colidx->data[vk],
                      this_colidx->data[vk + 1], &idx, &found);
    if (found) {
      ndx = s_d->size[0];
      vk = s_d->size[0];
      s_d->size[0]++;
      emxEnsureCapacity_real_T(s_d, vk);
      s_d->data[ndx] = this_d->data[idx - 1];
      ndx = s_rowidx->size[0];
      vk = s_rowidx->size[0];
      s_rowidx->size[0]++;
      emxEnsureCapacity_int32_T(s_rowidx, vk);
      s_rowidx->data[ndx] = ridx + 1;
      colNnz++;
    }

    k++;
  }

  s_colidx->data[1] = colNnz;
  if (s_d->size[0] == 0) {
    i = s_d->size[0];
    s_d->size[0] = 1;
    emxEnsureCapacity_real_T(s_d, i);
    s_d->data[0] = 0.0;
    i = s_rowidx->size[0];
    s_rowidx->size[0] = 1;
    emxEnsureCapacity_int32_T(s_rowidx, i);
    s_rowidx->data[0] = 0;
  }

  if (s_colidx->data[1] - 1 >= 1) {
    *s_maxnz = s_colidx->data[1] - 1;
  } else {
    *s_maxnz = 1;
  }
}

void sparse_plus(const emxArray_real_T *a_d, const emxArray_int32_T *a_colidx,
                 const emxArray_int32_T *a_rowidx, const emxArray_real_T *b_d,
                 const emxArray_int32_T *b_colidx, const emxArray_int32_T
                 *b_rowidx, int b_m, int b_n, i_sparse *s)
{
  float val;
  int aidx;
  int aidx_tmp;
  int bidx;
  int bidx_tmp;
  int c;
  int didx;
  int numalloc;
  boolean_T moreAToDo;
  boolean_T moreBToDo;
  didx = (a_colidx->data[a_colidx->size[0] - 1] + b_colidx->data[b_colidx->size
          [0] - 1]) - 2;
  numalloc = b_n * b_m;
  if (didx < numalloc) {
    numalloc = didx;
  }

  if (numalloc < 1) {
    numalloc = 1;
  }

  s->m = b_m;
  s->n = b_n;
  didx = s->d->size[0];
  s->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(s->d, didx);
  for (didx = 0; didx < numalloc; didx++) {
    s->d->data[didx] = 0.0f;
  }

  didx = s->colidx->size[0];
  s->colidx->size[0] = b_n + 1;
  emxEnsureCapacity_int32_T(s->colidx, didx);
  s->colidx->data[0] = 1;
  didx = s->rowidx->size[0];
  s->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(s->rowidx, didx);
  for (didx = 0; didx < numalloc; didx++) {
    s->rowidx->data[didx] = 0;
  }

  for (c = 0; c < b_n; c++) {
    s->colidx->data[c + 1] = 1;
  }

  b_sparse_fillIn(s);
  didx = 1;
  s->colidx->data[0] = 1;
  numalloc = s->n;
  for (c = 0; c < numalloc; c++) {
    aidx_tmp = a_colidx->data[c];
    aidx = aidx_tmp - 1;
    bidx_tmp = b_colidx->data[c];
    bidx = bidx_tmp - 1;
    moreAToDo = (aidx_tmp < a_colidx->data[c + 1]);
    moreBToDo = (bidx_tmp < b_colidx->data[c + 1]);
    while (moreAToDo || moreBToDo) {
      while ((aidx + 1 < a_colidx->data[c + 1]) && ((!moreBToDo) ||
              (a_rowidx->data[aidx] < b_rowidx->data[bidx]))) {
        if (a_d->data[aidx] != 0.0f) {
          s->d->data[didx - 1] = a_d->data[aidx];
          s->rowidx->data[didx - 1] = a_rowidx->data[aidx];
          didx++;
        }

        aidx++;
      }

      moreAToDo = (aidx + 1 < a_colidx->data[c + 1]);
      while ((bidx + 1 < b_colidx->data[c + 1]) && ((!moreAToDo) ||
              (b_rowidx->data[bidx] < a_rowidx->data[aidx]))) {
        if (b_d->data[bidx] != 0.0f) {
          s->d->data[didx - 1] = b_d->data[bidx];
          s->rowidx->data[didx - 1] = b_rowidx->data[bidx];
          didx++;
        }

        bidx++;
      }

      while ((aidx + 1 < a_colidx->data[c + 1]) && (bidx + 1 < b_colidx->data[c
              + 1]) && (a_rowidx->data[aidx] == b_rowidx->data[bidx])) {
        val = a_d->data[aidx] + b_d->data[bidx];
        if (val != 0.0f) {
          s->d->data[didx - 1] = val;
          s->rowidx->data[didx - 1] = b_rowidx->data[bidx];
          didx++;
        }

        bidx++;
        aidx++;
      }

      moreAToDo = (aidx + 1 < a_colidx->data[c + 1]);
      moreBToDo = (bidx + 1 < b_colidx->data[c + 1]);
    }

    s->colidx->data[c + 1] = didx;
  }
}

void sparse_times(const emxArray_int32_T *b_colidx, const emxArray_int32_T
                  *b_rowidx, int b_m, int b_n, i_sparse *s)
{
  int i;
  int numalloc;
  int nzs_tmp;
  nzs_tmp = b_colidx->data[b_colidx->size[0] - 1];
  s->m = b_m;
  s->n = b_n;
  if (b_colidx->data[b_colidx->size[0] - 1] - 1 >= 1) {
    numalloc = b_colidx->data[b_colidx->size[0] - 1] - 2;
  } else {
    numalloc = 0;
  }

  i = s->d->size[0];
  s->d->size[0] = numalloc + 1;
  emxEnsureCapacity_real_T(s->d, i);
  for (i = 0; i <= numalloc; i++) {
    s->d->data[i] = 0.0f;
  }

  i = s->colidx->size[0];
  s->colidx->size[0] = b_n + 1;
  emxEnsureCapacity_int32_T(s->colidx, i);
  s->colidx->data[0] = 1;
  i = s->rowidx->size[0];
  s->rowidx->size[0] = numalloc + 1;
  emxEnsureCapacity_int32_T(s->rowidx, i);
  for (i = 0; i <= numalloc; i++) {
    s->rowidx->data[i] = 0;
  }

  for (numalloc = 0; numalloc < b_n; numalloc++) {
    s->colidx->data[numalloc + 1] = 1;
  }

  b_sparse_fillIn(s);
  if (1 > b_colidx->data[b_colidx->size[0] - 1] - 1) {
    numalloc = 1;
  } else {
    numalloc = b_colidx->data[b_colidx->size[0] - 1];
  }

  for (i = 0; i <= numalloc - 2; i++) {
    s->rowidx->data[i] = b_rowidx->data[i];
  }

  i = s->colidx->size[0];
  s->colidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(s->colidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    s->colidx->data[i] = b_colidx->data[i];
  }

  for (numalloc = 0; numalloc <= nzs_tmp - 2; numalloc++) {
    s->d->data[numalloc] = 0.01f;
  }

  d_sparse_fillIn(s);
}

/* End of code generation (sparse1.c) */
