/*Dirlist.h -- The Dirlist Engine*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "dirlist.h"

extern FILE *file;
extern long long size_count;

// global since function recurses
char cwd1[MAX_DIR_LENGTH];
char cwdr[MAX_DIR_LENGTH];
bool cwd_pass = 0;
bool init_pass = 0;
unsigned int cwd1_len;
unsigned int cwdr_len;

void dir(char *path, DIR_OPTIONS *options) {
	DIR *dhandle;
	FILE *file_recurse;
	struct dirent *drecord; // this is the info of the directory
	struct stat sbuf; // this is the stat of a file
	char output[MAX_DIR_LENGTH]; // this might be a stack problem, but who cares? todo :)
	char ready_output[MAX_DIR_LENGTH];
	int x;
	
	memset(output, '\0', MAX_DIR_LENGTH); // just in case
	memset(ready_output, '\0', MAX_DIR_LENGTH);
	
	dhandle = opendir(path); // open the directory in the specified path
	if(dhandle == NULL) {
		printf("Error opening directory '%s'\n", path);
		exit(1);
	}
	
	x = chdir(path); // chdir returns 0 if successfull, -1 if fails
	if(x != 0) {
		printf("Error changing to '%s'\n", path);
		exit(1);
	}
	
	// get the first cwd and count it's length
	if(cwd_pass == 0) {
		getcwd(cwd1, MAX_DIR_LENGTH);
		cwd1_len = strlen(cwd1);
		cwd_pass = 1;
	}
	
	// init files
	if(init_pass == 0 && options->filetype == 0) {
		initfile_once(options);
		init_pass = 1; // do this only one time
	} 
	else if(options->filetype) {
		file_recurse = initfile_recurse(options);
	}

	while((drecord = readdir(dhandle)) != NULL) {
		stat(drecord->d_name, &sbuf);
		
		//! Found directory, recurse to it
		if(S_ISDIR(sbuf.st_mode)) {
			if( strcmp(drecord->d_name, ".") == 0 || // if you find the '.' or '..' directory, skip it
				strcmp(drecord->d_name, "..") == 0 )
					continue;
			
			if(options->recurse)
				dir(drecord->d_name, options); // inmediately enters a directory
		}
		
		//! Print the file names
		else {
			// display progress
			if(options->verbose == 0)
				progress(sbuf.st_size);
			
			//! load the output variable depending on the -c or -e option
			if((options->complete) || (options->filetype_args.complete))
				getcwd(output, MAX_DIR_LENGTH);
			else // also if option t:r is selected, it works like it is supposed to
				cut_cwd(output);
			
			//! prepare the output
			sprintf(ready_output, "%s%s%s%s\n",
				((options->complete) || (options->asterisk)) ? "" : "*", // decide to put the *
#ifdef WIN32
				output, output[0]=='\0' ? "" : "\\",
#else
				output, output[0]=='\0' ? "" : "/",
#endif
				/* 	add backslash  at the end if output is a folder, 
					nothing if same as relative dir (relative dir = NULL) 
				*/
				drecord->d_name // add the name of the file
			);
			
			//! console printing
			if(options->verbose) {
				if(options->filetype_args.summary == 0) 
					printf("%s", ready_output);
				else if(options->filetype_args.summary)
					printf("*%s\n", drecord->d_name);
			}
			
			//! file printing
			if(options->filetype_args.summary)
				fprintf(FILE_RECURSE(options->filetype), "*%s\n", drecord->d_name);
			else
				fprintf(FILE_RECURSE(options->filetype), "%s", ready_output);
		}
	}
	// every time it leaves a subfolder
	if(options->filetype) fclose(file_recurse);
	chdir("..");
	closedir(dhandle);
}

//! only one file in specified folder
void initfile_once(DIR_OPTIONS *options) {
	file_init(options->filepath_arg, options);
	if(options->verbose) 
		printf("\nInitial Directory: \n\"%s\"\n\n", cwd1);
	fprintf(file, "\nInitial Directory: \"%s\"\n\n", cwd1);
}

//! recursive file writing, many files in EACH folder
FILE *initfile_recurse(DIR_OPTIONS *options) {
	char complete_path[MAX_DIR_LENGTH];
	FILE *file_recurse;
	
	// get rid of all the crap they might have
	memset(cwdr, '\0', MAX_DIR_LENGTH);
	memset(complete_path, '\0', MAX_DIR_LENGTH);
	
	getcwd(cwdr, MAX_DIR_LENGTH);
	cwdr_len = strlen(cwdr);
	
	if(options->filename) { // user inputed filename
#ifdef WIN32
		sprintf(complete_path, "%s\\%s", cwdr, options->filename_arg);
#else
		sprintf(complete_path, "%s/%s", cwdr, options->filename_arg);
#endif
	} else {
#ifdef WIN32
		sprintf(complete_path, "%s\\dirlist.txt", cwdr);
#else
		sprintf(complete_path, "%s/dirlist.txt", cwdr);
#endif
	}
	
	file_recurse = file_init(complete_path, options);
	fprintf(file_recurse, "Directory of: \"%s\"\n\n", cwdr);
	
	return file_recurse;
}

/*! 
	This function loads the output variable
	with a summarized relative path
!*/
void cut_cwd(char *output) {
	// I may need to find a better way to do this
	char cwd2[MAX_DIR_LENGTH];
	unsigned long x;
	unsigned long cwd2_len;
	
	memset(output, '\0', MAX_DIR_LENGTH); // get rid of the crap ouput had

	// first, get the full current working directory and count it
	getcwd(cwd2, MAX_DIR_LENGTH);
	cwd2_len = strlen(cwd2);
	
	// read the characters one by one after length and store to output
	// if x == cwd2_len-cwd1_len, then skip. output[0] should now be NULL
	for(x=0; x < cwd2_len-cwd1_len; x++) {
#ifdef WIN32
		if(cwd2[cwd1_len] == '\\')
#else
		if(cwd2[cwd1_len] == '/') //!@ need to check if needed for linux
#endif
			output[x] = cwd2[cwd1_len+x+1];
		else
			output[x] = cwd2[cwd1_len+x];
	}
	
	/************************************************************************
	*	I did the for loop with the if-else because							*
	*	in the case of C:\, cwd2[cwd1_len (in this case, 3)] == '\0'		*
	*	but in any other folder like C:\My Documents (where the path		*
	*	does not finish with a \), then cwd2[cwd1_len (15)] == '\', because *
	*	the next character for the next directory would be \				*
	*	like in C:\My Documents\something									*
	*	And, naturally, I don't want to print something like				*
	*	*\something\file, but instead *something\file.						*
	************************************************************************/
}
