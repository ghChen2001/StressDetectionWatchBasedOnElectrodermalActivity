#include "count_steps.h"
#include "stdint.h"
#include "stdio.h" //using this for printing debug outputs
#include "math.h"
#include "csi_math.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "task.h"

//this algorithm is a simple adaptation of the following paper:
//"RecoFit - Using a Wearable Sensor to Find, Recognize, and Count Repetitive Exercises"

extern SemaphoreHandle_t xMutex_DSP;

#define NUM_AUTOCORR_LAGS               50  //number of lags to calculate for autocorrelation. 50 lags @20Hz corresponds to a step rate of 0.4Hz...its probably not physically possible to walk much slower than this
#define DERIV_FILT_LEN                  16  //length of derivative filter
#define LPF_FILT_LEN                    11  //length of FIR low pass filter
#define AUTOCORR_DELTA_AMPLITUDE_THRESH 5e6 //this is the min delta between peak and trough of autocorrelation peak
#define AUTOCORR_MIN_HALF_LEN           3   //this is the min number of points the autocorrelation peak should be on either side of the peak

static float deriv_coeffs[DERIV_FILT_LEN] = { -0.0210216563084191,
                                              0.0110337575188735,
                                              0.0653981513804192,
                                              -0.0666709164356842,
                                              -0.0702280778328434,
                                              0.0790477166009553,
                                              0.259505953190275,
                                              0.145012119300142,
                                              -0.145012119300142,
                                              -0.259505953190275,
                                              -0.0790477166009553,
                                              0.0702280778328434,
                                              0.0666709164356842,
                                              -0.0653981513804192,
                                              -0.0110337575188735,
                                              0.0210216563084191 };                                                                                                                                                                                                                       //coefficients of derivative filter from https://www.dsprelated.com/showarticle/814.php
static float32_t lpf_coeffs[LPF_FILT_LEN] = { 0.00756605569291192f, 0.0164918471480748f, -0.0535007814332861f, -0.0408358076273419f, 0.295934725740374f, 0.553482511046935f, 0.295934725740374f, -0.0408358076273419f, -0.0535007814332861f, 0.0164918471480748f, 0.00756605569291192f }; //coefficients of FIR low pass filter generated in matlab using FDATOOL
static float32_t state_fir[LPF_FILT_LEN + NUM_TUPLES - 1];
static float32_t state_fir_deriv[DERIV_FILT_LEN + NUM_AUTOCORR_LAGS - 1];
static float mag_sqrt[NUM_TUPLES] = { 0 };              //this holds the square root of magnitude data of X,Y,Z (so its length is NUM_SAMPLES/3)
static float lpf[NUM_TUPLES] = { 0 };                   //hold the low pass filtered signal
static float autocorr_buff[199] = { 0 };                //holds the autocorrelation results
static float deriv[NUM_AUTOCORR_LAGS] = { 0 };
static float autocorr_buff2[NUM_AUTOCORR_LAGS] = { 0 }; //holds the autocorrelation results
static uint8_t isInit = 0;
static uint8_t isInitDeriv = 0;                         //holds derivative
static csi_fir_instance_f32 S;
static csi_fir_instance_f32 S_deriv;

// static void derivative(float *autocorr_buff, float *deriv);
static void autocorr(float *lpf, float *autocorr_buff);
static void remove_mean(float *lpf);
static void lowpassfilt(float *mag_sqrt, float *lpf);
// static uint8_t get_precise_peakind(float *autocorr_buff, uint8_t peak_ind);
// static void get_autocorr_peak_stats(float *autocorr_buff, uint8_t *neg_slope_count, float *delta_amplitude_right, uint8_t *pos_slope_count, float *delta_amplitude_left, uint8_t peak_ind);

// //take a look at the original autocorrelation signal at index i and see if
// //its a real peak or if its just a fake "noisy" peak corresponding to
// //non-walking. basically just count the number of points of the
// //autocorrelation peak to the right and left of the peak. this function gets
// //the number of points to the right and left of the peak, as well as the delta amplitude
// static void get_autocorr_peak_stats(float *autocorr_buff, uint8_t *neg_slope_count, float *delta_amplitude_right, uint8_t *pos_slope_count, float *delta_amplitude_left, uint8_t peak_ind)
// {
//     //first look to the right of the peak. walk forward until the slope begins decreasing
//     uint8_t neg_slope_ind = peak_ind;
//     uint16_t loop_limit = NUM_AUTOCORR_LAGS - 1;
//     while ((autocorr_buff[neg_slope_ind + 1] - autocorr_buff[neg_slope_ind] < 0) && (neg_slope_ind < loop_limit)) {
//         *neg_slope_count = *neg_slope_count + 1;
//         neg_slope_ind = neg_slope_ind + 1;
//     }

//     //get the delta amplitude between peak and right trough
//     *delta_amplitude_right = autocorr_buff[peak_ind] - autocorr_buff[neg_slope_ind];

//     //next look to the left of the peak. walk backward until the slope begins increasing
//     uint8_t pos_slope_ind = peak_ind;
//     loop_limit = 0;
//     while ((autocorr_buff[pos_slope_ind] - autocorr_buff[pos_slope_ind - 1] > 0) && (pos_slope_ind > loop_limit)) {
//         *pos_slope_count = *pos_slope_count + 1;
//         pos_slope_ind = pos_slope_ind - 1;
//     }

//     //get the delta amplitude between the peak and the left trough
//     *delta_amplitude_left = autocorr_buff[peak_ind] - autocorr_buff[pos_slope_ind];
// }

// //use the original autocorrelation signal to hone in on the
// //exact peak index. this corresponds to the point where the points to the
// //left and right are less than the current point
// static uint8_t get_precise_peakind(float *autocorr_buff, uint8_t peak_ind)
// {
//     uint8_t loop_limit = 0;
//     if ((autocorr_buff[peak_ind] > autocorr_buff[peak_ind - 1]) && (autocorr_buff[peak_ind] > autocorr_buff[peak_ind + 1])) {
//         //peak_ind is perfectly set at the peak. nothing to do
//     } else if ((autocorr_buff[peak_ind] > autocorr_buff[peak_ind + 1]) && (autocorr_buff[peak_ind] < autocorr_buff[peak_ind - 1])) {
//         //peak is to the left. keep moving in that direction
//         loop_limit = 0;
//         while ((autocorr_buff[peak_ind] > autocorr_buff[peak_ind + 1]) && (autocorr_buff[peak_ind] < autocorr_buff[peak_ind - 1]) && (loop_limit < 10)) {
//             peak_ind = peak_ind - 1;
//             loop_limit++;
//         }
//     } else {
//         //peak is to the right. keep moving in that direction
//         loop_limit = 0;
//         while ((autocorr_buff[peak_ind] > autocorr_buff[peak_ind - 1]) && (autocorr_buff[peak_ind] < autocorr_buff[peak_ind + 1]) && (loop_limit < 10)) {
//             peak_ind = peak_ind + 1;
//             loop_limit++;
//         }
//     }
//     return peak_ind;
// }

// //calculate deriviative via FIR filter
// static void derivative(float *autocorr_buff, float *deriv)
// {
//     if (!isInitDeriv) {
//         csi_fir_init_f32(&S_deriv, DERIV_FILT_LEN, deriv_coeffs, state_fir_deriv, NUM_AUTOCORR_LAGS);
//         isInitDeriv = 1;
//     }
//     csi_fir_f32(&S_deriv, autocorr_buff, deriv, NUM_AUTOCORR_LAGS);
//     for (int i = 0; i < NUM_AUTOCORR_LAGS; i++) {
//         printf("deriv=%.3f, ", deriv[i]);
//     }
//     printf("\r\n");
// }

//autocorrelation function
//this takes a lot of computation. there are efficient implementations, but this is more intuitive
static void autocorr(float *lpf, float *autocorr_buff)
{
    xSemaphoreTake(xMutex_DSP, portMAX_DELAY);
    csi_correlate_f32(lpf, NUM_TUPLES, lpf, NUM_TUPLES, autocorr_buff);
    xSemaphoreGive(xMutex_DSP);
}

//calculate and remove the mean
static void remove_mean(float *lpf)
{
    float sum = 0.f;
    uint16_t i;
    for (i = 0; i < NUM_TUPLES; i++) {
        sum += lpf[i];
    }
    sum = sum / (NUM_TUPLES);

    for (i = 0; i < NUM_TUPLES; i++) {
        lpf[i] = lpf[i] - sum;
    }
    // for (int i = 0; i < NUM_TUPLES; i++) {
    //     printf("remove_mean acc=%.3f, ", lpf[i]);
    // }
    // printf("\r\n");
}

//FIR low pass filter
static void lowpassfilt(float *mag_sqrt, float *lpf)
{
    if (!isInit) {
        csi_fir_init_f32(&S, LPF_FILT_LEN, lpf_coeffs, state_fir, NUM_TUPLES);
        isInit = 1;
    }
    xSemaphoreTake(xMutex_DSP, portMAX_DELAY);
    csi_fir_f32(&S, mag_sqrt, lpf, NUM_TUPLES);
    xSemaphoreGive(xMutex_DSP);
    // for (int i = 0; i < NUM_TUPLES; i++) {
    //     printf("filtered acc=%.3f, ", lpf[i]);
    // }
    // printf("\r\n");
}

//algorithm interface
uint8_t count_steps(float *data)
{
    //assume data is in the format data = [x1,y1,z1,x2,y2,z2...etc]
    //calculate the magnitude of each of triplet ie temp_mag = [x1^2+y1^2+z1^2]
    //then temp_mag = sqrt(temp_mag)
    uint16_t i;
    float temp_mag;
    // printf("sqrtf\r\n");
    for (i = 0; i < NUM_TUPLES; i++) {
        temp_mag = (data[i * 3 + 0] * data[i * 3 + 0] + data[i * 3 + 1] * data[i * 3 + 1] + data[i * 3 + 2] * data[i * 3 + 2]);
        mag_sqrt[i] = sqrtf(temp_mag);
        // printf("mag_sqrt=%.3f, ", mag_sqrt[i]);
    }
    // printf("\r\n");

    //apply low pass filter to mag_sqrt, result is stored in lpf
    lowpassfilt(mag_sqrt, lpf);
    // printf("lowpassfilt\r\n");

    //remove mean from lpf, store result in lpf
    remove_mean(lpf);
    // printf("remove_mean\r\n");

    //do the autocorrelation on the lpf buffer, store the result in autocorr_buff
    autocorr(lpf, autocorr_buff);
    // printf("autocorr\r\n");

    uint8_t peak_ind = 0;
    for (int i = 0; i < NUM_AUTOCORR_LAGS; i++) {
        autocorr_buff2[i] = autocorr_buff[100 + i];
        // printf("autocorr_buff=%.3f, ", autocorr_buff2[i]);
    }
    // printf("\r\n");

    float max = -1e9;
    // max frequency = 2Hz, min frequency = 0.5Hz
    for (int i = 12; i < NUM_AUTOCORR_LAGS; i++) {
        if (autocorr_buff2[i] > max && autocorr_buff2[i] > 0){
            max = autocorr_buff2[i];
            peak_ind = i;
        }
    }

    // printf("peak ind: %i\n", peak_ind);

    uint8_t num_steps = 0;
    
    if(max > 2e6)
    {
        //the period is peak_ind/sampling_rate seconds. that corresponds to a frequency of 1/period
        //with the frequency known, and the number of seconds is 4 seconds, you can then find out the number of steps
        num_steps = (SAMPLING_RATE * WINDOW_LENGTH) / peak_ind;
    }
    else
    {
        //not a valid autocorrelation peak
        num_steps = 0;
    }

    return num_steps;
}
