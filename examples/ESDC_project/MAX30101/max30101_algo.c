/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "max30101_drv.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//  Average DC Estimator
uint16_t avg_dc_est(int32_t *p, uint16_t x)
{
    *p += ((((int32_t)x << 15) - *p) >> 4);
    return (*p >> 15);
}

//
//  Heart Rate/SpO2 Monitor function takes sample input 'dinIR' and dinRed.
//  Other inputs:
//      ns -> Sample Counter, increments with each sample input.
//      SampRate -> Input data real-time sample rate.
//      dinLShft -> Number of left shifts for data to be 16 bit wide.
//      compSpO2 -> If '1' compute SpO2 value,else compute HR only.

//
//  Outputs:
//      ir_ac_comp  -> AC component of the IR signal.
//      red_ac_comp -> AC component of the Red signal.
//      ir_ac_mag   -> Peak to Peak magnitude of the IR signal.
//      red_ac_mag  -> Peak to Peak magnitude of the Red signal.
//      HRbpm       -> Heart Rate in beats per minute.
//      SpO2        -> SpO2 value as %saturation.
//      DRdy        -> '1' when new data is available.
//

//Heart rate and SpO2 algorithm

void HRSpO2Func(uint32_t dinIR, uint32_t dinRed, uint32_t dinGreen, uint32_t ns, uint16_t SampRate, uint16_t compSpO2,
                int16_t *ir_ac_comp, int16_t *red_ac_comp, int16_t *green_ac_comp, int16_t *ir_ac_mag, int16_t *red_ac_mag,
                int16_t *green_ac_mag, uint16_t *HRbpm2, uint16_t *SpO2B, uint16_t *DRdy)
{
    static int32_t ir_avg_reg = 0;
    static int32_t red_avg_reg = 0;
    static int32_t green_avg_reg = 0;

    static int16_t ir_ac_sig_cur = 0;
    static int16_t ir_ac_sig_pre;
    static int16_t ir_ac_sig_min = 0;
    static int16_t ir_ac_sig_max = 0;
    static int16_t ir_avg_est;

    static int16_t ir_pedge = 0, ir_nedge = 0;
    static int16_t ir_pzxic, ir_pzxip;
    static int16_t ir_nzxic;

    static int16_t red_ac_sig_cur = 0;
    static int16_t red_ac_sig_min = 0;
    static int16_t red_ac_sig_max = 0;
    static int16_t red_avg_est;

    static int16_t green_avg_est;
    static int16_t green_ac_sig_cur = 0;
    
    static int16_t green_ac_sig_pre;
    static int16_t green_ac_sig_max;
    static int16_t green_ac_sig_min;
    static int16_t green_mac_FIFO[5];
    int16_t meanGreenMagFIFO;
    int16_t minAmpForHeartBeat;

    uint32_t IRData, RedData, greenData, rnum, rden, rdens;
    uint16_t zeros_in_HrQue = 0, posCount = 0;
    static uint32_t prevPeakLoc = 0;
    static int16_t IrFIFO[100];
    static int16_t HrQue[10], lastKnownGoodHr[10];
    static int16_t SPO2Que[5];
    int16_t SPO2score[5];
    static uint16_t HrQindex = 0, lengthOfposCountExceeding = 0;
    static uint16_t initHrQueCounter = 0, fingerOff = 0;

    static int16_t HrQueSmoothing[3];
    static int16_t SPO2QueSmoothing[3];

    int16_t k, j;
    uint32_t peakLoc;
    int16_t bufferIdx1, bufferIdx2;
    int16_t maxFIFO, IdxMaxFIFO;
    int16_t HRperiod2, HRComp2, deltaHR;
    int16_t cSpO2, SpO2;

    int16_t HrCount = 0, HrSum = 0, meanGreenMagFIFOcounter = 0;
    int16_t SPO2D, meanHrQ;
    int16_t dx[99], cumsumX[99];
    static int16_t SPO2QueCounter = 0; //, lastDisplayedHrValue;

    int16_t validSPO2Count = 0;
    int16_t validSPO2Sum = 0;
    int16_t SPO2scoreAverage = 0;
    int16_t SPO2scoreSum = 0;
    //  int16_t  deltaMeanLastKnownGoodHr=0,meanLastKnownGoodHr =0 ;
    //  int16_t     counterMeanLastKnownGoodHr =0 ;

    //  clear some vars if fresh new start
    if ((ns == 0) || (fingerOff > 300)) {
        ir_avg_reg = 0;
        red_avg_reg = 0;
        green_avg_reg = 0;

        ir_ac_sig_cur = 0;
        ir_ac_sig_pre = 0;
        ir_ac_sig_min = 0;
        ir_ac_sig_max = 0;

        ir_avg_est = 0;
        green_avg_est = 0;
        red_avg_est = 0;

        ir_pedge = 0;
        ir_nedge = 0;
        ir_pzxic = 0;
        ir_pzxip = 0;
        ir_nzxic = 0;
        //ir_nzxip =0;
        red_ac_sig_cur = 0;
        red_ac_sig_min = 0;
        red_ac_sig_max = 0;

        prevPeakLoc = 0;
        bufferIdx1 = 0;
        bufferIdx2 = 0;
        HrQindex = 0;
        initHrQueCounter = 0;
        lengthOfposCountExceeding = 0;
        fingerOff = 0;
        HRComp2 = 0;

        for (k = 0; k < 100; k++) {
            IrFIFO[k] = 0;
        }
        for (k = 0; k < 10; k++) {
            HrQue[k] = 0;
            lastKnownGoodHr[k] = 0;
        }
        for (k = 0; k < 3; k++) {
            HrQueSmoothing[k] = 70;
            SPO2QueSmoothing[k] = 97;
        }
        for (k = 0; k < 5; k++) {
            SPO2Que[k] = 97;
            SPO2score[k] = 0;
            green_mac_FIFO[k] = 0;
        }
        SPO2QueCounter = 0;
        *SpO2B = 97;
        *HRbpm2 = 0;
        *DRdy = 0;
    }

    //  Save current state
    green_ac_sig_pre = green_ac_sig_cur;
    //
    //  Process next data sample
    minAmpForHeartBeat = 0;
    IRData = dinIR;
    RedData = dinRed;
    greenData = dinGreen;

    ir_avg_est = avg_dc_est(&ir_avg_reg, IRData);
    red_avg_est = avg_dc_est(&red_avg_reg, RedData);
    green_avg_est = avg_dc_est(&green_avg_reg, greenData);

    lp_dfir_flt((uint16_t)(IRData - ir_avg_est), (uint16_t)(RedData - red_avg_est),
                (uint16_t)(greenData - green_avg_est), &ir_ac_sig_cur, &red_ac_sig_cur, &green_ac_sig_cur);

    *ir_ac_comp = ir_ac_sig_cur;
    *red_ac_comp = red_ac_sig_cur;
    *green_ac_comp = green_ac_sig_cur;

    //save to FIFO
    for (k = 1; k < 100; k++) {
        IrFIFO[100 - k] = IrFIFO[99 - k];
    }
    IrFIFO[0] = green_ac_sig_cur; // invert
    for (k = 0; k < 97; k++)
        dx[k] = IrFIFO[k + 2] - IrFIFO[k];
    dx[97] = dx[96];
    dx[98] = dx[96];

    for (k = 0; k < 99; k++) {
        if (dx[k] > 0)
            dx[k] = 1;
        else
            dx[k] = 0;
    }

    cumsumX[0] = 0;
    for (k = 1; k < 99; k++) {
        if (dx[k] > 0)
            cumsumX[k] = cumsumX[k - 1] + dx[k];
        else
            cumsumX[k] = 0;
    }
    // determine noise
    // ignore less than 3 conseuctive non-zeros's
    // detect # of sign change
    posCount = 0;
    for (k = 1; k < 99; k++) {
        if (cumsumX[k] > 0) {
            posCount++;
        } else if (cumsumX[k] == 0) {
            if (posCount < 4 && k >= 4) {
                for (j = k - 1; j > k - posCount - 1; j--)
                    cumsumX[j] = 0;
            }
            posCount = 0;
        }
    }
    // ignore less than 3 conseuctive zeros's

    posCount = 0;
    for (k = 1; k < 99; k++) {
        if (cumsumX[k] == 0) {
            posCount++;
        } else if (cumsumX[k] > 0) {
            if (posCount < 4 && k >= 4) {
                for (j = k - 1; j > k - posCount - 1; j--)
                    cumsumX[j] = 100;
            }
            posCount = 0;
        }
    }

    //// detect # of sign change
    posCount = 0; // sign change counter
    for (k = 0; k < 98; k++) {
        if (cumsumX[k] == 0 && cumsumX[k + 1] > 0) {
            posCount++;
        }
    }
    if (posCount >= 4) {
        lengthOfposCountExceeding++;
        // printf("PosCount =%i \n", posCount );
    } else
        lengthOfposCountExceeding = 0;
    //  Detect IR channel positive zero crossing (rising edge)
    if ((green_ac_sig_pre < 0) && (green_ac_sig_cur >= 0) && fingerOff == 0) {
        *ir_ac_mag = ir_ac_sig_max - ir_ac_sig_min;
        *red_ac_mag = red_ac_sig_max - red_ac_sig_min;
        *green_ac_mag = green_ac_sig_max - green_ac_sig_min;
        if (*green_ac_mag > 0) {
            for (k = 0; k < 4; k++)
                green_mac_FIFO[k] = green_mac_FIFO[k + 1];
            green_mac_FIFO[4] = *green_ac_mag;
            if (green_mac_FIFO[4] > 1000)
                green_mac_FIFO[4] = 1000;
        }
        meanGreenMagFIFO = 0;
        meanGreenMagFIFOcounter = 0;
        for (k = 0; k < 5; k++) {
            if (green_mac_FIFO[k] > 0) {
                meanGreenMagFIFO = meanGreenMagFIFO + green_mac_FIFO[k];
                meanGreenMagFIFOcounter++;
            }
        }
        if (meanGreenMagFIFOcounter >= 2) {
            meanGreenMagFIFO = meanGreenMagFIFO / meanGreenMagFIFOcounter;
            minAmpForHeartBeat = meanGreenMagFIFO / 4; //25% of mean of past heart beat
        } else
            minAmpForHeartBeat = 75;
        if (minAmpForHeartBeat < 75)
            minAmpForHeartBeat = 75;
        if (minAmpForHeartBeat > 400)
            minAmpForHeartBeat = 400;

        ir_pedge = 1;
        ir_nedge = 0;
        ir_ac_sig_max = 0;
        ir_pzxip = ir_pzxic;
        ir_pzxic = ns;
        bufferIdx1 = ir_pzxic - ir_nzxic;
        bufferIdx2 = ir_pzxic - ir_pzxip;

        if ((*green_ac_mag) > minAmpForHeartBeat && (*green_ac_mag) < 20000 && bufferIdx1 >= 0 && bufferIdx1 < 100 && bufferIdx2 >= 0 && bufferIdx2 < 100 && bufferIdx1 < bufferIdx2) { // was <5000
            maxFIFO = -32766;

            IdxMaxFIFO = 0;
            for (j = bufferIdx1; j <= bufferIdx2; j++) { // find max peak
                if (IrFIFO[j] > maxFIFO) {
                    maxFIFO = IrFIFO[j];
                    IdxMaxFIFO = j;
                }
            }
            peakLoc = ir_pzxic - IdxMaxFIFO + 1;

            if (prevPeakLoc != 0) {
                HRperiod2 = (uint16_t)(peakLoc - prevPeakLoc);
                if (HRperiod2 > 33 && HRperiod2 < 134) {
                    HRComp2 = (6000 / HRperiod2);
                    fingerOff = 0;
                } else
                    HRComp2 = 0;
            } else
                HRComp2 = 0;

            if (initHrQueCounter < 10 && HRComp2 > 0) {
                HrQue[HrQindex] = HRComp2;
                HrQindex++;
                initHrQueCounter++;
                if (HrQindex == 10)
                    HrQindex = 0;
            }

            if (initHrQueCounter > 7 && lengthOfposCountExceeding <= 3) {
                if (HRComp2 > 0) {
                    HrCount = 0;
                    HrSum = 0;
                    zeros_in_HrQue = 0;
                    for (k = 1; k < initHrQueCounter; k++) {
                        if (HrQue[k] > 0) {
                            HrSum += HrQue[k];
                            HrCount++;
                        } else
                            zeros_in_HrQue++;
                    }
                    meanHrQ = HrSum / HrCount;
                    deltaHR = lastKnownGoodHr[0] / 10;

                    if (HRComp2 > lastKnownGoodHr[0] - deltaHR && HRComp2 < lastKnownGoodHr[0] + deltaHR) {
                        for (k = 1; k < 10; k++) {
                            HrQue[10 - k] = HrQue[9 - k];
                        }
                        HrQue[0] = HRComp2;
                    } // HR smmothing using FIFO queue -

                    if (zeros_in_HrQue <= 2) {
                        for (k = 1; k < 3; k++) {
                            HrQueSmoothing[3 - k] = HrQueSmoothing[2 - k];
                        }
                        HrQueSmoothing[0] = meanHrQ;
                        HRComp2 = ((HrQueSmoothing[0] << 2) + (HrQueSmoothing[1] << 1) + (HrQueSmoothing[2] << 1)) >> 3;
                        *HRbpm2 = HRComp2;

                        for (k = 1; k < 10; k++) {
                            lastKnownGoodHr[10 - k] = lastKnownGoodHr[9 - k];
                        }
                        lastKnownGoodHr[0] = HRComp2;
                    }
                }

            }
            ///// if (initHrQueCounter > 7  && lengthOfposCountExceeding<5)
            else if (initHrQueCounter < 7) { // before que is filled up, display whatever it got.
                *HRbpm2 = HRComp2;

            } else {
                //  *HRbpm2 =  0 ;
                HrCount = 0;
                HrSum = 0;
                for (k = 0; k < 10; k++) {
                    if (lastKnownGoodHr[k] > 0) {
                        HrSum = HrSum + lastKnownGoodHr[k];
                        HrCount++;
                    }
                }
                if (HrCount > 0)
                    *HRbpm2 = HrSum / HrCount;
                else
                    *HRbpm2 = 0;
            }
            prevPeakLoc = peakLoc; // save peakLoc into Static var

            if (compSpO2) {
                rnum = (ir_avg_reg >> 20) * (*red_ac_mag);
                rden = (red_avg_reg >> 20) * (*ir_ac_mag);
                rdens = (rden >> 15);
                if (rdens > 0)
                    cSpO2 = 110 - (((25 * rnum) / (rdens)) >> 15);

                if (cSpO2 >= 100)
                    SpO2 = 100;
                else if (cSpO2 <= 70)
                    SpO2 = 70;
                else
                    SpO2 = cSpO2;

                SPO2Que[SPO2QueCounter] = SpO2;

                for (k = 0; k < 5; k++) {
                    SPO2score[k] = 0;
                    for (j = 0; j < 5; j++)
                        if (abs(SPO2Que[k] - SPO2Que[j]) > 5)
                            SPO2score[k]++;
                }

                SPO2scoreSum = 0;
                for (k = 0; k < 5; k++)
                    SPO2scoreSum += SPO2score[k];
                SPO2scoreAverage = SPO2scoreSum / 5;
                for (k = 1; k < 5; k++)
                    SPO2score[k] = SPO2score[k] - SPO2scoreAverage;

                validSPO2Count = 0;
                validSPO2Sum = 0;
                for (k = 1; k < 5; k++) {
                    if (SPO2score[k] <= 0) { // add for HR to report
                        validSPO2Sum += SPO2Que[k];
                        validSPO2Count++;
                    }
                }
                if (validSPO2Count > 0)
                    SPO2D = (validSPO2Sum / validSPO2Count) - 1;
                if (SPO2D > 100)
                    SPO2D = 100;

                SPO2QueCounter++;
                if (SPO2QueCounter == 5)
                    SPO2QueCounter = 0;

                for (k = 1; k < 3; k++) {
                    SPO2QueSmoothing[3 - k] = SPO2QueSmoothing[2 - k];
                }
                SPO2QueSmoothing[0] = SPO2D;
                *SpO2B = ((SPO2QueSmoothing[0] << 2) + (SPO2QueSmoothing[1] << 1) + (SPO2QueSmoothing[2] << 1)) >> 3;

                if (*SpO2B > 100)
                    *SpO2B = 100;

            } else {
                SpO2 = 0;
                *SpO2B = 0;
            }
            *DRdy = 1;
        }
    }
    //  Detect IR channel negative zero crossing (falling edge)
    if ((green_ac_sig_pre > 0) && (green_ac_sig_cur <= 0)) {
        ir_pedge = 0;
        ir_nedge = 1;
        ir_ac_sig_min = 0;
        ir_nzxic = ns;
    }
    //  Find Maximum IR & Red values in positive cycle
    if (ir_pedge && (green_ac_sig_cur > green_ac_sig_pre)) {
        ir_ac_sig_max = ir_ac_sig_cur;
        red_ac_sig_max = red_ac_sig_cur;
        green_ac_sig_max = green_ac_sig_cur;
    }

    //  Find minimum IR & Red values in negative cycle
    if (ir_nedge && (green_ac_sig_cur < green_ac_sig_pre)) {
        ir_ac_sig_min = ir_ac_sig_cur;
        red_ac_sig_min = red_ac_sig_cur;
        green_ac_sig_min = green_ac_sig_cur;
    }
    if (IRData < 50000)
    // finger-off
    {
        fingerOff++;
        *DRdy = 0;
    } else
        fingerOff = 0;
    /*if  (IRData <50000  &&  fingerOff>10 )
       fingerOff = 0; */
    if (*SpO2B == 0 || *HRbpm2 == 0)
        *DRdy = 0;

    /*if (ns > 2000 )
    {
        if (abs(lastDisplayedHrValue - *HRbpm2)>5)
            *HRbpm2 =lastDisplayedHrValue ;
        else
            lastDisplayedHrValue = *HRbpm2;
    }*/
    // *DRdy = minAmpForHeartBeat;
}

//  Symmetric Dual Low Pass FIR Filter
void lp_dfir_flt(int16_t din0, int16_t din1, int16_t din2, int16_t *dout0, int16_t *dout1, int16_t *dout2)
{
    static const uint16_t FIRCoeffs[12] = { 688, 1283, 2316, 3709, 5439, 7431,
                                            9561, 11666, 13563, 15074, 16047, 16384 };

    static int16_t cbuf0[32], cbuf1[32], cbuf2[32];
    static int16_t offset = 0;
    int32_t y0, y1, y2;
    int16_t i;

    cbuf0[offset] = din0;
    cbuf1[offset] = din1;
    cbuf2[offset] = din2;

    y0 = mul16(FIRCoeffs[11], cbuf0[(offset - 11) & 0x1F]);
    y1 = mul16(FIRCoeffs[11], cbuf1[(offset - 11) & 0x1F]);
    y2 = mul16(FIRCoeffs[11], cbuf2[(offset - 11) & 0x1F]);

    for (i = 0; i < 11; i++) {
        y0 += mul16(FIRCoeffs[i], cbuf0[(offset - i) & 0x1F] + cbuf0[(offset - 22 + i) & 0x1F]);
        y1 += mul16(FIRCoeffs[i], cbuf1[(offset - i) & 0x1F] + cbuf1[(offset - 22 + i) & 0x1F]);
        y2 += mul16(FIRCoeffs[i], cbuf2[(offset - i) & 0x1F] + cbuf2[(offset - 22 + i) & 0x1F]);
    }
    offset = (offset + 1) & 0x1F;

    *dout0 = (y0 >> 15);
    *dout1 = (y1 >> 15);
    *dout2 = (y2 >> 15);
}

//  Integer multiplier
int32_t mul16(int16_t x, int16_t y)
{
    return (int32_t)(x * y);
}