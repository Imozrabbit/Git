#include <stdio.h>
#include <stdlib.h>
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
        fprintf(stderr, ERROR_FILE_OPEN, filename);
        return (SAVE_FAILURE);
    }
    
    /* Capture the data in the csv file*/
    for (i=0; i< s.nbrSamples;i++)
    {
        fprintf(fic, "%lf, %lf\n",i/((double)s.samplingRate), s.data[i]);
    }
    fclose(fic);
    return (SAVE_SUCCESS);
}
