#include <unistd.h>
#include <stdio.h>
#include "synthe.h"
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "save.h"

int main(int argc, char *argv[])
{
    /* pcmSignal mySinus;
    generateSinus(&mySinus, 44100,0.2,220,0);
    csvSaveSignal("la.csv", mySinus);

    pcmSignal env_exp;
    generateExp(&env_exp, mySinus.samplingRate, 0.5);
    csvSaveSignal("exp.csv", env_exp);

    amp(&mySinus, 2.0);
    csvSaveSignal("multiplied.csv", mySinus);

    pcmSignal multiplied_of_2;
    multiply_2signal(&multiplied_of_2, &mySinus, &env_exp);
    csvSaveSignal("multiplied_of_2.csv", multiplied_of_2); */

    pcmSignal diapa;
    diapason(&diapa, 44100, 1.0, 440.0, 29000.0);
    csvSaveSignal("diapason.csv", diapa); 
    wavSaveSignal("diapason.wav", diapa);

    pcmSignal p[13];
    double fond_freq;
    int i;

    /* generate all 13 notes */
    for (i=0; i<13; i++){
        fond_freq = 220.0 * pow(2.0, i/12.0);
        piano(&p[i], 44100, fond_freq, 2500.0);
    } 

    /* combine them */
    for (i=1; i<13; i++){
        addsignal(&p[0], &p[i], 0.5);
    }

    /* convert to wav */
    wavSaveSignal("octave.wav", p[0]);

    return (EXIT_SUCCESS);
}
