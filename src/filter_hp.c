#include "filter_hp.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void applyHighPassFilter(float *signal, size_t length, float cutoffFrequency, float samplingRate)
{
    float alpha = 1.0f / (1.0f + 2.0f * M_PI * cutoffFrequency / samplingRate);
    float prevSample = 0.0f;
    for (size_t i = 0; i < length; i++)
    {
        float currentSample = signal[i];
        signal[i] = alpha * (currentSample - prevSample);
        prevSample = currentSample;
    }
}