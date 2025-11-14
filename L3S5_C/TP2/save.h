#define SAVE_FAILURE (1)
#define SAVE_SUCCESS 0

/* sauvegarde au format CSV */
int csvSaveSignal(
        const char *filename,
        pcmSignal s
);
