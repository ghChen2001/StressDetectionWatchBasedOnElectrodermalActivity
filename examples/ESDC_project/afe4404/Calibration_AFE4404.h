/*
 * Calibration_AFE4404.h
 *
 * Provides AFE4404 calibration API
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED  
 *
*/

#ifndef _CALIBRATION_AFE4404_H_
#define _CALIBRATION_AFE4404_H_

typedef enum 
{
	s10K=1, 
	s25K=2, 
	s50K=3, 
	s100K=4, 
	s250K=5, 
	s500K=6, 
	s1M=7, 
	s2M=8
} RF_VALUES;		//enumeration type for RF

typedef struct SystemParams //structure for storing the current system parameters
{
  RF_VALUES RF;												// current Rf
  int ILED;													// current ILED
  int ILED2;
  int AMB_DAC_LED;							// current Ambient cancellation value in LED phase
  int AMB_DAC_AMB;		// current Ambient cancellation value in AMB phase
  int AMB_DAC_SIGN_LED;// sign of Ambient cancellation in LED phase (1 means subtract, -1 means add)
  int AMB_DAC_SIGN_AMB;// sign of Ambient cancellation in AMB phase (1 means subtract, -1 means add)
  int Cf;
} SYSTEM_PARAMS;

typedef struct SignalParams //structure for storing the current signal parameters
{
  long DC;								// current DC value in LED/AMB phase
} SIGNAL_PARAMS;

typedef struct flags    			// structure for storing the flag conditions
{
  int GainCalib;						// =0 if Gain Calibration is successful
  int AmbCancel;					// =0 if Ambient cancellation is successful
  int DISABLE_HIGH_CHECK;	// internal flag for disabling high check for gain calibration
  int DISABLE_LOW_CHECK;// internal flag for disabling low check for gain calibration
} FLAGS;

typedef struct calib_enable			// structure for storing calibration mode
{
  int AMB;												// 1 means enabled
  int GAIN;
  int PERIODIC;
} CALIB_ENABLE;

typedef struct amb_function_ret  							// stucture for storing multiple returns for correctamb() function
{
  int AMB_DAC_VAL_LED;						
  int AMB_DAC_VAL_AMB;
  long Meas_DC_Est;
} AMB_FUNCTION_RET;

typedef enum 
{
  sInit,
  sWaitTime1,
  sWaitforData1,
  sGainadjust,
  sCalibrate,
  sWaitTime2a,
  sWaitforData2a,
  sWaitTime2b,
  sWaitforData2b,
  sWaitTime2,
  sWaitforData2,
  sWaitforData3,
  sWaitTime3,
  sWaitforData4,
  sCheck1,
  sCheck2,
  sCheck,
  sOver
} CALIBRATION_STATES; //enumeration type for calibration states

typedef enum 
{
  sAmbientDAC,
  sGain,
  sPeriodic,
  sFinish,
  sInitialize,
  sWaitforfinger,
  sIdle
} CALIBRATION_MODES; //enumeration type for calibration modes

typedef enum 
{
  sFreeze_periodic, 
  sAmbient_periodic, 
  sGain_periodic
} PERIODIC_MODES; //enumeration type for periodic calibration modes

typedef enum 
{ 
  sOffsetDACInit, 
  sOffsetDACWaitTime, 
  sOffsetDACWaitforData, 
  sOffsetDACCodeEst, 
  sOffsetDACOver
} OFFSETDAC_CALIB_STATES; //enumeration type for calibration states


static long Meas_DC_OFFSET_DAC_code[16];
static long Meas_DC_OFFSET_DAC_code_step[16];

/*******************************************************************************/
/*	        Function Declarations									       	   */
/*******************************************************************************/
void initCalibrationRoutine(void);
void AmbientCancellation(long,long);
void CalibrateAFE4404(long,long);
void GainCalibration(long,long);
void PeriodicCalibration(long,long);
void gaincalibrate(long, unsigned int);
AMB_FUNCTION_RET correctamb(long Meas_DC,RF_VALUES Rf_current, char ambient);
void setRfValue(RF_VALUES rfvalue);
void setCfValue(int);
unsigned char OFFSET_DAC_Code_Est(long AMBVALUE);

#endif
