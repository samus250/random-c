/**********************************************************
      Prime Numbers: primes.h. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful and
 comes with ABSOLUTELY NO WARRANTY.

************************************************************/

#include <stdbool.h>

#define VERSION "2.5.1"

#ifdef WIN32
#define VAL_FORMAT "%I64u"
#else
#define VAL_FORMAT "%llu"
#endif

typedef unsigned long long u_long_64;
typedef unsigned short int u_short;

void negative_input(void);
void bad_ranges(void);
void too_big(void);
void only_numbers(void);
void decimal_input(u_long_64 range, char *string);
void get_date(FILE *primefile);
void primegen(void);
void file_init(void);
void results(void);
bool optparse(char *string);

