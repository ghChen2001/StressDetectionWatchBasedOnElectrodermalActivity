/*
 * hrm.c
 *
 * Provides AFE4404 HRM API
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED  
 *
*/

#include "hrm.h"

unsigned long peakWindowHP[21], lastOnsetValueLED1, lastPeakValueLED1;
unsigned char HR[12], HeartRate, temp;
unsigned int interArrayWindow[120];
unsigned int iA_pointer, timer_30s, timer_max; // 30s window for interval
unsigned int lastPeak, lastOnset;
unsigned long movingWindowHP;
unsigned char ispeak = 0;
unsigned char movingWindowCount, movingWindowSize, smallest, foundPeak, totalFoundPeak;
unsigned int freq;
unsigned long currentRatio = 0;
unsigned int peakDistance;
void updateInterWindow(int *interArrayWindow);

void initStatHRM(void)
{
    unsigned char i;

    // Init HR variables
    lastPeak = 0;
    lastOnset = 0;
    movingWindowHP = 0;
    movingWindowCount = 0;

    for (i = 20; i >= 1; i--)
        peakWindowHP[(unsigned char)(i - 1)] = 0;

    for (i = 12; i >= 1; i--)
        HR[(unsigned char)(i - 1)] = 0;

    // Sampling frequency
    freq = 64;
    // Moving average window size (removes high frequency noise)
    movingWindowSize = freq / 50;
    // Length of the shortest pulse possible
    smallest = freq * 60 / 220; // Max heart rate = 220 bpm
    foundPeak = 0;
    totalFoundPeak = 0;
    HeartRate = 0;
    iA_pointer = 0;
    timer_30s = 0;
    timer_max = freq * 30;
}

void statHRMAlgo(unsigned long ppgData)
{
    unsigned char i;
    // moving average calculation
    movingWindowHP += ppgData;
    timer_30s++;
    if (timer_30s >= timer_max) {
        updateInterWindow(interArrayWindow);
        // printf("updateInterWindow\r\n");
    }

    if (movingWindowCount > movingWindowSize) {
        // Data processing
        movingWindowCount = 0;
        // update data buffer: move avg result of moving window into peakWindowHP
        updateWindow(peakWindowHP, movingWindowHP, movingWindowSize + 1);
        // reset moving average
        movingWindowHP = 0;
        ispeak = 0;
        if (lastPeak > smallest) {
            // looking for a local maximum using the 20 point buffer
            ispeak = 1;
            // Determine if the value in the middle of the 20 point buffer is the maximum
            for (i = 10; i >= 1; i--) {
                if (peakWindowHP[10] < peakWindowHP[(unsigned int)(10 - i)])
                    ispeak = 0;
                if (peakWindowHP[10] < peakWindowHP[(unsigned int)(10 + i)])
                    ispeak = 0;
            }
            if (ispeak == 1) {
                // if we have a local maximum
                // values for SPO2 ratio
                lastPeakValueLED1 = findMax(peakWindowHP); // find the max peak value
                totalFoundPeak++;

                // lastPeak is currect only if the totalFoundPeak is above 2
                if (totalFoundPeak > 2) {
                    // Update the HR and SPO2 buffer
                    updateHeartRate(HR, freq, lastPeak);
                }
                ispeak = 1;
                lastPeak = 0; // reset lastpeak
                foundPeak++;
            }
        }

        if ((lastOnset > smallest) && (ispeak == 0)) {
            // looking for a local minimum using the 20 point buffer
            ispeak = 1;
            for (i = 10; i >= 1; i--) {
                if (peakWindowHP[10] > peakWindowHP[(unsigned int)(10 - i)])
                    ispeak = 0;
                if (peakWindowHP[10] > peakWindowHP[(unsigned int)(10 + i)])
                    ispeak = 0;
            }

            // if we have a local minimum
            if (ispeak == 1) {
                // values for SPO2 ratio
                lastOnsetValueLED1 = findMin(peakWindowHP);
                totalFoundPeak++;
                if (totalFoundPeak > 2) {
                    // Update the HR and SPO2 buffer
                    //currentRatio= updateSPO2(SPO2,lastOnsetValueIR,lastOnsetValueRed,lastPeakValueIR,lastPeakValueRed);

                    // If you wanted to run an auto calibration here is the ratio that should be used
                    // AutoCalibrate=peakRed/ onsetRed;
                    // AutoCalibrate ratio should be greater that 1-2% if not you need to increase the LED current or adjust the setttings
                }
                lastOnset = 0;
                foundPeak++;
            }
        }

        if (foundPeak > 2) {
            // Every 4 new peaks update return values
            foundPeak = 0;
            temp = chooseRate(HR);
            if ((temp > 40) && (temp < 220))
                HeartRate = temp;
        }
    }
    movingWindowCount++;
    lastOnset++; //
    lastPeak++;  // Current distance from last peak.
}

void updateWindow(unsigned long *peakWindow, unsigned long Y, unsigned char n)
{
    // Moving average buffer for LED data
    unsigned char i;
    // Add New Number into peakWindow
    for (i = 20; i >= 1; i--) {
        peakWindow[i] = peakWindow[(unsigned char)(i - 1)];
    }
    peakWindow[0] = (Y / n);
}

unsigned char chooseRate(unsigned char *rate)
{
    // Returns the average rate, after removing the lowest and highest values (based on the number of found HR removing 2-4-6 values).
    unsigned char max, min, i, nb;
    unsigned int sum, fullsum;
    max = rate[0];
    min = rate[0];
    sum = 0;
    nb = 0;
    for (i = 7; i >= 1; i--) {
        if (rate[(unsigned int)(i - 1)] > 0) {
            if (rate[(unsigned int)(i - 1)] > max) {
                max = rate[(unsigned int)(i - 1)];
            }
            if (rate[(unsigned int)(i - 1)] < min) {
                min = rate[(unsigned int)(i - 1)];
            }
            sum += rate[(unsigned int)(i - 1)];
            nb++;
        }
    }

    if (nb > 2)
        fullsum = (sum - max - min) * 10 / (nb - 2);
    else
        fullsum = (sum)*10 / (nb);

    sum = fullsum / 10;

    if (fullsum - sum * 10 > 4)
        sum++;
    return sum;
}

void updateHeartRate(unsigned char *rate, unsigned int freq, unsigned int last)
{
    // Adds a new Heart rate into the array and lose the oldest
    unsigned char i;
    i = 60 * freq / last;        // last: points (distance) between this peak and last peak.
    if ((i > 40) && (i < 220)) { // Heart rate is between 40bpm and 220bpm.
        for (i = 11; i >= 1; i--) {
            rate[i] = rate[(unsigned char)(i - 1)];
        }
        rate[0] = 60 * freq / last;
    }
    interArrayWindow[iA_pointer] = last;
    if (iA_pointer == 0) {
        timer_30s = 0;
    }
    if (iA_pointer < 120) {
        iA_pointer++;
    }
}

unsigned long findMax(unsigned long *X)
{
    // Finds the maximum around the center of the buffer
    unsigned long res = X[8];
    unsigned char i;
    for (i = 12; i >= 9; i--) {
        if (res < X[i])
            res = X[i];
    }
    return res;
}

unsigned long findMin(unsigned long *X)
{
    // Finds the minimum around the center of the buffer
    unsigned long res = X[8];
    unsigned char i;
    for (i = 12; i >= 9; i--) {
        if (res > X[i])
            res = X[i];
    }
    return res;
}

void updateInterWindow(int *interArrayWindow)
{
    timer_30s -= interArrayWindow[0] * 2;
    // printf("interArrayWindow[0]: %d\r\n", interArrayWindow[0]);
    if (iA_pointer > 0) {
        for (int i = 0; i < iA_pointer - 1; i++) {
            interArrayWindow[i] = interArrayWindow[i + 1];
        }
        iA_pointer--;
    }

    // printf("iA_pointer: %d\r\n", iA_pointer);
}