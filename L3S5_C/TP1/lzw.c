#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lzw.h"

int main(int argc, char *argv[])
{
/*Variables*/
int i;
char input_filename[256];
char ouput_filename[256];

/*Afficher le nom de l'exécutable*/
printf("Executable path and name: %s\n", argv[0]);

/*Afficher les paramètres*/
for (i=1;i<=argc-1;i++){
    printf(AFFICHAGE_PARAMETRES,i,argv[i]);
}

/*Si on a plus de 2 paramètres ou pas de paramètre*/
if ((argc==1) || (argc>3)){
    fprintf(stderr,SYNTAX_ERROR);
    return(EXIT_FAILURE);
}

/*Mémoriser le premier paramètre*/
strcpy(input_filename, argv[1]);

/*Mémoriser le deuxième paramètre*/
if (argc == 2){
    /*S'il n'exist pas un deuxième paramètre, on prend le nom du premier paramètre*/
    strcpy(ouput_filename, argv[1]);
    char *c = strrchr(ouput_filename, '.');
    if (c == NULL){
        /*Cas de sans extension*/
        strcat(ouput_filename, EXTENSION);
    }
    else{
        /*Cas avec extension, on le remplace par la bonne*/
        strcpy(c, EXTENSION);
    }
}
else{
    /*S'il y a un deuxième paramètre, on teste son extension*/
    strcpy(ouput_filename, argv[2]);
    char *c = strrchr(ouput_filename, '.');
    if (c == NULL){
        /*Cas de sans extension*/
        strcat(ouput_filename, EXTENSION);
    }
    else{
        /*Cas avec extension, on le remplace par la bonne*/
        strcpy(c, EXTENSION);
    }
}

/*Afficher l'entrée et la sortie*/
printf(AFFICHAGE_ENTREE_SORTIE, input_filename, ouput_filename);

/*Fin de programme*/
return(EXIT_SUCCESS);
}