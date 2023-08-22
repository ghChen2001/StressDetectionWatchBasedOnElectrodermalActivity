/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * cvxEDA_types.h
 *
 * Code generation for function 'cvxEDA'
 *
 */

#ifndef CVXEDA_TYPES_H
#define CVXEDA_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  float *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_int32_T*/

#ifndef typedef_emxArray_int32_T
#define typedef_emxArray_int32_T

typedef struct emxArray_int32_T emxArray_int32_T;

#endif                                 /*typedef_emxArray_int32_T*/

#ifndef typedef_f_sparse
#define typedef_f_sparse

typedef struct {
  emxArray_real_T *d;
  emxArray_int32_T *colidx;
  emxArray_int32_T *rowidx;
  int m;
  int maxnz;
} f_sparse;

#endif                                 /*typedef_f_sparse*/

#ifndef typedef_g_sparse
#define typedef_g_sparse

typedef struct {
  emxArray_real_T *d;
  emxArray_int32_T *colidx;
  emxArray_int32_T *rowidx;
  int maxnz;
} g_sparse;

#endif                                 /*typedef_g_sparse*/

#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_boolean_T*/

#ifndef typedef_emxArray_boolean_T
#define typedef_emxArray_boolean_T

typedef struct emxArray_boolean_T emxArray_boolean_T;

#endif                                 /*typedef_emxArray_boolean_T*/

#ifndef typedef_h_sparse
#define typedef_h_sparse

typedef struct {
  emxArray_real_T *d;
  emxArray_int32_T *colidx;
  emxArray_int32_T *rowidx;
} h_sparse;

#endif                                 /*typedef_h_sparse*/

#ifndef typedef_i_sparse
#define typedef_i_sparse

typedef struct {
  emxArray_real_T *d;
  emxArray_int32_T *colidx;
  emxArray_int32_T *rowidx;
  int m;
  int n;
} i_sparse;

#endif                                 /*typedef_i_sparse*/

#ifndef typedef_cell_wrap_3
#define typedef_cell_wrap_3

typedef struct {
  emxArray_int32_T *f1;
} cell_wrap_3;

#endif                                 /*typedef_cell_wrap_3*/

#ifndef typedef_c_struct_T
#define typedef_c_struct_T

typedef struct {
  emxArray_real_T *xstar;
  float fstar;
  float firstorderopt;
  float lambda[121];
  int state;
  float maxConstr;
  int iterations;
  emxArray_real_T *searchDir;
} c_struct_T;

#endif                                 /*typedef_c_struct_T*/

#ifndef typedef_d_struct_T
#define typedef_d_struct_T

typedef struct {
  emxArray_real_T *workspace_double;
  emxArray_int32_T *workspace_int;
  emxArray_int32_T *workspace_sort;
} d_struct_T;

#endif                                 /*typedef_d_struct_T*/

#ifndef typedef_e_struct_T
#define typedef_e_struct_T

typedef struct {
  emxArray_real_T *grad;
  emxArray_real_T *Hx;
  boolean_T hasLinear;
  int nvar;
  int maxVar;
  float beta;
  float rho;
  int objtype;
  int prev_objtype;
  int prev_nvar;
  boolean_T prev_hasLinear;
  float gammaScalar;
} e_struct_T;

#endif                                 /*typedef_e_struct_T*/

#ifndef typedef_f_struct_T
#define typedef_f_struct_T

typedef struct {
  int ldq;
  emxArray_real_T *QR;
  emxArray_real_T *Q;
  emxArray_int32_T *jpvt;
  int mrows;
  int ncols;
  emxArray_real_T *tau;
  int minRowCol;
  boolean_T usedPivoting;
} f_struct_T;

#endif                                 /*typedef_f_struct_T*/

#ifndef typedef_g_struct_T
#define typedef_g_struct_T

typedef struct {
  emxArray_real_T *FMat;
  int ldm;
  int ndims;
  int info;
  float scaleFactor;
  boolean_T ConvexCheck;
  float regTol_;
  emxArray_real_T *workspace_;
  emxArray_real_T *workspace2_;
} g_struct_T;

#endif                                 /*typedef_g_struct_T*/

#ifndef typedef_h_struct_T
#define typedef_h_struct_T

typedef struct {
  int mConstr;
  int mConstrOrig;
  int mConstrMax;
  int nVar;
  int nVarOrig;
  int nVarMax;
  int ldA;
  emxArray_real_T *Aineq;
  float bineq[120];
  emxArray_real_T *lb;
  emxArray_real_T *ub;
  emxArray_int32_T *indexLB;
  emxArray_int32_T *indexUB;
  emxArray_int32_T *indexFixed;
  int mEqRemoved;
  emxArray_real_T *ATwset;
  float bwset[121];
  int nActiveConstr;
  float maxConstrWorkspace[121];
  int sizes[5];
  int sizesNormal[5];
  int sizesPhaseOne[5];
  int sizesRegularized[5];
  int sizesRegPhaseOne[5];
  int isActiveIdx[6];
  int isActiveIdxNormal[6];
  int isActiveIdxPhaseOne[6];
  int isActiveIdxRegularized[6];
  int isActiveIdxRegPhaseOne[6];
  boolean_T isActiveConstr[121];
  int Wid[121];
  int Wlocalidx[121];
  int nWConstr[5];
  int probType;
  float SLACK0;
} h_struct_T;

#endif                                 /*typedef_h_struct_T*/
#endif

/* End of code generation (cvxEDA_types.h) */
