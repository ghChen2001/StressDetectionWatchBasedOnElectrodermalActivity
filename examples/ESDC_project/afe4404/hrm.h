/*
 * hrm.h
 *
 * Provides AFE4404 HRM API
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED  
 *
*/

#ifndef _HRM_H_
#define _HRM_H_

void initStatHRM (void);
void statHRMAlgo (unsigned long ppgData);

void updateWindow(unsigned long *peakWindow, unsigned long Y, unsigned char n);
unsigned char chooseRate(unsigned char *rate);
void updateHeartRate(unsigned char *rate, unsigned int freq, unsigned int last);
unsigned long findMax(unsigned long *X);
unsigned long findMin(unsigned long *X);

#endif /*_HRM_H_*/
