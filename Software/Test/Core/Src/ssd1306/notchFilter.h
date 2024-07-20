#ifndef NOTCH_FILTER_H
#define NOTCH_FILTER_H

#include "math.h"

typedef struct {
  
  float alpha;
  float beta;
  
  float x[3];
  
  float y[3];

} NotchFilter;

void NotchFilter_Init(NotchFilter *filt, float centerFreqHZ, float notchWidthHz, float SampleTs);
flaot NotchFilter_Update(NotchFilter *filt, float in);

#endif