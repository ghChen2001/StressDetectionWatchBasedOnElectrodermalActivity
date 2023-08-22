#include "sparsEDA.h"
#include <malloc.h>
#include "bflb_core.h"
#include "../CControl/Headers/Functions.h"
// #include "../csparse/csparse.h"

uint16_t betapointer = 0;
float Rzeros[240][806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32))); // N*(6+5Lreg)
float beta[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));        // 6+5Lreg
float SCRline[800] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));     // 5Lreg
float SCRaux[160][5] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));   // Lreg*5
float slcAux[560] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));      // Ns
float driverAux[560] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));   // Ns
float resAux[560] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));      // Ns
float signalCut[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));   // N
float signalCutIn[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32))); // N
float signalEst[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));   // N
float remAout[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));     // N
float SCL1[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));        // N
float driver[160] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));      // Lreg
float SCRaux1[320] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));     // Nss=len
float scr_temp[320] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));    // Nss=len
float SignalEx[560] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));    //ExMumStart * sampleRate + len + ExNumEnd * sampleRate=560
int16_t inactiveSet[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
void sparsEDA(float *signalIn, uint8_t sampleRate, uint16_t len, float epsilon, uint16_t maxIters, float dMin, float thMin, float *SCR, float *SCL, float *MSE)
{
    uint16_t i, j;
    uint8_t ExMumStart = 10;
    uint8_t ExNumEnd = 20;
    // SignalIn should be extended to be processed completely
    // float SignalEx[ExMumStart * sampleRate + len + ExNumEnd * sampleRate];
    // printf("SignalEx Success\r\n");

    for (i = 0; i < ExMumStart * sampleRate; i++) {
        SignalEx[i] = signalIn[0];
    }

    for (; i < ExMumStart * sampleRate + len; i++) {
        SignalEx[i] = signalIn[i - ExMumStart * sampleRate];
    }

    for (; i < ExMumStart * sampleRate + len + ExNumEnd * sampleRate; i++) {
        SignalEx[i] = signalIn[len - 1];
    }

    // Down sampling now do not need (sr=8Hz)

    uint16_t Ns = ExMumStart * sampleRate + len + ExNumEnd * sampleRate; // 560
    uint16_t Nss = len;
    uint16_t pointerS = (ExMumStart * sampleRate) + 1;
    uint16_t pointerE = pointerS + Nss;

    float b0 = 0;

    // Overlap Save
    // Loop Analysis
    uint16_t durationR = 30;         // Slots 30 seconds (60s + 10s for deconvolution)
    uint16_t Lreg = 20 * sampleRate; // 20 seconds
    // uint16_t L = 10 * sampleRate;        // pulse
    uint16_t N = durationR * sampleRate; // 30 seconds
    uint8_t T = 6;                       // CSL additional columns

    // Toeplitz Matrix
    // SCR
    // float Rzeros[N][T + 5 * Lreg];
    // float **Rzeros = (float **)malloc(N * sizeof(float *));
    for (i = 0; i < N; i++) {
        for (j = 0; j < (T + 5 * Lreg); j++) {
            Rzeros[i][j] = 0;
        }
    }
    // cs *R = cs_spalloc(N, T + 5 * Lreg, 100, 1, 1);

    float srF[5] = { 0.5, 0.75, 1, 1.25, 1.5 }; // Multianalysis
    float taus[3] = { 0.5, 2, 1 };
    // float rf_biexp[(uint16_t)(srF[4] * sampleRate * 10)+1];
    // float rf_est[(uint16_t)(srF[4] * sampleRate * 10)+1];
    float *rf_biexp = (float *)malloc(((uint16_t)(srF[4] * sampleRate * 10) + 1) * sizeof(float));
    // printf("rf_biexp Success\r\n");
    float sum_rf_est = 0;
    for (i = 0; i < 5; i++) {
        srF[i] = srF[i] * sampleRate;

        sum_rf_est = 0;
        for (j = 0; j < (uint16_t)(10 * srF[i]) + 1; j++) {
            rf_biexp[j] = expf(-(j / srF[i]) / taus[1]) - expf(-(j / srF[i]) / taus[0]);
            rf_biexp[j] = taus[2] * rf_biexp[j];
            sum_rf_est += rf_biexp[j] * rf_biexp[j];
        }

        // for (j = 0; j < (uint16_t)(10 * srF[i]) + 1; j++) {
        //     rf_biexp[j] = taus[2] * rf_biexp[j];
        // }

        // for (j = 0; j < (uint16_t)(10 * srF[i]) + 1; j++) {
        //     sum_rf_est += rf_biexp[j] * rf_biexp[j];
        // }

        sum_rf_est = sqrtf(sum_rf_est);

        for (j = 0; j < (uint16_t)(10 * srF[i]) + 1; j++) {
            rf_biexp[j] /= sum_rf_est;
        }

        toeplitz(Rzeros, 0, N, i * Lreg + T, (i + 1) * Lreg + T, rf_biexp, (uint16_t)(10 * srF[i]) + 1);
        printf("toeplitz Success\r\n");
    }
    free(rf_biexp);

    for (i = 0; i < Lreg; i++) {
        Rzeros[i][0] = (float)i / (float)(Lreg - 1);
        // printf("Rzeros[%d][0]: %.3f; ", i, Rzeros[i][0]);
    }
    for (i = 0; i < Lreg; i++) {
        Rzeros[i][1] = -Rzeros[i][0];
        // printf("Rzeros[%d][1]: %.3f; ", i, Rzeros[i][1]);
    }
    for (i = Lreg / 3; i < Lreg; i++) {
        Rzeros[i][2] = 2 * ((float)i - (float)Lreg / 3) / (2 * (float)Lreg - 3);
        // printf("Rzeros[%d][2]: %.3f; ", i, Rzeros[i][2]);
    }
    for (i = Lreg / 3; i < Lreg; i++) {
        Rzeros[i][3] = -Rzeros[i][2];
    }
    for (i = 2 * Lreg / 3; i < Lreg; i++) {
        Rzeros[i][4] = ((float)i - 2 * (float)Lreg / 3) / ((float)Lreg - 3);
        // printf("Rzeros[%d][4]: %.3f; ", i, Rzeros[i][4]);
    }
    for (i = 2 * Lreg / 3; i < Lreg; i++) {
        Rzeros[i][5] = -Rzeros[i][4];
    }

    float Cte = 0;
    for (i = 0; i < N; i++) {
        Cte += Rzeros[i][0] * Rzeros[i][0];
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < 6; j++) {
            Rzeros[i][j] = Rzeros[i][j] / sqrtf(Cte);
        }
    }
    // printf("Before Loop\r\n");

    // Loop
    uint16_t cutS = 1;
    uint16_t cutE = N;
    // float slcAux[Ns];
    // float driverAux[Ns];
    // float resAux[Ns];
    for (i = 0; i < Ns; i++) {
        slcAux[i] = 0;
        driverAux[i] = 0;
        resAux[i] = 0;
    }
    uint16_t aux = 0;

    uint8_t jump = 1;

    // float signalCut[N];
    // float signalCutIn[N];
    // float beta[T + 5 * Lreg];
    // float signalEst[N];
    // float remAout[N];
    float res2 = 0;
    float res3 = 0;
    // float SCL1[N];
    // float SCRline[5 * Lreg];
    // float SCRaux[Lreg][5];
    // float driver[Lreg];

    for (i = 0; i < N; i++) {
        signalCut[i] = 0;
        signalCutIn[i] = 0;
        signalEst[i] = 0;
        remAout[i] = 0;
        SCL1[i] = 0;
    }
    for (i = 0; i < 6 + 5 * Lreg; i++) {
        beta[i] = 0;
    }
    for (i = 0; i < 5 * Lreg; i++) {
        SCRline[i] = 0;
    }

    printf("Enter Loop\r\n");

    while (cutE < Ns) {
        aux++;

        for (i = 0; i < cutE - cutS + 1; i++) {
            signalCut[i] = SignalEx[i + cutS - 1];
        }

        if (b0 == 0) {
            b0 = signalCut[0];
        }

        for (i = 0; i < cutE - cutS + 1; i++) {
            signalCutIn[i] = signalCut[i] - b0;
        }

        lasso(Rzeros, N, T + 5 * Lreg, signalCutIn, cutE - cutS + 1, sampleRate, maxIters, epsilon, beta);
        printf("lasso Success\r\n");

        for (i = 0; i < N; i++) {
            signalEst[i] = 0;
            for (j = 0; j < T + 5 * Lreg; j++) {
                signalEst[i] += Rzeros[i][j] * beta[j];
            }
            signalEst[i] += b0;
        }

        for (i = 0; i < cutE - cutS + 1; i++) {
            remAout[i] = powf((signalCut[i] - signalEst[i]), 2);
        }

        res2 = 0;
        for (i = 10 * sampleRate; i < 20 * sampleRate; i++) {
            res2 += remAout[i];
        }

        res3 = 0;
        for (i = 20 * sampleRate; i < 30 * sampleRate; i++) {
            res3 += remAout[i];
        }

        jump = 1;
        if (res2 < 1) {
            jump = 2;
            if (res3 < 1) {
                jump = 3;
            }
        }

        for (i = 0; i < N; i++) {
            SCL1[i] = 0;
            for (j = 0; j < 6; j++) {
                SCL1[i] += Rzeros[i][j] * beta[j];
            }
            SCL1[i] += b0;
        }

        for (i = 0; i < 5 * Lreg; i++) {
            SCRline[i] = beta[T + i];
        }

        for (i = 0; i < 5; i++) {
            for (j = 0; j < Lreg; j++) {
                SCRaux[j][i] = SCRline[i * Lreg + j];
            }
        }

        for (i = 0; i < Lreg; i++) {
            driver[i] = 0;
            for (j = 0; j < 5; j++) {
                driver[i] += SCRaux[i][j];
            }
        }

        for (i = 0; i < 6; i++) {
            b0 += Rzeros[jump * 10 * sampleRate - 1][i] * beta[i];
        }

        for (i = cutS; i < cutS + (jump * 10 * sampleRate); i++) {
            driverAux[i - 1] = driver[i - cutS];
            slcAux[i - 1] = SCL1[i - cutS];
            resAux[i - 1] = remAout[i - cutS];
        }
        cutS += (jump)*10 * sampleRate;
        cutE = cutS + N - 1;
    }
    printf("Loop Success\r\n");

    for (i = 0; i < Nss; i++) {
        SCRaux1[i] = 0;
        scr_temp[i] = 0;
    }

    // float SCRaux1[Nss];
    for (i = pointerS; i < pointerE; i++) {
        SCRaux1[i - pointerS] = driverAux[i];
    }
    // float SCL[pointerE - pointerS];
    for (i = pointerS; i < pointerE; i++) {
        SCL[i - pointerS] = slcAux[i];
    }
    // float MSE[pointerE - pointerS];
    for (i = pointerS; i < pointerE; i++) {
        MSE[i - pointerS] = resAux[i];
    }

    // POST-PROCESSING: REMOVING UNNECESSARY SCR COMPONENTS
    uint16_t ind[Nss];
    uint16_t indpointer = 0;
    // float scr_temp[Nss];
    uint16_t stpointer = 0;
    for (i = 0; i < Nss; i++) {
        if (SCRaux1[i] > 0) {
            ind[indpointer] = i;
            scr_temp[stpointer] = SCRaux1[i];
            indpointer++;
            stpointer++;
        }

        sort(scr_temp, 1, stpointer, 1, 2);
    }
    // float driver1[pointerE - pointerS];
    for (i = 0; i < Nss; i++) {
        SCR[i] = 0;
    }
    for (i = 0; i < indpointer; i++) {
        SCR[ind[i]] = scr_temp[i];
    }
    float scr_max = scr_temp[0];
    float threshold = thMin * scr_max;
    for (i = 0; i < Nss; i++) {
        if (SCR[i] < threshold) {
            SCR[i] = 0;
        }
    }
}

float x[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));        // 6+5Lreg = Rcolsize
float x_old[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));    // 6+5Lreg
float corr[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));     // 6+5Lreg
float ATv[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));      // 6+5Lreg
float gammaArr[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32))); // sizeInactiveSet = 6+5Lreg
float dx[806] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));       // 6+5Lreg
float A_[15 * 15] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float A[15 * 15] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float temp_0[15 * 15] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float temp_dx[15 * 15] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float temp_b[15] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float v[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
void lasso(float R[][806], uint16_t Rrowsize, uint16_t Rcolsize, float *signalCutIn, uint16_t len, uint8_t sampleRate, uint16_t maxIters, float epsilon, float *beta)
{
    // W: Rcolsize
    // N: Len(s)
    // length of beta = Rcolsize
    int16_t i, j;
    int16_t OptTol = -10;
    float solFreq = 0;
    float resStop2 = .0005;
    float lambdaStop = 0;
    float zeroTol = 1e-8;
    uint8_t numIters = 0;

    // float x[Rcolsize];
    for (i = 0; i < Rcolsize; i++) {
        x[i] = 0;
        x_old[i] = 0;
        gammaArr[i] = 0;
    }
    for (i = 0; i < 225; i++) {
        temp_dx[i] = 0;
    }
    for (i = 0; i < 15; i++) {
        temp_b[i] = 0;
    }

    // float x_old[Rcolsize];
    uint8_t iter = 0;

    // float corr[Rcolsize];
    for (i = 0; i < Rcolsize; i++) {
        corr[i] = 0.0;
        for (j = 0; j < Rrowsize; j++) {
            // if (i == 168)
            // {
            //     printf("R %d, %d, %.3f", j, i, R[j][i]);
            // }
            corr[i] += R[j][i] * signalCutIn[j];
        }
    }

    float lambda = -INFINITY;
    uint16_t newIndices[15] = { 0 };
    uint16_t nIpointer = 0;
    for (i = 0; i < Rcolsize; i++) {
        if (lambda < corr[i]) {
            lambda = corr[i];
        }
    }
    printf("lambda1 %.3f\r\n", lambda);

    for (i = 0; i < Rcolsize; i++) {
        // printf("corr %.3f\r\n", corr[i]);
        if (fabsf(lambda - corr[i]) < EDATOL) {
            newIndices[nIpointer] = i;
            nIpointer++;
            if (nIpointer >= 15) {
                break;
            }
        }
    }

    uint16_t collinearIndices[15] = { 0 };
    uint16_t cIpointer = 0;
    // float duals[Rrowsize];
    float res[len];
    float norm_res = 0;
    for (i = 0; i < len; i++) {
        res[i] = signalCutIn[i];
        norm_res += res[i] * res[i];
    }
    norm_res = sqrtf(norm_res);

    // Check stopping conditions
    if (((lambdaStop > 0) & (lambda < lambdaStop)) | ((epsilon > 0) & (norm_res < epsilon))) {
        numIters = 0;
    }

    // Initialize Cholesky factor of A_I
    // float R_I[15][15] = {0}; // To use Ccontrol, let R_I to be 1d array, Nmax = 10
    float **R_I = (float **)malloc(15 * sizeof(float *));
    for (i = 0; i < 15; i++) {
        R_I[i] = (float *)malloc(15 * sizeof(float));
        for (j = 0; j < 15; j++) {
            R_I[i][j] = 0;
        }
    } // Create 2d R_I array
    printf("R_I Success\r\n");

    uint8_t RIcol = 0;
    uint8_t RIrow = 0;
    uint16_t activeSet[15];
    uint16_t aSpointer = 0;
    uint16_t activationHist[15];
    uint16_t aHpointer = 0;
    uint8_t updCol_flag = 0;
    printf("pre updateChol\r\n");

    for (i = 0; i < 1; i++) {
        printf("pre updateChol2\r\n");
        iter++;
        printf("pre updateChol2\r\n");
        updateChol(R_I, len, R, Rrowsize, Rcolsize, 1, activeSet, aSpointer, newIndices[i], &updCol_flag);
        RIcol = 1;
        RIrow = 1;
        // for (int n = 0; n < RIcol; n++) {
        //     for (int m = 0; m < RIrow; m++) {
        //         printf("R_I[%d][%d]%.3f\r\n", n, m, R_I[n][m]);
        //     }
        // }
        activeSet[aSpointer] = newIndices[i];
        aSpointer++;
    }
    aHpointer = 0;
    for (i = 0; i < aSpointer; i++) {
        activationHist[aHpointer] = activeSet[i];
        aHpointer++;
    } // activationHist = activeSet

    // LOOP
    uint8_t done = 0;
    // Compute Lars direction - Equiangular vector
    // float dx[Rcolsize];
    for (i = 0; i < Rcolsize; i++) {
        dx[i] = 0;
    }
    printf("dx done\r\n");

    // Solve the equation (A_I'*A_I)dx_I = sgn(corr_I)
    // float A_[15 * 15] = { 0 };
    // float A[15 * 15] = { 0 };
    // float temp_0[15 * 15] = { 0 };
    // float temp_dx[15 * 15] = { 0 };
    // float temp_b[15] = { 0 };

    // float v[Rrowsize];
    // float R_temp[Rrowsize * aSpointer];
    // printf("R_temp done\r\n");

    // float ATv[Rcolsize];

    // uint32_t SizeRtemp2 = (uint32_t)Rcolsize * (uint32_t)Rrowsize;
    // float R_temp2[SizeRtemp2];

    // float *R_temp2 = (float *)malloc((uint32_t)Rcolsize * (uint32_t)Rrowsize * sizeof(float));
    // printf("R_temp2:%d\r\n", (uint32_t)Rcolsize * (uint32_t)Rrowsize);
    // for (int32_t n = 0; n < Rrowsize * Rcolsize; n++)
    // {
    //     R_temp2[n] = 0;
    // }

    // For  Lasso, Find first active vector to violate sign constraint
    float gammaI = INFINITY;
    uint16_t removelIndices[15] = { 0 };
    uint16_t rIpointer = 0;

    // Find first inactive vector to enter the active set
    // int16_t inactiveSet[Rcolsize];
    uint16_t sizeInactiveSet = Rcolsize;

    float gammalc = 0;
    // float gammaArr[sizeInactiveSet];
    uint16_t Imin = 0;

    float gammaMin = 0;

    uint16_t RMcol;
    printf("Enter lasso loop\r\n");
    uint16_t Miter = 0;

    while (!done) {
        Miter++;
        if (aHpointer == 4) {
            nIpointer = 0;
            lambda = 0;
            for (i = 0; i < Rcolsize; i++) {
                if (lambda < corr[i]) {
                    lambda = corr[i];
                }
            } // Find max corr in lambda

            for (i = 0; i < Rcolsize; i++) {
                if (fabs(lambda - corr[i]) < EDATOL) {
                    newIndices[nIpointer] = i;
                    nIpointer++;
                }
            }              // Find max corr index.

            aSpointer = 0; // Start again
            RIcol = 0;
            RIrow = 0;
            for (i = 0; i < nIpointer; i++) {
                iter++;
                updateChol(R_I, len, R, Rrowsize, Rcolsize, 1, activeSet, aSpointer, newIndices[i], &updCol_flag);

                RIcol++;
                RIrow++;
                // for (int n = 0; n < RIcol; n++) {
                //     for (int m = 0; m < RIrow; m++) {
                //         printf("R_I[%d][%d]%.3f\r\n", n, m, R_I[n][m]);
                //     }
                // }
                activeSet[aSpointer] = newIndices[i];
                aSpointer++;
                if (RIcol >= 15) {
                    break;
                }
            } // Update ActiveSet[]

            for (i = 0; i < aSpointer; i++) {
                activationHist[aHpointer] = activeSet[i];
                aHpointer++;
            } // Update activationHist
        } else {
            lambda = corr[activeSet[0]];
        }
        printf("lambda %.3f\r\n", lambda);

        // // Compute Lars direction - Equiangular vector
        // float dx[Rcolsize];

        // // Solve the equation (A_I'*A_I)dx_I = sgn(corr_I)
        // float A_[15 * 15] = {0};
        // float A[15 * 15] = {0};
        // float temp_0[15 * 15] = {0};
        // float temp_dx[15 * 15] = {0};
        // float temp_b[15] = {0};
        for (i = 0; i < RIrow; i++) {
            for (j = 0; j < RIcol; j++) {
                A_[i * RIcol + j] = R_I[i][j];
                A[i * RIcol + j] = R_I[i][j];
            }
        }
        tran(A_, RIrow, RIcol);                  // A_I'
        mul(A_, A, temp_0, RIrow, RIcol, RIcol); // (A_I'*A_I)
        for (i = 0; i < aSpointer; i++)          // temp_b: sgn(corr_I)
        {
            temp_b[i] = sign(corr[activeSet[i]]);
        }
        // printf("linsolve_gauss start\r\n");
        linsolve_gauss(temp_0, temp_dx, temp_b, aSpointer, aSpointer, 0); // Solve dx_I
        printf("linsolve_gauss done\r\n");
        for (i = 0; i < aSpointer; i++) {
            dx[activeSet[i]] = temp_dx[i];
        }

        // float v[Rrowsize];
        // float R_temp[Rrowsize * (aSpointer + 1)];
        // for (i = 0; i < Rrowsize; i++)
        // {
        //     for (j = 0; j < aSpointer; j++)
        //     {
        //         R_temp[i * aSpointer + j] = R[i][activeSet[j]];
        //     }
        // }
        // mul(R_temp, temp_dx, v, Rrowsize, aSpointer, 1); // Solve 1d array v
        for (i = 0; i < Rrowsize; i++) {
            v[i] = 0.0;
            for (j = 0; j < aSpointer; j++) {
                v[i] += R[i][activeSet[j]] * temp_dx[j];
            }
        }

        // // float ATv[Rcolsize];
        // // float R_temp2[Rcolsize * Rrowsize];
        // for (i = 0; i < Rrowsize; i++)
        // {
        //     for (j = 0; j < Rcolsize; j++)
        //     {
        //         // printf("R_temp2 %d\r\n", (uint32_t)i * (uint32_t)Rcolsize + (uint32_t)j);
        //         R_temp2[(uint32_t)i * (uint32_t)Rcolsize + (uint32_t)j] = R[i][j];
        //     }
        // }
        // tran(R_temp2, Rrowsize, Rcolsize);
        // mul(R_temp2, v, ATv, Rcolsize, Rrowsize, 1); // Solve 1d array ATv
        for (i = 0; i < Rcolsize; i++) {
            ATv[i] = 0.0;
            for (j = 0; j < Rrowsize; j++) {
                ATv[i] += R[j][i] * v[j];
            }
        } // Solve 1d array ATv

        // // For  Lasso, Find first active vector to violate sign constraint
        // float gammaI = infinityf();
        gammaI = INFINITY;
        // uint16_t removelIndices[15] = {0};
        // uint16_t rIpointer = 0;

        // // Find first inactive vector to enter the active set
        // uint16_t inactiveSet[Rcolsize];
        // uint16_t sizeInactiveSet = Rcolsize;
        sizeInactiveSet = Rcolsize;
        for (i = 0; i < Rcolsize; i++) {
            inactiveSet[i] = i;
        }
        for (i = 0; i < aSpointer; i++) {
            inactiveSet[activeSet[i]] = -1;
            sizeInactiveSet--;
        }
        for (i = 0; i < cIpointer; i++) {
            inactiveSet[collinearIndices[i]] = -1;
            sizeInactiveSet--;
        }

        // float gammalc = 0;
        // float gammaArr[sizeInactiveSet];
        // uint16_t Imin = 0;
        if (sizeInactiveSet == 0) {
            gammalc = 1;
            nIpointer = 0;
        } else {
            epsilon = 1e-12;
            gammalc = INFINITY;
            j = 0;
            for (i = 0; i < Rcolsize; i++) {
                if (inactiveSet[i] != -1) {
                    // printf("inactiveSet %d\r\n", inactiveSet[i]); // sizeof gammaArr equals to sizeInactiveSet
                    gammaArr[i] = (lambda - corr[inactiveSet[i]]) / (1 - ATv[inactiveSet[i]] + epsilon);
                    if (gammaArr[i] < 1e-5) {
                        gammaArr[i] = INFINITY;
                    }

                    if (gammalc >= gammaArr[i]) {
                        gammalc = gammaArr[i];
                        Imin = i;
                    }
                }
            }
            nIpointer = 0; // Reset NewIndices
            for (i = 0; i < Rcolsize; i++) {
                if (inactiveSet[i] != -1) {
                    if (fabsf(gammaArr[i] - gammalc) < EDATOL) {
                        newIndices[nIpointer] = inactiveSet[i];
                        nIpointer++;
                        if (nIpointer >= 15) {
                            nIpointer = 14;
                            break;
                        }
                    }
                }
            } // Reset NewIndices
        }

        // float gammaMin = 0;
        gammaMin = 0;
        if (gammalc > gammaI) {
            gammaMin = gammaI;
        } else {
            gammaMin = gammalc;
        }
        printf("gammaMin %.3f\r\n", gammaMin);

        // Compute the next Lars step
        for (i = 0; i < Rcolsize; i++) {
            x[i] += gammaMin * dx[i];
        }

        for (i = 0; i < len; i++) {
            res[i] -= gammaMin * v[i]; // Notice: len=Rrowsize
        }

        for (i = 0; i < Rcolsize; i++) {
            corr[i] -= gammaMin * ATv[i];
        }

        // Check stopping condition
        if (((lambda - gammaMin) < OptTol) || ((lambdaStop > 0) && (lambda <= lambdaStop)) || ((epsilon > 0) & (norm(res, len, 1, 2) <= epsilon))) {
            nIpointer = 0;
            rIpointer = 0;
            done = 1;
        }

        if (norm(res, sampleRate * 10, 1, 2) <= resStop2) {
            done = 1;
        }

        // Add new indices to active set
        if ((gammalc <= gammaI) && (nIpointer != 0) && (RIcol != 15)) {
            for (i = 0; i < nIpointer; i++) {
                iter++;
                updateChol(R_I, len, R, Rrowsize, Rcolsize, 1, activeSet, aSpointer, newIndices[i], &updCol_flag);
                RIcol++;
                RIrow++;
                // for (int n = 0; n < RIcol; n++) {
                //     for (int m = 0; m < RIrow; m++) {
                //         printf("R_I[%d][%d]%.3f\r\n", n, m, R_I[n][m]);
                //     }
                // }
                if (updCol_flag) {
                    collinearIndices[cIpointer] = newIndices[i];
                    cIpointer++;
                } else {
                    activeSet[aSpointer] = newIndices[i];
                    aSpointer++;
                    activationHist[aHpointer] = newIndices[i];
                    aHpointer++;
                }
                if (RIcol >= 15) {
                    break;
                }
            }
        }

        // uint16_t RMcol;

        // Remove violating indices from active set
        if (gammaI <= gammalc) {
            for (i = 0; i < rIpointer; i++) {
                iter++;
                for (j = 0; j < aSpointer; j++) {
                    if (activeSet[j] == removelIndices[i]) {
                        RMcol = j;
                    }
                }

                // Downdate the Cholesky factorization of A_I
                RIcol--;
                RIrow--;
                downdateChol(R_I, RIrow, RIcol, RMcol);
                for (i = RMcol; i < aSpointer - 1; i++) {
                    activeSet[i] = activeSet[i + 1];
                }
                aSpointer -= 1;

                // Reset collinear set
                cIpointer = 0;
            }

            // To avoid numerical errors
            for (i = 0; i < rIpointer; i++) {
                x[removelIndices[i]] = 0;
            }

            for (i = 0; i < aHpointer; i++) {
                for (j = 0; j < rIpointer; j++) {
                    if (activationHist[i] == removelIndices[j]) {
                        for (int16_t n = i; n < aHpointer - 1; n++) {
                            activationHist[n] = activationHist[n + 1];
                        }
                        aHpointer--;
                    }
                }
            }
        }

        if (iter >= maxIters) {
            done = 1;
        }

        for (i = 0; i < Rcolsize; i++) {
            if (x[i] < 0) {
                for (j = 0; j < Rcolsize; j++) {
                    x[j] = x_old[j];
                }
                done = 1;
                break;
            }
        }
        if (i >= Rcolsize) {
            for (j = 0; j < Rcolsize; j++) {
                x_old[j] = x[j];
            }
        }

        if (done || ((solFreq > 0) && !modf(iter, &solFreq))) {
            for (i = 0; i < Rcolsize; i++) {
                beta[i] = x[i];
            }
        }
        if (Miter >= 50) {
            break;
        }
    }
}

float newVec[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
// float uC_R_temp2[240] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
void updateChol(float **R_I, uint16_t len, float R[][806], uint16_t Rrowsize, uint16_t Rcolsize, uint8_t explicitA, uint16_t *activeSet, uint16_t aSpointer, uint16_t newIndex, float *outflag)
{
    printf("updateChol Enter\r\n");
    int16_t uCflag = 0;
    int16_t i, j;
    // float newVec[Rrowsize];
    for (i = 0; i < Rrowsize; i++) {
        newVec[i] = R[i][newIndex];
    }
    float p[aSpointer];

    if (aSpointer == 0) {
        R_I[0][0] = 0;
        for (i = 0; i < Rrowsize; i++) {
            R_I[0][0] += newVec[i] * newVec[i];
        }
        R_I[0][0] = sqrtf(R_I[0][0]);
    } else {
        if (explicitA) {
            // float uC_R_temp1[Rrowsize * aSpointer];
            // // float uC_R_temp2[Rrowsize];
            float uC_R_temp3[aSpointer]; // Answer
            // for (i = 0; i < Rrowsize; i++) {
            //     for (j = 0; j < aSpointer; j++) {
            //         uC_R_temp1[i * aSpointer + j] = R[i][activeSet[j]];
            //     }

            //     uC_R_temp2[i] = R[i][newIndex];
            // }
            // tran(uC_R_temp1, Rrowsize, aSpointer);                     // R_temp1 = R(:,activeSet)'
            // mul(uC_R_temp1, uC_R_temp2, uC_R_temp3, aSpointer, Rrowsize, 1); // R(:,activeSet)'*R(:,newIndex)

            for (i = 0; i < aSpointer; i++) {
                uC_R_temp3[i] = 0;
                for (j = 0; j < Rrowsize; j++) {
                    uC_R_temp3[i] += R[j][activeSet[i]] * R[j][newIndex];
                }
            }

            float tempRI[aSpointer * aSpointer]; // Set R_I to be a 1d array so we can use Ccontrol lib
            for (i = 0; i < aSpointer; i++) {
                for (j = 0; j < aSpointer; j++) {
                    tempRI[i * aSpointer + j] = R_I[i][j];
                }
            }
            tran(tempRI, aSpointer, aSpointer); // tempRI : R_I'

            linsolve_gauss(tempRI, p, uC_R_temp3, aSpointer, aSpointer, 0);
        } else {
            // Must be explicitA in this example
        }

        float q = 0;
        for (i = 0; i < Rrowsize; i++) {
            q += newVec[i] * newVec[i];
        }
        for (i = 0; i < aSpointer; i++) {
            q -= p[i] * p[i];
        }
        if (q < EDATOL) {
            *outflag = 1; // Collinear vector
        } else {
            for (i = 0; i < aSpointer; i++) {
                R_I[i][aSpointer] = p[i];
            }
            for (i = 0; i < aSpointer; i++) {
                R_I[aSpointer][i] = 0;
            }
            R_I[aSpointer][aSpointer] = sqrtf(q);
        }
    }
}

void downdateChol(float **R_I, uint8_t RIrow, uint8_t RIcol, uint8_t RMcol)
{
    printf("downdateChol\r\n");
    int16_t i, j;
    for (i = 0; i < RIrow; i++) {
        R_I[i][RMcol] = 0;
    }

    float R[2] = { 0 };
    float G[4] = { 0 };
    for (i = RMcol; i < RIcol; i++) {
        R[0] = R_I[RMcol][RMcol];
        R[1] = R_I[RMcol + 1][RMcol];
        planerot(R, G);
        R_I[RMcol][RMcol] = R[0];
        R_I[RMcol + 1][RMcol] = R[1];
        if (i < RIcol) {
            for (j = i + 1; j < RIcol; j++) {
                float temp = G[0] * R_I[i][j] + G[1] * R_I[i + 1][j];
                R_I[i + 1][j] = G[2] * R_I[i][j] + G[3] * R_I[i + 1][j];
                R_I[i][j] = temp;
            }
        }
    }
    for (i = 0; i < RIcol; i++) {
        R_I[i][RIrow] = 0;
    }
    // Don't care.
    return;
}

void toeplitz(float R[][806], uint16_t rowStart, uint16_t rowEnd, uint16_t colStart, uint16_t colEnd, float *rf_est, uint16_t estlen)
{
    uint16_t i, j;
    for (i = colStart; i < colEnd; i++) {
        R[rowStart][i] = 0;
    }
    j = 0;
    for (i = rowStart; i < rowEnd; i++) {
        if (j < estlen) {
            R[i][colStart] = rf_est[j];
            j++;
        } else {
            R[i][colStart] = 0;
        }
    }
    for (i = rowStart + 1; i < rowEnd; i++) {
        for (j = colStart + 1; j < colEnd; j++) {
            R[i][j] = R[i - 1][j - 1];
        }
    }
}

void planerot(float R[2], float G[4])
{
    float absxk;
    float r;
    float scale;
    float t;

    if (R[1] != 0.0) {
        scale = 1e-12;
        absxk = fabsf(R[0]);
        if (absxk > 1e-12) {
            r = 1.0;
            scale = absxk;
        } else {
            t = absxk / 1e-12;
            r = t * t;
        }

        absxk = fabsf(R[1]);
        if (absxk > scale) {
            t = scale / absxk;
            r = r * t * t + 1.0;
            scale = absxk;
        } else {
            t = absxk / scale;
            r += t * t;
        }

        r = scale * sqrtf(r);
        scale = R[0] / r;
        G[0] = scale;
        G[2] = R[1] / r;
        G[1] = -R[1] / r;
        G[3] = scale;
        R[0] = r;
        R[1] = 0.0;
    } else {
        G[1] = 0.0;
        G[2] = 0.0;
        G[0] = 1.0;
        G[3] = 1.0;
    }
}
