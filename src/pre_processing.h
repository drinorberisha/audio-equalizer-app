
#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <stddef.h>
#include <stdlib.h>

void removeDCOffset(float *signal, size_t length);
void spectralSubtraction(float *signal, size_t length, const float *noiseProfile);
void applyNoiseReduction(float *audioData, size_t dataSize);

#endif