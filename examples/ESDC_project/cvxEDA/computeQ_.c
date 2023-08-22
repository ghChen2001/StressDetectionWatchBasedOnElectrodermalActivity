/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeQ_.c
 *
 * Code generation for function 'computeQ_'
 *
 */

/* Include files */
#include "computeQ_.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xgerc.h"

/* Function Definitions */
void computeQ_(f_struct_T *obj, int nrows)
{
  emxArray_real_T *work;
  float c;
  int b_i;
  int exitg1;
  int i;
  int i1;
  int iQR0;
  int ia;
  int iac;
  int iaii;
  int idx;
  int itau;
  int iy;
  int lastc;
  int lastv;
  int lda;
  int m;
  boolean_T exitg2;
  i = obj->minRowCol;
  for (idx = 0; idx < i; idx++) {
    iQR0 = obj->ldq * idx + idx;
    lastc = obj->mrows - idx;
    for (lastv = 0; lastv <= lastc - 2; lastv++) {
      i1 = (iQR0 + lastv) + 1;
      obj->Q->data[i1] = obj->QR->data[i1];
    }
  }

  m = obj->mrows;
  lastv = obj->minRowCol;
  lda = obj->ldq;
  if (nrows >= 1) {
    i = nrows - 1;
    for (iQR0 = lastv; iQR0 <= i; iQR0++) {
      ia = iQR0 * lda;
      i1 = m - 1;
      for (b_i = 0; b_i <= i1; b_i++) {
        obj->Q->data[ia + b_i] = 0.0f;
      }

      obj->Q->data[ia + iQR0] = 1.0f;
    }

    emxInit_real_T(&work, 1);
    itau = obj->minRowCol - 1;
    iQR0 = obj->Q->size[1];
    i = work->size[0];
    work->size[0] = iQR0;
    emxEnsureCapacity_real_T(work, i);
    for (i = 0; i < iQR0; i++) {
      work->data[i] = 0.0f;
    }

    for (b_i = obj->minRowCol; b_i >= 1; b_i--) {
      iaii = b_i + (b_i - 1) * lda;
      if (b_i < nrows) {
        obj->Q->data[iaii - 1] = 1.0f;
        idx = iaii + lda;
        if (obj->tau->data[itau] != 0.0f) {
          lastv = (m - b_i) + 1;
          iQR0 = (iaii + m) - b_i;
          while ((lastv > 0) && (obj->Q->data[iQR0 - 1] == 0.0f)) {
            lastv--;
            iQR0--;
          }

          lastc = nrows - b_i;
          exitg2 = false;
          while ((!exitg2) && (lastc > 0)) {
            iQR0 = idx + (lastc - 1) * lda;
            ia = iQR0;
            do {
              exitg1 = 0;
              if (ia <= (iQR0 + lastv) - 1) {
                if (obj->Q->data[ia - 1] != 0.0f) {
                  exitg1 = 1;
                } else {
                  ia++;
                }
              } else {
                lastc--;
                exitg1 = 2;
              }
            } while (exitg1 == 0);

            if (exitg1 == 1) {
              exitg2 = true;
            }
          }
        } else {
          lastv = 0;
          lastc = 0;
        }

        if (lastv > 0) {
          if (lastc != 0) {
            for (iy = 0; iy < lastc; iy++) {
              work->data[iy] = 0.0f;
            }

            iy = 0;
            i = idx + lda * (lastc - 1);
            for (iac = idx; lda < 0 ? iac >= i : iac <= i; iac += lda) {
              iQR0 = iaii;
              c = 0.0f;
              i1 = (iac + lastv) - 1;
              for (ia = iac; ia <= i1; ia++) {
                c += obj->Q->data[ia - 1] * obj->Q->data[iQR0 - 1];
                iQR0++;
              }

              work->data[iy] += c;
              iy++;
            }
          }

          xgerc(lastv, lastc, -obj->tau->data[itau], iaii, work, obj->Q, idx,
                lda);
        }
      }

      if (b_i < m) {
        iQR0 = iaii + 1;
        i = (iaii + m) - b_i;
        for (lastv = iQR0; lastv <= i; lastv++) {
          obj->Q->data[lastv - 1] *= -obj->tau->data[itau];
        }
      }

      obj->Q->data[iaii - 1] = 1.0f - obj->tau->data[itau];
      for (iQR0 = 0; iQR0 <= b_i - 2; iQR0++) {
        obj->Q->data[(iaii - iQR0) - 2] = 0.0f;
      }

      itau--;
    }

    emxFree_real_T(&work);
  }
}

/* End of code generation (computeQ_.c) */
