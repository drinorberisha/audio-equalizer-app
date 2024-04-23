#include "pre_processing.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void removeDCOffset(float *signal, size_t length)
{
    float mean = 0.0f;
    for (size_t i = 0; i < length; i++)
    {
        mean += signal[i];
    }
    mean /= length;

    for (size_t i = 0; i < length; i++)
    {
        signal[i] -= mean;
    }
    printf("Removed DC offset\n");
}

void spectralSubtraction(float *signal, size_t length, const float *noiseProfile)
{
    for (size_t i = 0; i < length; i++)
    {
        signal[i] -= noiseProfile[i];
        if (signal[i] < 0.0f)
        {
            signal[i] = 0.0f;
        }
    }
    printf("Performed spectral subtraction\n");
}

// Function to apply noise reduction using a simple moving average filter
void applyNoiseReduction(float *audioData, size_t dataSize)
{
    // Define the window size for the moving average filter
    const int windowSize = 5; // Adjust as needed

    // Temporary buffer to store filtered audio data
    float *filteredData = (float *)malloc(dataSize * sizeof(float));
    if (filteredData == NULL)
    {
        // Error handling: Unable to allocate memory
        return;
    }

    // Apply the moving average filter
    for (size_t i = 0; i < dataSize; i++)
    {
        float sum = 0.0f;
        int count = 0;

        // Compute the average of neighboring samples within the window
        for (int j = -windowSize / 2; j <= windowSize / 2; j++)
        {
            int index = i + j;
            if (index >= 0 && index < dataSize)
            {
                sum += audioData[index];
                count++;
            }
        }

        // Compute the average value and store it in the filtered data buffer
        filteredData[i] = sum / count;
    }

    // Copy the filtered data back to the original buffer
    for (size_t i = 0; i < dataSize; i++)
    {
        audioData[i] = filteredData[i];
    }

    // Free the temporary buffer
    free(filteredData);
    printf("Applied Noise Reduction\n");
}
