/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * my_findpeaks.c
 *
 * Code generation for function 'my_findpeaks'
 *
 */

/* Include files */
#include "my_findpeaks.h"
#include "eml_setop.h"
#include "my_findpeaks_emxutil.h"
#include "my_findpeaks_types.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void my_findpeaks(const float input[120], float distance, float len,
                  emxArray_real32_T *z)
{
  double d;
  double d1;
  float yk;
  float ykfirst;
  int c_data[240];
  int iwork_data[240];
  int sortIdx_data[240];
  int b_iFinite_data[120];
  int iInfinite_data[120];
  int iInflect_data[120];
  int iPk_data[120];
  int c_size[1];
  int iFinite_size[1];
  int iInfinite_size[1];
  int iInflect_size[1];
  int iPk_size[1];
  int iwork_size[1];
  int b_i;
  int i;
  int k;
  int kEnd;
  int kfirst;
  int n;
  int nInf;
  int nPk;
  int p;
  int q;
  int qEnd;
  int sortIdx_size_idx_0;
  signed char locs_temp_data[240];
  unsigned char tmp_data[240];
  signed char iFinite_data[120];
  char dir;
  char previousdir;
  boolean_T b_tmp_data[240];
  boolean_T idelete_data[240];
  boolean_T isinfyk;
  boolean_T isinfykfirst;
  nPk = -1;
  nInf = -1;
  dir = 'n';
  kfirst = -1;
  ykfirst = rtInfF;
  isinfykfirst = true;
  for (k = 0; k < 120; k++) {
    yk = input[k];
    if (rtIsNaNF(yk)) {
      yk = rtInfF;
      isinfyk = true;
    } else if (rtIsInfF(yk) && (yk > 0.0F)) {
      isinfyk = true;
      nInf++;
      iInfinite_data[nInf] = k + 1;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
      } else if (yk < ykfirst) {
        dir = 'd';
        if (('d' != previousdir) && (previousdir == 'i')) {
          nPk++;
          b_iFinite_data[nPk] = kfirst + 1;
        }
      } else {
        dir = 'i';
      }

      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }

  if (1 > nPk + 1) {
    p = -1;
  } else {
    p = nPk;
  }

  kfirst = p + 1;
  for (i = 0; i <= p; i++) {
    iFinite_data[i] = (signed char)b_iFinite_data[i];
  }

  for (i = 0; i < kfirst; i++) {
    b_iFinite_data[i] = iFinite_data[i];
  }

  if (1 > nInf + 1) {
    kfirst = -1;
  } else {
    kfirst = nInf;
  }

  nPk = kfirst + 1;
  for (i = 0; i <= kfirst; i++) {
    iFinite_data[i] = (signed char)iInfinite_data[i];
  }

  iInfinite_size[0] = kfirst + 1;
  for (i = 0; i < nPk; i++) {
    iInfinite_data[i] = iFinite_data[i];
  }

  nPk = 0;
  n = p + 1;
  for (k = 0; k < n; k++) {
    i = b_iFinite_data[k];
    ykfirst = input[i - 1];
    if ((ykfirst > 0.0F) && (ykfirst - fmaxf(input[i - 2], input[i]) >= 0.0F)) {
      nPk++;
      iPk_data[nPk - 1] = i;
    }
  }

  if (1 > nPk) {
    iPk_size[0] = 0;
  } else {
    iPk_size[0] = nPk;
  }

  do_vectors(iPk_data, iPk_size, iInfinite_data, iInfinite_size, c_data, c_size,
             iInflect_data, iInflect_size, b_iFinite_data, iFinite_size);
  if ((c_size[0] == 0) || (distance == 0.0F)) {
    if (c_size[0] < 1) {
      n = 0;
    } else {
      n = c_size[0];
    }

    if (n > 0) {
      sortIdx_data[0] = 1;
      kfirst = 1;
      for (k = 2; k <= n; k++) {
        kfirst++;
        sortIdx_data[k - 1] = kfirst;
      }
    }

    iwork_size[0] = n;
    if (0 <= n - 1) {
      memcpy(&iwork_data[0], &sortIdx_data[0], n * sizeof(int));
    }
  } else {
    n = c_size[0] + 1;
    sortIdx_size_idx_0 = (unsigned char)c_size[0];
    p = (unsigned char)c_size[0];
    if (0 <= p - 1) {
      memset(&sortIdx_data[0], 0, p * sizeof(int));
    }

    i = c_size[0] - 1;
    for (k = 1; k <= i; k += 2) {
      ykfirst = input[c_data[k - 1] - 1];
      if ((ykfirst >= input[c_data[k] - 1]) || rtIsNaNF(ykfirst)) {
        sortIdx_data[k - 1] = k;
        sortIdx_data[k] = k + 1;
      } else {
        sortIdx_data[k - 1] = k + 1;
        sortIdx_data[k] = k;
      }
    }

    if ((c_size[0] & 1) != 0) {
      sortIdx_data[c_size[0] - 1] = c_size[0];
    }

    b_i = 2;
    while (b_i < n - 1) {
      kfirst = b_i << 1;
      nPk = 1;
      for (nInf = b_i + 1; nInf < n; nInf = qEnd + b_i) {
        p = nPk - 1;
        q = nInf;
        qEnd = nPk + kfirst;
        if (qEnd > n) {
          qEnd = n;
        }

        k = 0;
        kEnd = qEnd - nPk;
        while (k + 1 <= kEnd) {
          ykfirst = input[c_data[sortIdx_data[p] - 1] - 1];
          i = sortIdx_data[q - 1];
          if ((ykfirst >= input[c_data[i - 1] - 1]) || rtIsNaNF(ykfirst)) {
            iwork_data[k] = sortIdx_data[p];
            p++;
            if (p + 1 == nInf) {
              while (q < qEnd) {
                k++;
                iwork_data[k] = sortIdx_data[q - 1];
                q++;
              }
            }
          } else {
            iwork_data[k] = i;
            q++;
            if (q == qEnd) {
              while (p + 1 < nInf) {
                k++;
                iwork_data[k] = sortIdx_data[p];
                p++;
              }
            }
          }

          k++;
        }

        for (k = 0; k < kEnd; k++) {
          sortIdx_data[(nPk + k) - 1] = iwork_data[k];
        }

        nPk = qEnd;
      }

      b_i = kfirst;
    }

    for (i = 0; i < sortIdx_size_idx_0; i++) {
      locs_temp_data[i] = (signed char)((signed char)(c_data[sortIdx_data[i] - 1]
        - 1) + 1);
    }

    if (0 <= sortIdx_size_idx_0 - 1) {
      memset(&idelete_data[0], 0, sortIdx_size_idx_0 * sizeof(boolean_T));
    }

    for (b_i = 0; b_i < sortIdx_size_idx_0; b_i++) {
      if (!idelete_data[b_i]) {
        i = (signed char)((signed char)(c_data[sortIdx_data[b_i] - 1] - 1) + 1);
        d = (float)i - distance;
        d1 = (float)i + distance;
        for (i = 0; i < sortIdx_size_idx_0; i++) {
          kfirst = locs_temp_data[i];
          b_tmp_data[i] = ((kfirst >= d) && (kfirst <= d1));
        }

        for (i = 0; i < sortIdx_size_idx_0; i++) {
          idelete_data[i] = (idelete_data[i] || b_tmp_data[i]);
        }

        idelete_data[b_i] = false;
      }
    }

    kfirst = (unsigned char)c_size[0] - 1;
    nInf = 0;
    nPk = 0;
    for (b_i = 0; b_i <= kfirst; b_i++) {
      if (!idelete_data[b_i]) {
        nInf++;
        tmp_data[nPk] = (unsigned char)(b_i + 1);
        nPk++;
      }
    }

    iwork_size[0] = nInf;
    for (i = 0; i < nInf; i++) {
      iwork_data[i] = sortIdx_data[tmp_data[i] - 1];
    }

    my_sort(iwork_data, iwork_size);
  }

  if (iwork_size[0] > 120) {
    memcpy(&sortIdx_data[0], &iwork_data[0], 120U * sizeof(int));
    iwork_size[0] = 120;
    memcpy(&iwork_data[0], &sortIdx_data[0], 120U * sizeof(int));
  }

  p = iwork_size[0];
  for (i = 0; i < p; i++) {
    iPk_data[i] = c_data[iwork_data[i] - 1];
  }

  i = z->size[0];
  z->size[0] = (int)len;
  emxEnsureCapacity_real32_T(z, i);
  p = (int)len;
  for (i = 0; i < p; i++) {
    z->data[i] = 0.0F;
  }

  p = iwork_size[0];
  for (i = 0; i < p; i++) {
    z->data[c_data[iwork_data[i] - 1] - 1] = input[iPk_data[i] - 1];
  }
}

/* End of code generation (my_findpeaks.c) */
