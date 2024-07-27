#ifndef NOTCH_FILTER_H
#define NOTCH_FILTER_H

#include "math.h"
#include "stdint.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

typedef struct
{  
  float alpha;
  float beta;
  
  float x[3];  
  float y[3];
} NotchFilter;

void NotchFilter_Init(NotchFilter *filt, float centerFreqHZ, float notchWidthHz, float SampleTs);
float NotchFilter_Update(NotchFilter *filt, float in);

#endif