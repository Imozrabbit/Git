#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "msg.h"
#include "synthe.h"
#include "save.h"

/* sauvegarde au format CSV */
int csvSaveSignal(
    const char *filename,
    pcmSignal s
)
{
    /* variables */
    unsigned int i;
    FILE *fic;

    /* ouvrir le fichier */
    fic = fopen(filename,"w");
    if (fic == NULL)
    {
        fprintf(stderr, ERROR_FILE_OPEN, filename); return (SAVE_FAILURE);
    }
    
    /* Capture the data in the csv file*/
    for (i=0; i< s.nbrSamples;i++)
    {
        fprintf(fic, "%lf, %lf\n",i/((double)s.samplingRate), s.data[i]);
    }
    fclose(fic);
    return (SAVE_SUCCESS);
}


/* ------------------------------------------------------------------------- */
/* sauvegarde au format wav*/
int wavSaveSignal(
        const char *filename,
        pcmSignal s
        )
{
    /* create the heading */
    struct wav_heading wav_heading;

    strncpy(wav_heading.riff, "RIFF",4);
    strncpy(wav_heading.wave, "WAVE",4);
    strncpy(wav_heading.fmt, "fmt ",4);
    strncpy(wav_heading.data, "data",4);

    wav_heading.size_format = 16;
    wav_heading.encodage = 1;
    wav_heading.canal = 1;
    wav_heading.frequency = 44100;
    wav_heading.bit = 16;
    wav_heading.speed = wav_heading.frequency * wav_heading.canal * wav_heading.bit/8;
    wav_heading.package_size = wav_heading.bit/8 * wav_heading.canal;
    wav_heading.data_size = s.nbrSamples * wav_heading.canal * wav_heading.bit/8;
    wav_heading.chunk_size = wav_heading.data_size + 36;

    /* create the output file */
    FILE *fic;
    fic = fopen(filename, "wb");

    /* Verify if can open the file */
    if (fic == NULL){
        fprintf(stderr, ERROR_FILE_OPEN, filename);
        return (SAVE_FAILURE);
    }

    /* write in heading */
    fwrite(&wav_heading, sizeof(wav_heading), 1, fic);

    /* write in each sample */
    unsigned int i;
    for (i=0 ; i<s.nbrSamples; i++)
    {
        /* HACK: force the sample inside of the double range */
        double x = s.data[i];
        if (x>32767.0){
            x = 32767.0;
        }
        if (x<-32768.0){
            x = -32768.0;
        }
        int16_t y = (int16_t) x; /* typecast converting double to 2byte_int */
        fwrite(&y, sizeof(y), 1, fic);
    }

    /* close the file */
    fclose(fic);
    return (SAVE_SUCCESS);
    }
