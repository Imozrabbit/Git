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

    generateSinus(&mySinus, 44100,0.2,220,0);
    csvSaveSignal("la.csv", mySinus);
    generateExp(&env_exp, mySinus.samplingRate, 0.5);
    csvSaveSignal("exp.csv", env_exp);

    amp(&mySinus, 2.0);
    csvSaveSignal("multiplied.csv", mySinus);

    pcmSignal multiplied_of_2;
    multiply_2signal(&multiplied_of_2, &mySinus, &env_exp);
    csvSaveSignal("multiplied_of_2", multiplied_of_2);

    pcmSignal final_signal_diapason;
    diaposon(&final_signal_diapason, 44100, 1.0, 440.0, 29000.0);
    csvSaveSignal("final.csv", final_signal_diapason);

    return (EXIT_SUCCESS);
}
