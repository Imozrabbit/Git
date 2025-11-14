#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lzw.h"

int main(int argc, string argv[]){
/*Variables*/
int i;
int j;
int n;
int int_reponse;
int code_ASCII;
int fenetre_count;
int counter;
int max;
int fenetre[2];
unsigned int count[128][128];
char input_filename[256];
char output_filename[256];
char dictionnaire_filename[256];
int dico_nb[128][1];
char dico[128][2];
Reponse reponse;
string extension_output;
string extension_dictionnaire;
FILE *input_file;
FILE *output_file;
FILE *dictionnaire;

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
if(argc == 2){
    /*S'il n'exist pas un deuxième paramètre, on prend le nom du premier paramètre*/
    strcpy(output_filename, argv[1]);
    extension_output = strrchr(output_filename, '.');
    if (extension_output == NULL){
        /*Cas de sans extension*/
        strcat(output_filename, EXTENSION);
    }
    else{
        /*Cas avec extension, on le remplace par la bonne*/
        strcpy(extension_output, EXTENSION);
    }
}
else{
    /*S'il y a un deuxième paramètre, on teste son extension*/
    strcpy(output_filename, argv[2]);
    extension_output = strrchr(output_filename, '.');
    if (extension_output == NULL){
        /*Cas de sans extension*/
        strcat(output_filename, EXTENSION);
    }
    else{
        /*Cas avec extension, on le remplace par la bonne*/
        strcpy(extension_output, EXTENSION);
    }
}

/*Afficher l'entrée et la sortie*/
printf(AFFICHAGE_ENTREE_SORTIE, input_filename, output_filename);

/*Ouvrir le fichier d'entrée*/
input_file = fopen(argv[1], "rb");
if (input_file == NULL){
    /*Si le fichier n'existe pas*/
    fprintf(stderr,AFFICHAGE_ERROR_OUVERTURE_FICHIER, argv[1]);
    return(EXIT_FAILURE);
}

/*Réinitialiser le fichier de sortie*/
output_file = fopen(output_filename,"r");
if (output_file != NULL){
    /*Si le fichier existe, on demande l'utilisateur ce qu'on fait ensuite*/
    fclose(output_file);
    while (1){
        printf(MESSAGE_ECRASER);
        int_reponse = fgetc(stdin); /*getchar outputs an interger*/
        while((fgetc(stdin)) != '\n'); /*empty the input string to the last element which is new line*/
        int_reponse = toupper((char)int_reponse); /*toupper also outputs an interger*/
        reponse = (char)int_reponse;
        /*while(fgetc(stdin)!=EOF);*/
        if (reponse == 'Y'){
            /*Cas où on écrase le fichier et crée un nouveau*/
            output_file = fopen(output_filename,"wb");
            printf("Overwritten completed\n");
            break;
        }
        else if (reponse == 'N'){
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
    output_file = fopen(output_filename,"wb");
}

/*Créer un dictionnaire en mode écriture*/
strcpy(dictionnaire_filename, output_filename);
extension_dictionnaire = strrchr(dictionnaire_filename, '.');
strcpy(extension_dictionnaire,".CSV");
dictionnaire = fopen(dictionnaire_filename, "w");

/*Vérifier le fichier est de ASCII*/
while((code_ASCII=fgetc(input_file)) != EOF){
    if (code_ASCII > 127){
        printf("This file is not formatted ASCII.");
        return(EXIT_FAILURE);
    }
}

/*Compter les couplets*/
rewind(input_file);
for (i=0;i<128;i++){ /*initialiser la tableau*/
    for (j=0;j<128;j++){
        count[i][j]=0;
    }
}
fenetre_count = 0;
/* Façon débile.*/
while ((counter = fgetc(input_file)) != EOF){
    if (fenetre_count < 2){
        fenetre[fenetre_count] = counter;
        fenetre_count++;
    }
    else{
        count[fenetre[0]][fenetre[1]]++;
        fenetre[0] = fenetre[1];
        fenetre[1] = counter;
    }
}
count[fenetre[0]][fenetre[1]]++;


/*Enregistrer les top 128 couplets les plus utilisés*/
for (i=0;i<128;i++){ /*initialiser la tableau*/
    dico[i][0] = dico[i][1] = dico_nb[i][0] = 0;
}

/*Récupérer le max des couplets*/
rewind(input_file);
max = 0;
int nb_colonne;
int nb_ligne;
for (i=0;i<128;i++){
    for (j=0;j<128;j++){
        if (count[i][j]>max){
            max = count[i][j];
            count[i][j] = 0;
            dico[0][0] = (char)i;
            dico[0][1] = (char)j;
            dico_nb[0][0] = max;
            nb_ligne = i;
            nb_colonne = j;
        }
    }
}

unsigned int k;
int index;
for (index = 1;index < 128;index++){
    int sec = 0;
    for (i=0;i<128;i++){
        for (j=0;j<128;j++){
            k = count[i][j];
            if ((k < max)&&(k > sec)){
                sec = k;
                nb_ligne = i;
                nb_colonne = j;
            }
        }
    }
    dico_nb[index][0] = sec;
    max = sec;
    dico[index][0] = (char)nb_ligne;
    dico[index][1] = (char)nb_colonne;
}

/*Exporter les données vers la dictionnaire*/
for (n=0;n<128;n++){
    fprintf(dictionnaire,"\"%c%c\"; %i\n", 
            iscntrl(dico[n][0])?' ':dico[n][0], 
            iscntrl(dico[n][1])?'w':dico[n][1],
            dico_nb[n][0]);
}

/*Generation de nom du fichier d'orgine*/
char *original_filename = "pico\0";

#if 0
for (n=0;n<128;n++){
    fprintf(dico,"\"%c%c\"\n", 
            iscntrl(dico[n][0])?' ':dico[n][0], 
            iscntrl(dico[n][2])?'w':dico[n][1]);
}

/**/
int prev;
int curr;
if ((prev = fgetc(input_file)) != EOF){
    while ((curr = fgetc(input_file)) != EOF){
        count[prev][curr]++;
        prev = curr;
    }
}
#endif

/*Fin de programme*/
fclose(input_file);
fclose(output_file);
fclose(dictionnaire);
return(EXIT_SUCCESS);
}
