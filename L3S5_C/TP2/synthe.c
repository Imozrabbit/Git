#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "synthe.h"
#include "msg.h"

/* ------------------------------------------------------------ */
int generateSinus(
        pcmSignal *signal,               /* pointeur vers structure pcm_signal */
        unsigned int smplRate,      /* frequence d'echantillonnage */
        double duration,            /* duree */
        double frequency,           /* frequence */
        double phiDeg               /* phase en degres */
        )
{
/* variabels */
unsigned int i;
const double PI = 3.14159265359;

signal->nbrSamples = (unsigned int) ceil(duration*smplRate+1);
signal->samplingRate = smplRate;
signal->data = (double*)malloc(sizeof(double)*(signal->nbrSamples));

/* Verify if have enough memory */
if (signal->data == NULL)
{
    fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
    return (SYNTHE_FAILURE);
    }

/* synthese des echantillons */
for (i=0;i<signal->nbrSamples;i++)
{
    signal->data[i] = sin( 2*PI*(frequency/signal->samplingRate)*i + (PI/180)*phiDeg );  
}

return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int generateExp(
        pcmSignal *env_exp,
        unsigned int smplRate,
        double tau
        )
{
    unsigned int i;
    double duration = 15.0 * tau * log(2);

    env_exp->samplingRate = smplRate;
    env_exp->nbrSamples = (unsigned int) ceil(duration * smplRate+1);
    env_exp->data = (double*)malloc( sizeof(double) * (env_exp->nbrSamples) );

    if (env_exp->data == NULL){
        fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
        return (SYNTHE_FAILURE);
    }

    for (i = 0; i < env_exp->nbrSamples; i++) {
        env_exp->data[i] = exp( (-1) * ( i / (smplRate*tau) ) );
    }

    return (SYNTHE_SUCCESS);
    
}


/* ------------------------------------------------------------ */
int generateADSR(
        pcmSignal *env_ADSR,
        unsigned int smplRate,
        double attack_duration,
        double attack_amplitude,
        double decay_duration,
        double sustain_duration,
        double release_duration
        )
{
    env_ADSR->samplingRate = smplRate;
    unsigned int i_a = (unsigned int) ceil(attack_duration * smplRate + 1); /* nomber of samples in attack mode */
    unsigned int i_d= (unsigned int) ceil(decay_duration * smplRate + 1); /* nomber of samples in decay mode */
    unsigned int i_s = (unsigned int) ceil(sustain_duration * smplRate + 1); /* nomber of samples in sustain mode */
    unsigned int i_r = (unsigned int) ceil(release_duration * smplRate + 1); /* nomber of samples in release mode */
    env_ADSR->nbrSamples = i_a + i_d + i_r + i_s; /* total number of samples */

    env_ADSR->data = (double*) malloc( sizeof(double) * (env_ADSR->nbrSamples) );
    if (env_ADSR->data == NULL){
        fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
        return (SYNTHE_FAILURE);
    }

    unsigned int i;
    for (i=0 ; i<env_ADSR->nbrSamples ; i++){
        double t = i/(double)smplRate;
        /* During attack */
        if (i<i_a) {
            /*env_ADSR->data[i] = (attack_amplitude * i) / (i_a - 1.0);*/
            env_ADSR->data[i] = attack_amplitude * (t/attack_duration);
        }

        /* During decay */
        else if ( (i >= i_a) && (i < (i_a + i_d)) ) {
            /*env_ADSR->data[i] = attack_amplitude + (1.0 - attack_amplitude) * (i - i_a) / (i_d - 1.0);*/
            env_ADSR->data[i] = attack_amplitude + (1 - attack_amplitude) * (t - attack_duration)/decay_duration;
        }

        /* During sustain */
        else if ((i >= (i_a + i_d)) && (i < (i_a + i_d + i_s))){
            env_ADSR->data[i] = 1.0;
        }

        /* During release */
        else{
            /*env_ADSR->data[i] = 1.0 * (1 - (i - i_a - i_d - i_s)/(i_r - 1.0) );*/
            env_ADSR->data[i] = 1.0 - (t - (attack_duration + decay_duration + sustain_duration))/release_duration;
        }
    }

    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int free_memory(
    pcmSignal *s
        )
{
    free(s->data);
    s->data = NULL;
    s->nbrSamples = 0;
    s->samplingRate = 0;
    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int amp(
        pcmSignal *s_original,
        double gain
       )
{
    unsigned int i;
    for (i=0 ; i<s_original->nbrSamples ; i++){
        s_original->data[i] = s_original->data[i] * gain;
    }
    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int multiply_2signal(
        pcmSignal *s_final,
        pcmSignal *s_1,
        pcmSignal *s_2
        )
{
    unsigned int i;

    /* Verify if sampling frequency is the same for s_1 and s_2 */
    if (s_1->samplingRate != s_2->samplingRate){
        fprintf(stderr, "Not the same sampling rate for the 2 signals.\n");
        return (SYNTHE_FAILURE);
    }
    s_final->samplingRate = s_1->samplingRate;

    /* pick the least nomber of samples between the 2 signals as the final sample number */
    if (s_1->nbrSamples > s_2->nbrSamples){
       s_final->nbrSamples = s_2->nbrSamples;
    }
    else{
       s_final->nbrSamples = s_1->nbrSamples;
    }

    /* Allocate the memory for s_final.data*/
    s_final->data = (double*) malloc(sizeof(double) * s_final->nbrSamples);
    if (s_final->data == NULL){
        fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
        return (SYNTHE_FAILURE);
    }

    /* do the multiplication of 2 signals */
    for (i=0; i<s_final->nbrSamples; i++){
        s_final->data[i] = s_1->data[i] * s_2->data[i];
    }
    
    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int somme(
        pcmSignal *s_final,
        pcmSignal *s_1,
        pcmSignal *s_2
        )
{
    unsigned int nbrSamples_smaller;
    pcmSignal *s_bigger;

    if (s_1->samplingRate != s_2->samplingRate){
        printf("The 2 signals don't have the same sampling rate.");
        return (SYNTHE_FAILURE);
    }
    s_final->samplingRate = s_1->samplingRate;

    if (s_1->nbrSamples > s_2->nbrSamples){
        s_final->nbrSamples = s_1->nbrSamples;
        nbrSamples_smaller = s_2->nbrSamples;
        s_bigger = s_1;
    }
    else{
        s_final->nbrSamples = s_2->nbrSamples;
        nbrSamples_smaller = s_1->nbrSamples;
         s_bigger = s_2;
    }

    s_final->data = (double*)malloc(sizeof(double)*s_final->nbrSamples);
    if (s_final->data == NULL){
        fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
        return (SYNTHE_FAILURE);
    }

    unsigned int i;
    for (i=0; i<s_final->nbrSamples;i++){
        if (i<nbrSamples_smaller){
            s_final->data[i] = s_1->data[i] + s_2->data[i];
        }
        else{
            s_final->data[i] = s_bigger->data[i];
        }
    }

    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int diapason(
        pcmSignal *final_signal_realistic,
        unsigned int smplRate,
        double tau,
        double frequency,
        double gain
        )
{
    pcmSignal env_exp;
    generateExp(&env_exp, smplRate, tau);

    pcmSignal sinus;
    double duration = 15 * tau * log(2);
    generateSinus(&sinus, smplRate, duration, frequency, 0.0);

    amp(&sinus, gain);

    pcmSignal final_signal;
    multiply_2signal(&final_signal, &sinus, &env_exp);

    pcmSignal env_ADSR;
    generateADSR(&env_ADSR, smplRate, 0.02, 1.1, 0.01, 1.0, 0.1);
    multiply_2signal(final_signal_realistic, &final_signal, &env_ADSR);

    free_memory(&env_exp);
    free_memory(&sinus);
    free_memory(&final_signal);
    free_memory(&env_ADSR);

    return (SYNTHE_SUCCESS);
}



/* ------------------------------------------------------------ */
int piano(
    pcmSignal *piano,
    unsigned int samplingRate,
    double fond_freq,
    double gain
)
{
    pcmSignal harmonic_1;
    pcmSignal harmonic_1_exp;
    pcmSignal note_1;
    pcmSignal harmonic_2;
    pcmSignal harmonic_2_exp;
    pcmSignal note_2; 
    pcmSignal harmonic_3;
    pcmSignal harmonic_3_exp;
    pcmSignal note_3;

    generateSinus(&harmonic_1, samplingRate, 1.0, fond_freq, 0.0);
    generateExp(&harmonic_1_exp, samplingRate, 1.0);
    multiply_2signal(&note_1, &harmonic_1, &harmonic_1_exp);

    generateSinus(&harmonic_2, samplingRate, 1.0, fond_freq*2.0, 0.0);
    generateExp(&harmonic_2_exp, samplingRate, 0.7);
    multiply_2signal(&note_2, &harmonic_2, &harmonic_2_exp);

    generateSinus(&harmonic_3, samplingRate, 1.0, fond_freq*3.0, 0.0);
    generateExp(&harmonic_3_exp, samplingRate, 0.3);
    multiply_2signal(&note_3, &harmonic_3, &harmonic_3_exp);

    pcmSignal sum1;
    pcmSignal sum;
    /* Do not use the same sum for those 2 functions, because the somme
     * function malloc the sum.data, and if use it in both functions, it
     * is going to be weird */
    somme(&sum1, &note_1, &note_2);
    somme(&sum, &sum1, &note_3);

    pcmSignal adsr;
    generateADSR(&adsr, samplingRate, 0.02, 0.6, 0.01, 1.0, 0.1);

    multiply_2signal(piano, &adsr, &sum);
    amp(piano, gain);

    free_memory(&harmonic_1);
    free_memory(&harmonic_1_exp);
    free_memory(&note_1);
    free_memory(&harmonic_2);
    free_memory(&harmonic_2_exp);
    free_memory(&note_2);
    free_memory(&harmonic_3);
    free_memory(&harmonic_3_exp);
    free_memory(&note_3);
    free_memory(&sum);
    free_memory(&adsr);

    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int addsignal(
        pcmSignal *dest,
        pcmSignal *src,
        double decalage
        )
{
    unsigned int i;
    if (dest->data == NULL){
        dest->nbrSamples = src->nbrSamples;
        /* create new memory block for data points */
        dest->data = (double*)malloc(sizeof(double)*dest->nbrSamples); 
        if (dest->data == NULL){
            fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
            return (SYNTHE_FAILURE);
        }
        /* import source data into destination */
        for (i=0; i<dest->nbrSamples; i++){
            dest->data[i] = src->data[i];
        }
    }
    else
    {
        /* calculate the new memory block size considering the delay too */
        unsigned int pause_nbrSamples = (unsigned int)decalage * src->samplingRate;
        unsigned int new_nbrSamples = dest->nbrSamples + src->nbrSamples + pause_nbrSamples;
        /* make the memory block bigger and verify if sucess */
        dest->data = (double*)realloc(dest->data, sizeof(double)*new_nbrSamples);
        if (dest->data == NULL){
            fprintf(stderr, ERROR_NOT_ENOUGH_MEMORY);
            return (SYNTHE_FAILURE);
        }
        /* add the pause */
        for (i=0; i<pause_nbrSamples; i++){
            dest->data[i+dest->nbrSamples] = 0.0;
        }
        /* NOTE: This is how to move the pointer to the beginning of the added block after the pause
                        - double *pdata = dest->data;
                        - pdata += (dest->nbrSamples + pause_nbrSamples);
                        - dest->nbrSamples = new_nbrSamples;

        But it is not needed here*/

        /* write the src data after this */
        for (i=0; i<src->nbrSamples; i++){
            dest->data[i + dest->nbrSamples + pause_nbrSamples] = src->data[i];
        }
        dest->nbrSamples = new_nbrSamples;
    }
    return (SYNTHE_SUCCESS);
}
