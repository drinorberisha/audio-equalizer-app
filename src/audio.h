
#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>

#define BUFFER_SIZE 48000
#define AUDIO_FILE "audio.wav"

// Function prototypes
FILE *openAudioFile(const char *filename);
void processAudio(FILE *audioFile);

#endif