#ifndef count_steps_h
#define count_steps_h
#include "stdint.h"

#define SAMPLING_RATE           25                       //25 hz sampling rate
#define NUM_TUPLES              100                      //100 sets of accelerometer readings (so in other words, 100*3 = 300 samples)
#define WINDOW_LENGTH           NUM_TUPLES/SAMPLING_RATE //window length in seconds

uint8_t count_steps(float *data);

#endif /* count_steps_h */