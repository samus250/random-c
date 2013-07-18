#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define PROGRAM_NAME "Hexdump"
#define PROGRAM_VERSION "v 0.1.2"

void welcome(void)
{
  printf("\n"
      PROGRAM_NAME " " PROGRAM_VERSION " - Copyright (C) 2008 samus250\n"
      "This software comes with ABSOLUTELY NO WARRANTY.\n\n");
  return;
}

void help(void)
{
  printf("Usage: [input file] [output file] [array name]\n");
  return;
}

char *loadfile(FILE *input_file, unsigned long long *file_size)
{
  unsigned char *output_data = NULL;
  unsigned long long x = 0;

  if(!(output_data = malloc(*file_size))) {
    return output_data;
  }

  while(x < *file_size) {
    output_data[x++] = fgetc(input_file);
  }

  return output_data;
}

void init_header(FILE *output)
{
  char *wdays[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
  };
  struct tm *t;
  time_t now;

  time(&now); // get epoch time and store to struct
  t = localtime(&now);

  fprintf(output, 
      "/*\n"
      " Generated %s, %d/%d/%d at %d:%02d:%02d %s\n" 
      " with " PROGRAM_NAME " " PROGRAM_VERSION ", written by samus250.\n*/\n"
      "\n",
      wdays[t->tm_wday],
      t->tm_mon + 1, t->tm_mday, t->tm_year + 1900,
      t->tm_hour>12 ? t->tm_hour-12 : t->tm_hour,
      t->tm_min,
      t->tm_sec,
      t->tm_hour>12 ? "PM" : "AM");
  return;
}

void struppercpy(char *output, char *string)
{   
  while(*output++ = toupper(*string++));
}

void write_data(FILE *output, char name[], unsigned long long filesize,
    unsigned char *data)
{
  unsigned long long x = 0;
  unsigned int rowcount = 0;
  char uppername[strlen(name)];

  struppercpy(uppername, name);

  fprintf(output, "#define %s_DATA_SIZE %llu\n", uppername, filesize);
  fprintf(output, "#define %s_DATA_NAME %s\n\n", uppername, name);
  fprintf(output, "const unsigned char %s[%llu] = {\n\t", name, filesize);
  while(x < (filesize - 1)) { // break out of while before reaching last value
    fprintf(output, "0x%.2X, ", data[x++]);
    rowcount++;
    if(rowcount >= 16) {
      fprintf(output, "\n\t");
      rowcount = 0;
    }
  }
  fprintf(output, "0x%.2X\n};\n", data[x]);
  return;
}

int main(int argc, char *argv[])
{
  FILE *input = NULL, *output = NULL;
  unsigned char *read_data = NULL;
  unsigned long long file_size = 0;
  struct stat filestats;
  clock_t ticks;

  // First of all.
  welcome();

  if(argc < 2) {
    help();
    return 0;
  }

  ticks = clock();
  if(!(input = fopen(argv[1], "rb"))) {
    printf("- Error opening input file!\n");
    printf("- Check the given file name and your administrator permissions.\n");
    return 1;
  }

  printf("- Loading file!\n");
  // Get file size.

  if(stat(argv[1], &filestats)) {
    // Error getting filesize from stat.
    while(fgetc(input) != EOF)
      file_size++;
  } else {
      // Got file size with no problem.
      file_size = filestats.st_size;
  }

  // Load the file into RAM.
  if(!(read_data = loadfile(input, &file_size))) {
    printf("- Error loading data to RAM!\n");
    return 1;
  }

  // Create file for writing.
  if(!(output = fopen(argv[2], "w"))) {
    printf("- Error creating file for writing!\n");
    printf("- Maybe disk is write protected or you don't have permissions.\n");
    free(read_data);
    return 1;
  }

  // Write it.
  printf("- Writing file!\n");
  init_header(output);
  write_data(output, argv[3], file_size, read_data);

  ticks = clock() - ticks;

  printf("- Finished!\n\n");
  printf("- Total operation took %.2f seconds!\n",
      ticks / (double)CLOCKS_PER_SEC);

  free(read_data);

  return 0;
}
