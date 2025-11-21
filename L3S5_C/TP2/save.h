#include <stdint.h>
#define SAVE_FAILURE (1)
#define SAVE_SUCCESS 0
#define WAV_EXTENSION ".wav"

/* create a struc containing all the necessary info for wav format */
    struct wav_heading{
        /* First Section */
        char riff[4];
        unsigned int chunk_size;
        char wave[4];
        /* Second Section */
        char fmt[4];
        int size_format;
        uint16_t encodage;
        uint16_t canal;
        unsigned int frequency;
        unsigned int speed;
        uint16_t package_size;
        uint16_t bit;
        /* Third Section */
        char data[4];
        unsigned int data_size;
    };

/* sauvegarde au format CSV */
int csvSaveSignal(
        const char *filename,
        pcmSignal s
);

int wavSaveSignal(
        const char *filename,
        pcmSignal s
        );
