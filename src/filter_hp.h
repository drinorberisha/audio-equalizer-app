#include <corecrt.h>

#ifndef FILTER_HP_H
#define FILTER_HP_H

void applyHighPassFilter(float *signal, size_t length, float cutoffFrequency, float samplingRate);

#endif