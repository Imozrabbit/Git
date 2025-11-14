#include <signal.h>
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
signal->data = (double*)malloc(sizeof(double)*(signal->nbrSamples));/*  WARN: need to understand what is (double*) doing here  */

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
        /* During attack */
        if (i<i_a) {
            env_ADSR->data[i] = (attack_amplitude * i) / (i_a - 1.0);
        }

        /* During decay */
        else if ( (i >= i_a) && (i < (i_a + i_d)) ) {
            env_ADSR->data[i] = attack_amplitude + (1.0 - attack_amplitude) * (i - i_a) / (i_d - 1.0);
        }

        /* During sustain */
        else if ((i >= (i_a + i_d)) && (i < (i_a + i_d + i_s))){
            env_ADSR->data[i] = 1.0;
        }

        /* During release */
        else{
            env_ADSR->data[i] = 1.0 * (1 - (i - i_a - i_d - i_s)/(i_r - 1.0) );
        }
    }

    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int free_memory(
        double *data,
        unsigned int nbrSamples
        )
{
    free(data);
    nbrSamples = 0;
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

    /* pick the least nomber of samples between the 2 signals as the final sample number */
    if (s_1->nbrSamples > s_2->nbrSamples){
       s_final->nbrSamples = s_2->nbrSamples;
    }
    else{
       s_final->nbrSamples = s_1->nbrSamples;
    }

    /* do the multiplication of 2 signals */
    for (i=0; i<s_final->nbrSamples; i++){
        s_final->data[i] = s_1->data[i] * s_2->data[i];
    }
    
    return (SYNTHE_SUCCESS);
}


/* ------------------------------------------------------------ */
int diaposon(
        pcmSignal *final_signal_realistic,
        unsigned int smplRate,
        double tau,
        double frequency,
        double gain
        )
{
    pcmSignal *env_exp;
    generateExp(env_exp, smplRate, 1.0);

    pcmSignal *sinus;
    double duration = 15 * tau * log(2);
    generateSinus(sinus, smplRate, duration, 440.0, 0.0);

    amp(sinus, 29000.0);

    pcmSignal *final_signal;
    multiply_2signal(final_signal, sinus, env_exp);

    pcmSignal *env_ADSR;
    generateADSR(env_ADSR, smplRate, 0.02, 1.1, 0.01, 1.0, 0.1);
    multiply_2signal(final_signal_realistic, final_signal, env_ADSR);

    free_memory(env_exp->data, env_exp->nbrSamples);
    free_memory(sinus->data, sinus->nbrSamples);
    free_memory(final_signal->data, final_signal->nbrSamples);

    return (SYNTHE_SUCCESS);
}
