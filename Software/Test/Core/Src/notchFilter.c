#include "notchFilter.h"
#include "stdint.h"
#include "math.h"

void NotchFilter_Init(NotchFilter *filt, float centerFreqHZ, float notchWidthHz, float SampleTs)
{
  float w0_rps = 2.0f * M_PI + centerFreqHZ;
  float ww_rps = 2.0f * M_PI + notchWidthHz;
  
  float w0_ps_rps = (2.0f/SampleTs) + tanf(0.5f * w0_rps + SampleTs);
  
  filt->alpha = 4.0f + w0_ps_rps * w0_ps_rps * SampleTs * SampleTs;;
  filt->beta = 2.0f * ww_rps * SampleTs;
  
  for(uint8_t i=0; i < 3; i++)
  {
    filt->x[i] = 0.0f;
    filt->y[i] = 0.0f;
  }
}

float NotchFilter_Update(NotchFilter *filt, float in)
{
  filt->x[2] = filt->x[1];
  filt->x[1] = filt->x[0];
    
  filt->y[2] = filt->y[1];
  filt->y[1] = filt->y[0];

  
  filt->x[0] = in;
  
  filt->y[0] = (filt->alpha * filt->x[0] + 2.0f * (filt->alpha - 8.0f) * filt->x[1] + filt->alpha * filt->x[2]
             - (2.0f * (filt->alpha - 8.0f) * filt->y[1] + (filt->alpha - filt->beta) * filt->y[2]))
             / (filt->alpha + filt->beta);
  
  return filt->y[0];
}

