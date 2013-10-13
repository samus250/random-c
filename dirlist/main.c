/*Dirlist Main*/

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "dirlist.h"

extern FILE *file;

unsigned long long file_count;
unsigned long long size_count;

int main(int argc, char *argv[]) {
	int x;
	char *c;
	DIR_OPTIONS options; // could've been global.
	
	// the absolutely first thing to do
	welcome();
	
	if(argc == 1) {
		help();
		return 0;
	}
	
	if(optparse(argc, argv, &options)) {
		printf("Scanning...\n");
		dir(argv[optind], &options);
		printf("\nOperation Finished Successfully!\n");
		if(options.filetype == 0) fclose(file); // close the file after everything is done
	}
	
	return 0;
}

bool optparse(int argc, char **argv, DIR_OPTIONS *options) {
	int c;
	bool out = 1;
	int option_index = 0; // getopt returns the index number on option find
	
	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"recursive", no_argument, 0, 'r'},
		{"complete", no_argument, 0, 'c'},
		{"output", required_argument, 0, 'o'},
		{"verbose", no_argument, 0, 'v'},
		{"filetype", required_argument, 0, 't'},
		{"filename", required_argument, 0, 'f'},
		{"asterisk", no_argument, 0, 'a'},
		{0, 0, 0, 0}
	};
	
	// init the default values
	options->help = 0;
	options->recurse = 0;
	options->complete = 0;
	options->output = 0;
	options->verbose = 0;
	options->filetype = 0;
	options->filename = 0;
	options->asterisk = 0;
	options->other = 0;
	options->filetype_args.summary = 0;
	options->filetype_args.complete = 0;
	options->filetype_args.relative = 0;
	
	do {
		c=getopt_long(argc, argv, "hrco:vt:ef:a", long_options, &option_index);
		
		switch(c) {
			case 'h':
				options->help = 1;
				help();
				out = 0;
			case 'r':
				options->recurse = 1;
				break;
			case 'c':
				options->complete = 1;
				break;
			case 'v':
				options->verbose = 1;
				break;
			case 'o':
				options->output = 1;
				// copy its argument to filepath
				strcpy(options->filepath_arg,optarg);
				break;
			case 't':
				options->filetype = 1; 
				if(!strcmp(optarg, ":s"))
					options->filetype_args.summary = 1;
				else if(!strcmp(optarg, ":r"))
					options->filetype_args.relative = 1;
				else if(!strcmp(optarg, ":c")) {
					options->filetype_args.complete = 1;
					options->complete = 1;
				} else {
					printf("Invalid argument for option -t\n");
					out = 0;
				}
				break;
			case 'f':
				options->filename = 1;
				// maybe I can replace this for a pointer...
				strcpy(options->filename_arg, optarg);
				break;
			case 'a':
				options->asterisk = 1;
				break;
			case -1:
				break;
			default:
				options->other++;
				break;
		}
	} while(c != -1);
	
	// Sanity checks
	if(options->output == 1 && options->filetype == 1) {
		printf("The -o and -t options cannot be used at the same time\n");
		out = 0;
	}
	if(options->output == 1 && options->filename == 1) {
		printf("The -o and -f options cannot be used at the same time\n");
		out = 0;
	}
	if(options->complete == 1 && options->filetype == 1) {
		printf("The -c option will be ignored.\n");
		printf("Press the Enter key to start operation.\n");
		getchar();
		options->complete = 0;
	}

	return out;
}

void progress(size_t file_size) {
	file_count++;
	size_count += file_size;
	
	printf("\rFiles Scanned: " VAL_FORMAT " "
		"Bytes Scanned: " VAL_FORMAT, file_count,size_count
	);
	
	return;
}

void help(void) { // still have to fix the help a bit.
	printf(
		/* 80 col guide
		*|------------------------------------------------------------------------------|
		*/
		"Program Usage: "
		"dirlist [DIRECTORY] [OPTIONS]\n\n"
		"[DIRECTORY] is the complete path to the directory you want dirlist to scan.\n"
		"[OPTIONS] are the options listed below, you may define many combinations of\n"
		"options, except certain of these which are listed below.\n\n"
		"Program Options:\n"
		" -r\n"
		" --recursive      -Turn on recursive folder scanning.\n\n"
		
		" -c\n"
		" --complete       -Print complete path instead of default relative path.\n"
		"                  -This option is ignored if the -t:c option is specified.\n"
		
		" -o [PATH]\n"
		" --output [PATH]  -Save output file to [PATH]. This option cannot be used with\n"
		"                   the -t or -f options\n"
		" -v\n"
		" --verbose        -Print what you are doing on screen. The -c, t:s, t:r and t:c\n"
		"                   options will affect what is printed on screen. This option\n"
		"                   is known to slow the process significantly.\n"
		
		" -t:type\n"
		" --filetype:type  -This will set the location of the created text files to each\n"
		"                   folder found. There are three types of this option:\n"
		"                   t:s - will print the files found on a summarized output.\n"
		"                   t:r - will print the files found as a relative path of the\n"
		"                         specified [DIRECTORY].\n"
		"                   t:c - will print the complete path of the files found.\n"
		
		" -f [FILEMAME]\n"
		" --filename[NAME] -This will be the name of the file when the -t option is used\n"
		"                   If you do not specify, the default name \"dirlist.txt\" is\n"
		"                   selected.\n"
		" -h\n"
		" --help           -Print this help text and quit\n"
		" -a\n"
		" --asterisk       -Do not insert the default asterisk when printing relative dir\n\n"
#ifdef WIN32
		"Examples: >dirlist \"C:\\My Docs\" -c -r -v -o \".\\dirlist.txt\"\n"
		"          >dirlist \"C:\\My Docs\" -r -t:s -f \"my_files.txt\"\n\n"
#else
#endif
		
		"If no argument is specified, then I will print this help text.\n"
	);
}

void welcome(void) {
	printf(
		"\nDirlist " VERSION " - Copyright (C) 2008 samus250\n"
		"This software comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; see COPYING for details.\n"
		"\n"
	);
}
