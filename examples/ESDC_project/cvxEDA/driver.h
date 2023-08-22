/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * driver.h
 *
 * Code generation for function 'driver'
 *
 */

#ifndef DRIVER_H
#define DRIVER_H

/* Include files */
#include "cvxEDA_internal_types.h"
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void driver(const emxArray_real_T *H, const emxArray_real_T *f, c_struct_T
              *solution, d_struct_T *memspace, h_struct_T *workingset,
              f_struct_T *qrmanager, g_struct_T *cholmanager, e_struct_T
              *objective, b_struct_T runTimeOptions);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (driver.h) */
