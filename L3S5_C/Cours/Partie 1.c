/*1. {} for grouping instructions, do not forget ;*/

/*2. for sign a variable its type :*/
int X;
float y;
void _uneFonction();
double atan2();

/*3. signed = -2^(n-1) : 2^(n-1)-1 = left most negative -128*/
char c; /*ASCII ou entier 8 bits*/
short int i; /*entier 16 bits*/
int n; /*entier 32 bits*/
long l; /*entier 64 bits*/

/*4. unsigned = 0 : (2^n)-1 = left most positive 128*/
unsigned char c; /*ASCII ou entier 8 bits*/
unsigned short int i; /*entier 16 bits*/
unsigned int n; /*entier 32 bits*/
unsigned long l; /*entier 64 bits*/

/*5. IEEE 754
    - float = 1 sign bit + 8 exponent bit + 23 mantissa bit
*/

/*6. Conversion*/
i = (int)c;
c = (char)i;
f = (float)i // (float)c;

/*7. typedef unTypeDeDonnée nom*/
typedef enum{lundi, mardi, dimanche} jours; /*define "jours" as a new data type which can be the values in enum*/
jours jourDebut,jourFin; /*create 2 new variables that are type "jours"*/
jourDebut = lundi; jourFin = mardi;

/*8. enum objets' index can be changed, but default index starts at 0*/

/*9. struc : combines all kinds of data of different types including other structures, enumerations etc.*/
typedef struct {
    double re;
    double im;} complex;
complex a;
double norm;
a.re = 3.0;
a.im = 4.0;
norm = sqrt(a.re*a.re + a.im*a.im); /*need to #include <math.h> first*/

/*10. union : same as struc but only takes up the size of its largest element, meanwhile struc accumulates all its elements' size*/

/*11. Size
    - char = 1 octet
    - int, float = 4 octets
    - double = 8 octets
    - sizeof(...) : to check out the size of a variable
*/

/*12. Adresse and pointer
    - Convention : for a string s, its adresse is the same as the first character of this string. string s = char *s
*/
int n = 50;
int* adresse; //int* means that int is the data type for adresse
int *p = &n; //*p = pointer for storing the adresse of n in the memory ; & for finding out the adresse

/*13. % symbol
    - %s = C string, expect a pointer to a char buffer that is null terminated
    - %c = one character, take an integer to convert to the character
    - %d = signed int in decimal
*/

/*14. Null and nul
    - NULL : adresse 0x0 where nothing is supposed to be there
    - NUL = \0 : 8 bits of 0
*/