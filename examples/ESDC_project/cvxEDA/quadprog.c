/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * quadprog.c
 *
 * Code generation for function 'quadprog'
 *
 */

/* Include files */
#include "quadprog.h"
#include "computeFval.h"
#include "cvxEDA_emxutil.h"
#include "cvxEDA_internal_types.h"
#include "cvxEDA_types.h"
#include "driver.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void quadprog(const emxArray_real_T *H, const emxArray_real_T *f, const emxArray_real_T *Aineq, const emxArray_real_T *x0, emxArray_real_T *x, float *fval)
{
    static const signed char b_iv[6] = { 1, 0, 0, 120, 0, 0 };

    static const signed char iv4[6] = { 1, 0, 0, 120, 1, 0 };

    static const signed char iv5[6] = { 1, 0, 0, 120, 120, 0 };

    static const signed char iv6[6] = { 1, 0, 0, 120, 121, 0 };

    static const signed char b_iv1[5] = { 0, 0, 120, 1, 0 };

    static const signed char iv2[5] = { 0, 0, 120, 120, 0 };

    static const signed char iv3[5] = { 0, 0, 120, 121, 0 };

    static const signed char obj_tmp[5] = { 0, 0, 120, 0, 0 };

    b_struct_T expl_temp;
    c_struct_T solution;
    d_struct_T memspace;
    e_struct_T QPObjective;
    f_struct_T QRManager;
    g_struct_T CholRegManager;
    h_struct_T WorkingSet;
    float H_infnrm;
    float colSum;
    float f_infnrm;
    float tol;
    int b_i;
    int i;
    int idxFillStart;
    int idx_row;
    int maxDims;
    int nVar;
    signed char i1;
    emxInitStruct_struct_T(&solution);
    nVar = x0->size[0] - 1;
    i = solution.xstar->size[0];
    solution.xstar->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_real_T(solution.xstar, i);
    solution.fstar = 0.0;
    solution.firstorderopt = 0.0;
    memset(&solution.lambda[0], 0, 121U * sizeof(float));
    solution.state = 0;
    solution.maxConstr = 0.0;
    solution.iterations = 0;
    i = solution.searchDir->size[0];
    solution.searchDir->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_real_T(solution.searchDir, i);
    idxFillStart = x0->size[0];
    for (i = 0; i <= idxFillStart; i++) {
        solution.searchDir->data[i] = 0.0f;
    }

    for (b_i = 0; b_i <= nVar; b_i++) {
        solution.xstar->data[b_i] = x0->data[b_i];
    }

    printf("quadprog 1\r\n");

    emxInitStruct_struct_T1(&QPObjective);
    emxInitStruct_struct_T2(&QRManager);
    i = QPObjective.grad->size[0];
    QPObjective.grad->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_real_T(QPObjective.grad, i);
    i = QPObjective.Hx->size[0];
    QPObjective.Hx->size[0] = x0->size[0];
    emxEnsureCapacity_real_T(QPObjective.Hx, i);
    QPObjective.maxVar = x0->size[0] + 1;
    QPObjective.beta = 0.0f;
    QPObjective.rho = 0.0f;
    QPObjective.prev_objtype = 3;
    QPObjective.prev_nvar = 0;
    QPObjective.prev_hasLinear = false;
    QPObjective.gammaScalar = 0.0f;
    QPObjective.nvar = x0->size[0];
    QPObjective.hasLinear = true;
    QPObjective.objtype = 3;
    maxDims = x0->size[0] + 1;
    if (maxDims <= 121) {
        maxDims = 121;
    }

    QRManager.ldq = maxDims;
    i = QRManager.QR->size[0] * QRManager.QR->size[1];
    QRManager.QR->size[0] = maxDims;
    QRManager.QR->size[1] = maxDims;
    emxEnsureCapacity_real_T(QRManager.QR, i);
    i = QRManager.Q->size[0] * QRManager.Q->size[1];
    QRManager.Q->size[0] = maxDims;
    QRManager.Q->size[1] = maxDims;
    emxEnsureCapacity_real_T(QRManager.Q, i);
    idxFillStart = maxDims * maxDims;
    for (i = 0; i < idxFillStart; i++) {
        QRManager.Q->data[i] = 0.0f;
    }

    printf("quadprog 2\r\n");

    i = QRManager.jpvt->size[0];
    QRManager.jpvt->size[0] = maxDims;
    emxEnsureCapacity_int32_T(QRManager.jpvt, i);
    for (i = 0; i < maxDims; i++) {
        QRManager.jpvt->data[i] = 0;
    }

    emxInitStruct_struct_T3(&CholRegManager);
    emxInitStruct_struct_T4(&WorkingSet);
    QRManager.mrows = 0;
    QRManager.ncols = 0;
    i = QRManager.tau->size[0];
    QRManager.tau->size[0] = maxDims;
    emxEnsureCapacity_real_T(QRManager.tau, i);
    QRManager.minRowCol = 0;
    QRManager.usedPivoting = false;
    i = CholRegManager.FMat->size[0] * CholRegManager.FMat->size[1];
    CholRegManager.FMat->size[0] = maxDims;
    CholRegManager.FMat->size[1] = maxDims;
    emxEnsureCapacity_real_T(CholRegManager.FMat, i);
    CholRegManager.ldm = maxDims;
    CholRegManager.ndims = 0;
    CholRegManager.info = 0;
    CholRegManager.ConvexCheck = true;
    i = CholRegManager.workspace_->size[0] * CholRegManager.workspace_->size[1];
    CholRegManager.workspace_->size[0] = maxDims;
    CholRegManager.workspace_->size[1] = 48;
    emxEnsureCapacity_real_T(CholRegManager.workspace_, i);
    i = CholRegManager.workspace2_->size[0] * CholRegManager.workspace2_->size[1];
    CholRegManager.workspace2_->size[0] = maxDims;
    CholRegManager.workspace2_->size[1] = 48;
    emxEnsureCapacity_real_T(CholRegManager.workspace2_, i);
    CholRegManager.scaleFactor = 100.0f;
    idxFillStart = x0->size[0];
    WorkingSet.mConstr = 120;
    WorkingSet.mConstrOrig = 120;
    WorkingSet.mConstrMax = 121;
    WorkingSet.nVar = x0->size[0];
    WorkingSet.nVarOrig = x0->size[0];
    WorkingSet.nVarMax = x0->size[0] + 1;
    WorkingSet.ldA = x0->size[0] + 1;
    i = WorkingSet.Aineq->size[0] * WorkingSet.Aineq->size[1];
    WorkingSet.Aineq->size[0] = x0->size[0] + 1;
    WorkingSet.Aineq->size[1] = 120;
    emxEnsureCapacity_real_T(WorkingSet.Aineq, i);
    i = WorkingSet.lb->size[0];
    WorkingSet.lb->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_real_T(WorkingSet.lb, i);
    i = WorkingSet.ub->size[0];
    WorkingSet.ub->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_real_T(WorkingSet.ub, i);
    i = WorkingSet.indexLB->size[0];
    WorkingSet.indexLB->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_int32_T(WorkingSet.indexLB, i);
    i = WorkingSet.indexUB->size[0];
    WorkingSet.indexUB->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_int32_T(WorkingSet.indexUB, i);
    i = WorkingSet.indexFixed->size[0];
    WorkingSet.indexFixed->size[0] = x0->size[0] + 1;
    emxEnsureCapacity_int32_T(WorkingSet.indexFixed, i);
    WorkingSet.mEqRemoved = 0;
    i = WorkingSet.ATwset->size[0] * WorkingSet.ATwset->size[1];
    WorkingSet.ATwset->size[0] = x0->size[0] + 1;
    WorkingSet.ATwset->size[1] = 121;
    emxEnsureCapacity_real_T(WorkingSet.ATwset, i);
    WorkingSet.nActiveConstr = 0;
    for (b_i = 0; b_i < 5; b_i++) {
        i1 = obj_tmp[b_i];
        WorkingSet.sizes[b_i] = i1;
        WorkingSet.sizesNormal[b_i] = i1;
        WorkingSet.sizesPhaseOne[b_i] = b_iv1[b_i];
        WorkingSet.sizesRegularized[b_i] = iv2[b_i];
        WorkingSet.sizesRegPhaseOne[b_i] = iv3[b_i];
    }

    printf("quadprog 3\r\n");

    for (b_i = 0; b_i < 6; b_i++) {
        WorkingSet.isActiveIdxRegPhaseOne[b_i] = b_iv[b_i];
    }

    for (b_i = 0; b_i < 5; b_i++) {
        WorkingSet.isActiveIdxRegPhaseOne[b_i + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[b_i];
    }

    for (b_i = 0; b_i < 6; b_i++) {
        i = WorkingSet.isActiveIdxRegPhaseOne[b_i];
        WorkingSet.isActiveIdx[b_i] = i;
        WorkingSet.isActiveIdxNormal[b_i] = i;
        WorkingSet.isActiveIdxRegPhaseOne[b_i] = iv4[b_i];
    }

    for (b_i = 0; b_i < 5; b_i++) {
        WorkingSet.isActiveIdxRegPhaseOne[b_i + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[b_i];
    }

    for (b_i = 0; b_i < 6; b_i++) {
        WorkingSet.isActiveIdxPhaseOne[b_i] = WorkingSet.isActiveIdxRegPhaseOne[b_i];
        WorkingSet.isActiveIdxRegPhaseOne[b_i] = iv5[b_i];
    }

    for (b_i = 0; b_i < 5; b_i++) {
        WorkingSet.isActiveIdxRegPhaseOne[b_i + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[b_i];
    }

    for (b_i = 0; b_i < 6; b_i++) {
        WorkingSet.isActiveIdxRegularized[b_i] =
            WorkingSet.isActiveIdxRegPhaseOne[b_i];
        WorkingSet.isActiveIdxRegPhaseOne[b_i] = iv6[b_i];
    }

    for (b_i = 0; b_i < 5; b_i++) {
        WorkingSet.isActiveIdxRegPhaseOne[b_i + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[b_i];
        WorkingSet.nWConstr[b_i] = 0;
    }

    WorkingSet.probType = 3;
    WorkingSet.SLACK0 = 1.0E-5f;
    for (b_i = 0; b_i < 120; b_i++) {
        for (idx_row = 0; idx_row < idxFillStart; idx_row++) {
            WorkingSet.Aineq->data[idx_row + WorkingSet.Aineq->size[0] * b_i] =
                Aineq->data[b_i + 120 * idx_row];
        }

        WorkingSet.bineq[b_i] = 0.0;
    }

    printf("quadprog 4\r\n");
    setProblemType(&WorkingSet, 3);
    idxFillStart = WorkingSet.isActiveIdx[2];
    for (b_i = idxFillStart; b_i < 122; b_i++) {
        WorkingSet.isActiveConstr[b_i - 1] = false;
    }

    emxInitStruct_struct_T5(&memspace);
    WorkingSet.nWConstr[0] = 0;
    WorkingSet.nWConstr[1] = 0;
    WorkingSet.nWConstr[2] = 0;
    WorkingSet.nWConstr[3] = 0;
    WorkingSet.nWConstr[4] = 0;
    WorkingSet.nActiveConstr = 0;
    WorkingSet.SLACK0 = 0.0;
    i = memspace.workspace_double->size[0] * memspace.workspace_double->size[1];
    memspace.workspace_double->size[0] = maxDims;
    if (2 < x0->size[0] + 1) {
        memspace.workspace_double->size[1] = x0->size[0] + 1;
    } else {
        memspace.workspace_double->size[1] = 2;
    }

    printf("quadprog 5\r\n");

    emxEnsureCapacity_real_T(memspace.workspace_double, i);
    i = memspace.workspace_int->size[0];
    memspace.workspace_int->size[0] = maxDims;
    emxEnsureCapacity_int32_T(memspace.workspace_int, i);
    i = memspace.workspace_sort->size[0];
    memspace.workspace_sort->size[0] = maxDims;
    emxEnsureCapacity_int32_T(memspace.workspace_sort, i);
    idxFillStart = WorkingSet.nVarOrig;
    tol = 1.0f;
    for (b_i = 0; b_i < 120; b_i++) {
        colSum = 0.0f;
        for (idx_row = 0; idx_row < idxFillStart; idx_row++) {
            colSum += fabsf(WorkingSet.Aineq->data[idx_row + WorkingSet.Aineq->size[0] *
                                                                b_i]);
        }

        tol = fmaxf(tol, colSum);
    }

    printf("quadprog 6\r\n");
    H_infnrm = 0.0f;
    f_infnrm = 0.0f;
    i = H->size[1];
    for (b_i = 0; b_i < i; b_i++) {
        colSum = 0.0f;
        idxFillStart = H->size[0];
        for (idx_row = 0; idx_row < idxFillStart; idx_row++) {
            colSum += fabsf(H->data[idx_row + H->size[0] * b_i]);
        }

        H_infnrm = fmaxf(H_infnrm, colSum);
        f_infnrm = fmaxf(f_infnrm, fabsf(f->data[b_i]));
    }

    expl_temp.RemainFeasible = false;
    expl_temp.ProbRelTolFactor = fmaxf(fmaxf(tol, f_infnrm), H_infnrm);
    expl_temp.ConstrRelTolFactor = tol;
    expl_temp.MaxIterations = 10 * (x0->size[0] + 120);
    driver(H, f, &solution, &memspace, &WorkingSet, &QRManager, &CholRegManager,
           &QPObjective, expl_temp);
    i = x->size[0];
    x->size[0] = x0->size[0];
    emxEnsureCapacity_real_T(x, i);
    emxFreeStruct_struct_T4(&WorkingSet);
    emxFreeStruct_struct_T3(&CholRegManager);
    emxFreeStruct_struct_T2(&QRManager);
    for (b_i = 0; b_i <= nVar; b_i++) {
        x->data[b_i] = solution.xstar->data[b_i];
    }
    printf("quadprog 7\r\n");

    if (solution.state > 0) {
        *fval = solution.fstar;
    } else {
        *fval = computeFval(&QPObjective, memspace.workspace_double, H, f,
                            solution.xstar);
    }

    emxFreeStruct_struct_T5(&memspace);
    emxFreeStruct_struct_T1(&QPObjective);
    emxFreeStruct_struct_T(&solution);
}

/* End of code generation (quadprog.c) */
