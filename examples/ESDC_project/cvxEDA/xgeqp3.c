/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xgeqp3.c
 *
 * Code generation for function 'xgeqp3'
 *
 */

/* Include files */
#include "xgeqp3.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "xzgeqp3.h"
#include "xzlarf.h"
#include "xzlarfg.h"
#include <math.h>

/* Function Definitions */
void xgeqp3(emxArray_real_T *A, int m, int n, emxArray_int32_T *jpvt,
            emxArray_real_T *tau)
{
  emxArray_real_T *vn1;
  emxArray_real_T *vn2;
  emxArray_real_T *work;
  float d;
  float s;
  float temp;
  int b_i;
  int i;
  int ii;
  int ip1;
  int ix;
  int iy;
  int k;
  int ma;
  int minmana;
  int minmn_tmp;
  int mmi;
  int nmi;
  int pvt;
  ma = A->size[0];
  pvt = A->size[0];
  minmana = A->size[1];
  if (pvt < minmana) {
    minmana = pvt;
  }

  if (m < n) {
    minmn_tmp = m;
  } else {
    minmn_tmp = n;
  }

  i = tau->size[0];
  tau->size[0] = minmana;
  emxEnsureCapacity_real_T(tau, i);
  for (i = 0; i < minmana; i++) {
    tau->data[i] = 0.0;
  }

  if (minmn_tmp < 1) {
    for (ii = 0; ii < n; ii++) {
      jpvt->data[ii] = ii + 1;
    }
  } else {
    minmana = 0;
    for (ii = 0; ii < n; ii++) {
      if (jpvt->data[ii] != 0) {
        minmana++;
        if (ii + 1 != minmana) {
          ix = ii * ma;
          iy = (minmana - 1) * ma;
          for (k = 0; k < m; k++) {
            temp = A->data[ix];
            A->data[ix] = A->data[iy];
            A->data[iy] = temp;
            ix++;
            iy++;
          }

          jpvt->data[ii] = jpvt->data[minmana - 1];
          jpvt->data[minmana - 1] = ii + 1;
        } else {
          jpvt->data[ii] = ii + 1;
        }
      } else {
        jpvt->data[ii] = ii + 1;
      }
    }

    if (minmana >= minmn_tmp) {
      minmana = minmn_tmp;
    }

    qrf(A, m, n, minmana, tau);
    if (minmana < minmn_tmp) {
      emxInit_real_T(&work, 1);
      ma = A->size[0];
      i = work->size[0];
      work->size[0] = A->size[1];
      emxEnsureCapacity_real_T(work, i);
      pvt = A->size[1];
      for (i = 0; i < pvt; i++) {
        work->data[i] = 0.0;
      }

      emxInit_real_T(&vn1, 1);
      i = vn1->size[0];
      vn1->size[0] = A->size[1];
      emxEnsureCapacity_real_T(vn1, i);
      pvt = A->size[1];
      for (i = 0; i < pvt; i++) {
        vn1->data[i] = 0.0f;
      }

      emxInit_real_T(&vn2, 1);
      i = vn2->size[0];
      vn2->size[0] = A->size[1];
      emxEnsureCapacity_real_T(vn2, i);
      pvt = A->size[1];
      for (i = 0; i < pvt; i++) {
        vn2->data[i] = 0.0f;
      }

      i = minmana + 1;
      for (ii = i; ii <= n; ii++) {
        d = xnrm2(m - minmana, A, (minmana + (ii - 1) * ma) + 1);
        vn1->data[ii - 1] = d;
        vn2->data[ii - 1] = d;
      }

      i = minmana + 1;
      for (b_i = i; b_i <= minmn_tmp; b_i++) {
        ip1 = b_i + 1;
        iy = (b_i - 1) * ma;
        ii = (iy + b_i) - 1;
        nmi = (n - b_i) + 1;
        mmi = m - b_i;
        if (nmi < 1) {
          minmana = -2;
        } else {
          minmana = -1;
          if (nmi > 1) {
            ix = b_i;
            temp = fabsf(vn1->data[b_i - 1]);
            for (k = 2; k <= nmi; k++) {
              ix++;
              s = fabsf(vn1->data[ix - 1]);
              if (s > temp) {
                minmana = k - 2;
                temp = s;
              }
            }
          }
        }

        pvt = b_i + minmana;
        if (pvt + 1 != b_i) {
          ix = pvt * ma;
          for (k = 0; k < m; k++) {
            temp = A->data[ix];
            A->data[ix] = A->data[iy];
            A->data[iy] = temp;
            ix++;
            iy++;
          }

          minmana = jpvt->data[pvt];
          jpvt->data[pvt] = jpvt->data[b_i - 1];
          jpvt->data[b_i - 1] = minmana;
          vn1->data[pvt] = vn1->data[b_i - 1];
          vn2->data[pvt] = vn2->data[b_i - 1];
        }

        if (b_i < m) {
          temp = A->data[ii];
          d = xzlarfg(mmi + 1, &temp, A, ii + 2);
          tau->data[b_i - 1] = d;
          A->data[ii] = temp;
        } else {
          d = 0.0f;
          tau->data[b_i - 1] = 0.0f;
        }

        if (b_i < n) {
          temp = A->data[ii];
          A->data[ii] = 1.0f;
          xzlarf(mmi + 1, nmi - 1, ii + 1, d, A, (ii + ma) + 1, ma, work);
          A->data[ii] = temp;
        }

        for (ii = ip1; ii <= n; ii++) {
          pvt = b_i + (ii - 1) * ma;
          d = vn1->data[ii - 1];
          if (d != 0.0f) {
            temp = fabsf(A->data[pvt - 1]) / d;
            temp = 1.0f - temp * temp;
            if (temp < 0.0f) {
              temp = 0.0f;
            }

            s = d / vn2->data[ii - 1];
            s = temp * (s * s);
            if (s <= 1.4901161193847656E-8f) {
              if (b_i < m) {
                d = xnrm2(mmi, A, pvt + 1);
                vn1->data[ii - 1] = d;
                vn2->data[ii - 1] = d;
              } else {
                vn1->data[ii - 1] = 0.0f;
                vn2->data[ii - 1] = 0.0f;
              }
            } else {
              vn1->data[ii - 1] = d * sqrtf(temp);
            }
          }
        }
      }

      emxFree_real_T(&vn2);
      emxFree_real_T(&vn1);
      emxFree_real_T(&work);
    }
  }
}

/* End of code generation (xgeqp3.c) */
