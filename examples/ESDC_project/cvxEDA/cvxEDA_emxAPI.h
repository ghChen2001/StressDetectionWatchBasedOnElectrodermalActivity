/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * cvxEDA_emxAPI.h
 *
 * Code generation for function 'cvxEDA_emxAPI'
 *
 */

#ifndef CVXEDA_EMXAPI_H
#define CVXEDA_EMXAPI_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  extern emxArray_int32_T *emxCreateND_int32_T(int numDimensions, const int
    *size);
  extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);
  extern emxArray_int32_T *emxCreateWrapperND_int32_T(int *data, int
    numDimensions, const int *size);
  extern emxArray_real_T *emxCreateWrapperND_real_T(float *data, int
    numDimensions, const int *size);
  extern emxArray_int32_T *emxCreateWrapper_int32_T(int *data, int rows, int
    cols);
  extern emxArray_real_T *emxCreateWrapper_real_T(float *data, int rows, int
    cols);
  extern emxArray_int32_T *emxCreate_int32_T(int rows, int cols);
  extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
  extern void emxDestroyArray_int32_T(emxArray_int32_T *emxArray);
  extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
  extern void emxDestroy_sparse(f_sparse emxArray);
  extern void emxDestroy_sparse1(g_sparse emxArray);
  extern void emxInit_sparse(f_sparse *pStruct);
  extern void emxInit_sparse1(g_sparse *pStruct);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (cvxEDA_emxAPI.h) */
