/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * eml_setop.h
 *
 * Code generation for function 'eml_setop'
 *
 */

#ifndef EML_SETOP_H
#define EML_SETOP_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void do_vectors(const int a_data[], const int a_size[1], const int b_data[],
                  const int b_size[1], int c_data[], int c_size[1], int ia_data[],
                  int ia_size[1], int ib_data[], int ib_size[1]);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (eml_setop.h) */
