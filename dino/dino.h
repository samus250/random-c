/*dino.h*/

#ifndef _DINO_H_
#define _DINO_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define VERSION "v 1.0.5"

void automatic(int argc, char *argv[]);
void manual(int argc, char *argv[]);
char eng_din(char ch);
char din_eng(char ch);
void help(int argc, char *argv[]);

/*struct type for program options*/

struct arguments {
bool help;
bool manual;
bool dino;
bool y_bypass;
unsigned int other;
};

#endif

