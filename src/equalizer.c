#include "equalizer.h"
#include <fftw3.h>
#include "global_values.h"

int isBassFrequency(double frequency);
int isMidrangeFrequency(double frequency);
int isTrebleFrequency(double frequency);

void applyEqualization(char *buffer, size_t size, double *frequencyBins, size_t numBins)
{

    fftw_complex *in, *out;
    fftw_plan plan;

    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);

    for (size_t i = 0; i < size; i++)
    {
        // real part
        in[i][0] = buffer[i];
        // imaginary part
        in[i][1] = 0;
    }

    plan = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // loop through each sample in the audio buffer
    for (size_t i = 0; i < numBins; i++)
    {
        if (i < size)
        {
            double binFrequency = frequencyBins[i];

            if (isBassFrequency(binFrequency))
            {
                // Apply gain adjustment for bass frequencies
                out[i][0] *= bassGain;
                out[i][1] *= bassGain;
            }
            else if (isMidrangeFrequency(binFrequency))
            {
                // Apply gain adjustment for midrange frequencies
                out[i][0] *= midrangeGain;
                out[i][1] *= midrangeGain;
            }
            else if (isTrebleFrequency(binFrequency))
            {
                // Apply gain adjustment for treble frequencies
                out[i][0] *= trebleGain;
                out[i][1] *= trebleGain;
            }
            else
            {
                printf("Warning: Frequency bin %zu has an unexpected value: %.2f\n", i, binFrequency);
            }
        }
        else
        {
            printf("Error: Index %zu exceeds the size of the FFT output (%zu)\n", i, size);
        }
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    printf("I am done with equalization");
}

int isBassFrequency(double frequency)
{
    return (frequency >= 20 && frequency < 200);
}

int isMidrangeFrequency(double frequency)
{
    return (frequency >= 200 && frequency < 2000);
}

int isTrebleFrequency(double frequency)
{
    return (frequency >= 2000 && frequency < 20000);
}
