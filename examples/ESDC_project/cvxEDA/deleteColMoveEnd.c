/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * deleteColMoveEnd.c
 *
 * Code generation for function 'deleteColMoveEnd'
 *
 */

/* Include files */
#include "deleteColMoveEnd.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xrotg.h"

/* Function Definitions */
void deleteColMoveEnd(f_struct_T *obj, int idx)
{
  emxArray_real_T *x;
  float c;
  float s;
  float temp;
  int QRk0;
  int b_i;
  int b_k;
  int endIdx;
  int i;
  int k;
  int n;
  if (obj->usedPivoting) {
    i = 1;
    while ((i <= obj->ncols) && (obj->jpvt->data[i - 1] != idx)) {
      i++;
    }

    idx = i;
  }

  if (idx >= obj->ncols) {
    obj->ncols--;
  } else {
    obj->jpvt->data[idx - 1] = obj->jpvt->data[obj->ncols - 1];
    b_i = obj->minRowCol;
    for (k = 0; k < b_i; k++) {
      obj->QR->data[k + obj->QR->size[0] * (idx - 1)] = obj->QR->data[k +
        obj->QR->size[0] * (obj->ncols - 1)];
    }

    obj->ncols--;
    QRk0 = obj->mrows;
    i = obj->ncols;
    if (QRk0 < i) {
      i = QRk0;
    }

    obj->minRowCol = i;
    if (idx < obj->mrows) {
      QRk0 = obj->mrows - 1;
      endIdx = obj->ncols;
      if (QRk0 < endIdx) {
        endIdx = QRk0;
      }

      k = endIdx;
      emxInit_real_T(&x, 2);
      while (k >= idx) {
        temp = obj->QR->data[k + obj->QR->size[0] * (idx - 1)];
        xrotg(&obj->QR->data[(k + obj->QR->size[0] * (idx - 1)) - 1], &temp, &c,
              &s);
        obj->QR->data[k + obj->QR->size[0] * (idx - 1)] = temp;
        obj->QR->data[k + obj->QR->size[0] * (k - 1)] = 0.0f;
        QRk0 = k + obj->ldq * idx;
        n = obj->ncols - idx;
        b_i = x->size[0] * x->size[1];
        x->size[0] = obj->QR->size[0];
        x->size[1] = obj->QR->size[1];
        emxEnsureCapacity_real_T(x, b_i);
        i = obj->QR->size[0] * obj->QR->size[1];
        for (b_i = 0; b_i < i; b_i++) {
          x->data[b_i] = obj->QR->data[b_i];
        }

        if (n >= 1) {
          i = QRk0 - 1;
          for (b_k = 0; b_k < n; b_k++) {
            temp = c * x->data[i] + s * x->data[QRk0];
            x->data[QRk0] = c * x->data[QRk0] - s * x->data[i];
            x->data[i] = temp;
            QRk0 += obj->ldq;
            i += obj->ldq;
          }
        }

        b_i = obj->QR->size[0] * obj->QR->size[1];
        obj->QR->size[0] = x->size[0];
        obj->QR->size[1] = x->size[1];
        emxEnsureCapacity_real_T(obj->QR, b_i);
        i = x->size[0] * x->size[1];
        for (b_i = 0; b_i < i; b_i++) {
          obj->QR->data[b_i] = x->data[b_i];
        }

        QRk0 = obj->ldq * (k - 1);
        n = obj->mrows;
        b_i = x->size[0] * x->size[1];
        x->size[0] = obj->Q->size[0];
        x->size[1] = obj->Q->size[1];
        emxEnsureCapacity_real_T(x, b_i);
        i = obj->Q->size[0] * obj->Q->size[1];
        for (b_i = 0; b_i < i; b_i++) {
          x->data[b_i] = obj->Q->data[b_i];
        }

        if (obj->mrows >= 1) {
          i = obj->ldq + QRk0;
          for (b_k = 0; b_k < n; b_k++) {
            temp = c * x->data[QRk0] + s * x->data[i];
            x->data[i] = c * x->data[i] - s * x->data[QRk0];
            x->data[QRk0] = temp;
            i++;
            QRk0++;
          }
        }

        b_i = obj->Q->size[0] * obj->Q->size[1];
        obj->Q->size[0] = x->size[0];
        obj->Q->size[1] = x->size[1];
        emxEnsureCapacity_real_T(obj->Q, b_i);
        i = x->size[0] * x->size[1];
        for (b_i = 0; b_i < i; b_i++) {
          obj->Q->data[b_i] = x->data[b_i];
        }

        k--;
      }

      b_i = idx + 1;
      for (k = b_i; k <= endIdx; k++) {
        temp = obj->QR->data[k + obj->QR->size[0] * (k - 1)];
        xrotg(&obj->QR->data[(k + obj->QR->size[0] * (k - 1)) - 1], &temp, &c,
              &s);
        obj->QR->data[k + obj->QR->size[0] * (k - 1)] = temp;
        QRk0 = k * (obj->ldq + 1);
        n = obj->ncols - k;
        b_k = x->size[0] * x->size[1];
        x->size[0] = obj->QR->size[0];
        x->size[1] = obj->QR->size[1];
        emxEnsureCapacity_real_T(x, b_k);
        i = obj->QR->size[0] * obj->QR->size[1];
        for (b_k = 0; b_k < i; b_k++) {
          x->data[b_k] = obj->QR->data[b_k];
        }

        if (n >= 1) {
          i = QRk0 - 1;
          for (b_k = 0; b_k < n; b_k++) {
            temp = c * x->data[i] + s * x->data[QRk0];
            x->data[QRk0] = c * x->data[QRk0] - s * x->data[i];
            x->data[i] = temp;
            QRk0 += obj->ldq;
            i += obj->ldq;
          }
        }

        b_k = obj->QR->size[0] * obj->QR->size[1];
        obj->QR->size[0] = x->size[0];
        obj->QR->size[1] = x->size[1];
        emxEnsureCapacity_real_T(obj->QR, b_k);
        i = x->size[0] * x->size[1];
        for (b_k = 0; b_k < i; b_k++) {
          obj->QR->data[b_k] = x->data[b_k];
        }

        QRk0 = obj->ldq * (k - 1);
        n = obj->mrows;
        b_k = x->size[0] * x->size[1];
        x->size[0] = obj->Q->size[0];
        x->size[1] = obj->Q->size[1];
        emxEnsureCapacity_real_T(x, b_k);
        i = obj->Q->size[0] * obj->Q->size[1];
        for (b_k = 0; b_k < i; b_k++) {
          x->data[b_k] = obj->Q->data[b_k];
        }

        if (obj->mrows >= 1) {
          i = obj->ldq + QRk0;
          for (b_k = 0; b_k < n; b_k++) {
            temp = c * x->data[QRk0] + s * x->data[i];
            x->data[i] = c * x->data[i] - s * x->data[QRk0];
            x->data[QRk0] = temp;
            i++;
            QRk0++;
          }
        }

        b_k = obj->Q->size[0] * obj->Q->size[1];
        obj->Q->size[0] = x->size[0];
        obj->Q->size[1] = x->size[1];
        emxEnsureCapacity_real_T(obj->Q, b_k);
        i = x->size[0] * x->size[1];
        for (b_k = 0; b_k < i; b_k++) {
          obj->Q->data[b_k] = x->data[b_k];
        }
      }

      emxFree_real_T(&x);
    }
  }
}

/* End of code generation (deleteColMoveEnd.c) */
