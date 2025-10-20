#define string char *

#define AFFICHAGE_PARAMETRES "\tParameter n.%i: %s\n"
#define SYNTAX_ERROR "\n\n\tSYNTAX : lzw input_ascii_file [output_lzw_file]\n"
#define EXTENSION ".lzw"
#define AFFICHAGE_ENTREE_SORTIE "Input: %s\nOutput: %s\n"
#define AFFICHAGE_ERROR_OUVERTURE_FICHIER "The file spicified in the first parameter (%s) doesn't exist.\n"
#define MESSAGE_ECRASER "Do you want to delete the content in the existing file? (y/n)\n"

typedef enum{Y,N} Reponse;