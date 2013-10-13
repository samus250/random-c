/*Dirlist.h Header File 2008*/

#ifndef _DIRLIST_H_
#define _DIRLIST_H_

#include <stdbool.h>

#define MAX_DIR_LENGTH 1024
#define VERSION "1.2.5"

#ifdef WIN32
#define VAL_FORMAT "%I64u"
#else
#define VAL_FORMAT "%llu"
#endif

#define FILE_RECURSE(X) ((X) ? file_recurse : file)

// The filetype option
struct filetype {
	bool summary;
	bool relative;
	bool complete;
};

// Struct for program options
typedef struct arguments {
	bool help;
	bool recurse;
	bool complete;
	bool output;
	bool verbose;
	bool filetype;
	bool filename;
	bool asterisk;
	unsigned int other;
	struct filetype filetype_args;
	char directory[MAX_DIR_LENGTH];
	char filepath_arg[MAX_DIR_LENGTH];
	char filename_arg[MAX_DIR_LENGTH];
} DIR_OPTIONS;

// Prototypes
bool optparse(int argc, char **argv, DIR_OPTIONS *options);
void help(void);
void welcome(void);
void dir(char *path, DIR_OPTIONS *options);
FILE *file_init(char *path, DIR_OPTIONS *options);
void print_file(FILE *file);
char *get_date(void);
void initfile_once(DIR_OPTIONS *options);
FILE *initfile_recurse(DIR_OPTIONS *options);
void cut_cwd(char *output);
void progress(size_t file_size);
void sizeof_dir(char *path, DIR_OPTIONS *options);

#endif

