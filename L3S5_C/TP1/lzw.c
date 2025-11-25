#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lzw.h"

int main(int argc, char *argv[])
{
    /* Variables */
    int i;
    int j;
    int k;
    char input_filename[256];
    char output_filename[256];

    /* ------------------------------------------------------------------------------------------- */
    /* check if there are 2 to 3 parameters */
    if ((argc == 1) || (argc > 3)){
        fprintf(stderr, ERROR_SYNTAXE);
        return 1;
    }

    /* register the input filename */
    strcpy(input_filename, argv[1]);
    
    /* register the output filename */
    if (argc == 2){
        /* if there isn't a second parameter */
        strcpy(output_filename, input_filename);
        char *extension = strrchr(output_filename, '.');
        if (extension == NULL){
            /* no extension found */
            strcat(output_filename, EXTENSION);
        }
        else{
            /* extension found */
            extension = EXTENSION;
        }
    }
    else{
        /* if there are 2 parameters */
        strcpy(output_filename, argv[2]);
        char *extension = strrchr(output_filename, '.');
        if (extension == NULL){
            /* no extension found */
            strcat(output_filename, EXTENSION);
        }
        else{
            /* extension found */
            extension = EXTENSION;
        }
    }

    printf("Name of the program is : %s\n", argv[0]);
    printf("Parameter 1 : %s\n", argv[1]);
    printf("Parameter 2 : %s\n", argv[2]);
    printf("Input file name : %s\nOutput file name : %s\n", input_filename, output_filename);

    /* ------------------------------------------------------------------------------------------- */
    /* variables */
    FILE *p_input_filename;
    FILE *p_output_filename;
    char dictionnaire[256];
    FILE *p_dictionnaire;

    /* check if input file exists*/
    p_input_filename = fopen(input_filename, "rb");
    if (p_input_filename == NULL){
        fprintf(stderr, ERROR_FILE_OPEN);
        return 1;
    }

    /* check if output file exists and try to open it */
    p_output_filename = fopen(output_filename, "r");
    if (p_output_filename != NULL){
        /* if outpout file already there, ask user if overwrite it */
        fclose(p_output_filename);
        printf(IF_OVERWRITE);
        char respond;
        scanf("%c", &respond);
        while( (fgetc(stdin)) != '\n' );
        char Respond = (char) toupper( (int)respond );
        if (Respond == 'N'){
            /* No overwrite */
            fclose(p_input_filename);
            return 1;
        }
        else{
            /* Yes overwrite */
            p_output_filename = fopen(output_filename,"wb");
        }
    }
    else{
        /* if output file doesn't exist */
        p_output_filename = fopen(output_filename,"wb");
    }

    /* create a file named dictionnaire in writing mode */
    strcpy(dictionnaire, output_filename);
    char *csv = strrchr(dictionnaire, '.');
    strcpy(csv, ".csv");
    p_dictionnaire = fopen(dictionnaire,"w");


    /* ------------------------------------------------------------------------------------------- */
    /*check if file is of format ASCII */
    int ascii;
    while ( (ascii = fgetc(p_input_filename)) != EOF ){
        if (ascii > 127){
            fprintf(stderr, "File is not of ASCII format.\n");
            return 1;
        }
    }
    /* Analyse the file */
    int count[128][128];
    for (i=0;i<128;i++){
        /*initialize the table*/
        for (j=0;j<128;j++){
            count[i][j] = 0;
        }
    }
    rewind(p_input_filename);
    /* first 2 chars */
    int first_case = fgetc(p_input_filename);
    if (first_case == EOF){
        /*if the input file has nothing */
        fprintf(stderr, "The input file has no data.\n");
    }
    int second_case = fgetc(p_input_filename);
    if (second_case == EOF){
        /*if the input file has only 1 character */
        fprintf(stderr, "The input file has only 1 character.\n");
    }
    count[first_case][second_case] += 1;
    while (1){
        /* rotation */
        first_case = second_case;
        second_case = fgetc(p_input_filename);
        if (second_case == EOF){
            break;
        }
        count[first_case][second_case] += 1;
    }
    /* 128 most frequent couplets */
    char dico[128][2];
    int dico_num[128][1];
    int max = 0;
    int picked_index = -1;
    int max_i_num;
    int max_j_num;
    
    for (k=0;k<128;k++){
        for (i=0;i<128;i++){
            for (j=0;j<128;j++){
                if (count[i][j] > max){
                    max = count[i][j];
                    max_i_num = i;
                    max_j_num = j;
                }
            }
        }
        dico[k][0] = (char)max_i_num;
        dico[k][1] = (char)max_j_num;
        dico_num[k][0] = max;
        count[max_i_num][max_j_num] = picked_index;
        picked_index --;
        max = 0;
    }

    /* write in csv file */
    for (i=0;i<128;i++){
        fprintf(p_dictionnaire,"%c%c : %d\n", dico[i][0], dico[i][1], dico_num[i][0]);
    }


    /* ------------------------------------------------------------------------------------------- */
    char *compressed = "Lorem_Ipsum.lzw";
    FILE *p_compressed;
    /* Check if can open the compressed file */
    p_compressed = fopen(compressed,"wb");
    if (p_compressed == NULL){
        fprintf(stderr, ERROR_FILE_OPEN);
        return 1;
    }

    /* write the input file name as header ending with a ASCII 0*/
    fprintf(p_compressed,"%s",input_filename);
    fputc('\0', p_compressed);

    /* write dico[128][2] */
    for (i=0;i<128;i++){
        fprintf(p_compressed,"%c%c", dico[i][0],dico[i][1]);
    }

    /* rescan the input file */
    rewind(p_input_filename);
    int tampon_1;
    int tampon_2;
    tampon_1 = fgetc(p_input_filename);
    tampon_2 = fgetc(p_input_filename);
    while (1){
        if (count[tampon_1][tampon_2] < 0){
            fprintf(p_compressed, "%c", (char)((-count[tampon_1][tampon_2]-1)+128));
            tampon_1 = fgetc(p_input_filename);
            if (tampon_1 == EOF){
                break;
            }
            tampon_2 = fgetc(p_input_filename);
            if (tampon_2 == EOF){
                break;
            }
        }
        else{
            fprintf(p_compressed,"%c", tampon_1);
            tampon_1 = tampon_2;
            tampon_2 = fgetc(p_input_filename);
            if (tampon_2 == EOF){
                break;
            }
        }
    }

    /* Seek and tell the size of the original file and the compressed one */
    rewind(p_input_filename);
    rewind(p_compressed);
    fseek(p_input_filename,0,SEEK_END);
    fseek(p_compressed, 0, SEEK_END);
    unsigned int size_original = ftell(p_input_filename);
    unsigned int size_compressed = ftell(p_compressed);
    printf("The ratio is %.2lf %%\n", ((double)size_compressed)/((double)size_original)*100 );

    
    /* ------------------------------------------------------------------------------------------- */
    /* close all files */
    fclose(p_output_filename);
    fclose(p_input_filename);
    fclose(p_dictionnaire);
    fclose(p_compressed);
    return 0;
}
