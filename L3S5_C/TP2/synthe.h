#define SYNTHE_SUCCESS 0
#define SYNTHE_FAILURE (1)

typedef struct
{
    unsigned int samplingRate; /*frequence échantillonnage*/
    unsigned int nbrSamples; /*nombre d'échantillons*/
    double *data; /*pointeur vers échantillons*/
}pcmSignal;

/* Prototypes des fonctions */
int generateSinus(
    pcmSignal *signal,           /*pointeur vers structure pcm_signal*/
    unsigned int smplRate,       /*fréquence d'échantillonnage*/
    double duration,             /*durée*/
    double frequency,            /*fréquence*/
    double phiDeg                /*phase en degrés*/
    );

/* Calculates the duration based on tau, then the number of samples will be calculated and logged into env_exp.nbrSamples.
 * Then capture sample rate into env_exp.samplingRate
 * At last log every exponential value into env_exp.data */
int generateExp(
        pcmSignal *env_exp,
        unsigned int smplRate,
        double tau
        );

int generateADSR(
        pcmSignal *env_ADSR,
        unsigned int smplRate,
        double attack_duration,
        double attack_amplitude,
        double decay_duration,
        double sustain_duration, /* sustain level is 1 */
        double release_duration
        );

int free_memory(
        pcmSignal *s
        );

int amp(
        pcmSignal *s_original,
        double gain
       );

int multiply_2signal(
        pcmSignal *s_final,
        pcmSignal *s_1,
        pcmSignal *s_2
        );

int somme(
        pcmSignal *s_final,
        pcmSignal *s_1,
        pcmSignal *s_2
        );

int diapason(
        pcmSignal *final_signal_realistic,
        unsigned int smplRate,
        double tau,
        double frequency,
        double gain
        );

int piano(
        pcmSignal *piano,
        unsigned int samplingRate,
        double fond_freq,
        double gain
        );

int addsignal(
        pcmSignal *dest,
        pcmSignal *src,
        double decalage
        );
