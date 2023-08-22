/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * fillIn.c
 *
 * Code generation for function 'fillIn'
 *
 */

/* Include files */
#include "fillIn.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_sparse_fillIn(i_sparse *this)
{
  float val;
  int c;
  int currRowIdx;
  int exitg1;
  int i;
  int i1;
  int idx;
  int ridx;
  idx = 1;
  i = this->colidx->size[0];
  for (c = 0; c <= i - 2; c++) {
    ridx = this->colidx->data[c];
    this->colidx->data[c] = idx;
    do {
      exitg1 = 0;
      i1 = this->colidx->data[c + 1];
      if (ridx < i1) {
        val = 0.0f;
        currRowIdx = this->rowidx->data[ridx - 1];
        while ((ridx < i1) && (this->rowidx->data[ridx - 1] == currRowIdx)) {
          val += this->d->data[ridx - 1];
          ridx++;
        }

        if (val != 0.0f) {
          this->d->data[idx - 1] = val;
          this->rowidx->data[idx - 1] = currRowIdx;
          idx++;
        }
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  this->colidx->data[this->colidx->size[0] - 1] = idx;
}

void c_sparse_fillIn(h_sparse *this)
{
  float val;
  int c;
  int currRowIdx;
  int i;
  int idx;
  int ridx;
  idx = 1;
  i = this->colidx->size[0];
  for (c = 0; c <= i - 2; c++) {
    ridx = this->colidx->data[c];
    this->colidx->data[c] = idx;
    while (ridx < this->colidx->data[c + 1]) {
      currRowIdx = this->rowidx->data[ridx - 1];
      val = this->d->data[ridx - 1];
      ridx++;
      if (val != 0.0f) {
        this->d->data[idx - 1] = val;
        this->rowidx->data[idx - 1] = currRowIdx;
        idx++;
      }
    }
  }

  this->colidx->data[this->colidx->size[0] - 1] = idx;
}

void d_sparse_fillIn(i_sparse *this)
{
  float val;
  int c;
  int currRowIdx;
  int i;
  int idx;
  int ridx;
  idx = 1;
  i = this->colidx->size[0];
  for (c = 0; c <= i - 2; c++) {
    ridx = this->colidx->data[c];
    this->colidx->data[c] = idx;
    while (ridx < this->colidx->data[c + 1]) {
      currRowIdx = this->rowidx->data[ridx - 1];
      val = this->d->data[ridx - 1];
      ridx++;
      if (val != 0.0f) {
        this->d->data[idx - 1] = val;
        this->rowidx->data[idx - 1] = currRowIdx;
        idx++;
      }
    }
  }

  this->colidx->data[this->colidx->size[0] - 1] = idx;
}

void sparse_fillIn(h_sparse *this)
{
  float val;
  int c;
  int currRowIdx;
  int exitg1;
  int i;
  int i1;
  int idx;
  int ridx;
  idx = 1;
  i = this->colidx->size[0];
  for (c = 0; c <= i - 2; c++) {
    ridx = this->colidx->data[c];
    this->colidx->data[c] = idx;
    do {
      exitg1 = 0;
      i1 = this->colidx->data[c + 1];
      if (ridx < i1) {
        val = 0.0f;
        currRowIdx = this->rowidx->data[ridx - 1];
        while ((ridx < i1) && (this->rowidx->data[ridx - 1] == currRowIdx)) {
          val += this->d->data[ridx - 1];
          ridx++;
        }

        if (val != 0.0f) {
          this->d->data[idx - 1] = val;
          this->rowidx->data[idx - 1] = currRowIdx;
          idx++;
        }
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  this->colidx->data[this->colidx->size[0] - 1] = idx;
}

/* End of code generation (fillIn.c) */
