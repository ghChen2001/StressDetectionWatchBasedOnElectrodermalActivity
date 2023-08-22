/*
 * Calibration_AFE4404.c
 *
 * Provides AFE4404 calibration API
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED  
 *
*/

#include "AFE4404.h"
#include "Calibration_AFE4404.h"

/*******************************************************************************/
/*	        Global Variables										       	   */
/*******************************************************************************/
/* Constants Definitions */
const unsigned long ADC_FULL_SCALE = 0x200000;   				// ADC Full scale
const unsigned long ADC_TEN_PERCENT_FS = 209715; 				// 10% of ADC Full scale
const unsigned long ADC_ONE_PERCENT_FS = 20971; 				// 1% of ADC Full scale

const unsigned int TFREEZE = 2;   								// Time in seconds between two periodic calibrations
const unsigned int TIMEWAIT = 2; 								// Duration (Number of PRFs) to wait after a configuration change

const unsigned int BLOCK_SIZE_BITS = 3; 						// Samples Window size = 2^BLOCK_SIZE_BITS for measuring DC value
const unsigned int BLOCK_SIZE = 8; 							// Number of samples required to calculate the DC value

const unsigned int LED1_reg_shift = 0;							// LED1 current reg - LSB bit position
const unsigned int LED2_reg_shift = 6;							// LED2 current reg - LSB bit position
const unsigned int LED3_reg_shift = 12;							// LED3 current reg - LSB bit position

const unsigned long LED1_mask = 0x00003F;						// LED1 current - mask bits
const unsigned long LED2_mask = 0x000FC0;						// LED2 current - mask bits
const unsigned long LED3_mask = 0x03F000;						// LED3 current - mask bits

const unsigned int AMB_reg_shift_led1 = 5;						// Shift position for OFFDAC - LED1
const unsigned int AMB_reg_shift_amb1 = 10;						// Shift position for OFFDAC - AMB1
const unsigned int AMB_reg_shift_led2 = 15;						// Shift position for OFFDAC - LED2
const unsigned int AMB_reg_shift_amb2 = 0;						// Shift position for OFFDAC - LED3 / AMB2

const unsigned long AMB1_mask = 0x007FE0; 						// Mask for OFFDAC for LED1 and AMB1
const unsigned long AMB2_mask = 0x0FFC00; 						// Mask for OFFDAC for LED2 and AMB1
const unsigned long AMB3_mask = 0x007C1F; 						// Mask for OFFDAC for LED3 and AMB1
const unsigned long AMB23_mask = 0x0FFC1F;						// Mask for OFFDAC for LED2, LED3 and AMB1
const unsigned long AMB123_mask = 0xFFFFF;						// Mask for OFFDAC for LED1, LED2, LED3 and AMB1

const unsigned long AMB1_mask_LED = 0x0003E0;					// Mask for OFFDAC for LED1 only (no AMB1)
const unsigned long AMB2_mask_LED = 0x0F8000;					// Mask for OFFDAC for LED2 only (no AMB1)
const unsigned long AMB3_mask_LED = 0x00001F;					// Mask for OFFDAC for LED3 only (no AMB1)
const unsigned long AMB23_mask_LED = 0x0F801F;					// Mask for OFFDAC for LED2 and LED3 only (no AMB1)
const unsigned long AMB123_mask_LED = 0x0F83FF;					// Mask for OFFDAC for LED1, LED2 and LED3 only (no AMB1)

const unsigned int ILED_CURR_MIN_code = 2; 							// LED min current reqd. for application - 3.2 mA assuming 100mA range (This is default value)
const unsigned int ILED_CURR_MAX_code = 55; 							// LED max current reqd. for application - 88 mA assuming 100mA range (This is default value)

const short unsigned int LOW_THR_PERCENT = 10;       			// Low Threshold Percent
const short unsigned int HIGH_THR_PERCENT = 90;      			// High Threshold percent
const short unsigned int HYS_PERCENT = 3;            			// Hysteresis percent
const short int TARGET_THR_PERCENT = 50;    					// Target Threshold percent

long TARGET_THR; 												// Target threshold for Gain Calibration
long LOW_THR_HYS_LOW; 											// Low threshold for gain calibration in periodic mode
long LOW_THR_HYS_LOW_USER;
long LOW_THR_HYS_LOW_LED2;
long LOW_THR_HYS_HIGH; 											// Low threshold for gain calibration
long HIGH_THR_HYS_LOW; 											// High threshold for gain calibration
long HIGH_THR_HYS_HIGH; 										// High threshold for gain calibration in periodic mode

unsigned long ADC_CODE_AMB_DAC_MIN = 43690;			 			// ADC output for 0.25uA pleth current with Rf = 50K
unsigned long ADC_CODE_AMB_DAC_STEP = 65000;		 			// Step size at the ADC output for an increase of 0.5uA pleth current with Rf = 50K ==> 0.5uA * 100K in codes
unsigned long ADC_CODE_AMB_DAC_STEP_IDEAL = 87381;

extern volatile int g_OneSecondFlag; 							// Flag to indicate the HRM has generated an SNR value (This flag is set in the ISR for ADC ready interrupt after every second)
unsigned int Ambient_DAC_enabled = 0; 							// Indicates whether the AMB_DAC cancellation is used or not

CALIBRATION_STATES AmbientDACcalibration_state = sInit;
CALIBRATION_STATES Gaincalibration_state = sInit;
CALIBRATION_MODES calibration_mode = sAmbientDAC;
PERIODIC_MODES PeriodicCalibration_state = sFreeze_periodic;
OFFSETDAC_CALIB_STATES OffsetDAC_code_Est_state = sOffsetDACInit;
CALIB_ENABLE Calibration_en;
FLAGS flag;
SYSTEM_PARAMS system;
SIGNAL_PARAMS AFE4404_signal;

int Calibration = 1;  						// Indicates whether calibration is ON
unsigned int Periodic_started = 0; 			// Indicates whether periodic calibration has started or not
int Cf_array[8] = { 250, 250, 250, 250, 250, 250, 250, 250 };
unsigned long AFE44xx_Current_Register_Settings[5] = { 0, 0, 0, 0, 0 }; // Array that holds the latest AFE register settings
// For any AFE register change - update the appropriate element
// in the array corresponding to the register
// 0 for register 0x22 - LED current
// 3 for register 0x3A - Amb gain
unsigned int Enable_sep_gain = 1;
unsigned int DC_can = 0;
int LED_pulse_width = 0;
// Selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode)
char LED_Sel = 1;
RF_VALUES RFValue_Init_GainCal = s2M; 							// Initial Rf for Gain calibration (if ambient dac is not enabled)
unsigned int periodic_cal_req = 0;
unsigned long Ipleth = 1875;
unsigned int LED_DC_can = 6;

// Use this for 50mA range
// int Ipleth_array[5] = { 1250, 1875, 2500, 3125, 3750 }; // codes for 1uA, 1.5uA ... 3uA expressed as 1000nA/0.8mA -- unit code is 0.8mA
// Use this for 100mA range
int Ipleth_array[5] = { 625, 938, 1250, 1563, 1875 }; // codes for 1uA, 1.5uA ... 3uA expressed as 1000nA/1.6mA -- unit code is 1.6mA
long CTR = 0;

long meas_DC_arr[8];


/*********************************************************************
* @fn      		setRfValue
*
* @brief		takes in Rf in RF_VALUES type and update the AFE44xx_Current_Register_Settings[1] accordingly
*
* @param   		RF_VALUES
*
* @return  		void
*
*********************************************************************/
void setRfValue(RF_VALUES rfvalue) 
{
  switch (rfvalue) 
  {
    case (s2M):	// Set TIA gain to 2 MOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00007);
	system.Cf = Cf_array[0];
    break;
    
    case (s1M): // Set TIA gain to 1 MOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00006);
	system.Cf = Cf_array[1];
    break;
    
    case (s500K): // Set TIA gain to 500 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00000);
	system.Cf = Cf_array[2];
    break;
    
    case (s250K): // Set TIA gain to 250 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00001);
    system.Cf = Cf_array[3];
	break;
    
    case (s100K): // Set TIA gain to 100 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00002);
    system.Cf = Cf_array[4];
	break;
    
    case (s50K): // Set TIA gain to 50 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00003);
    system.Cf = Cf_array[5];
	break;
    
    case (s25K): // Set TIA gain to 25 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00004);
	system.Cf = Cf_array[6];
    break;
    
    case (s10K): // Set TIA gain to 10 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00005);
	system.Cf = Cf_array[7];
    break;
    
    default: // Set Default TIA gain to 10 kOhm.
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00007)) | (0x00005);
	system.Cf = Cf_array[7];
  }
}

/*********************************************************************
* @fn      		setCfValue
*
* @brief		takes in Cf in int type and update the AFE44xx_Current_Register_Settings[2] accordingly
*
* @param   		int
*
* @return  		void
*
*********************************************************************/
void setCfValue(int Cfvalue) 
{
  switch (Cfvalue) 
  {
    case (250):	// Set Cf = 25 pF
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00030);
    break;
    
    case (200):	// Set Cf = 20 pF
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00020);
    break;
    
    case (100):	// Set Cf = 10 pF
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00010);
    break;
    
    case (50): // Set Cf = 5 pF
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00000);
    break;
    
    case (25): // Set Cf = 2.5 pF
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00008);
    break;
	
    default: // Set Cf = 2.5 pF
    AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00008);
  }
}

/*********************************************************************
* @fn      		initCalibrationRoutine
*
* @brief		Initializes Calibration routine variables
*
* @param   		void
*
* @return  		void
*
*********************************************************************/
void initCalibrationRoutine(void) 
{
	unsigned int Calibration_enabled = 0;
	unsigned int Ipleth_num;
  
	Calibration = 1;
	Calibration_en.AMB = 1; // Check for calibration modes
	Calibration_en.GAIN = 1;
	Calibration_en.PERIODIC = 1;
  
	// Selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode)
	LED_Sel = 1;
  
	Ipleth_num = 4;
	Ipleth = Ipleth_array[Ipleth_num];
	LED_DC_can = Ipleth_num + 2; // When Iplethnum = 0, (1uA) corresponds to reg settings of 2 (reason for why +2)
  
	LED_pulse_width = 25;  //LED pulse width in us
	if (LED_pulse_width >= 500) 
	{
		Cf_array[0] = 250;
	} 
	else if (LED_pulse_width >= 400) 
	{
		Cf_array[0] = 200;
	} 
	else if (LED_pulse_width >= 200) 
	{
		Cf_array[0] = 100;
		Cf_array[1] = 200;
	} 
	else if (LED_pulse_width >= 100) 
	{
		Cf_array[0] = 50;
		Cf_array[1] = 100;
		Cf_array[2] = 200;
	} 
	else if (LED_pulse_width >= 50) 
	{
		Cf_array[0] = 25;
		Cf_array[1] = 50;
		Cf_array[2] = 100;
		Cf_array[3] = 200;
	} 
	else if (LED_pulse_width >= 25) 
	{
		Cf_array[0] = 25;
		Cf_array[1] = 25;
		Cf_array[2] = 50;
		Cf_array[3] = 100;
		Cf_array[4] = 200;
	} 
	else 
	{
		Cf_array[0] = 25;
		Cf_array[1] = 25;
		Cf_array[2] = 25;
		Cf_array[3] = 25;
		Cf_array[4] = 25;
		Cf_array[5] = 25;
		Cf_array[6] = 25;
		Cf_array[7] = 25;
	}
  
	calibration_mode = sAmbientDAC; //calibration mode is set to Ambient Cancellation
	AmbientDACcalibration_state = sInit;
	Gaincalibration_state = sInit;
	PeriodicCalibration_state = sFreeze_periodic;
  
	Periodic_started = 0;
	flag.GainCalib = 0;							//flags are initialized to zero
	flag.AmbCancel = 0;
  
	system.ILED = ILED_CURR_MIN_code;
	system.ILED2 = ILED_CURR_MIN_code;
	system.AMB_DAC_LED = 0;
	system.AMB_DAC_SIGN_LED = 1;          // sign: 1 - positive and 0 - negative
	system.AMB_DAC_AMB = 0;
	system.AMB_DAC_SIGN_AMB = 1;
	AFE4404_signal.DC = 0;
	system.RF = s50K;            //sets the initial system and AFE4404_signal parameters
	system.Cf = 25;
  
	Calibration_enabled = Calibration_en.AMB + Calibration_en.GAIN + Calibration_en.PERIODIC;
  
	if (Calibration_enabled != 0) 
	{
		AFE44xx_Current_Register_Settings[2] = 0;
    //AFE44xx_Current_Register_Settings[1] = 0;
		// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
		if (LED_Sel == 2) 
		{
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & (~LED2_mask));	
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & (~AMB2_mask));
		}
		else if (LED_Sel == 3) 
		{
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & (~LED3_mask));	
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & (~AMB3_mask));
		}
		else //Default is LED1
		{
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & (~LED1_mask));	
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & (~AMB1_mask));
		}

		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
		AFE4404_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);    //0x20
    //AFE4404_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
		AFE4404_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2] | 0x000100);
		AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
	}
  
	//Enable_sep_gain = (AFE44xx_Current_Register_Settings[1] & 0x8000)>>15;
	LOW_THR_HYS_LOW = ((LOW_THR_PERCENT - HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	LOW_THR_HYS_LOW_USER = LOW_THR_HYS_LOW;
	LOW_THR_HYS_LOW_LED2 = LOW_THR_HYS_LOW;
	LOW_THR_HYS_HIGH = ((LOW_THR_PERCENT + HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	HIGH_THR_HYS_LOW = ((HIGH_THR_PERCENT - HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	HIGH_THR_HYS_HIGH = ((HIGH_THR_PERCENT + HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	TARGET_THR = ((TARGET_THR_PERCENT * ADC_FULL_SCALE) / 100);
	DC_can = 0;
}

/*********************************************************************
* @fn      		AmbientCancellation
*
* @brief		Looks at the Ambient values and cancels it off using the AMBIENT_DAC
*
* @param   		int,int
*
* @return  		void
*
*********************************************************************/
void AmbientCancellation(long LEDVALUE, long AMBVALUE) 
{
  AMB_FUNCTION_RET amb_results;
  static long ILEDCode, ILEDCode2;
  static RF_VALUES RFValue;
  long LEDInterimCode;
  
  long AMBInterimCode;
  static short int waitTimeCount = 0;
  static short int blockSizeCount = 0;
  static long Meas_DC = 0;
  static long Meas_DC_s50K = 0;
  
  static int AMB_DAC_VALUE_LED = 0;
  static long AMB_DAC_VALUE_LED_interim = 0;
  static int AMB_DAC_VALUE_SIGN_LED = 0;
  
  static int AMB_DAC_VALUE_AMB = 0;
  static long AMB_DAC_VALUE_AMB_interim = 0;
  static int AMB_DAC_VALUE_SIGN_AMB = 0;
  
  static int sign_AMB_DAC_LED = 1;
  static int sign_AMB_DAC_AMB = 1;
  
  int Rf_factor[8] = { 5, 2, 1, 2, 5, 10, 20, 40 };
  char Rf_num;
  
  if (Calibration_en.AMB == 0)
    AmbientDACcalibration_state = sOver; // If ambient cancellation is not enabled by host, go to state sOver
  
  switch (AmbientDACcalibration_state) 
  {  
    case (sInit):
    /* 	Sinit initializes the ambient cancellation routine - 
		sets Rf to 50K, ILED to ILED min in the initial execution 
		and retains the system.Rf and system.ILED in the periodic calibration
	*/
    ILEDCode = system.ILED; // sets the ILED code to ILED min initially and the current ILED in periodic calibration
	// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
	if (LED_Sel == 2) 
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode = (ILEDCode << LED2_reg_shift);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode;
	} 
	else if (LED_Sel == 3) 
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode = (ILEDCode << LED3_reg_shift);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode;
	} 
	else // Default is LED1
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode = (ILEDCode << LED1_reg_shift);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode;
	}
    
    RFValue = system.RF; // sets RFValue to 50K in the initial mode and retains the current value in periodic calibration
    setRfValue(RFValue);
    setCfValue(system.Cf);
    // updates the TIA GAIN register value
    if (Periodic_started == 0) // writes to the register in the initail mode only
    {
      AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
      AFE4404_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]); //0x20
      //AFE4404_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
      AFE4404_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2] | 0x000100);
      AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
      
    }
    AmbientDACcalibration_state = sWaitTime1;			// Go to sWaitTime1
    break;
    
    case (sWaitTime1):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) {
      waitTimeCount = 0;
      blockSizeCount = 0;
      Meas_DC = 0;
      AmbientDACcalibration_state = sWaitforData1;  // Go to sWaitforData1
    }
    break;
    
    case (sWaitforData1):
    /* Calculate the average of AMBVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += AMBVALUE;
    if (blockSizeCount == BLOCK_SIZE) {
      blockSizeCount = 0;
      Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      AmbientDACcalibration_state = sCalibrate;		// Go to sCalibrate
    }
    break;
	
    case (sCalibrate):
    /* Calculate the Ambient cancellation required and writes it to the register */
    amb_results = correctamb(Meas_DC, RFValue, 1);
    /* calls the correctamb() function with parameters as AMBVALUE, current Rf, '1' stands for ambient cancellation
    which indicates that the cancellation has to be done in both the LED and AMB phase*/
    AMB_DAC_VALUE_SIGN_LED = amb_results.AMB_DAC_VAL_LED; // Takes in the Ambient cancellation required for the LED phase
    AMB_DAC_VALUE_SIGN_AMB = amb_results.AMB_DAC_VAL_AMB; // Takes in the Ambient cancellation required for the AMB phase
    
    if (AMB_DAC_VALUE_SIGN_LED >= 0)	// Isolates the sign and the Value
    {
      sign_AMB_DAC_LED = 1;
      AMB_DAC_VALUE_LED = AMB_DAC_VALUE_SIGN_LED;
    } 
	else 
	{
      sign_AMB_DAC_LED = 0;
      AMB_DAC_VALUE_LED = -1 * AMB_DAC_VALUE_SIGN_LED;
    }
    
	if (AMB_DAC_VALUE_SIGN_AMB >= 0) 
	{
      sign_AMB_DAC_AMB = 1;
      AMB_DAC_VALUE_AMB = AMB_DAC_VALUE_SIGN_AMB;
    } 
	else 
	{
      sign_AMB_DAC_AMB = 0;
      AMB_DAC_VALUE_AMB = -1 * AMB_DAC_VALUE_SIGN_AMB;
    }
    
    AMB_DAC_VALUE_LED_interim = AMB_DAC_VALUE_LED | (sign_AMB_DAC_LED << 4); //Calculates the register code for the corresponding Ambient cancellation required in LED phase
    AMB_DAC_VALUE_AMB_interim = AMB_DAC_VALUE_AMB | (sign_AMB_DAC_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
    
	if (LED_Sel == 2) 
	{
		AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led1)
			| (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1); //Interim value to be written to the register
	} 
	else if (LED_Sel == 3) 
	{
		AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led1)
			| (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_amb2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1); //Interim value to be written to the register
	} 
	else 
	{ // Default is LED1
		AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led2) | (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led1)
			| (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1); //Interim value to be written to the register
	}
	
	// Updates the AMB_DAC register value
	AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB123_mask)) | AMBInterimCode;
    
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
    AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
    
    if ((AMB_DAC_VALUE_LED == 0) && (AMB_DAC_VALUE_AMB == 0)) // if the Ambient cancellation is non zero, Ambient_DAC_enabled is set to 1
      Ambient_DAC_enabled = 0; // This is to ensure that max Rf used is 250K whenever the Ambient DAC is used
    else
      Ambient_DAC_enabled = 1;
    AmbientDACcalibration_state = sWaitTime2;			// Go to sWaitTime2
    break;
    
    case (sWaitTime2):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
		waitTimeCount = 0;
		//blockSizeCount = 0;
		Meas_DC = 0;
		AmbientDACcalibration_state = sWaitforData2;// Go to sWaitforData2
    }
    break;
    
    case (sWaitforData2):
    /* Calculate the average of AMBVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += AMBVALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
		AmbientDACcalibration_state = sCheck;				// Go to sCheck
    }
    break;
    
    case (sCheck):
    /* Checks if the Ambient cancellation has actually cancelled the external ambient as expected - if not sets a flag */
    if (Meas_DC < 0)
      Meas_DC *= (-1);
    Rf_num = system.RF - 1;
    if (Rf_num < 2)	// Translates the Meas_DC to Meas_DC_s50K which means the DC value assuming the RF was 50K
      Meas_DC_s50K = Meas_DC / Rf_factor[Rf_num];
    else
      Meas_DC_s50K = Meas_DC * Rf_factor[Rf_num];
    
    if (Meas_DC_s50K > (ADC_CODE_AMB_DAC_MIN + ADC_CODE_AMB_DAC_STEP)) //Checks if the ambient current is less than 0.75uA
    {
      flag.AmbCancel = 0;
    } 
	else 
	{
      flag.AmbCancel = 1 << 4;
    }
    AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(0x10)) | flag.AmbCancel; //updates the HRM_Calib_rate register value
    
    system.RF = RFValue;		// Updates the system and AFE4404_signal parameters
    system.ILED = ILEDCode;
    system.ILED2 = ILEDCode2;
    system.AMB_DAC_LED = AMB_DAC_VALUE_LED;
    system.AMB_DAC_SIGN_LED = 2 * sign_AMB_DAC_LED - 1;
    system.AMB_DAC_AMB = AMB_DAC_VALUE_AMB;
    system.AMB_DAC_SIGN_AMB = 2 * sign_AMB_DAC_AMB - 1;
    AFE4404_signal.DC = Meas_DC;
    
    AmbientDACcalibration_state = sOver;					// Go to sOver
    break;
    
    case (sOver):
    /* Does the termination procedure for the Ambient cancellation routine */
    if (Periodic_started == 0)
      calibration_mode = sGain; // In the initial calibration, Go to sGain mode
    else
      calibration_mode = sPeriodic; // In the periodic calibration, stay in sPeriodic mode always
    
    AmbientDACcalibration_state = sInit; // Go to sInit in the next execution of this procedure (which will be during periodic calibration)
    PeriodicCalibration_state = sGain_periodic;
  }
}

/*********************************************************************
* @fn      		GainCalibration
*
* @brief		Looks at the LED values and tries to bring it to TARGET_THR by varying TIA gain and LED currents
*
* @param   		long, long
*
* @return  		void
*
*********************************************************************/
void GainCalibration(long LEDMINUSAMBVALUE, long LEDVALUE) 
{
static long LEDInterimCode;
static short int waitTimeCount = 0;
static short int blockSizeCount = 0;
static long Meas_DC = 0;
static int sign_DC;
  //static long Meas_DC_LED2 = 0;
  //static int sign_DC_LED2;
static RF_VALUES RFValue;
static long AMB_DAC_VALUE_AMB, sign_AMB_DAC_AMB, AMB_DAC_VALUE_AMB_interim, AMBInterimCode;
static long AMB_DAC_VALUE_LED_interim_UPDATED, AMBInterimCode_UPDATED;
static long AMB_DAC_VALUE_SIGN_UPDATED, sign_AMB_DAC_UPDATED, AMB_DAC_VALUE_UPDATED;

if (Calibration_en.GAIN == 0)
	Gaincalibration_state = sOver;
switch (Gaincalibration_state) 
{
	case (sInit):
	/* Sets Rf to 250K/2M depending on whether the Ambient DAC is enabled or not and ILED to min ILED in the initial calibration. */
	system.ILED = ILED_CURR_MIN_code; // sets the ILED to minimum ILED always (both in initial calibration and periodic calibration
    //system.ILED2 = ILED_CURR_MIN_code;
	// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
	if (LED_Sel == 2) 
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode = (ILED_CURR_MIN_code << LED2_reg_shift);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode; 
	} 
	else if (LED_Sel == 3) 
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode = (ILED_CURR_MIN_code << LED3_reg_shift);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode;
	} 
	else // Default is LED1
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode = (ILED_CURR_MIN_code << LED1_reg_shift);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode;
	}
	RFValue = system.RF;
    
    if (Ambient_DAC_enabled == 1) // If Ambient cancellation is non zero, then use 250K as the max Rf, otherwise 2M
      RFValue_Init_GainCal = s250K;
    else
      RFValue_Init_GainCal = s2M;
    
    RFValue = RFValue_Init_GainCal;
    setRfValue(RFValue);
    setCfValue(system.Cf);
    // Updates the TIA GAIN register value
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
    AFE4404_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);    //0x20
    //AFE4404_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
    AFE4404_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2] | 0x000100);
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
    
    Gaincalibration_state = sWaitTime1;					// Go to sWaitTime1
	break;
    
    case (sWaitTime1):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
		waitTimeCount = 0;
		Meas_DC = 0;
      //Meas_DC_LED2 = 0;
		Gaincalibration_state = sWaitforData1;		// Go to sWaitforData1
    }
    break;
    
    case (sWaitforData1):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += LEDMINUSAMBVALUE;
    //Meas_DC_LED2 += LED2VALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      //Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
		Gaincalibration_state = sGainadjust;			// Go to sGainadjust
    }
    break;
    
    case (sGainadjust):
    /* Sets the Rf such that LEDVALUE will less than Target threshold - Done only in initial calibration*/
    if (Meas_DC < 0)// Convert the Meas_DC to positive and store the sign information separately
    {
      Meas_DC = -1 * Meas_DC;
      sign_DC = 0;
    } 
	else
      sign_DC = 1;
    
    //    if (Meas_DC_LED2 < 0)// Convert the Meas_DC to positive and store the sign information separately
    //    {
    //      Meas_DC_LED2 = -1 * Meas_DC_LED2;
    //      sign_DC_LED2 = 0;
    //    } 
    //	else
    //      sign_DC_LED2 = 1;
    
    //if ((Meas_DC > TARGET_THR) || (Meas_DC_LED2 > TARGET_THR))// The sInit had set the appropriate Rf and minimmum ILED. Now if the Meas_DC is still higher than Target threshold, it enters this condition
    if (Meas_DC > TARGET_THR)
    {
		if (RFValue > s10K)	// Keep decrementing the Rf till Meas_DC becomes less than Target threshold
		{
			RFValue--;
			setRfValue(RFValue);
			setCfValue(system.Cf);
			// Update the TIA GAIN register value
			AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
        //AFE4404_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
			AFE4404_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2] | 0x000100);
			AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
			Gaincalibration_state = sWaitTime1;			// Go to sWaitTime1
		} 
		else 
			Gaincalibration_state = sCalibrate;
	} 
	else 
		Gaincalibration_state = sCalibrate;
    system.RF = RFValue;
    break;
    
    case (sCalibrate):
    /* calls the gaincalibrate function and update the register settings */
    gaincalibrate(Meas_DC, sign_DC);// This function estimates the CTR and ajust gain,ambdac and ledcurrent accordingly
    
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
    AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
    AFE4404_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);    //0x20
    //AFE4404_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
    AFE4404_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2] | 0x000100);
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
    
    Gaincalibration_state = sWaitTime2a;			// Go to sWaitTime2a
    break;
    
    case (sWaitTime2a):
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
		waitTimeCount = 0;
		Meas_DC = 0;
      //Meas_DC_LED2 = 0;
		Gaincalibration_state = sWaitforData2a;		// Go to sWaitforData2
    }
    break;
    
    case (sWaitforData2a):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += LEDMINUSAMBVALUE;
    //Meas_DC_LED2 += LED2VALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      //Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
		Gaincalibration_state = sCheck1;					// Go to sCheck1
    }
    break;
    
    case (sCheck1):
    if (Meas_DC < 0) 
	{
		if (Meas_DC < -1529173) //equivalent to -1.75uA pleth current with Rf = 250K
		{
			AMB_DAC_VALUE_SIGN_UPDATED = system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED - 4; // Subtract 2uA dac current.
		} 
		else if (Meas_DC < -1092266) //equivalent to -1.25uA pleth current with Rf = 250K
		{
			AMB_DAC_VALUE_SIGN_UPDATED = system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED - 3; // Subtract 1.5uA dac current.
		} 
		else if (Meas_DC < -655360) //equivalent to -0.75uA pleth current with Rf = 250K (-655360 -0.75uA)
		{
			AMB_DAC_VALUE_SIGN_UPDATED = system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED - 2; // Subtract 1uA dac current.
		}
		else if (Meas_DC < -218453) //equivalent to -0.25u pleth current with Rf = 250K (-218453 -0.25uA)
		{
			AMB_DAC_VALUE_SIGN_UPDATED = system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED - 1; // Subtract 0.5uA dac current.
		} 
		else 
		{
			AMB_DAC_VALUE_SIGN_UPDATED = system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED; // Subtract 0uA dac current.
		}
      
		if (AMB_DAC_VALUE_SIGN_UPDATED >= 0) 
		{
			AMB_DAC_VALUE_UPDATED = AMB_DAC_VALUE_SIGN_UPDATED;
			sign_AMB_DAC_UPDATED = 1;
		} 
		else 
		{
			AMB_DAC_VALUE_UPDATED = (-1) * AMB_DAC_VALUE_SIGN_UPDATED;
			sign_AMB_DAC_UPDATED = 0;
		}
      
		AMB_DAC_VALUE_LED_interim_UPDATED = AMB_DAC_VALUE_UPDATED | (sign_AMB_DAC_UPDATED << 4);
		if (LED_Sel == 2) 
		{
			//Interim value to be written to the register
			AMBInterimCode_UPDATED = (AMB_DAC_VALUE_LED_interim_UPDATED << AMB_reg_shift_led2);
			// Updates the AMB_DAC register value
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB2_mask_LED)) | AMBInterimCode_UPDATED;
		} 
		else if (LED_Sel == 3) 
		{
			//Interim value to be written to the register
			AMBInterimCode_UPDATED = (AMB_DAC_VALUE_LED_interim_UPDATED << AMB_reg_shift_amb2);
			// Updates the AMB_DAC register value
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB3_mask_LED)) | AMBInterimCode_UPDATED;
		} 
		else 
		{ // Default is LED1
			//Interim value to be written to the register
			AMBInterimCode_UPDATED = (AMB_DAC_VALUE_LED_interim_UPDATED << AMB_reg_shift_led1);
			// Updates the AMB_DAC register value
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB1_mask_LED)) | AMBInterimCode_UPDATED;
		}
	 
      
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
		AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
		system.AMB_DAC_LED = AMB_DAC_VALUE_UPDATED;
		system.AMB_DAC_SIGN_LED = 2 * sign_AMB_DAC_UPDATED - 1;
    }
    Gaincalibration_state = sWaitTime2b;
    break;
    
    case (sWaitTime2b):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
		waitTimeCount = 0;
		Meas_DC = 0;
      //Meas_DC_LED2 = 0;
		Gaincalibration_state = sWaitforData2b;		// Go to sWaitforData2
    }
    break;
    
    case (sWaitforData2b):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += LEDMINUSAMBVALUE;
    //Meas_DC_LED2 += LED2VALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      //Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
		Gaincalibration_state = sCheck2;					// Go to sCheck
    }
    break;
    
    case (sCheck2):
    if (Meas_DC < 0)
      Meas_DC *= (-1);
    if (Meas_DC > 0) 
	{
		RFValue = system.RF;
      if (Meas_DC <= ADC_ONE_PERCENT_FS)
		{
			if (RFValue < s1M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
			{
				RFValue += 2;
				setRfValue(RFValue);
				setCfValue(system.Cf);
			} 
			else if (RFValue < s2M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
			{
				RFValue += 1;
				setRfValue(RFValue);
				setCfValue(system.Cf);
			}
		} 
      else if (Meas_DC <= (3 * ADC_ONE_PERCENT_FS))
		{
			if (RFValue < s2M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
			{
				RFValue += 1;
				setRfValue(RFValue);
				setCfValue(system.Cf);
			}
		}
		// Update the TIA GAIN register value
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
      //AFE4404_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
		AFE4404_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2] | 0x000100);
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
		system.RF = RFValue;
    }
    Gaincalibration_state = sWaitTime2;
    break;
    
    case (sWaitTime2):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
      waitTimeCount = 0;
      Meas_DC = 0;
      //Meas_DC_LED2 = 0;
      Gaincalibration_state = sWaitforData2;		// Go to sWaitforData2
    }
    break;
    
    case (sWaitforData2):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += LEDMINUSAMBVALUE;
    //Meas_DC_LED2 += LED2VALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
      blockSizeCount = 0;
      Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      //Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
      Gaincalibration_state = sCheck;						// Go to sCheck
    }
    break;
    
    case (sCheck):
    /* Checks if the Gain calibration has actually worked as expected - if not sets a flag */
    LOW_THR_HYS_LOW = Meas_DC - 419430;
      
    if (LOW_THR_HYS_LOW > LOW_THR_HYS_LOW_USER)
		LOW_THR_HYS_LOW = LOW_THR_HYS_LOW_USER;
      
    if (DC_can == 0) 
	{
        if (LOW_THR_HYS_LOW < 104857) 		// < 5%
			LOW_THR_HYS_LOW = 104857;
    }
    else 
	{
        if (LOW_THR_HYS_LOW < -1048576) 	// < -50%
			LOW_THR_HYS_LOW = -1048576;
    }
    
    if (Meas_DC < 0)
		Meas_DC *= (-1);
    //    if (Meas_DC_LED2 < 0)
    //		Meas_DC_LED2 *= (-1);
    //if((Meas_DC > TARGET_THR) || (Meas_DC_LED2 > TARGET_THR))
    if (Meas_DC > TARGET_THR) 
		flag.GainCalib = 0;
	else
		flag.GainCalib = 1 << 5;
    AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(0x20)) | flag.GainCalib; //Update the HRM_Calib_rate register value
    
    AFE4404_signal.DC = Meas_DC;
    Gaincalibration_state = sOver;							// Go to sOver
    break;
    
    case (sOver):
    /* Does the termination procedure for the Gain calibration routine */
    Gaincalibration_state = sWaitforData3;// Go to sWaitforData3 in the next execution of this function
    if (Calibration_en.PERIODIC == 1) 
	{
      calibration_mode = sPeriodic;
      Periodic_started = 1;
    } 
	else
      calibration_mode = sFinish;
    
    PeriodicCalibration_state = sFreeze_periodic;
    break;
    
    case (sWaitforData3):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples - Also checks if the Gain calibration has to be done or not*/
    if (blockSizeCount == 0) 
	{
      Meas_DC = 0;
      //Meas_DC_LED2 = 0;
    }
    blockSizeCount++;
    Meas_DC += LEDVALUE;
    //Meas_DC_LED2 += LED2VALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      //Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
      
		if ((Meas_DC > HIGH_THR_HYS_HIGH) || (Meas_DC < LOW_THR_HYS_LOW))
			periodic_cal_req = 1;
		else 
			periodic_cal_req = 0;
        
		//if((Meas_DC > HIGH_THR_HYS_HIGH) || (Meas_DC < LOW_THR_HYS_LOW) || (Meas_DC_LED2 > HIGH_THR_HYS_HIGH) || (Meas_DC_LED2 < LOW_THR_HYS_LOW_LED2))
		// If Meas_DC is outside the low or high threshold
		if (periodic_cal_req == 1) 
		{
			if (Meas_DC < 0)// Convert the Meas_DC to positive and store the sign information separately
			{
				Meas_DC = -1 * Meas_DC;
				sign_DC = 0;
			} 
			else
				sign_DC = 1;
        //			if (Meas_DC_LED2 < 0)// Convert the Meas_DC to positive and store the sign information separately
        //			{
        //				Meas_DC_LED2 = -1 * Meas_DC_LED2;
        //				sign_DC_LED2 = 0;
        //			} 
        //			else
        //				sign_DC_LED2 = 1;
        
        //if ((Meas_DC > 1887436) || (Meas_DC_LED2 > 1887436)) // Meas DC > 90%
        if (Meas_DC > 1887436)
			{
				system.ILED = ILED_CURR_MIN_code; // sets the ILED to minimum ILED
          //system.ILED2 = ILED_CURR_MIN_code;
				
				// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
				if (LED_Sel == 2) 
				{
					// shifts the ILED code according to the current field corresponding to the LED number
					LEDInterimCode = (ILED_CURR_MIN_code << LED2_reg_shift); 
					//updates the ILED register value
					AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode;
				} 
				else if (LED_Sel == 3) 
				{
					// shifts the ILED code according to the current field corresponding to the LED number
					LEDInterimCode = (ILED_CURR_MIN_code << LED3_reg_shift);
					//updates the ILED register value
					AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode;
				} 
				else 
				{ // Default is LED1
					// shifts the ILED code according to the current field corresponding to the LED number
					LEDInterimCode = (ILED_CURR_MIN_code << LED1_reg_shift);
					//updates the ILED register value
					AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode;
				}
          
				AMB_DAC_VALUE_AMB = system.AMB_DAC_AMB;
				sign_AMB_DAC_AMB = (1 + system.AMB_DAC_SIGN_AMB) >> 1;
				AMB_DAC_VALUE_AMB_interim = AMB_DAC_VALUE_AMB | (sign_AMB_DAC_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
				
				if (LED_Sel == 2) 
				{
					//Interim value to be written to the register
					AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led2) ^ (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1);
					// Updates the AMB_DAC register value
					AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB2_mask)) | AMBInterimCode;
				} 
				else if (LED_Sel == 3) 
				{
					//Interim value to be written to the register
					AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb2) ^ (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1);
					// Updates the AMB_DAC register value
					AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB3_mask)) | AMBInterimCode;
				} 
				else  // Default is LED1
				{ 
					//Interim value to be written to the register
					AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led1) ^ (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1);
					// Updates the AMB_DAC register value
					AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB1_mask)) | AMBInterimCode;
				}
				system.AMB_DAC_LED = system.AMB_DAC_AMB;
				system.AMB_DAC_SIGN_LED = system.AMB_DAC_SIGN_AMB;
			  
				AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
				AFE4404_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);        //0x20
				AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
				AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
          Gaincalibration_state = sWaitTime1;		// Go to sWaitTime3
			} 
			else 
				Gaincalibration_state = sCalibrate;	// Go to sCalibrate
		} 
		else 
		{
			Gaincalibration_state = sWaitforData3;// if Meas_DC is within the limits - Come to the same state in the next execution of this function
			PeriodicCalibration_state = sFreeze_periodic;// Go to sFreeze_periodic mode
		}
    }
    break;
    
    case (sWaitTime3):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
		waitTimeCount = 0;
		Meas_DC = 0;
      //Meas_DC_LED2 = 0;
		Gaincalibration_state = sWaitforData4;		// Go to sWaitforData4
    }
    break;
    
    case (sWaitforData4):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
    blockSizeCount++;
    Meas_DC += LEDVALUE;
    //Meas_DC_LED2 += LED2VALUE;
    if (blockSizeCount == BLOCK_SIZE) 
	{
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
      //Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
		Gaincalibration_state = sCalibrate;			// Go to sCalibrate
    }
    break;
    
  }
}

/*********************************************************************
* @fn      	CalibrateAFE4404
*
* @brief	wrapper around the individual calibration routines - calls each function based on the calibration mode
*
* @param   	LEDVALUE,AMBVALUE
*
* @return  	void
*
*********************************************************************/
void CalibrateAFE4404(long LEDVALUE, long AMBVALUE) 
{
	switch (calibration_mode) 
	{
		case (sInitialize):
		//calls the initCalibrationRoutine
		initCalibrationRoutine();
		break;
    
		case (sAmbientDAC):
		//calls the AmbientCancellation function
		AmbientCancellation(LEDVALUE, AMBVALUE);
		break;
    
		case (sGain):
		//calls the GainCalibration function
    GainCalibration(LEDVALUE - AMBVALUE, LEDVALUE);
		break;
    
		case (sPeriodic):
		//calls the PeriodicCalibration function
    PeriodicCalibration(LEDVALUE, AMBVALUE);
		break;
    
		case (sFinish):
		//ends the calibration
		Calibration = 0;
		break;
    
		default:
		break;
	}
}


/*********************************************************************
* @fn      		PeriodicCalibration
*
* @brief			Take care of calling the individual calibration routines in periodic calibration mode
*
* @param   		LEDVALUE,AMBVALUE
*
* @return  		void
*
*********************************************************************/
void PeriodicCalibration(long LEDVALUE, long AMBVALUE) 
{
	static short int Tfreeze = 0;
	switch (PeriodicCalibration_state) 
	{
		case (sFreeze_periodic):
		//Waits for TFREEZE seconds
		if (g_OneSecondFlag == 1) 
		{
			g_OneSecondFlag = 0;
			Tfreeze++;
			if (Tfreeze == TFREEZE) 
			{
				PeriodicCalibration_state = sAmbient_periodic;
				Tfreeze = 0;
			}
		}
		break;
    
		case (sAmbient_periodic):
		//calls the AmbientCancellation function
		AmbientCancellation(LEDVALUE, AMBVALUE);
		break;
		
		case (sGain_periodic):
		//calls the GainCalibration function
    GainCalibration(LEDVALUE - AMBVALUE, LEDVALUE);
		break;
	}
}

/*********************************************************************
* @fn      		correctamb
*
* @brief			takes in ambient value and current RF value and corrects the ambient current if any
*
* @param   		Meas_DC, Rf_current, ambient
*
* @return  		void
*
*********************************************************************/
AMB_FUNCTION_RET correctamb(long Meas_DC, RF_VALUES Rf_current, char ambient) {
  AMB_FUNCTION_RET amb_results;
  static long Meas_DC_Est_s50K = 0;
  static long Meas_DC_s50K = 0;
  static long AMB_DAC_VALUE = 0;
  int Rf_factor[8] = { 5, 2, 1, 2, 5, 10, 20, 40 };
  int sign_DC = 1;
  char Rf_num;
  
  Rf_num = Rf_current - 1;
  if (Rf_num <= 2)			// Converts the Meas_DC to case when Rf is 50K
    Meas_DC_s50K = Meas_DC * Rf_factor[Rf_num];
  else
    Meas_DC_s50K = Meas_DC / Rf_factor[Rf_num];
  if (Meas_DC_s50K < 0) {
    Meas_DC_s50K = -1 * Meas_DC_s50K;
    sign_DC = -1;
  }
  AMB_DAC_VALUE = (((Meas_DC_s50K << 3) / ADC_CODE_AMB_DAC_STEP) + 0x4) >> 3;	//Estimate the ambient cancellation required
  if (sign_DC == 1) {
    if (system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED + AMB_DAC_VALUE > 15) //15
      AMB_DAC_VALUE = 15 - system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED;
  } else {
    if (system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED - AMB_DAC_VALUE < -15)
      AMB_DAC_VALUE = 15 + system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED;
  }
  //
    if(AMB_DAC_VALUE > 5)
      AMB_DAC_VALUE = 5;
  
  Meas_DC_Est_s50K = sign_DC
    * (Meas_DC_s50K - AMB_DAC_VALUE * ADC_CODE_AMB_DAC_STEP);
  if (Rf_num > 2)						// Convert back to the current RF case
    amb_results.Meas_DC_Est = Meas_DC_Est_s50K * Rf_factor[Rf_num];
  else
    amb_results.Meas_DC_Est = Meas_DC_Est_s50K / Rf_factor[Rf_num];
  
  amb_results.AMB_DAC_VAL_LED =
    (sign_DC == 1) ?
      system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED
        + AMB_DAC_VALUE :
  system.AMB_DAC_SIGN_LED * system.AMB_DAC_LED
    - AMB_DAC_VALUE;
  if (ambient == 1) {
    amb_results.AMB_DAC_VAL_AMB =
      (sign_DC == 1) ?
        system.AMB_DAC_SIGN_AMB * system.AMB_DAC_AMB
          + AMB_DAC_VALUE :
    system.AMB_DAC_SIGN_AMB * system.AMB_DAC_AMB
      - AMB_DAC_VALUE;
  } else {
    amb_results.AMB_DAC_VAL_AMB = system.AMB_DAC_SIGN_AMB
      * system.AMB_DAC_AMB;
  }
  return amb_results;
}

/*********************************************************************
* @fn      		gaincalibrate
*
* @brief			Calculate the CTR and calibrates to get good AFE4404_signal
*
* @param   		Meas_DC, Rf_current, ambient
*
* @return  		void
*
*********************************************************************/

void gaincalibrate(long Meas_DC, unsigned int sign_DC) 
{
  static long ILEDCode_gc, LEDInterimCode_gc;
static RF_VALUES RFValue;
static long AMB_DAC_VAL_LED;
static long AMB_DAC_VAL_AMB;
static long AMB_DAC_SIGN_LED;
static long AMB_DAC_SIGN_AMB;
static long AMB_DAC_VAL_LED_SIGN;
static long AMB_DAC_VAL;
// Use this for 50mA range
//static long Meas_DC_1K_1code_10CTR = 28;      // 1000 * 2 * 0.8mA * 10nA/1mA volts - convert into codes
// Use this for 100mA range
static long Meas_DC_1K_1code_10CTR = 56; // 1000 * 2 * 0.8mA * 10nA/1mA volts - convert into codes
static long Meas_DC_1K_500nAPleth = 1748; // 1000 * 0.5uA * 2
static long Meas_DC_10CTR;
static int AMB_DAC_VALUE_LED_interim;
static int sign_AMB_DAC;
static long AMBInterimCode;
static long Ipleth_achieved;
int Rfvalues[8] = { 10, 25, 50, 100, 250, 500, 1000, 2000 };
int Rf_int;
  unsigned long ADC_CODE_AMB_DAC_STEP_TMP;
 
RFValue = system.RF;				//takes in the current system parameters
ILEDCode_gc = system.ILED;
  //ILEDCode2_gc = system.ILED2;
AMB_DAC_VAL_LED = system.AMB_DAC_LED;
AMB_DAC_SIGN_LED = system.AMB_DAC_SIGN_LED;
AMB_DAC_VAL_AMB = system.AMB_DAC_AMB;
AMB_DAC_SIGN_AMB = system.AMB_DAC_SIGN_AMB;
 
Rf_int = Rfvalues[(int) RFValue - 1];

Meas_DC_10CTR = Meas_DC_1K_1code_10CTR * Rf_int * ILEDCode_gc; //Calculates the DC value assuming that the CTR is 10nA/mA ==>
	 
// Updates the Meas_DC by incorporating the DC cancellation done at the input if any
Meas_DC = ((2 * sign_DC - 1) * Meas_DC) + ((AMB_DAC_SIGN_LED * AMB_DAC_VAL_LED - AMB_DAC_SIGN_AMB * AMB_DAC_VAL_AMB) * Meas_DC_1K_500nAPleth * Rf_int);
	 
if (Meas_DC < 0) 
{
	sign_DC = 0;
	Meas_DC = (-1) * Meas_DC;
}
else
	sign_DC = 1;

CTR = (Meas_DC * 10) / Meas_DC_10CTR;		// Calculates the CTR
ILEDCode_gc = Ipleth << 4; // Estimates the ILED required to achieve a pleth current of 3uA
ILEDCode_gc = ILEDCode_gc / CTR;
ILEDCode_gc = ILEDCode_gc + 0x08;
ILEDCode_gc = ILEDCode_gc >> 4;
   
//ILEDCode_gc = ((Ipleth<<4)/CTR + 0x8)>>4;  // Estimates the ILED required to achieve a pleth current of 3uA
Ipleth_achieved = ILEDCode_gc * CTR;
  ADC_CODE_AMB_DAC_STEP_TMP = ((ADC_CODE_AMB_DAC_STEP_IDEAL << 8)/ADC_CODE_AMB_DAC_STEP);
  AMB_DAC_VAL = ((LED_DC_can * ADC_CODE_AMB_DAC_STEP_TMP  * (Ipleth_achieved << 8)) / Ipleth + 0x8000) >> 16;		// Calculates the DC cancellation required
RFValue = s250K;

// Limits the ILED by max and min values and adjust the DC cancellation appropriately
if (ILEDCode_gc > ILED_CURR_MAX_code) 
{ 
	AMB_DAC_VAL = (AMB_DAC_VAL * ILED_CURR_MAX_code) / ILEDCode_gc;
	ILEDCode_gc = ILED_CURR_MAX_code;// Pick the max ILED if the ILED code required was higher
} 
else 
{
	if (ILEDCode_gc < ILED_CURR_MIN_code) 
	{
		AMB_DAC_VAL = (AMB_DAC_VAL * ILED_CURR_MIN_code) / ILEDCode_gc;
		ILEDCode_gc = ILED_CURR_MIN_code;
	}
}
	 
//  if (ILEDCode2_gc > ILED_CURR_MAX_code) 
//    ILEDCode2_gc = ILED_CURR_MAX_code;// Pick the max ILED if the ILED code required was higher
//  else 
//  {
//    if (ILEDCode2_gc < ILED_CURR_MIN_code) 
//      ILEDCode2_gc = ILED_CURR_MIN_code;
//  }	 
  
  if ((AMB_DAC_VAL == 11) || (AMB_DAC_VAL == 12))
    RFValue = s100K;
  if ((AMB_DAC_VAL == 13) || (AMB_DAC_VAL == 14))
    RFValue = s50K;
  if ((AMB_DAC_VAL == 15) || (AMB_DAC_VAL == 16))
    RFValue = s25K;
  if (AMB_DAC_VAL > 16)
    RFValue = s10K;
  
  if (AMB_DAC_VAL > 10) //AMB DAC range is divided as 8 for LED phase only and 7 for ambient cancellation
    AMB_DAC_VAL = 10;
	 
if (AMB_DAC_VAL > 0)
	DC_can = 1;
else
	DC_can = 0;

// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
if (LED_Sel == 2) 
{
	// shifts the ILED code according to the current field corresponding to the LED number
	LEDInterimCode_gc = (ILEDCode_gc << LED2_reg_shift);
	//updates the ILED register value
	AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode_gc;  
} 
else if (LED_Sel == 3) 
{
	// shifts the ILED code according to the current field corresponding to the LED number
	LEDInterimCode_gc = (ILEDCode_gc << LED3_reg_shift);
	//updates the ILED register value
	AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode_gc;  
} 
else 
{ // Default is LED1
	// shifts the ILED code according to the current field corresponding to the LED number
	LEDInterimCode_gc = (ILEDCode_gc << LED1_reg_shift); 
	//updates the ILED register value
	AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode_gc;  
}

setRfValue(RFValue);
setCfValue(system.Cf);
// Update the TIA GAIN register value	 
AMB_DAC_VAL_LED_SIGN = (sign_DC == 1) ? AMB_DAC_SIGN_AMB * AMB_DAC_VAL_AMB + AMB_DAC_VAL : AMB_DAC_SIGN_AMB * AMB_DAC_VAL_AMB - AMB_DAC_VAL;
if (AMB_DAC_VAL_LED_SIGN >= 0) 
{
	sign_AMB_DAC = 1;
	AMB_DAC_VAL_LED = AMB_DAC_VAL_LED_SIGN;
} 
else 
{
	sign_AMB_DAC = 0;
	AMB_DAC_VAL_LED = -1 * AMB_DAC_VAL_LED_SIGN;
}
AMB_DAC_VALUE_LED_interim = AMB_DAC_VAL_LED | (sign_AMB_DAC << 4);
	 
// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
if (LED_Sel == 2) 
{
	//Interim value to be written to the register
	AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led2); 
	// Updates the AMB_DAC register value
	AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB2_mask_LED)) | AMBInterimCode;  
} 
else if (LED_Sel == 3) 
{
	//Interim value to be written to the register
	AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_amb2); 
	// Updates the AMB_DAC register value
	AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB3_mask_LED)) | AMBInterimCode;  
} 
else 
{ // Default is LED1
	//Interim value to be written to the register
	AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led1);
	// Updates the AMB_DAC register value
	AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB1_mask_LED)) | AMBInterimCode;  
}
	 
//Updates the system parameters
system.RF = RFValue;
system.ILED = ILEDCode_gc;
  //system.ILED2 = ILEDCode2_gc;
system.AMB_DAC_LED = AMB_DAC_VAL_LED;
system.AMB_DAC_SIGN_LED = 2 * sign_AMB_DAC - 1;
}

/*********************************************************************
* @fn      		OFFSET_DAC_Code_Est
*
* @brief		Estimate step size at the ADC output for an increase of 
*                      0.5uA pleth current with Rf = 50K ==> 0.5uA * 100K in codes
*
* @param   		void
*
* @return  		void
*
*********************************************************************/
unsigned char OFFSET_DAC_Code_Est(long AMBVALUE)
{
static short int waitTimeCount = 0;
static short int blockSizeCount = 0;
static long Meas_DC = 0;
 
static int AMB_DAC_VALUE_AMB = 0;
static long AMB_DAC_VALUE_AMB_interim = 0;
static int AMB_DAC_VALUE_SIGN_AMB = 0;
static long AMB_mask = 0x007C00;   // Amb masked
  
switch (OffsetDAC_code_Est_state)
{
	case (sOffsetDACInit):
    AMB_DAC_VALUE_AMB = 0;
    AMB_DAC_VALUE_SIGN_AMB = 0;
    AMB_DAC_VALUE_AMB_interim = AMB_DAC_VALUE_AMB | (AMB_DAC_VALUE_SIGN_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
    AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~AMB_mask) | (AMB_DAC_VALUE_AMB_interim << 10);
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000);            // write mode
    AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]); // 
    AFE4404_Reg_Write(AFE_EXT_CLK_DIV_REG, 0x420);          //CONTROL3 register - Disconnect PD inputs
    AFE4404_Reg_Write(AFE_TIAAMBGAIN, 0x03 | 0x000100);                // TIA gain = 50K                
    AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001);            // read mode
    OffsetDAC_code_Est_state = sOffsetDACWaitTime;
    break;
    
    case (sOffsetDACWaitTime):
    /* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
    waitTimeCount++;
    if (waitTimeCount > TIMEWAIT) 
	{
		waitTimeCount = 0;
		Meas_DC = 0;
		blockSizeCount = 0;
		OffsetDAC_code_Est_state = sOffsetDACWaitforData;// Go to sOffsetDACWaitforData
    }
    break;
    
    case (sOffsetDACWaitforData):
    /* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
	meas_DC_arr[blockSizeCount] = AMBVALUE;
    blockSizeCount++;
    Meas_DC += AMBVALUE;
    if (blockSizeCount == 8) 
    {
		blockSizeCount = 0;
		Meas_DC = (Meas_DC >> 3);
		OffsetDAC_code_Est_state = sOffsetDACCodeEst;                 //sOffsetDACCodeEst
    }
    break;
    
    case (sOffsetDACCodeEst):
    Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB] = Meas_DC;
    if (AMB_DAC_VALUE_AMB != 0)
    {
		Meas_DC_OFFSET_DAC_code_step[AMB_DAC_VALUE_AMB] = Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB] - Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB-1];
		//ADC_CODE_AMB_DAC_STEP = Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB] - Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB-1];
		//ADC_CODE_AMB_DAC_MIN = ADC_CODE_AMB_DAC_STEP >> 1;
    }
    AMB_DAC_VALUE_AMB++;
    if (AMB_DAC_VALUE_AMB == 16) //2
    {
		AMB_DAC_VALUE_AMB = 0;
		OffsetDAC_code_Est_state = sOffsetDACOver;                 	// sOver
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000);            	// write mode
		AFE4404_Reg_Write(AFE_DAC_SETTING_REG, 0); // 
		AFE4404_Reg_Write(AFE_EXT_CLK_DIV_REG, 0x000020);          		// CONTROL3 register
		AFE4404_Reg_Write(AFE_TIAAMBGAIN, 0 | 0x000100);                		// TIA gain = 50K                
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001);            	// read mode
		return 1;
    }
    else
    {
		AMB_DAC_VALUE_AMB_interim = AMB_DAC_VALUE_AMB | (AMB_DAC_VALUE_SIGN_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
		AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~AMB_mask) | (AMB_DAC_VALUE_AMB_interim << 10);
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000000);            	// write mode
		AFE4404_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]); // 
		AFE4404_Reg_Write(AFE_CONTROL0, 0x00000001);            	// read mode
		OffsetDAC_code_Est_state = sOffsetDACWaitTime;            	// sWaitTime1
    }
    break;
    
    case (sOffsetDACOver):
    return 1;
  }
  return 0;
}
