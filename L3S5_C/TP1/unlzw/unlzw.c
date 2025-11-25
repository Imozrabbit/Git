#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "unlzw.h"

int main(int argc, char *argv[]){
    char input_filename[256];
    char output_filename[256];
    FILE *p_input_filename;
    FILE *p_output_filename;
    int i = 0;
    int c;

/* ---------------------------------------------------------------------------------------------------------------------*/
    /* check if have enough arguments to begin with */
    if ((argc < 2)||(argc>3)){
        fprintf(stderr, ERROR_SYNTAX);
        return 1;
    }

    /* log the input filename */
    strcpy(input_filename, argv[1]);
    /* try to open the input file */
    p_input_filename = fopen(input_filename, "rb");
    if (p_input_filename == NULL){
        /* if cannot find the file */
        fprintf(stderr, ERROR_FILE_OPEN);
        return 1;
    }
    
    /* log the output filename */
    if (argc == 2){
        /* if only have input filename, we use the header stored in the compressed file for the output filename */
        while (1){
            c = fgetc(p_input_filename);
            if ((char)c == '\0'){
                /* pointer stops at the ASCII 0 */
                break;
            }
            output_filename[i++] = (char)c;
        }
        output_filename[i] = '\0';
        i = 0;
    }
    if (argc == 3){
        /* check the extension of the second argument */
        strcpy(output_filename, argv[2]);
        char *extension = strrchr(output_filename, '.');
        if (extension == NULL){
            strcat(output_filename, ".txt");
        }
        else{
            strcpy(extension, ".txt");
        }
    }
    /* check if there is already a file with the same name as the output file */
    p_output_filename = fopen(output_filename, "r");
    if (p_output_filename != NULL){
        fclose(p_output_filename);
        /* if yes, then ask user if overwrite it */
        printf("File already exists, do you want to overwrite it? (y for yes, n for no)\n");
        char respond; 
        scanf("%c", &respond);
        while( fgetc(stdin) != '\n' );
        char Respond = (char) toupper( (int)respond );
        if (Respond == 'N'){
            /* no overwrite, then stop program */
            fprintf(stderr, "Overwrite stop, program end.\n");
            return 1;
        }
        else{
            /* if yes, then overwrite and start at wb mode */
            p_output_filename = fopen(output_filename, "wb");
        }
    }
    else{
        /* if no, just create one and start at wb mode */
        p_output_filename = fopen(output_filename, "wb");
    }

/* ---------------------------------------------------------------------------------------------------------------------*/
    /* move the pointer to the dictionnaire */
    rewind(p_input_filename);
    while (1){
        c = fgetc(p_input_filename);
        if ((char)c == '\0'){
            break;
        }
    }

    /* uncompress the dictionnaire */
    char dico[128][2];
    FILE *p_dictionnaire = fopen("dictionnaire.csv", "w");
    /* log data into dico[128][2], and write into the dictionnaire */
    for (i=0;i<128;i++){
        dico[i][0] = (char)fgetc(p_input_filename);
        dico[i][1] = (char)fgetc(p_input_filename);
        fprintf(p_dictionnaire, "%c%c\n", dico[i][0], dico[i][1]);
    }


/* ---------------------------------------------------------------------------------------------------------------------*/
    /* reconstruct the original file */
    while (1){
        c = fgetc(p_input_filename);
        if (c == EOF){
            break;
        }
        if (c > 127){
            int index = c - 128;
            fprintf(p_output_filename, "%c%c", dico[index][0], dico[index][1]);
        }
        else{
            fprintf(p_output_filename, "%c", (char)c);
        }
    }

/* ---------------------------------------------------------------------------------------------------------------------*/
    /* close all files */
    fclose(p_input_filename);
    fclose(p_output_filename);
    fclose(p_dictionnaire);
    return 0;
}
