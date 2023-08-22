/*
 * AFE4404.c
 *
 * Provides AFE4404 API
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED  
 *
*/

#include "AFE4404.h"

/*******************************************************************************/
/*	        Prototypes									                       */
/*******************************************************************************/

/*******************************************************************************/
/*	        Global Variables										       	   */
/*******************************************************************************/

/**********************************************************************************************************/
/*	External Variables																			          */
/**********************************************************************************************************/

/**********************************************************************************************************/
/*	        AFE4404 Registers Initialization          				                  					  		  */
/**********************************************************************************************************/

void AFE4404_Reg_Init(void)
{
  // AFE4404_Disable_Read();
  AFE4404_Disable_Read();
  AFE4404_Reg_Write(1, 100);    //AFE_LED2STC          TE3
  AFE4404_Reg_Write(2, 399);    //AFE_LED2ENDC        TE4
  AFE4404_Reg_Write(3, 802);    //AFE_LED1LEDSTC
  AFE4404_Reg_Write(4, 1201);   //AFE_LED1LEDENDC
  AFE4404_Reg_Write(5, 501);    //AFE_ALED2STC
  AFE4404_Reg_Write(6, 800);    //AFE_ALED2ENDC
  AFE4404_Reg_Write(7, 902);    //AFE_LED1STC
  AFE4404_Reg_Write(8, 1201);   //AFE_LED1ENDC
  AFE4404_Reg_Write(9, 0);      //AFE_LED2LEDSTC      TE1
  AFE4404_Reg_Write(10, 399);   //AFE_LED2LEDENDC     TE2 
  AFE4404_Reg_Write(11, 1303);  //AFE_ALED1STC        
  AFE4404_Reg_Write(12, 1602);  //AFE_ALED1ENDC   
  AFE4404_Reg_Write(13, 409);   //AFE_LED2CONVST  
  AFE4404_Reg_Write(14, 1468);  //AFE_LED2CONVEND
  AFE4404_Reg_Write(15, 1478);  //AFE_ALED2CONVST
  AFE4404_Reg_Write(16, 2537);  //AFE_ALED2CONVEND  NACK
  // AFE4404_Reg_Write(16, 0x001700);  //AFE_ALED2CONVEND
  AFE4404_Reg_Write(17, 2547);  //AFE_LED1CONVST    NACK
  AFE4404_Reg_Write(18, 3606);  //AFE_LED1CONVEND   NACK
  AFE4404_Reg_Write(19, 3616);  //AFE_ALED1CONVST   NACK
  AFE4404_Reg_Write(20, 4675);  //AFE_ALED1CONVEND
  AFE4404_Reg_Write(21, 401);   //AFE_ADCRSTSTCT0
  AFE4404_Reg_Write(22, 407);   //AFE_ADCRSTENDCT0
  AFE4404_Reg_Write(23, 1470);  //AFE_ADCRSTSTCT1
  AFE4404_Reg_Write(24, 1476);  //AFE_ADCRSTENDCT1
  AFE4404_Reg_Write(25, 2539);  //AFE_ADCRSTSTCT2   NACK
  AFE4404_Reg_Write(26, 2545);  //AFE_ADCRSTENDCT2  NACK
  AFE4404_Reg_Write(27, 3608);  //AFE_ADCRSTSTCT3   NACK
  AFE4404_Reg_Write(28, 3614);  //AFE_ADCRSTENDCT3  NACK
  AFE4404_Reg_Write(54, 401);   //AFE_LED3LEDSTC
  AFE4404_Reg_Write(55, 800);   //AFE_LED3LEDENDC
  AFE4404_Reg_Write(29, 39999); //AFE_PRPCOUNT      NACK
  AFE4404_Reg_Write(30, 0x000103);	//AFE_CONTROL1 TimerEN = 1; NUMAV = 3
  AFE4404_Reg_Write(32, 0x008003);  //AFE_TIA_SEP_GAIN (LED2) ENSEPGAIN = 1; LED2/LED3 gain = 50K
  AFE4404_Reg_Write(33, 0x000103);  //AFE_TIA_GAIN (LED1) LED1/LED1AMB gain = 50K
  AFE4404_Reg_Write(58, 0x000000);  //AFE_DAC_SETTING_REG
  AFE4404_Reg_Write(34, 0x0030CF); 	//LED3 - 3.125mA; LED2 - 3.125mA; LED1 - 12.5mA
  // AFE4404_Reg_Write(35, 0x124018); 	//DYN1, LEDCurr, DYN2, Ext CLK, DYN3, DYN4 //0x000200); - 0x200 Osc mode //AFE_CONTROL2
  AFE4404_Reg_Write(35, 0x124218);  //DYN1, LEDCurr, DYN2, Ext CLK, DYN3, DYN4 //0x000200); - 0x200 Osc mode //AFE_CONTROL2
  AFE4404_Reg_Write(49, 0x000020); 	//ENABLE_INPUT_SHORT
  AFE4404_Reg_Write(57, 0);     	//CLKDIV_PRF
  AFE4404_Reg_Write(50, 5475);  	//AFE_DPD1STC
  AFE4404_Reg_Write(51, 39199); 	//AFE_DPD1ENDC  NACK
  AFE4404_Reg_Write(52, 0x000FFF);
  AFE4404_Reg_Write(53, 0x003FFF);
  AFE4404_Enable_Read();
}

/**********************************************************************************************************/
/*	        AFE4404_Init         				                                          				  */
/**********************************************************************************************************/
void AFE4404_Init(void)
{
  // AFE4404_RESETZ_Init ();
  AFE4404_Enable_HWPDN ();
  AFE4404_Disable_HWPDN ();
  AFE4404_Trigger_HWReset ();
  // AFE4404_ADCRDY_Interrupt_Init(); // Included in AFE4404_PinInit
  AFE4404_Reg_Init();
}

// /**********************************************************************************************************/
// /*	        AFE4404_RESETZ_Init         				                                          		  */
// /**********************************************************************************************************/
// void AFE4404_RESETZ_Init (void)
// {
//   SET_AFE_RESETZ_AS_OUTPUT();
//   SET_AFE_RESETZ_PIN_HIGH();
// }


// /**********************************************************************************************************/
// /*	        AFE4404_Enable_HWPDN         				                                          		  */
// /**********************************************************************************************************/
// void AFE4404_Enable_HWPDN (void)
// {
//   SET_AFE_RESETZ_PIN_LOW();
//   __delay_cycles(160000);        // ~10ms delay with 16MHz clock
// }

// /**********************************************************************************************************/
// /*	        AFE4404_Disable_HWPDN         				                                          		  */
// /**********************************************************************************************************/
// void AFE4404_Disable_HWPDN (void)
// {
//   SET_AFE_RESETZ_PIN_HIGH();
//   __delay_cycles(160000);        // ~10ms delay with 16MHz clock
// }

// /**********************************************************************************************************/
// /*	        AFE4404_Trigger_HWReset         				                                              */
// /**********************************************************************************************************/
// void AFE4404_Trigger_HWReset (void)
// {
//   SET_AFE_RESETZ_PIN_LOW();
//   __delay_cycles(480);           // ~30us delay
//   SET_AFE_RESETZ_PIN_HIGH();
//   __delay_cycles(160000);        // ~10ms delay with 16MHz clock
// }


/**********************************************************************************************************/
/*	        AFE4404_Diagnostics_Check         				                                              */
/**********************************************************************************************************/
/*unsigned char AFE4404_Diagnostics_Check (void)
{
  AFE4404_Disable_Read();
  AFE4404_Reg_Write(0x23,0x000200);     // Enable internal oscillator
  AFE4404_Reg_Write(0x1E,0x000100);     // TIMER_EN
  AFE4404_Reg_Write(0,4);               // DIAG_EN
  __delay_cycles(160000);       // ~10ms delay
  AFE4404_Enable_Read();
  return ((unsigned char)AFE4404_Reg_Read(48));	//DIAG status
}*/

// /*********************************************************************************************************/
// /* AFE4404_Reg_Write																	                 */
// /*********************************************************************************************************/
// void AFE4404_Reg_Write (unsigned char reg_address, unsigned long data)
// {
//   unsigned char configData[3];
//   configData[0]=(unsigned char)(data >>16);
//   configData[1]=(unsigned char)(((data & 0x00FFFF) >>8));
//   configData[2]=(unsigned char)(((data & 0x0000FF)));
//   I2C_write(AFE4404_I2C_DEFAULT_ADDRESS, reg_address, configData, 3);
// }

// /**********************************************************************************************************/
// /* AFE4404_Reg_Read           					  			  											  */
// /**********************************************************************************************************/
// signed long AFE4404_Reg_Read(unsigned char Reg_address)
// {
//   unsigned char configData[3];
//   signed long retVal;
//   I2C_read (AFE4404_I2C_DEFAULT_ADDRESS, Reg_address, configData, 3);
//   retVal = configData[0];
//   retVal = (retVal << 8) | configData[1];
//   retVal = (retVal << 8) | configData[2];
//   if (Reg_address >= 0x2A && Reg_address <= 0x2F)
//   {
// 	if (retVal & 0x00200000) 	// check if the ADC value is positive or negative
// 	{
// 	  retVal &= 0x003FFFFF;		// convert it to a 22 bit value
// 	  return (retVal^0xFFC00000);
// 	}
//   }
//   return retVal;
// }

// /**********************************************************************************************************/
// /*	        AFE4404_Enable_Read        	                  					  							  */
// /**********************************************************************************************************/
// void AFE4404_Enable_Read (void)
// {
//   unsigned char configData[3];
//   configData[0]=0x00;
//   configData[1]=0x00;
//   configData[2]=0x01;
//   I2C_write (AFE4404_I2C_DEFAULT_ADDRESS, CONTROL0, configData, 3);
// }

// /**********************************************************************************************************/
// /*	        AFE4404_Disable_Read       	                  					  							  */
// /**********************************************************************************************************/
// void AFE4404_Disable_Read (void)
// {
//   unsigned char configData[3];
//   configData[0]=0x00;
//   configData[1]=0x00;
//   configData[2]=0x00;
//   I2C_write (AFE4404_I2C_DEFAULT_ADDRESS, CONTROL0, configData, 3);
// }

/**********************************************************************************************************/
/* AFE4404_ADCRDY_Interrupt_Init												                          */
/**********************************************************************************************************/
// void AFE4404_ADCRDY_Interrupt_Init (void)
// {
  
//   SET_AFE_ADC_RDY_AS_INPUT();
//   //ENABLE_MSP_INT_PU_RES_ON_ADC_RDY_PIN();
//   //SET_MSP_INT_PU_RES_ON_ADC_RDY_PIN();
//   SEL_H2L_EDGE_ON_ADC_RDY_PIN();
//   AFE4404_ADCRDY_Interrupt_Disable();
// }

// /**********************************************************************************************************/
// /* AFE4404_ADCRDY_Interrupt_Enable												                          */
// /**********************************************************************************************************/
// void AFE4404_ADCRDY_Interrupt_Enable (void)
// {
//   CLEAR_INT_FLAG_ON_ADC_RDY_PIN();      //P2IFG &= ~AFE_ADC_DRDY;                           	// P2.3 IFG cleared
//   ENABLE_INT_ON_ADC_RDY_PIN();          //P2IE |= AFE_ADC_DRDY;                             	// P2.3 interrupt enabled
// }

// /**********************************************************************************************************/
// /* AFE4404_ADCRDY_Interrupt_Disable												                          */
// /**********************************************************************************************************/
// void AFE4404_ADCRDY_Interrupt_Disable (void)
// {
//   CLEAR_INT_FLAG_ON_ADC_RDY_PIN();      //P2IFG &= ~AFE_ADC_DRDY;                           	// P2.3 IFG cleared
//   DISABLE_INT_ON_ADC_RDY_PIN();         //P2IE &= ~AFE_ADC_DRDY;                             	// P2.3 interrupt disabled
// }
// End of file
