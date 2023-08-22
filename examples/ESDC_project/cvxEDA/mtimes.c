/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mtimes.c
 *
 * Code generation for function 'mtimes'
 *
 */

/* Include files */
#include "mtimes.h"
#include "cvxEDA_data.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_types.h"
#include "fillIn.h"
#include "introsort.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void b_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, const float
                     b[240], float c[240])
{
  float bc;
  int acol;
  int ap;
  int apend;
  int c_tmp;
  int coff;
  int i;
  int j;
  int nap;
  memset(&c[0], 0, 240U * sizeof(float));
  if (a_colidx->data[a_colidx->size[0] - 1] - 1 != 0) {
    for (j = 0; j < 2; j++) {
      coff = j * 120 - 1;
      for (acol = 0; acol < 120; acol++) {
        bc = b[acol + 120 * j];
        i = a_colidx->data[acol];
        apend = a_colidx->data[acol + 1];
        nap = apend - a_colidx->data[acol];
        if (nap >= 4) {
          apend = (apend - nap) + ((nap / 4) << 2);
          for (ap = i; ap <= apend - 1; ap += 4) {
            c_tmp = a_rowidx->data[ap - 1] + coff;
            c[c_tmp] += a_d->data[ap - 1] * bc;
            c_tmp = a_rowidx->data[ap] + coff;
            c[c_tmp] += a_d->data[ap] * bc;
            c_tmp = a_rowidx->data[ap + 1] + coff;
            c[c_tmp] += a_d->data[ap + 1] * bc;
            c_tmp = a_rowidx->data[ap + 2] + coff;
            c[c_tmp] += a_d->data[ap + 2] * bc;
          }

          nap = a_colidx->data[acol + 1] - 1;
          for (ap = apend; ap <= nap; ap++) {
            c_tmp = (a_rowidx->data[ap - 1] + 120 * j) - 1;
            c[c_tmp] += a_d->data[ap - 1] * bc;
          }
        } else {
          apend--;
          for (ap = i; ap <= apend; ap++) {
            c_tmp = a_rowidx->data[ap - 1] + coff;
            c[c_tmp] += a_d->data[ap - 1] * bc;
          }
        }
      }
    }
  }
}

void c_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, const
                     emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                     const emxArray_int32_T *b_rowidx, int b_n, emxArray_real_T *
                     c_d, emxArray_int32_T *c_colidx, emxArray_int32_T *c_rowidx,
                     int *c_n)
{
  float wd[120];
  float bd;
  int flag[120];
  int acidx;
  int aend;
  int bcidx;
  int cmax;
  int cnnz;
  int cstart;
  int exitg1;
  int i;
  int j;
  int numalloc;
  boolean_T needSort;
  i = c_colidx->size[0];
  c_colidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(c_colidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    c_colidx->data[i] = 0;
  }

  memset(&flag[0], 0, 120U * sizeof(int));
  cnnz = 0;
  j = 0;
  do {
    exitg1 = 0;
    if (j <= b_n - 1) {
      bcidx = b_colidx->data[j];
      cstart = cnnz;
      cmax = cnnz + 120;
      c_colidx->data[j] = cnnz + 1;
      while ((bcidx < b_colidx->data[j + 1]) && (cnnz <= cmax)) {
        numalloc = b_rowidx->data[bcidx - 1];
        aend = a_colidx->data[numalloc] - 1;
        i = a_colidx->data[numalloc - 1];
        for (acidx = i; acidx <= aend; acidx++) {
          numalloc = a_rowidx->data[acidx - 1] - 1;
          if (flag[numalloc] != j + 1) {
            flag[numalloc] = j + 1;
            cnnz++;
          }
        }

        bcidx++;
      }

      if (cnnz < cstart) {
        exitg1 = 1;
      } else {
        j++;
      }
    } else {
      c_colidx->data[b_n] = cnnz + 1;
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  if (cnnz >= 1) {
    numalloc = cnnz;
  } else {
    numalloc = 1;
  }

  i = c_d->size[0];
  c_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c_d, i);
  for (i = 0; i < numalloc; i++) {
    c_d->data[i] = 0.0f;
  }

  i = c_rowidx->size[0];
  c_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c_rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c_rowidx->data[i] = 0;
  }

  memset(&flag[0], 0, 120U * sizeof(int));
  cmax = 0;
  cnnz = -1;
  for (j = 0; j < b_n; j++) {
    needSort = false;
    cstart = cnnz + 2;
    numalloc = (b_colidx->data[j + 1] - cmax) - 1;
    if (numalloc != 0) {
      if (numalloc == 1) {
        bcidx = a_colidx->data[b_rowidx->data[cmax]] - 1;
        i = a_colidx->data[b_rowidx->data[cmax] - 1];
        for (aend = i; aend <= bcidx; aend++) {
          cnnz++;
          numalloc = a_rowidx->data[aend - 1];
          c_rowidx->data[cnnz] = numalloc;
          wd[numalloc - 1] = a_d->data[aend - 1] * b_d->data[cmax];
        }

        cmax++;
      } else {
        bcidx = a_colidx->data[b_rowidx->data[cmax]] - 1;
        i = a_colidx->data[b_rowidx->data[cmax] - 1];
        for (aend = i; aend <= bcidx; aend++) {
          cnnz++;
          numalloc = a_rowidx->data[aend - 1];
          flag[numalloc - 1] = cnnz + 1;
          c_rowidx->data[cnnz] = numalloc;
          wd[numalloc - 1] = a_d->data[aend - 1] * b_d->data[cmax];
        }

        for (cmax++; cmax + 1 < b_colidx->data[j + 1]; cmax++) {
          bd = b_d->data[cmax];
          bcidx = a_colidx->data[b_rowidx->data[cmax]] - 1;
          i = a_colidx->data[b_rowidx->data[cmax] - 1];
          for (aend = i; aend <= bcidx; aend++) {
            numalloc = a_rowidx->data[aend - 1];
            if (flag[numalloc - 1] < cstart) {
              cnnz++;
              flag[numalloc - 1] = cnnz + 1;
              c_rowidx->data[cnnz] = numalloc;
              wd[numalloc - 1] = a_d->data[aend - 1] * bd;
              needSort = true;
            } else {
              wd[numalloc - 1] += a_d->data[aend - 1] * bd;
            }
          }
        }
      }
    }

    numalloc = c_colidx->data[j + 1] - 1;
    cstart = c_colidx->data[j];
    if (needSort) {
      c_introsort(c_rowidx, c_colidx->data[j], c_colidx->data[j + 1] - 1);
    }

    for (bcidx = cstart; bcidx <= numalloc; bcidx++) {
      c_d->data[bcidx - 1] = wd[c_rowidx->data[bcidx - 1] - 1];
    }
  }

  numalloc = 1;
  i = c_colidx->size[0];
  for (cmax = 0; cmax <= i - 2; cmax++) {
    bcidx = c_colidx->data[cmax];
    c_colidx->data[cmax] = numalloc;
    while (bcidx < c_colidx->data[cmax + 1]) {
      cstart = c_rowidx->data[bcidx - 1];
      bd = c_d->data[bcidx - 1];
      bcidx++;
      if (bd != 0.0f) {
        c_d->data[numalloc - 1] = bd;
        c_rowidx->data[numalloc - 1] = cstart;
        numalloc++;
      }
    }
  }

  c_colidx->data[c_colidx->size[0] - 1] = numalloc;
  *c_n = b_n;
}

void d_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, const
                     emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                     const emxArray_int32_T *b_rowidx, emxArray_real_T *c_d,
                     emxArray_int32_T *c_colidx, emxArray_int32_T *c_rowidx, int
                     *c_m)
{
  emxArray_int32_T *flag;
  emxArray_real_T *wd;
  float bd;
  int bcidx;
  int cmax;
  int cnnz;
  int cstart;
  int exitg1;
  int i;
  int j;
  int numalloc;
  int pa;
  int pb;
  boolean_T needSort;
  i = c_colidx->size[0];
  c_colidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(c_colidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    c_colidx->data[i] = 0;
  }

  emxInit_int32_T(&flag, 1);
  i = flag->size[0];
  flag->size[0] = a_m;
  emxEnsureCapacity_int32_T(flag, i);
  for (i = 0; i < a_m; i++) {
    flag->data[i] = 0;
  }

  cnnz = 0;
  j = 0;
  do {
    exitg1 = 0;
    if (j < 120) {
      bcidx = b_colidx->data[j];
      cstart = cnnz;
      cmax = cnnz + a_m;
      c_colidx->data[j] = cnnz + 1;
      while ((bcidx < b_colidx->data[j + 1]) && (cnnz <= cmax)) {
        numalloc = b_rowidx->data[bcidx - 1];
        pb = a_colidx->data[numalloc] - 1;
        i = a_colidx->data[numalloc - 1];
        for (pa = i; pa <= pb; pa++) {
          numalloc = a_rowidx->data[pa - 1] - 1;
          if (flag->data[numalloc] != j + 1) {
            flag->data[numalloc] = j + 1;
            cnnz++;
          }
        }

        bcidx++;
      }

      if (cnnz < cstart) {
        exitg1 = 1;
      } else {
        j++;
      }
    } else {
      c_colidx->data[120] = cnnz + 1;
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  if (cnnz >= 1) {
    numalloc = cnnz;
  } else {
    numalloc = 1;
  }

  i = c_d->size[0];
  c_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c_d, i);
  for (i = 0; i < numalloc; i++) {
    c_d->data[i] = 0.0;
  }

  i = c_rowidx->size[0];
  c_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c_rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c_rowidx->data[i] = 0;
  }

  emxInit_real_T(&wd, 1);
  i = wd->size[0];
  wd->size[0] = a_m;
  emxEnsureCapacity_real_T(wd, i);
  i = flag->size[0];
  flag->size[0] = a_m;
  emxEnsureCapacity_int32_T(flag, i);
  for (i = 0; i < a_m; i++) {
    flag->data[i] = 0;
  }

  pb = 0;
  cnnz = -1;
  for (j = 0; j < 120; j++) {
    needSort = false;
    bcidx = cnnz + 2;
    cstart = b_colidx->data[j + 1];
    numalloc = (cstart - pb) - 1;
    if (numalloc != 0) {
      if (numalloc == 1) {
        cmax = a_colidx->data[b_rowidx->data[pb]] - 1;
        i = a_colidx->data[b_rowidx->data[pb] - 1];
        for (pa = i; pa <= cmax; pa++) {
          cnnz++;
          numalloc = a_rowidx->data[pa - 1];
          c_rowidx->data[cnnz] = numalloc;
          wd->data[numalloc - 1] = a_d->data[pa - 1] * b_d->data[pb];
        }

        pb++;
      } else {
        cmax = a_colidx->data[b_rowidx->data[pb]] - 1;
        i = a_colidx->data[b_rowidx->data[pb] - 1];
        for (pa = i; pa <= cmax; pa++) {
          cnnz++;
          numalloc = a_rowidx->data[pa - 1];
          flag->data[numalloc - 1] = cnnz + 1;
          c_rowidx->data[cnnz] = numalloc;
          wd->data[numalloc - 1] = a_d->data[pa - 1] * b_d->data[pb];
        }

        for (pb++; pb + 1 < cstart; pb++) {
          bd = b_d->data[pb];
          cmax = a_colidx->data[b_rowidx->data[pb]] - 1;
          i = a_colidx->data[b_rowidx->data[pb] - 1];
          for (pa = i; pa <= cmax; pa++) {
            numalloc = a_rowidx->data[pa - 1];
            if (flag->data[numalloc - 1] < bcidx) {
              cnnz++;
              flag->data[numalloc - 1] = cnnz + 1;
              c_rowidx->data[cnnz] = numalloc;
              wd->data[numalloc - 1] = a_d->data[pa - 1] * bd;
              needSort = true;
            } else {
              wd->data[numalloc - 1] += a_d->data[pa - 1] * bd;
            }
          }
        }
      }
    }

    numalloc = c_colidx->data[j + 1] - 1;
    i = c_colidx->data[j];
    if (needSort) {
      c_introsort(c_rowidx, c_colidx->data[j], c_colidx->data[j + 1] - 1);
    }

    for (bcidx = i; bcidx <= numalloc; bcidx++) {
      c_d->data[bcidx - 1] = wd->data[c_rowidx->data[bcidx - 1] - 1];
    }
  }

  emxFree_int32_T(&flag);
  emxFree_real_T(&wd);
  numalloc = 1;
  i = c_colidx->size[0];
  for (cmax = 0; cmax <= i - 2; cmax++) {
    bcidx = c_colidx->data[cmax];
    c_colidx->data[cmax] = numalloc;
    while (bcidx < c_colidx->data[cmax + 1]) {
      cstart = c_rowidx->data[bcidx - 1];
      bd = c_d->data[bcidx - 1];
      bcidx++;
      if (bd != 0.0f) {
        c_d->data[numalloc - 1] = bd;
        c_rowidx->data[numalloc - 1] = cstart;
        numalloc++;
      }
    }
  }

  c_colidx->data[c_colidx->size[0] - 1] = numalloc;
  *c_m = a_m;
}

void e_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, int
                     a_n, emxArray_real_T *c)
{
  float bc;
  int acol;
  int ap;
  int apend;
  int apend1;
  int coff;
  int i;
  int nap;
  i = c->size[0] * c->size[1];
  c->size[0] = a_m;
  c->size[1] = 2;
  emxEnsureCapacity_real_T(c, i);
  apend = a_m << 1;
  for (i = 0; i < apend; i++) {
    c->data[i] = 0.0;
  }

  if ((a_n != 0) && (a_m != 0) && (a_colidx->data[a_colidx->size[0] - 1] - 1 !=
       0)) {
    for (acol = 0; acol < a_n; acol++) {
      bc = dv[acol];
      i = a_colidx->data[acol];
      apend = a_colidx->data[acol + 1];
      nap = apend - a_colidx->data[acol];
      if (nap >= 4) {
        apend1 = (apend - nap) + ((nap / 4) << 2);
        for (ap = i; ap <= apend1 - 1; ap += 4) {
          nap = a_rowidx->data[ap - 1];
          c->data[nap + -1] += a_d->data[ap - 1] * bc;
          c->data[a_rowidx->data[ap] + -1] += a_d->data[ap] * bc;
          nap = a_rowidx->data[ap + 1];
          c->data[nap + -1] += a_d->data[ap + 1] * bc;
          nap = a_rowidx->data[ap + 2];
          c->data[nap + -1] += a_d->data[ap + 2] * bc;
        }

        nap = a_colidx->data[acol + 1] - 1;
        for (ap = apend1; ap <= nap; ap++) {
          apend = a_rowidx->data[ap - 1] - 1;
          c->data[apend] += a_d->data[ap - 1] * bc;
        }
      } else {
        apend--;
        for (ap = i; ap <= apend; ap++) {
          nap = a_rowidx->data[ap - 1];
          c->data[nap + -1] += a_d->data[ap - 1] * bc;
        }
      }
    }

    coff = c->size[0] - 1;
    for (acol = 0; acol < a_n; acol++) {
      bc = dv[acol + 120];
      i = a_colidx->data[acol];
      nap = a_colidx->data[acol + 1] - a_colidx->data[acol];
      if (nap >= 4) {
        apend1 = (a_colidx->data[acol + 1] - nap) + ((nap / 4) << 2);
        for (ap = i; ap <= apend1 - 1; ap += 4) {
          nap = a_rowidx->data[ap - 1];
          nap += coff;
          c->data[nap] += a_d->data[ap - 1] * bc;
          c->data[a_rowidx->data[ap] + coff] += a_d->data[ap] * bc;
          nap = a_rowidx->data[ap + 1];
          nap += coff;
          c->data[nap] += a_d->data[ap + 1] * bc;
          nap = a_rowidx->data[ap + 2];
          nap += coff;
          c->data[nap] += a_d->data[ap + 2] * bc;
        }

        nap = a_colidx->data[acol + 1] - 1;
        for (ap = apend1; ap <= nap; ap++) {
          apend = a_rowidx->data[ap - 1] - 1;
          c->data[apend + c->size[0]] += a_d->data[ap - 1] * bc;
        }
      } else {
        apend = a_colidx->data[acol + 1] - 1;
        for (ap = i; ap <= apend; ap++) {
          nap = a_rowidx->data[ap - 1];
          nap += coff;
          c->data[nap] += a_d->data[ap - 1] * bc;
        }
      }
    }
  }
}

void f_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, const
                     emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                     const emxArray_int32_T *b_rowidx, int b_n, i_sparse *c)
{
  emxArray_int32_T *ccolidx;
  emxArray_int32_T *flag;
  emxArray_real_T *wd;
  float bd;
  int acidx;
  int aend;
  int bcidx;
  int cmax;
  int cnnz;
  int cstart;
  int exitg1;
  int i;
  int j;
  int numalloc;
  boolean_T needSort;
  emxInit_int32_T(&ccolidx, 1);
  i = ccolidx->size[0];
  ccolidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(ccolidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    ccolidx->data[i] = 0;
  }

  emxInit_int32_T(&flag, 1);
  i = flag->size[0];
  flag->size[0] = a_m;
  emxEnsureCapacity_int32_T(flag, i);
  for (i = 0; i < a_m; i++) {
    flag->data[i] = 0;
  }

  cnnz = 0;
  j = 0;
  do {
    exitg1 = 0;
    if (j <= b_n - 1) {
      bcidx = b_colidx->data[j];
      cstart = cnnz;
      cmax = cnnz + a_m;
      ccolidx->data[j] = cnnz + 1;
      while ((bcidx < b_colidx->data[j + 1]) && (cnnz <= cmax)) {
        numalloc = b_rowidx->data[bcidx - 1];
        aend = a_colidx->data[numalloc] - 1;
        i = a_colidx->data[numalloc - 1];
        for (acidx = i; acidx <= aend; acidx++) {
          numalloc = a_rowidx->data[acidx - 1] - 1;
          if (flag->data[numalloc] != j + 1) {
            flag->data[numalloc] = j + 1;
            cnnz++;
          }
        }

        bcidx++;
      }

      if (cnnz < cstart) {
        exitg1 = 1;
      } else {
        j++;
      }
    } else {
      ccolidx->data[b_n] = cnnz + 1;
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  c->m = a_m;
  c->n = b_n;
  if (cnnz >= 1) {
    numalloc = cnnz;
  } else {
    numalloc = 1;
  }

  i = c->d->size[0];
  c->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c->d, i);
  for (i = 0; i < numalloc; i++) {
    c->d->data[i] = 0.0f;
  }

  i = c->colidx->size[0];
  c->colidx->size[0] = b_n + 1;
  emxEnsureCapacity_int32_T(c->colidx, i);
  c->colidx->data[0] = 1;
  i = c->rowidx->size[0];
  c->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c->rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c->rowidx->data[i] = 0;
  }

  for (numalloc = 0; numalloc < b_n; numalloc++) {
    c->colidx->data[numalloc + 1] = 1;
  }

  b_sparse_fillIn(c);
  i = c->colidx->size[0];
  c->colidx->size[0] = ccolidx->size[0];
  emxEnsureCapacity_int32_T(c->colidx, i);
  numalloc = ccolidx->size[0];
  for (i = 0; i < numalloc; i++) {
    c->colidx->data[i] = ccolidx->data[i];
  }

  emxInit_real_T(&wd, 1);
  i = wd->size[0];
  wd->size[0] = a_m;
  emxEnsureCapacity_real_T(wd, i);
  i = flag->size[0];
  flag->size[0] = a_m;
  emxEnsureCapacity_int32_T(flag, i);
  for (i = 0; i < a_m; i++) {
    flag->data[i] = 0;
  }

  cstart = 0;
  cnnz = -1;
  for (j = 0; j < b_n; j++) {
    needSort = false;
    cmax = cnnz + 2;
    numalloc = (b_colidx->data[j + 1] - cstart) - 1;
    if (numalloc != 0) {
      if (numalloc == 1) {
        bcidx = a_colidx->data[b_rowidx->data[cstart]] - 1;
        i = a_colidx->data[b_rowidx->data[cstart] - 1];
        for (aend = i; aend <= bcidx; aend++) {
          cnnz++;
          numalloc = a_rowidx->data[aend - 1];
          c->rowidx->data[cnnz] = numalloc;
          wd->data[numalloc - 1] = a_d->data[aend - 1] * b_d->data[cstart];
        }

        cstart++;
      } else {
        bcidx = a_colidx->data[b_rowidx->data[cstart]] - 1;
        i = a_colidx->data[b_rowidx->data[cstart] - 1];
        for (aend = i; aend <= bcidx; aend++) {
          cnnz++;
          numalloc = a_rowidx->data[aend - 1];
          flag->data[numalloc - 1] = cnnz + 1;
          c->rowidx->data[cnnz] = numalloc;
          wd->data[numalloc - 1] = a_d->data[aend - 1] * b_d->data[cstart];
        }

        for (cstart++; cstart + 1 < b_colidx->data[j + 1]; cstart++) {
          bd = b_d->data[cstart];
          bcidx = a_colidx->data[b_rowidx->data[cstart]] - 1;
          i = a_colidx->data[b_rowidx->data[cstart] - 1];
          for (aend = i; aend <= bcidx; aend++) {
            numalloc = a_rowidx->data[aend - 1];
            if (flag->data[numalloc - 1] < cmax) {
              cnnz++;
              flag->data[numalloc - 1] = cnnz + 1;
              c->rowidx->data[cnnz] = numalloc;
              wd->data[numalloc - 1] = a_d->data[aend - 1] * bd;
              needSort = true;
            } else {
              wd->data[numalloc - 1] += a_d->data[aend - 1] * bd;
            }
          }
        }
      }
    }

    numalloc = ccolidx->data[j + 1] - 1;
    cmax = ccolidx->data[j];
    if (needSort) {
      c_introsort(c->rowidx, ccolidx->data[j], ccolidx->data[j + 1] - 1);
    }

    for (bcidx = cmax; bcidx <= numalloc; bcidx++) {
      c->d->data[bcidx - 1] = wd->data[c->rowidx->data[bcidx - 1] - 1];
    }
  }

  emxFree_int32_T(&flag);
  emxFree_int32_T(&ccolidx);
  emxFree_real_T(&wd);
  d_sparse_fillIn(c);
}

void g_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, const float
                     b[120], float c[120])
{
  float bc;
  int acol;
  int ap;
  int apend;
  int c_tmp;
  int i;
  int nap;
  memset(&c[0], 0, 120U * sizeof(float));
  if (a_colidx->data[a_colidx->size[0] - 1] - 1 != 0) {
    for (acol = 0; acol < 120; acol++) {
      bc = b[acol];
      i = a_colidx->data[acol];
      apend = a_colidx->data[acol + 1];
      nap = apend - a_colidx->data[acol];
      if (nap >= 4) {
        apend = (apend - nap) + ((nap / 4) << 2);
        for (ap = i; ap <= apend - 1; ap += 4) {
          c_tmp = a_rowidx->data[ap - 1] - 1;
          c[c_tmp] += a_d->data[ap - 1] * bc;
          c[a_rowidx->data[ap] - 1] += a_d->data[ap] * bc;
          c_tmp = a_rowidx->data[ap + 1] - 1;
          c[c_tmp] += a_d->data[ap + 1] * bc;
          c_tmp = a_rowidx->data[ap + 2] - 1;
          c[c_tmp] += a_d->data[ap + 2] * bc;
        }

        nap = a_colidx->data[acol + 1] - 1;
        for (ap = apend; ap <= nap; ap++) {
          c_tmp = a_rowidx->data[ap - 1] - 1;
          c[c_tmp] += a_d->data[ap - 1] * bc;
        }
      } else {
        apend--;
        for (ap = i; ap <= apend; ap++) {
          c_tmp = a_rowidx->data[ap - 1] - 1;
          c[c_tmp] += a_d->data[ap - 1] * bc;
        }
      }
    }
  }
}

void h_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, int
                     a_n, const float b[120], emxArray_real_T *c)
{
  float bc;
  int acol;
  int ap;
  int apend;
  int c_tmp;
  int i;
  int nap;
  i = c->size[0];
  c->size[0] = a_m;
  emxEnsureCapacity_real_T(c, i);
  for (i = 0; i < a_m; i++) {
    c->data[i] = 0.0f;
  }

  if ((a_n != 0) && (a_m != 0) && (a_colidx->data[a_colidx->size[0] - 1] - 1 !=
       0)) {
    for (acol = 0; acol < a_n; acol++) {
      bc = b[acol];
      i = a_colidx->data[acol];
      apend = a_colidx->data[acol + 1];
      nap = apend - a_colidx->data[acol];
      if (nap >= 4) {
        apend = (apend - nap) + ((nap / 4) << 2);
        for (ap = i; ap <= apend - 1; ap += 4) {
          c_tmp = a_rowidx->data[ap - 1] - 1;
          c->data[c_tmp] += a_d->data[ap - 1] * bc;
          c->data[a_rowidx->data[ap] - 1] += a_d->data[ap] * bc;
          c_tmp = a_rowidx->data[ap + 1] - 1;
          c->data[c_tmp] += a_d->data[ap + 1] * bc;
          c_tmp = a_rowidx->data[ap + 2] - 1;
          c->data[c_tmp] += a_d->data[ap + 2] * bc;
        }

        nap = a_colidx->data[acol + 1] - 1;
        for (ap = apend; ap <= nap; ap++) {
          c_tmp = a_rowidx->data[ap - 1] - 1;
          c->data[c_tmp] += a_d->data[ap - 1] * bc;
        }
      } else {
        apend--;
        for (ap = i; ap <= apend; ap++) {
          c_tmp = a_rowidx->data[ap - 1] - 1;
          c->data[c_tmp] += a_d->data[ap - 1] * bc;
        }
      }
    }
  }
}

void i_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, int a_m, const
                     emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                     const emxArray_int32_T *b_rowidx, emxArray_real_T *c_d,
                     emxArray_int32_T *c_colidx, emxArray_int32_T *c_rowidx, int
                     *c_m, int *c_maxnz)
{
  emxArray_int32_T *flag;
  emxArray_real_T *wd;
  float bd;
  int bcidx;
  int c;
  int cnnz;
  int i;
  int numalloc;
  int paend;
  int pb;
  boolean_T needSort;
  i = c_colidx->size[0];
  c_colidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(c_colidx, i);
  pb = b_colidx->size[0];
  for (i = 0; i < pb; i++) {
    c_colidx->data[i] = 0;
  }

  emxInit_int32_T(&flag, 1);
  i = flag->size[0];
  flag->size[0] = a_m;
  emxEnsureCapacity_int32_T(flag, i);
  for (i = 0; i < a_m; i++) {
    flag->data[i] = 0;
  }

  cnnz = 0;
  bcidx = b_colidx->data[0];
  c_colidx->data[0] = 1;
  while ((bcidx < b_colidx->data[1]) && (cnnz <= a_m)) {
    pb = b_rowidx->data[bcidx - 1];
    paend = a_colidx->data[pb] - 1;
    i = a_colidx->data[pb - 1];
    for (pb = i; pb <= paend; pb++) {
      c = a_rowidx->data[pb - 1] - 1;
      if (flag->data[c] != 1) {
        flag->data[c] = 1;
        cnnz++;
      }
    }

    bcidx++;
  }

  c_colidx->data[1] = cnnz + 1;
  if (cnnz >= 1) {
    numalloc = cnnz;
  } else {
    numalloc = 1;
  }

  i = c_d->size[0];
  c_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c_d, i);
  for (i = 0; i < numalloc; i++) {
    c_d->data[i] = 0.0f;
  }

  i = c_rowidx->size[0];
  c_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c_rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c_rowidx->data[i] = 0;
  }

  emxInit_real_T(&wd, 1);
  i = wd->size[0];
  wd->size[0] = a_m;
  emxEnsureCapacity_real_T(wd, i);
  i = flag->size[0];
  flag->size[0] = a_m;
  emxEnsureCapacity_int32_T(flag, i);
  for (i = 0; i < a_m; i++) {
    flag->data[i] = 0;
  }

  cnnz = -1;
  needSort = false;
  if (b_colidx->data[1] - 1 != 0) {
    if (b_colidx->data[1] - 1 == 1) {
      paend = a_colidx->data[b_rowidx->data[0]] - 1;
      i = a_colidx->data[b_rowidx->data[0] - 1];
      for (bcidx = i; bcidx <= paend; bcidx++) {
        cnnz++;
        pb = a_rowidx->data[bcidx - 1];
        c_rowidx->data[cnnz] = pb;
        wd->data[pb - 1] = a_d->data[bcidx - 1] * b_d->data[0];
      }
    } else {
      paend = a_colidx->data[b_rowidx->data[0]] - 1;
      i = a_colidx->data[b_rowidx->data[0] - 1];
      for (bcidx = i; bcidx <= paend; bcidx++) {
        cnnz++;
        pb = a_rowidx->data[bcidx - 1];
        flag->data[pb - 1] = cnnz + 1;
        c_rowidx->data[cnnz] = pb;
        wd->data[pb - 1] = a_d->data[bcidx - 1] * b_d->data[0];
      }

      for (pb = 1; pb + 1 < b_colidx->data[1]; pb++) {
        bd = b_d->data[pb];
        paend = a_colidx->data[b_rowidx->data[pb]] - 1;
        i = a_colidx->data[b_rowidx->data[pb] - 1];
        for (bcidx = i; bcidx <= paend; bcidx++) {
          c = a_rowidx->data[bcidx - 1];
          if (flag->data[c - 1] < 1) {
            cnnz++;
            flag->data[c - 1] = cnnz + 1;
            c_rowidx->data[cnnz] = c;
            wd->data[c - 1] = a_d->data[bcidx - 1] * bd;
            needSort = true;
          } else {
            wd->data[c - 1] += a_d->data[bcidx - 1] * bd;
          }
        }
      }
    }
  }

  emxFree_int32_T(&flag);
  pb = c_colidx->data[1] - 1;
  bcidx = c_colidx->data[0];
  if (needSort) {
    c_introsort(c_rowidx, c_colidx->data[0], c_colidx->data[1] - 1);
  }

  for (paend = bcidx; paend <= pb; paend++) {
    c_d->data[paend - 1] = wd->data[c_rowidx->data[paend - 1] - 1];
  }

  emxFree_real_T(&wd);
  pb = 1;
  i = b_colidx->size[0];
  for (c = 0; c <= i - 2; c++) {
    bcidx = c_colidx->data[c];
    c_colidx->data[c] = pb;
    while (bcidx < c_colidx->data[c + 1]) {
      paend = c_rowidx->data[bcidx - 1];
      bd = c_d->data[bcidx - 1];
      bcidx++;
      if (bd != 0.0f) {
        c_d->data[pb - 1] = bd;
        c_rowidx->data[pb - 1] = paend;
        pb++;
      }
    }
  }

  c_colidx->data[c_colidx->size[0] - 1] = pb;
  *c_m = a_m;
  *c_maxnz = numalloc;
}

void j_sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T
                     *a_colidx, const emxArray_int32_T *a_rowidx, const
                     emxArray_real_T *b_d, const emxArray_int32_T *b_colidx,
                     const emxArray_int32_T *b_rowidx, h_sparse *c)
{
  float wd[120];
  float bd;
  int flag[120];
  int aend;
  int bcidx;
  int cnnz;
  int i;
  int i1;
  int numalloc;
  boolean_T needSort;
  i = c->colidx->size[0];
  c->colidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(c->colidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    c->colidx->data[i] = 0;
  }

  memset(&flag[0], 0, 120U * sizeof(int));
  cnnz = 0;
  bcidx = b_colidx->data[0];
  c->colidx->data[0] = 1;
  while ((bcidx < b_colidx->data[1]) && (cnnz <= 120)) {
    numalloc = b_rowidx->data[bcidx - 1];
    aend = a_colidx->data[numalloc] - 1;
    i = a_colidx->data[numalloc - 1];
    for (numalloc = i; numalloc <= aend; numalloc++) {
      i1 = a_rowidx->data[numalloc - 1] - 1;
      if (flag[i1] != 1) {
        flag[i1] = 1;
        cnnz++;
      }
    }

    bcidx++;
  }

  if (cnnz >= 0) {
    c->colidx->data[1] = cnnz + 1;
  }

  if (cnnz >= 1) {
    numalloc = cnnz;
  } else {
    numalloc = 1;
  }

  i = c->d->size[0];
  c->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c->d, i);
  for (i = 0; i < numalloc; i++) {
    c->d->data[i] = 0.0f;
  }

  i = c->rowidx->size[0];
  c->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c->rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c->rowidx->data[i] = 0;
  }

  memset(&flag[0], 0, 120U * sizeof(int));
  cnnz = -1;
  needSort = false;
  if (b_colidx->data[1] - 1 != 0) {
    if (b_colidx->data[1] - 1 == 1) {
      bcidx = a_colidx->data[b_rowidx->data[0]] - 1;
      i = a_colidx->data[b_rowidx->data[0] - 1];
      for (aend = i; aend <= bcidx; aend++) {
        cnnz++;
        i1 = a_rowidx->data[aend - 1];
        c->rowidx->data[cnnz] = i1;
        wd[i1 - 1] = a_d->data[aend - 1] * b_d->data[0];
      }
    } else {
      bcidx = a_colidx->data[b_rowidx->data[0]] - 1;
      i = a_colidx->data[b_rowidx->data[0] - 1];
      for (aend = i; aend <= bcidx; aend++) {
        cnnz++;
        numalloc = a_rowidx->data[aend - 1];
        flag[numalloc - 1] = cnnz + 1;
        c->rowidx->data[cnnz] = numalloc;
        wd[numalloc - 1] = a_d->data[aend - 1] * b_d->data[0];
      }

      for (numalloc = 1; numalloc + 1 < b_colidx->data[1]; numalloc++) {
        bd = b_d->data[numalloc];
        bcidx = a_colidx->data[b_rowidx->data[numalloc]] - 1;
        i = a_colidx->data[b_rowidx->data[numalloc] - 1];
        for (aend = i; aend <= bcidx; aend++) {
          i1 = a_rowidx->data[aend - 1];
          if (flag[i1 - 1] < 1) {
            cnnz++;
            flag[i1 - 1] = cnnz + 1;
            c->rowidx->data[cnnz] = i1;
            wd[i1 - 1] = a_d->data[aend - 1] * bd;
            needSort = true;
          } else {
            wd[i1 - 1] += a_d->data[aend - 1] * bd;
          }
        }
      }
    }
  }

  numalloc = c->colidx->data[1] - 1;
  bcidx = c->colidx->data[0];
  if (needSort) {
    c_introsort(c->rowidx, c->colidx->data[0], c->colidx->data[1] - 1);
  }

  for (aend = bcidx; aend <= numalloc; aend++) {
    c->d->data[aend - 1] = wd[c->rowidx->data[aend - 1] - 1];
  }

  c_sparse_fillIn(c);
}

void sparse_mtimes(const emxArray_real_T *a_d, const emxArray_int32_T *a_colidx,
                   const emxArray_int32_T *a_rowidx, const emxArray_real_T *b_d,
                   const emxArray_int32_T *b_colidx, const emxArray_int32_T
                   *b_rowidx, h_sparse *c)
{
  emxArray_int32_T *ccolidx;
  float wd[120];
  float bd;
  int flag[120];
  int bcidx;
  int cmax;
  int cnnz;
  int cstart;
  int exitg1;
  int i;
  int j;
  int numalloc;
  int pa;
  int pb;
  boolean_T needSort;
  emxInit_int32_T(&ccolidx, 1);
  i = ccolidx->size[0];
  ccolidx->size[0] = b_colidx->size[0];
  emxEnsureCapacity_int32_T(ccolidx, i);
  numalloc = b_colidx->size[0];
  for (i = 0; i < numalloc; i++) {
    ccolidx->data[i] = 0;
  }

  memset(&flag[0], 0, 120U * sizeof(int));
  cnnz = 0;
  j = 0;
  do {
    exitg1 = 0;
    if (j < 120) {
      bcidx = b_colidx->data[j];
      cstart = cnnz;
      cmax = cnnz + 120;
      ccolidx->data[j] = cnnz + 1;
      while ((bcidx < b_colidx->data[j + 1]) && (cnnz <= cmax)) {
        numalloc = b_rowidx->data[bcidx - 1];
        pb = a_colidx->data[numalloc] - 1;
        i = a_colidx->data[numalloc - 1];
        for (pa = i; pa <= pb; pa++) {
          numalloc = a_rowidx->data[pa - 1] - 1;
          if (flag[numalloc] != j + 1) {
            flag[numalloc] = j + 1;
            cnnz++;
          }
        }

        bcidx++;
      }

      if (cnnz < cstart) {
        exitg1 = 1;
      } else {
        j++;
      }
    } else {
      ccolidx->data[120] = cnnz + 1;
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  if (cnnz >= 1) {
    numalloc = cnnz;
  } else {
    numalloc = 1;
  }

  i = c->d->size[0];
  c->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(c->d, i);
  for (i = 0; i < numalloc; i++) {
    c->d->data[i] = 0.0;
  }

  i = c->colidx->size[0];
  c->colidx->size[0] = 121;
  emxEnsureCapacity_int32_T(c->colidx, i);
  c->colidx->data[0] = 1;
  i = c->rowidx->size[0];
  c->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(c->rowidx, i);
  for (i = 0; i < numalloc; i++) {
    c->rowidx->data[i] = 0;
  }

  for (numalloc = 0; numalloc < 120; numalloc++) {
    c->colidx->data[numalloc + 1] = 1;
  }

  sparse_fillIn(c);
  i = c->colidx->size[0];
  c->colidx->size[0] = ccolidx->size[0];
  emxEnsureCapacity_int32_T(c->colidx, i);
  numalloc = ccolidx->size[0];
  for (i = 0; i < numalloc; i++) {
    c->colidx->data[i] = ccolidx->data[i];
  }

  memset(&flag[0], 0, 120U * sizeof(int));
  pb = 0;
  cnnz = -1;
  for (j = 0; j < 120; j++) {
    needSort = false;
    cstart = cnnz + 2;
    cmax = b_colidx->data[j + 1];
    numalloc = (cmax - pb) - 1;
    if (numalloc != 0) {
      if (numalloc == 1) {
        bcidx = a_colidx->data[b_rowidx->data[pb]] - 1;
        i = a_colidx->data[b_rowidx->data[pb] - 1];
        for (pa = i; pa <= bcidx; pa++) {
          cnnz++;
          numalloc = a_rowidx->data[pa - 1];
          c->rowidx->data[cnnz] = numalloc;
          wd[numalloc - 1] = a_d->data[pa - 1] * b_d->data[pb];
        }

        pb++;
      } else {
        bcidx = a_colidx->data[b_rowidx->data[pb]] - 1;
        i = a_colidx->data[b_rowidx->data[pb] - 1];
        for (pa = i; pa <= bcidx; pa++) {
          cnnz++;
          numalloc = a_rowidx->data[pa - 1];
          flag[numalloc - 1] = cnnz + 1;
          c->rowidx->data[cnnz] = numalloc;
          wd[numalloc - 1] = a_d->data[pa - 1] * b_d->data[pb];
        }

        for (pb++; pb + 1 < cmax; pb++) {
          bd = b_d->data[pb];
          bcidx = a_colidx->data[b_rowidx->data[pb]] - 1;
          i = a_colidx->data[b_rowidx->data[pb] - 1];
          for (pa = i; pa <= bcidx; pa++) {
            numalloc = a_rowidx->data[pa - 1];
            if (flag[numalloc - 1] < cstart) {
              cnnz++;
              flag[numalloc - 1] = cnnz + 1;
              c->rowidx->data[cnnz] = numalloc;
              wd[numalloc - 1] = a_d->data[pa - 1] * bd;
              needSort = true;
            } else {
              wd[numalloc - 1] += a_d->data[pa - 1] * bd;
            }
          }
        }
      }
    }

    numalloc = ccolidx->data[j + 1] - 1;
    i = ccolidx->data[j];
    if (needSort) {
      c_introsort(c->rowidx, ccolidx->data[j], ccolidx->data[j + 1] - 1);
    }

    for (bcidx = i; bcidx <= numalloc; bcidx++) {
      c->d->data[bcidx - 1] = wd[c->rowidx->data[bcidx - 1] - 1];
    }
  }

  emxFree_int32_T(&ccolidx);
  c_sparse_fillIn(c);
}

/* End of code generation (mtimes.c) */
