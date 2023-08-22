#ifndef __MAX30101_ALGO_H_
#define __MAX30101_ALGO_H_

#include <stdio.h>

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
                int16_t *ir_ac_comp, int16_t *red_ac_comp, int16_t *green_ac_comp, int16_t *ir_ac_mag, int16_t *red_ac_mag, int16_t *green_ac_mag, uint16_t *HRbpm2, uint16_t *SpO2B, uint16_t *DRdy);

//helper functions for the heart rate and SpO2 function
uint16_t avg_dc_est(int32_t *p, uint16_t x);

void lp_dfir_flt(int16_t din0, int16_t din1, int16_t din2, int16_t *dout0, int16_t *dout1, int16_t *dout2);
int32_t mul16(int16_t x, int16_t y);

/* Op Sensor FIFO nearly full callback */
volatile bool op_sensorIntFlag = 0;
void op_sensor_callback()
{
    op_sensorIntFlag = 1;
}

#endif