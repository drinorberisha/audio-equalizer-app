#include "audio.h"
#include "equalizer.h"
#include "filter_lp.h"
#include "filter_hp.h"
#include "global_values.h"
#include "pre_processing.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define LOW_PASS_CUTOFF_FREQUENCY 1500.0
#define HIGH_PASS_CUTOFF_FREQUENCY 200.0
#define M_PI 3.14159265358979323846
#define SAMPLING_FREQUENCY 52000
#define NUM_CHANNELS 2
#define BITS_PER_SAMPLE 16

bool containsLowFrequencyContent(float *buffer, size_t size);
bool containsHighFrequencyContent(float *buffer, size_t size);

void writeWavHeader(FILE *file, uint32_t sampleRate, uint16_t numChannels, uint16_t bitsPerSample, uint32_t dataSize)
{
    fprintf(file, "RIFF");
    uint32_t fileSize = dataSize + 36;
    fwrite(&fileSize, sizeof(uint32_t), 1, file);

    // Write the format header
    fprintf(file, "WAVEfmt ");
    uint32_t fmtChunkSize = 16; // Size of the fmt subchunk (16 for PCM)
    fwrite(&fmtChunkSize, sizeof(uint32_t), 1, file);
    uint16_t audioFormat = 1; // PCM format
    fwrite(&audioFormat, sizeof(uint16_t), 1, file);
    fwrite(&numChannels, sizeof(uint16_t), 1, file);
    fwrite(&sampleRate, sizeof(uint32_t), 1, file);
    uint32_t byteRate = sampleRate * numChannels * bitsPerSample / 8;
    fwrite(&byteRate, sizeof(uint32_t), 1, file);
    uint16_t blockAlign = numChannels * bitsPerSample / 8;
    fwrite(&blockAlign, sizeof(uint16_t), 1, file);
    fwrite(&bitsPerSample, sizeof(uint16_t), 1, file);

    // Write the data header
    fprintf(file, "data");
    fwrite(&dataSize, sizeof(uint32_t), 1, file);
}

void writeWavData(FILE *file, float *buffer, size_t dataSize)
{
    fwrite(buffer, sizeof(char), dataSize, file);
}

FILE *openAudioFile(const char *filename)
{
    return fopen(filename, "rb");
}

void processAudio(FILE *audioFile)
{
    FILE *outputFile = fopen("output.wav", "wb");
    if (outputFile == NULL)
    {
        printf("Error opening output file\n");
        return;
    }

    writeWavHeader(outputFile, SAMPLING_FREQUENCY, NUM_CHANNELS, BITS_PER_SAMPLE, 0); // dataSize set to 0 initially

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    fseek(audioFile, 44, 0); // Skip 44 bytes for the header

    bytesRead = fread(buffer, 1, BUFFER_SIZE, audioFile);

    float samples[BUFFER_SIZE];
    for (int i = 0; i < bytesRead - 1; i++)
    {
        samples[i] = (buffer[i] + buffer[i + 1]) / 2;
    }
    //   Process audio data (e.g., apply filters)
    removeDCOffset(samples, bytesRead);
    // Apply noise reduction if needed

    applyNoiseReduction(samples, bytesRead);

    double frequencyBins[] = {20.0, 200.0, 2000.0, 20000.0};
    size_t numBins = sizeof(frequencyBins) / sizeof(double);

    applyEqualization(samples, bytesRead, frequencyBins, numBins);
    if (containsLowFrequencyContent(samples, bytesRead) && bassGain > 0.0f)
    {
        float alpha = 1 - exp(-2 * M_PI * LOW_PASS_CUTOFF_FREQUENCY / SAMPLING_FREQUENCY);
        applyLowPassFilter(samples, bytesRead, alpha);
    }

    if (containsHighFrequencyContent(samples, bytesRead) && trebleGain > 0.0f)
    {
        applyHighPassFilter((float *)samples, bytesRead, HIGH_PASS_CUTOFF_FREQUENCY, SAMPLING_FREQUENCY);
    }
    writeWavData(outputFile, samples, bytesRead);

    // Calculate dataSize
    fseek(outputFile, 0, SEEK_END);
    uint32_t dataSize = ftell(outputFile) - 44; // Subtract the size of the header
    fseek(outputFile, 4, SEEK_SET);
    fwrite(&dataSize, sizeof(uint32_t), 1, outputFile);

    // Close the output .wav file
    fclose(outputFile);
}

// Function to determine if the audio buffer contains low-frequency content
bool containsLowFrequencyContent(float *buffer, size_t size)
{
    // Define a threshold for low-frequency content detection
    float threshold = 0.1; // Adjust as needed

    // Loop through the audio buffer and check if any sample exceeds the threshold
    for (size_t i = 0; i < size; i++)
    {
        // Assuming buffer contains audio samples represented as floating point numbers
        if (buffer[i] > threshold)
        {
            return true; // Low-frequency content detected
        }
    }

    return false; // No low-frequency content detected
}

// Function to determine if the audio buffer contains high-frequency content
bool containsHighFrequencyContent(float *buffer, size_t size)
{
    // Define a threshold for high-frequency content detection
    float threshold = 0.9; // Adjust as needed

    // Loop through the audio buffer and check if any sample exceeds the threshold
    for (size_t i = 0; i < size; i++)
    {
        // Assuming buffer contains audio samples represented as floating point numbers
        if (buffer[i] > threshold)
        {
            return true; // High-frequency content detected
        }
    }

    return false; // No high-frequency content detected
}