/*1. Gestion de mémoire
    - pointeur vers le premier octet du bloc alloué
    - void* malloc(size_t size)
        i. malloc(strlen(s) +1) : size of the string + the last null byte
    - void* calloc(size_t size) : comme malloc mais initialise tous les octets alloués à 0
        i. rarement utile, plus lent
    - void free(void*)
    - void* realloc(void* ptr, size_t size) : modifier la taille du bloc mémoire alloué
*/

/*2. table
    - int tab[16]
        i. tab is a pointer of type int*
        ii. tab[n] = *(tab+n)
    - int tab[16][32]
        i. tab is a pointer of type int**
        ii. tab[n] = *(tab+n)
        iii. tab[n] is a pointer of type int*
        iv. tab[n][m] = *((tab+n)+m)
        v. tab[n][m] is an interger of type int
*/