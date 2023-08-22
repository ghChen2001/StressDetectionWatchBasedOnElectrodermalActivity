/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * eml_setop.c
 *
 * Code generation for function 'eml_setop'
 *
 */

/* Include files */
#include "eml_setop.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void do_vectors(const int a_data[], const int a_size[1], const int b_data[],
                const int b_size[1], int c_data[], int c_size[1], int ia_data[],
                int ia_size[1], int ib_data[], int ib_size[1])
{
  int ak;
  int b_ialast;
  int b_iblast;
  int bk;
  int iafirst;
  int ialast;
  int ibfirst;
  int iblast;
  int na;
  int nb;
  int nc;
  int ncmax;
  int nia;
  int nib;
  na = a_size[0];
  nb = b_size[0];
  ncmax = a_size[0] + b_size[0];
  c_size[0] = ncmax;
  ia_size[0] = a_size[0];
  ib_size[0] = b_size[0];
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 0;
  ibfirst = 0;
  iblast = 0;
  while ((ialast + 1 <= na) && (iblast + 1 <= nb)) {
    b_ialast = ialast + 1;
    ak = a_data[ialast];
    while ((b_ialast < a_size[0]) && (a_data[b_ialast] == ak)) {
      b_ialast++;
    }

    ialast = b_ialast - 1;
    b_iblast = iblast + 1;
    bk = b_data[iblast];
    while ((b_iblast < b_size[0]) && (b_data[b_iblast] == bk)) {
      b_iblast++;
    }

    iblast = b_iblast - 1;
    if (ak == bk) {
      nc++;
      c_data[nc] = ak;
      nia++;
      ia_data[nia] = iafirst;
      ialast = b_ialast;
      iafirst = b_ialast + 1;
      iblast = b_iblast;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      nc++;
      nia++;
      c_data[nc] = ak;
      ia_data[nia] = iafirst;
      ialast = b_ialast;
      iafirst = b_ialast + 1;
    } else {
      nc++;
      nib++;
      c_data[nc] = bk;
      ib_data[nib - 1] = ibfirst + 1;
      iblast = b_iblast;
      ibfirst = b_iblast;
    }
  }

  while (ialast + 1 <= na) {
    b_ialast = ialast + 1;
    while ((b_ialast < a_size[0]) && (a_data[b_ialast] == a_data[ialast])) {
      b_ialast++;
    }

    nc++;
    nia++;
    c_data[nc] = a_data[ialast];
    ia_data[nia] = iafirst;
    ialast = b_ialast;
    iafirst = b_ialast + 1;
  }

  while (iblast + 1 <= nb) {
    b_iblast = iblast + 1;
    while ((b_iblast < b_size[0]) && (b_data[b_iblast] == b_data[iblast])) {
      b_iblast++;
    }

    nc++;
    nib++;
    c_data[nc] = b_data[iblast];
    ib_data[nib - 1] = ibfirst + 1;
    iblast = b_iblast;
    ibfirst = b_iblast;
  }

  if (a_size[0] > 0) {
    if (1 > nia + 1) {
      ia_size[0] = 0;
    } else {
      ia_size[0] = nia + 1;
    }
  }

  if (b_size[0] > 0) {
    if (1 > nib) {
      ib_size[0] = 0;
    } else {
      ib_size[0] = nib;
    }
  }

  if (ncmax > 0) {
    if (1 > nc + 1) {
      c_size[0] = 0;
    } else {
      c_size[0] = nc + 1;
    }
  }
}

/* End of code generation (eml_setop.c) */
