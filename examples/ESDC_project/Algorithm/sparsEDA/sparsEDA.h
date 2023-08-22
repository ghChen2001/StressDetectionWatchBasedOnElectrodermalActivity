#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define EDATOL (float)1e-5

/*
 * @brief  spareEDA 
 *
 * @param  signalIn    Input signal (Need to be extended for continous computing)
 *
 */
void sparsEDA(float *signalIn, uint8_t sampleRate, uint16_t len, float epsilon, uint16_t maxIters, float dMin, float thMin, float *SCR, float *SCL, float *MSE);

void lasso(float R[][806], uint16_t Rrowsize, uint16_t Rcolsize, float *signalCutIn, uint16_t len, uint8_t sampleRate, uint16_t maxIters, float epsilon, float *beta);

void toeplitz(float R[][806], uint16_t rowStart, uint16_t rowEnd, uint16_t colStart, uint16_t colEnd, float *rf_est, uint16_t estlen);

void updateChol(float **R_I, uint16_t len, float R[][806], uint16_t Rrowsize, uint16_t Rcolsize, uint8_t explicitA, uint16_t *activeSet, uint16_t aSpointer, uint16_t newIndex, float *outflag);

void downdateChol(float **R_I, uint8_t RIrow, uint8_t RIcol, uint8_t RMcol);

void planerot(float R[2], float G[4]);
