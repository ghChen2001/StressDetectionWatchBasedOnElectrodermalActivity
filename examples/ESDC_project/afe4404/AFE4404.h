/*
 * AFE4404.h
 *
 * Provides AFE4404 API
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED  
 *
*/

#ifndef _AFE44xx_H_
#define _AFE44xx_H_

#define __AFE4404__

/*----------------------------------------------------------------------------+
| Constant Definition                                                         |
+----------------------------------------------------------------------------*/
#define AFE4404_I2C_DEFAULT_ADDRESS 0x58

#define PRF             100
#define DUTYCYCLE       1
#define AFECLK          (4000000ul)

/*----------------------------------------------------------------------------+
| Bits Definition                                                         |
+----------------------------------------------------------------------------*/
// CONTROL0 - Write Only register
#define    CONTROL0        0x00
#define    CONTROL0_VAL (0x000000ul)
#define    SPI_READ     (0x000001ul)        //SPI read
#define    TM_COUNT_RST (0x000002ul)        //Timer counter reset
#define    DIAG_EN      (0x000004ul)        //Diagnostic enable
#define    SW_RESET     (0x000008ul)        //Software reset

// CONTROL1 - Read/Write register
#define CONTROL1        0x1E
#define    CONTROL1_VAL                         (TIMEREN + NUMAV_2)
#define    TIMEREN                              (0x000100ul)            //Timer enable
#define    NUMAV_0                              (0x000000ul)            //Number of averages-1 (4 bits)
#define    NUMAV_1                              (0x000001ul)            //Number of averages-1 (4 bits)
#define    NUMAV_2                              (0x000002ul)            //Number of averages-1 (4 bits)
#define    NUMAV_3                              (0x000003ul)            //Number of averages-1 (4 bits)
#define    NUMAV_4                              (0x000004ul)            //Number of averages-1 (4 bits)
#define    NUMAV_5                              (0x000005ul)            //Number of averages-1 (4 bits)
#define    NUMAV_6                              (0x000006ul)            //Number of averages-1 (4 bits)
#define    NUMAV_7                              (0x000007ul)            //Number of averages-1 (4 bits)
#define    NUMAV_8                              (0x000008ul)            //Number of averages-1 (4 bits)
#define    NUMAV_9                              (0x000009ul)            //Number of averages-1 (4 bits)
#define    NUMAV_10                             (0x00000Aul)            //Number of averages-1 (4 bits)
#define    NUMAV_11                             (0x00000Bul)            //Number of averages-1 (4 bits)
#define    NUMAV_12                             (0x00000Cul)            //Number of averages-1 (4 bits)
#define    NUMAV_13                             (0x00000Dul)            //Number of averages-1 (4 bits)
#define    NUMAV_14                             (0x00000Eul)            //Number of averages-1 (4 bits)
#define    NUMAV_15                             (0x00000Ful)            //Number of averages-1 (4 bits)

// TIA GAIN SEPARATE
#define TIA_GAIN_SEP         0x20
#define    TIA_GAIN_VAL                  (0x000000ul)
#define    TIA_GAIN_SEP_500K            (0x000000ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_250K            (0x000001ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_100K            (0x000002ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_50K             (0x000003ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_25K             (0x000004ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_10K             (0x000005ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_1M              (0x000006ul)        //Program RF for LED2/LED3
#define    TIA_GAIN_SEP_2M              (0x000007ul)        //Program RF for LED2/LED3

#define    TIA_CF_SEP_5P                (0x000000ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_2_5P              (0x000008ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_10P               (0x000010ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_7_5P              (0x000018ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_20P               (0x000020ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_17_5P             (0x000028ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_25P               (0x000030ul)        //Program CF for LED2/LED3
#define    TIA_CF_SEP_22_5P             (0x000038ul)        //Program CF for LED2/LED3

#define    ENSEPGAIN                    (0x008000ul)        //Enable separate gains

// TIA GAIN
#define TIA_GAIN            0x21
#define    TIA_GAIN_VAL                 (0x000000ul)
#define    TIA_GAIN_500K                (0x000000ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_250K                (0x000001ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_100K                (0x000002ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_50K                 (0x000003ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_25K                 (0x000004ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_10K                 (0x000005ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_1M                  (0x000006ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB
#define    TIA_GAIN_2M                  (0x000007ul)        //When ENSEPGAIN=0,Program RF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program RF for LED1/AMB

#define    TIA_CF_5P                    (0x000000ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_2_5P                  (0x000008ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_10P                   (0x000010ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_7_5P                  (0x000018ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_20P                   (0x000020ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_17_5P                 (0x000028ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_25P                   (0x000030ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB
#define    TIA_CF_22_5P                 (0x000038ul)        //When ENSEPGAIN=0,Program CF for LED1, LED2, LED3 and AMB. When ENSEPGAIN=1, Program CF for LED1/AMB

#define    PROG_TG_EN                   (0x000100ul)        //Replaces ADC_RDY output with a fully programmable signal from the timing engine

#define LEDCNTRL        0x22
#define    LEDCNTRL_VAL                (0x0030CFul)

#define CONTROL2        0x23
#define CONTROL2_VAL                    (0x124018ul)
#define PDN_AFE_DISABLED                (0x000000ul)        //Normal mode
#define PDN_AFE_ENABLED                 (0x000001ul)        //AFE power-down

#define PDN_RX_DISABLED                 (0x000000ul)        //Normal mode
#define PDN_RX_ENABLED                  (0x000002ul)        //Rx power-down

#define DYNAMIC1                        (0x000008ul)        // Dynamic1
#define DYNAMIC2                        (0x000010ul)        // Dynamic2
#define DYNAMIC3                        (0x004000ul)        // Dynamic3
#define DYNAMIC4                        (0x100000ul)        // Dynamic4

#define OSC_DISABLE                     (0x000000ul)        //Internal Osc is disabled
#define OSC_ENABLE                      (0x000200ul)        //Internal Osc is enabled

#define ILED_2X_DISABLED                (0x000000ul)        //LED current range is 0-50 mA
#define ILED_2X_ENABLED                 (0x020000ul)        //LED current range is 0-100 mA

#define CLKDIV1         0x29
#define    CLKDIV_CLKOUT_0              (0x000000ul)        //Clock output division ratio 1 - Output Clk freq = 4MHz
#define    CLKDIV_CLKOUT_1              (0x000001ul)        //Clock output division ratio 2 - Output Clk freq = 2MHz
#define    CLKDIV_CLKOUT_2              (0x000002ul)        //Clock output division ratio 4 - Output Clk freq = 1MHz
#define    CLKDIV_CLKOUT_3              (0x000003ul)        //Clock output division ratio 8 - Output Clk freq = 0.5MHz
#define    CLKDIV_CLKOUT_4              (0x000004ul)        //Clock output division ratio 16 - Output Clk freq = 0.25MHz
#define    CLKDIV_CLKOUT_5              (0x000005ul)        //Clock output division ratio 32 - Output Clk freq = 0.125MHz
#define    CLKDIV_CLKOUT_6              (0x000006ul)        //Clock output division ratio 64 - Output Clk freq = 0.0625MHz
#define    CLKDIV_CLKOUT_7              (0x000007ul)        //Clock output division ratio 128 - Output Clk freq = 0.03125MHz

#define    DISABLE_CLKOUT               (0x000200ul)        //CLKOUT DISABLED
#define    ENABLE_CLKOUT                (0x000200ul)        //CLKOUT ENABLED

// Read only registers
#define LED2VAL         0x2A
#define LED3VAL         0x2B
#define LED1VAL         0x2C
#define ALED1VAL        0x2D
#define LED2_ALED2VAL   0x2E
#define LED1_ALED1VAL   0x2F
#define DIAG            0x30
// End of Read only registers

#define CONTROL3        0x31
#define         CLKDIV_EXTMODE_0        (0x000000ul)            // Div by 2
#define         CLKDIV_EXTMODE_1        (0x000001ul)            // Div by 8
#define         CLKDIV_EXTMODE_2        (0x000000ul)            // Do not use - set to default 'div by 2'
#define         CLKDIV_EXTMODE_3        (0x000003ul)            // Div by 12
#define         CLKDIV_EXTMODE_4        (0x000004ul)            // Div by 4
#define         CLKDIV_EXTMODE_5        (0x000005ul)            // Div by 1
#define         CLKDIV_EXTMODE_6        (0x000005ul)            // Div by 6
#define         CLKDIV_EXTMODE_7        (0x000000ul)            // Do not use - set to default 'div by 2'

#define         DISABLE_INPUT_SHORT     (0x000000ul)            // INP/INN not shorted to VCM whenever the TIA is powerdown
#define         ENABLE_INPUT_SHORT      (0x000020ul)            // INP/INN shorted to VCM whenever the TIA is powerdown
#define         PD_DISCONNECT_DISABLE   (0x000000ul)            // PD_DISCONNECT_DISABLE
#define         PD_DISCONNECT_ENABLE    (0x000400ul)            // PD_DISCONNECT_ENABLE

#define CLKDIV2         0x39
#define CLKDIV_PRF_RATIO                1
#define CLKDIV_PRF_0                    (0x000000ul)            // Division Ratio = 1; Freq of TG = 4MHz; Lowest PRF setting 61Hz
#define CLKDIV_PRF_1                    (0x000000ul)            // Do not use - set to default div ratio = 1
#define CLKDIV_PRF_2                    (0x000000ul)            // Do not use - set to default div ratio = 1
#define CLKDIV_PRF_3                    (0x000000ul)            // Do not use - set to default div ratio = 1
#define CLKDIV_PRF_4                    (0x000004ul)            // Division Ratio = 2; Freq of TG = 2MHz; Lowest PRF setting 31Hz
#define CLKDIV_PRF_5                    (0x000005ul)            // Division Ratio = 4; Freq of TG = 1MHz; Lowest PRF setting 15Hz
#define CLKDIV_PRF_6                    (0x000006ul)            // Division Ratio = 8; Freq of TG = 0.5MHz; Lowest PRF setting 8Hz (limit to 10Hz)
#define CLKDIV_PRF_7                    (0x000007ul)            // Division Ratio = 16; Freq of TG = 0.25MHz; Lowest PRF setting 4Hz (limit to 10Hz)

#define AMBDAC          0x3A

// Timing register constants

#define PRPCOUNT        0x1D
#define PRP             ((AFECLK/(PRF*CLKDIV_PRF_RATIO)-1)                // for 100HZ - 39999 

#define DELTA                   (((PRP+1)*DUTYCYCLE)/100)       
#define CONV_DELTA              ((LED2LEDENDC_VAL>((((NUMAV+2)*50*4)+60)/CLKDIV_PRF_RATIO))?(LED2LEDENDC_VAL):((((NUMAV+2)*50*4)+60)/CLKDIV_PRF_RATIO))
#define SAMPLINGSTART_DELAY     (((80/CLKDIV_PRF_RATIO)<(2*(LED2LEDENDC_VAL-LED2LEDSTC_VAL+1)/10))?(80/CLKDIV_PRF_RATIO):(2*(LED2LEDENDC_VAL-LED2LEDSTC_VAL+1)/10))
#define ADCRESET_DELAY          ((6/CLKDIV_PRF_RATIO>1)?(6/CLKDIV_PRF_RATIO>1):1)

// Timing registers
#define LED2LEDSTC      0x09
#define LED2LEDSTC_VAL  0

#define LED2LEDENDC     0x0A
#define LED2LEDENDC_VAL ((DUTYCYCLE*AFECLK)/(100*CLKDIV_PRF_RATIO*PRF)-1)

#define LED2STC         0x01
#define LED2STC_VAL     (LED2LEDSTC_VAL+SAMPLINGSTART_DELAY)

#define LED2ENDC        0x02
#define LED2ENDC_VAL    LED2LEDENDC_VAL

#define ADCRSTSTCT0     0x15
#define ADCRSTSTCT0_VAL (LED2ENDC_VAL+2)

#define ADCRSTENDCT0    0x16
#define ADCRSTENDCT0_VAL (ADCRSTSTCT0_VAL+ADCRESET_DELAY)

#define LED2CONVST      0x0D
#define LED2CONVST_VAL  (ADCRSTENDCT0_VAL+1)

#define LED2CONVEND     0x0E
#define LED2CONVEND_VAL (LED2CONVST_VAL+CONV_DELTA-1)


#define LED3LEDSTC      0x36
#define LED3LEDSTC_VAL  (LED2ENDC_VAL+1)

#define LED3LEDENDC     0x37
#define LED3LEDENDC_VAL (LED3LEDSTC_VAL+LED2LEDENDC_VAL)

#define ALED2STC        0x05
#define ALED2STC_VAL    (LED3LEDSTC_VAL+SAMPLINGSTART_DELAY)

#define ALED2ENDC       0x06
#define ALED2ENDC_VAL   LED3LEDENDC_VAL

#define ADCRSTSTCT1     0x17
#define ADCRSTSTCT1_VAL (LED2CONVEND_VAL+2)

#define ADCRSTENDCT1    0x18
#define ADCRSTENDCT1_VAL (ADCRSTSTCT1_VAL+ADCRESET_DELAY)

#define ALED2CONVST     0x0F
#define ALED2CONVST_VAL (ADCRSTENDCT1_VAL+1)

#define ALED2CONVEND    0x10
#define ALED2CONVEND_VAL (ALED2CONVST_VAL+CONV_DELTA-1)


#define LED1LEDSTC      0x03
#define LED1LEDSTC_VAL  (ALED2ENDC_VAL+1)

#define LED1LEDENDC     0x04
#define LED1LEDENDC_VAL (LED1LEDSTC_VAL+LED2LEDENDC_VAL)

#define LED1STC         0x07
#define LED1STC_VAL     (LED1LEDSTC_VAL+SAMPLINGSTART_DELAY)

#define LED1ENDC        0x08
#define LED1ENDC_VAL    (LED1LEDENDC_VAL)

#define ADCRSTSTCT2     0x19
#define ADCRSTSTCT2_VAL (ALED2CONVEND_VAL+2)

#define ADCRSTENDCT2    0x1A
#define ADCRSTENDCT2_VAL (ADCRSTSTCT2_VAL+ADCRESET_DELAY)

#define LED1CONVST      0x11
#define LED1CONVST_VAL  (ADCRSTENDCT2_VAL+1)

#define LED1CONVEND     0x12
#define LED1CONVEND_VAL (LED1CONVST_VAL+CONV_DELTA-1)


#define ALED1STC        0x0B
#define ALED1STC_VAL    (LED1ENDC_VAL+SAMPLINGSTART_DELAY)

#define ALED1ENDC       0x0C
#define ALED1ENDC_VAL   (ALED1STC_VAL+(LED1ENDC_VAL-LED1STC_VAL))

#define ADCRSTSTCT3     0x1B
#define ADCRSTSTCT3_VAL (LED1CONVEND_VAL+2)

#define ADCRSTENDCT3    0x1C
#define ADCRSTENDCT3_VAL (ADCRSTSTCT3_VAL+ADCRESET_DELAY)

#define ALED1CONVST     0x13
#define ALED1CONVST_VAL (ADCRSTENDCT3_VAL+1)

#define ALED1CONVEND    0x14
#define ALED1CONVEND_VAL (ALED1CONVST_VAL+CONV_DELTA-1)

#define PDNCYCLESTC     0x32
#define PDNCYCLESTC_VAL  (ALED1CONVEND_VAL+(200*AFECLK)/(CLKDIV_PRF_RATIO*1000000))       

#define PDNCYCLEENDC    0x33
#define PDNCYCLEENDC_VAL (PRP-(200*AFECLK)/(CLKDIV_PRF_RATIO*1000000))

#define PROG_TG_STC     0x34
#define PROG_TG_STC_VAL (0x000000ul)

#define PROG_TG_ENDC     0x35
#define PROG_TG_ENDC_VAL (0x000000ul)




#define AFE_CONTROL0           0x0     
#define AFE_LED2STC            0x1      //01
#define AFE_LED2ENDC           0x2      //02      
#define AFE_LED1LEDSTC         0x3      //03
#define AFE_LED1LEDENDC        0x4      //04
#define AFE_ALED2STC           0x5      //05
#define AFE_ALED2ENDC          0x6      //06
#define AFE_LED1STC            0x7      //07
#define AFE_LED1ENDC           0x8      //08
#define AFE_LED2LEDSTC         0x9      //09
#define AFE_LED2LEDENDC        0xA      //0A
#define AFE_ALED1STC           0xB      //0B
#define AFE_ALED1ENDC          0xC      //0C
#define AFE_LED2CONVST         0xD      //0D
#define AFE_LED2CONVEND        0xE      //0E
#define AFE_ALED2CONVST        0xF      //0F
#define AFE_ALED2CONVEND       0x10     //10
#define AFE_LED1CONVST         0x11     //11
#define AFE_LED1CONVEND        0x12     //12
#define AFE_ALED1CONVST        0x13     //13
#define AFE_ALED1CONVEND       0x14     //14
#define AFE_ADCRSTSTCT0        0x15     //15
#define AFE_ADCRSTENDCT0       0x16     //16
#define AFE_ADCRSTSTCT1        0x17     //17
#define AFE_ADCRSTENDCT1       0x18     //18
#define AFE_ADCRSTSTCT2        0x19     //19
#define AFE_ADCRSTENDCT2       0x1A     //1A
#define AFE_ADCRSTSTCT3        0x1B     //1B
#define AFE_ADCRSTENDCT3       0x1C     //1C
#define AFE_PRPCOUNT           0x1D     //1D
#define AFE_CONTROL1           0x1E     //1E
#define AFE_SPARE1             0x1F     //1F
#define AFE_TIAGAIN            0x20     //20
#define AFE_TIAAMBGAIN         0x21     //21
#define AFE_LEDCNTRL           0x22     //22
#define AFE_CONTROL2           0x23     //23
#define AFE_SPARE2             0x24     //24
#define AFE_SPARE3             0x25     //25
#define AFE_SPARE4             0x26     //26
#define AFE_RESERVED1          0x27     //27
#define AFE_RESERVED2          0x28     //28
#define AFE_ALARM              0x29     //29
#define AFE_LED2VAL            0x2A     //2A
#define AFE_ALED2VAL           0x2B     //2B
#define AFE_LED1VAL            0x2C     //2C
#define AFE_ALED1VAL           0x2D     //2D
#define AFE_LED2ALED2VAL       0x2E     //2E
#define AFE_LED1ALED1VAL       0x2F     //2F
#define AFE_RESERVED           0x30     //30
#define AFE_EXT_CLK_DIV_REG	   0x31		//31
#define AFE_PDNCYCLESTC        0x32     //32
#define AFE_PDNCYCLEENDC       0x33     //33
#define AFE_PROG_TG_STC        0x34     //34
#define AFE_PROG_TG_ENDC       0x35     //35
#define AFE_LED3LEDSTC         0x36     //36
#define AFE_LED3LEDENDC        0x37     //37
#define AFE_RESERVED3          0x38     //38
#define AFE_CLK_DIV_REG        0x39     //39
#define AFE_DAC_SETTING_REG    0x3A     //3A
#define AFE_DECIMATION		   0x3D		//3D
#define AFE_AVG_LED2_ALED2VAL  0x3F		//3F
#define AFE_AVG_LED1_ALED1VAL  0x40		//40

/****************************************************************/
/* Global functions												*/
/****************************************************************/
void AFE4404_Init(void);
void AFE4404_RESETZ_Init (void);
void AFE4404_Enable_HWPDN (void);
void AFE4404_Disable_HWPDN (void);
void AFE4404_Trigger_HWReset (void);
void AFE4404_ADCRDY_Interrupt_Init (void);
void AFE4404_ADCRDY_Interrupt_Enable (void);
void AFE4404_ADCRDY_Interrupt_Disable (void);
void AFE4404_Reg_Init(void);

//unsigned char AFE4404_Diagnostics_Check (void);

void AFE4404_Reg_Write(unsigned char reg_address, unsigned long reg_data);
signed long AFE4404_Reg_Read(unsigned char reg_address);
void AFE4404_Enable_Read (void);
void AFE4404_Disable_Read (void);


#endif /*_AFE44xx_H_*/
