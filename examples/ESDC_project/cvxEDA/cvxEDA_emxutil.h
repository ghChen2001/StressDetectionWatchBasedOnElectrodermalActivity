/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * cvxEDA_emxutil.h
 *
 * Code generation for function 'cvxEDA_emxutil'
 *
 */

#ifndef CVXEDA_EMXUTIL_H
#define CVXEDA_EMXUTIL_H

/* Include files */
#include "cvxEDA_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int
    oldNumel);
  extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);
  extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
  extern void emxFreeMatrix_cell_wrap_3(cell_wrap_3 pMatrix[2]);
  extern void emxFreeStruct_cell_wrap_3(cell_wrap_3 *pStruct);
  extern void emxFreeStruct_sparse(h_sparse *pStruct);
  extern void emxFreeStruct_sparse1(i_sparse *pStruct);
  extern void emxFreeStruct_sparse2(f_sparse *pStruct);
  extern void emxFreeStruct_sparse3(g_sparse *pStruct);
  extern void emxFreeStruct_struct_T(c_struct_T *pStruct);
  extern void emxFreeStruct_struct_T1(e_struct_T *pStruct);
  extern void emxFreeStruct_struct_T2(f_struct_T *pStruct);
  extern void emxFreeStruct_struct_T3(g_struct_T *pStruct);
  extern void emxFreeStruct_struct_T4(h_struct_T *pStruct);
  extern void emxFreeStruct_struct_T5(d_struct_T *pStruct);
  extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
  extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);
  extern void emxFree_real_T(emxArray_real_T **pEmxArray);
  extern void emxInitMatrix_cell_wrap_3(cell_wrap_3 pMatrix[2]);
  extern void emxInitStruct_cell_wrap_3(cell_wrap_3 *pStruct);
  extern void emxInitStruct_sparse(h_sparse *pStruct);
  extern void emxInitStruct_sparse1(i_sparse *pStruct);
  extern void emxInitStruct_sparse2(f_sparse *pStruct);
  extern void emxInitStruct_sparse3(g_sparse *pStruct);
  extern void emxInitStruct_struct_T(c_struct_T *pStruct);
  extern void emxInitStruct_struct_T1(e_struct_T *pStruct);
  extern void emxInitStruct_struct_T2(f_struct_T *pStruct);
  extern void emxInitStruct_struct_T3(g_struct_T *pStruct);
  extern void emxInitStruct_struct_T4(h_struct_T *pStruct);
  extern void emxInitStruct_struct_T5(d_struct_T *pStruct);
  extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int
    numDimensions);
  extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
  extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#ifdef __cplusplus

}
#endif
#endif

/* End of code generation (cvxEDA_emxutil.h) */
