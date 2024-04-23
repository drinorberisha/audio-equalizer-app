#include "main.h"
#include "audio.h"
#include "ui.h"
#include "global_values.h"
#include <stdio.h>
#include <stdlib.h>

// Function to handle user input and adjust gain values
void adjustGainValues()
{
    // Prompt user for input and update gain values
    printf("Enter new bass gain: ");
    scanf("%f", &bassGain);
    printf("Enter new midrange gain: ");
    scanf("%f", &midrangeGain);
    printf("Enter new treble gain: ");
    scanf("%f", &trebleGain);
}

int main()
{
    // Intialize any necessary components
    displayProgramRunningMessage();

    // Main loop for audio processing
    while (1)
    {
        // Open audio file for reading
        FILE *audioFile = openAudioFile(AUDIO_FILE);
        if (audioFile == NULL)
        {
            printf("Error opening audio file\n");
            return 1;
        }

        // Check for user input to adjust gain values
        int choice;
        printf("Options:\n");
        printf("1. Adjust Gain Values\n");
        printf("2. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Process user input
        switch (choice)
        {
        case 1:
            adjustGainValues();
            break;
        case 2:
            return 0; // Quit the program
        default:
            printf("Invalid choice. Please try again.\n");
        }

        // Read and process audio data with current gain values
        processAudio(audioFile);

        // Close audio file
        fclose(audioFile);
    }

    return 0;
}
