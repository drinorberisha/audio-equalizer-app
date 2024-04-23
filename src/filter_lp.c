#include "filter_lp.h"

void applyLowPassFilter(char *buffer, size_t size, float alpha)
{
    // intialize previous output to 0
    static float prevOutput = 0.0f;
    // In a real application, implement the filter algorithm here
    for (size_t i = 0; i < size; i++)
    {
        // Filter algorithm
        float currentOutput = (1 - alpha) * buffer[i] + alpha * prevOutput;
        prevOutput = currentOutput;
        buffer[i] = currentOutput;
    }
}