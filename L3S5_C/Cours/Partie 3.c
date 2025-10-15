/*1. Fonctions
    - type_ret maFonction (arguments) {
      ...
      return(expression);
      }
    - type_ret is the datatype of the 'return' of this function, it can be void or others
    - we can creat a new file called mesfonctions.h to store all the functions we create
*/

/*2. Compilation (petit projet)
    - translate C to machine code : 
        i. creat a .o file
        ii. combine all .h files to one .0 file
        iii. complier like gcc : gcc -ansi -wall -o monExecutable main.c
            -ansi : strictly respect ANSI norme
            -Wall : print all the warnings
    - #include <file.h> vs "file.h"
        i. <> is the standard library, location determined by the system or user setting
        ii. "" is the specific library for the project
    - #define : for define all the symbolic constants and macros which can replace every macro in the code by what we define
        i. #define PI 3.141596
        ii. #define nomMacro(a,b,c) ((a*b)/c)
    - #ifdef and #ifndef : compile base on the existance or non-existance of a defined symbol
    - #if, #elif and #else : if condition on the level of compiler, decides if compiler will do its job based on the conditions
*/

/*3. Standard library
    - Example: stdio.h, math.h, stdlib.h, string.h, ctype.h, limits.h, locale.h
*/