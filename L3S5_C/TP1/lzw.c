#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lzw.h"

int main(int argc, string argv[])
{
/*Variables*/
int i;
char input_filename[256];
char output_filename[256];
char dictionnaire_filename[256];

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
    strcpy(output_filename, argv[1]);
    string c = strrchr(output_filename, '.');
    if (c == NULL){
        /*Cas de sans extension*/
        strcat(output_filename, EXTENSION);
    }
    else{
        /*Cas avec extension, on le remplace par la bonne*/
        strcpy(c, EXTENSION);
    }
}
else{
    /*S'il y a un deuxième paramètre, on teste son extension*/
    strcpy(output_filename, argv[2]);
    string c = strrchr(output_filename, '.');
    if (c == NULL){
        /*Cas de sans extension*/
        strcat(output_filename, EXTENSION);
    }
    else{
        /*Cas avec extension, on le remplace par la bonne*/
        strcpy(c, EXTENSION);
    }
}

/*Afficher l'entrée et la sortie*/
printf(AFFICHAGE_ENTREE_SORTIE, input_filename, output_filename);

/*Ouvrir le fichier d'entrée*/
FILE *f_e = fopen(argv[1], "rb");
if (f_e == NULL){
    /*Si le fichier n'existe pas*/
    fprintf(stderr,AFFICHAGE_ERROR_OUVERTURE_FICHIER, argv[1]);
    fclose(f_e);
    return(EXIT_FAILURE);
}
else{
    fclose(f_e);
}

/*Réinitialiser le fichier de sortie*/
FILE *f_s = fopen(output_filename,"r");
if (f_s != NULL){
    /*Si le fichier existe, on demande l'utilisateur ce qu'on fait ensuite*/
    fclose(f_s);
    while (1){
        printf(MESSAGE_ECRASER);
        int ch = fgetc(stdin); /*fgetc outputs an interger*/
        ch = toupper((unsigned char)ch); /*toupper also outputs an interger*/
        Reponse r = (char)ch;
        if (r == 'Y'){
            /*Cas où on écrase le fichier et crée un nouveau*/
            FILE *f_s = fopen(output_filename,"wb");
            printf("ecrase");
            fclose(f_s);
            break;
        }
        else if (r == 'N'){
            /*cas où on fait rien et quitte*/
            return(EXIT_FAILURE);
            break;
        }
        else{
            /*cas où l'utilisateur tape n'importe quoi*/
            printf("It is a yes or no question, Y for yes and n for no, choose accordingly.\n");
    }
    }
}
else{
    /*Si le fichier n'existe pas, on le crée*/
    FILE *f_s = fopen(output_filename,"wb");
    fclose(f_s);
}

/*Créer un dictionnaire en mode écriture*/
strcpy(dictionnaire_filename, output_filename);
string d = strrchr(dictionnaire_filename, '.');
strcpy(d,".CSV");
FILE *dic = fopen(dictionnaire_filename, "w");
fclose(dic);

/*Vérifier le fichier est de ASCII*/



/*Fin de programme*/
return(EXIT_SUCCESS);
}