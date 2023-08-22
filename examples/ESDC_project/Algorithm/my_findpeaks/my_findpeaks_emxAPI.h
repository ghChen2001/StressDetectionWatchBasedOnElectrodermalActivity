/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * my_findpeaks_emxAPI.h
 *
 * Code generation for function 'my_findpeaks_emxAPI'
 *
 */

#ifndef MY_FINDPEAKS_EMXAPI_H
#define MY_FINDPEAKS_EMXAPI_H

/* Include files */
#include "my_findpeaks_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  extern emxArray_real32_T *emxCreateND_real32_T(int numDimensions, const int
    *size);
  extern emxArray_real32_T *emxCreateWrapperND_real32_T(float *data, int
    numDimensions, const int *size);
  extern emxArray_real32_T *emxCreateWrapper_real32_T(float *data, int rows, int
    cols);
  extern emxArray_real32_T *emxCreate_real32_T(int rows, int cols);
  extern void emxDestroyArray_real32_T(emxArray_real32_T *emxArray);
  extern void emxInitArray_real32_T(emxArray_real32_T **pEmxArray, int
    numDimensions);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (my_findpeaks_emxAPI.h) */
