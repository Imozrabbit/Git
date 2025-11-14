#include <stdio.h>
#include "synthe.h"
#include <stdlib.h>
#include "msg.h"
#include "save.h"

int main(int argc, char *argv[])
{
    /* variable */
    pcmSignal mySinus;
    pcmSignal env_exp; /* Enveloppe exponentielle pour le signal */
    pcmSignal env_ADSR; /* Enveloppe ADSR pour le signal */
    pcmSignal s_diapason;

    generateSinus(&mySinus, 44100,0.2,220,0);
    csvSaveSignal("la.csv", mySinus);
    generateExp(&env_exp, mySinus.samplingRate, 0.5);
    csvSaveSignal("exp.csv", env_exp);
    /*generateADSR(&env_ADSR, mySinus.samplingRate, 2.0, 1.5, 0.5, 1.0, 0.3);
    csvSaveSignal("ADSR.csv", env_ADSR); */
    amp(&mySinus, 2.0);
    /* free_memory(mySinus.data, mySinus.nbrSamples);
    free_memory(env_exp.data, env_exp.nbrSamples);
    free_memory(env_ADSR.data, env_ADSR.nbrSamples); */

    diaposon(&s_diapason, 44100, 1.0, 440.0, 2900.0);
    csvSaveSignal("diaposon.csv", s_diapason);

    /*free_memory(s_diapason.data, s_diapason.nbrSamples);*/

    return (EXIT_SUCCESS);
}
