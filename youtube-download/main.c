/* The YouTube Downloader */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "youtube_downloader.h"

// the updater
int new_update(CURL *curl, char *url, char *curr_ver, int curr_ver_sz, char *err_buff);
int perform_update(CURL *curl, char *url, FILE *fp, char *err_buff);
char *update_url(CURL *curl, char *url, char *up_basename, size_t up_basename_sz, char *err_buff);

// the downloader
int video_title(CURL *curl, char *lookup_url, char *watch_url, char *name, char *err_buff);
int generate_link(CURL *curl, char *lookup_url, char *watch_url, char *direct_url, char *err_buff);
int download_vid(CURL *curl, char *url, FILE *fp, char *err_buff);

// a global struct for program options
struct {
    bool update;
    bool interactive;
    char *llist;
    char *downdir;
    char *update_downdir;
} options;

void welcome(void)
{
    fprintf (stderr,
		"YouTube Downloader " PROGRAM_VERSION " - Copyright (C) 2009\n"
		"This software comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; see COPYING for details.\n"
		"\n"
		);
	fflush (stderr);
}

void help(void)
{
	fprintf(stderr,
		/* 80 col guide
		*|------------------------------------------------------------------------------|
		*/
		"Program Usage:\n"
		"youtube_downloader [OPTIONS(ARGUMENTS)]\n\n"
        
		"Program Options:\n"
		" -h\n"
		" --help                 -Show this help text.\n\n"
		
		" -n\n"
        " --no-update            -Don't check for updates.\n\n"
        
        " -i\n"
        " --no-interactive       -For now it does nothing, but it will make updates\n"
        "                         automatically instead, so that you can pipe input.\n\n"
        
        " -l (FILE)\n"
        " --link-list (FILE)     -Download links found in FILE\n\n"
        
        " -d (PATH)\n"
        " --download-path (PATH) -Download the videos to PATH\n\n"
        
        " -u (PATH)\n"
        " --update-download-path (PATH)\n"
        "                        -Download the update to PATH\n\n"
        
		"If no argument is specified, then I will assume no options and take links\n"
        "from stdin.\n"
	);
}

bool optparse(int argc, char **argv)
{
    bool out = true;
    int c;
    int option_index = 0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"no-update", no_argument, 0, 'n'},
        {"no-interactive", no_argument, 0, 'i'},
        {"link-list", required_argument, 0, 'l'},
        {"donwload-path", required_argument, 0, 'd'},
        {"update-download-path", required_argument, 0, 'u'}
    };
    
    // init all options to default values
    options.update = true;
    options.interactive = true;
    options.llist = NULL;
    options.downdir = NULL;
    options.update_downdir = NULL;
    
    do {
        c = getopt_long(argc, argv, "hnil:d:u:", long_options, &option_index);
        // REMEMBER, YOU ARE ALWAYS POINTING TO THE ARGUMENTS WHEN LOOKING
        // FOR THE PATHS!!!
        switch(c) {
            case 'h':
                help();
                exit(0);
                break;
            case 'n':
                options.update = false;
                break;
            case 'i':
                options.update = false;
                options.interactive = false;
                break;
            case 'l':
                options.llist = optarg;
                break;
            case 'd':
                options.downdir = optarg;
                break;
            case 'u':
                options.update_downdir = optarg;
                break;
            case -1:
                break;
            default:
                // what the fuck???
                exit(7);
                break;
        }
    } while(c != -1);
    
    if (optind < argc) {
		// Command-line arguments remaining. Ignore them, warning the user.
		fprintf (stderr, "WARNING: Extra parameters ignored.\n");
	}
    
    // make some sanity checks... maybe...
    return out;
}

char *pathcpy(char *checked_path, char *path)
{
    memset(checked_path, '\0', PATH_MAX);
    if(!path) {
        DIR *dir;
        
        // always use . by default
        strcpy(checked_path, ".\\");
        
        // and check it just in case
        if(!(dir = opendir(checked_path))) {
            // shit
            return NULL;
        } else {
            closedir(dir);
            return checked_path;
        }
        
    } else {
        // path is too long
        if(strlen(path) > PATH_MAX)
            return NULL;
        
        strncpy(checked_path, path, PATH_MAX - 1);
        
        // insert the last '\' if it's not there
        if(checked_path[strlen(checked_path) - 1] != '\\')
            strcat(checked_path, "\\");
        
        // try to check the path
        DIR *dir;
        if(!(dir = opendir(checked_path))) {
            // try to  create it
            if(mkdir(path) == -1) {
                // we can't make the dir
                return NULL;
            } else {
                // success
                return checked_path;
            }
        }
        
        // success
        closedir(dir);
    }
    
    return checked_path;
}

char *check_existance(char *file)
{
    FILE *fp = NULL;
    
    if(!file)
        return NULL;
    if(!(fp = fopen(file, "r"))) {
        // simply try to open it...
        return NULL;
    } else {
        fclose(fp);
        return file;
    }
}

int main(int argc, char *argv[])
{
    CURL *curl;
    char curl_error[CURL_ERROR_SIZE];
    char update_path[PATH_MAX]; // path for update download
    char download_path[PATH_MAX]; // path for video download
    
    // first of all
    welcome();
    
    if(!optparse(argc, argv))
        return 0;
    
    // immediately check for paths and copy them
    if(!(pathcpy(update_path, options.update_downdir))) {
        fprintf(stderr, 
            "The path to download the update is not accessible.\n"
            "You entered: %s.\n", options.update_downdir
        );
        return 0;
    }
    
    if(!(pathcpy(download_path, options.downdir))) {
        fprintf(stderr,
            "The path to download the videos is not accessible.\n"
            "You entered: %s.\n", options.downdir
        );
        return 0;
    }
    
    // first you must check if it was specified
    if(options.llist) {
        if(!(check_existance(options.llist))) {
            fprintf(stderr,
                "The link list file given is not accessible.\n"
                "You entered: %s.\n", options.llist
            );
            return 0;
        }
    }
    
    //! after all the path parsing and shit, we begin
    if(!(curl = curl_easy_init())) {
        fprintf(stderr, "Something went horribly wrong!\n");
        fprintf(stderr, "Aborting...\n");
        return 0;
    }
    
    // the first thing after initializing, would be to check for updates
    if(options.update) {
        char update_ver[UPDATE_VER_SIZE];
        int new_update_res;
        
        fprintf(stderr, "Checking for updates...\n");
        new_update_res = new_update(curl, YD_UPDATECHECKLINK, update_ver, UPDATE_VER_SIZE - 1, curl_error);
        if(new_update_res == UPDATE_YES) {
            int tries = 0;
            bool done = true;
            
            fprintf(stderr, "\nA new update was found!\n"
                            "Current Version: "PROGRAM_VERSION"\n"
                            "New Version:     %s\n\n", update_ver
            );
            fprintf(stderr, "Do you want to download the update? [Y/n]: ");
            
            do {
                char ans;
                char update_basename[UPDATE_BASENAME_SIZE];
                char *up_url = NULL;
                FILE *fp = NULL;
                
                scanf("%1s", &ans);
                switch(toupper(ans)) {
                    case 'Y':
                        // prepare the download path, up_url was malloced by update_url
                        if(!(up_url = update_url(curl, YD_UPDATELINK, update_basename, UPDATE_BASENAME_SIZE - 1, curl_error))) { 
                            fprintf(stderr, "Error: %s\n", curl_error);
                            fprintf(stderr, "Update not downloaded successfully.\n");
                            break;
                        }
                        
                        // just cat the base to the path (being careful not to overlap)
                        strncat(update_path, update_basename, PATH_MAX - strlen(update_path) - 1);
                        
                        if(!(fp = fopen(update_path, "wb"))) {
                            fprintf(stderr, "Error Opening file for writing.\n");
                            fprintf(stderr, "Update will not be downloaded.\n");
                        } else {
                            fprintf(stderr, "Downloading update...\n");
                            int perform_update_res = perform_update(curl, up_url, fp, curl_error);
                            if(perform_update_res == UPDATE_OK) {
                                fprintf(stderr, "\n\nUpdate successfully downloaded!\n");
                                fclose(fp);
                            } else {
                                fprintf(stderr, "\n\nError: %s\n", curl_error);
                                fprintf(stderr, "Update not downloaded successfully.\n");
                            }
                            free(up_url);
                        }
                        
                        done = true;
                        break;
                    
                    case 'N':
                        fprintf(stderr, "Not downloading update...\n");
                        done = true;
                        break;
                    
                    default:
                        done = false;
                        if(++tries > 3) // number of tries for a stupid user
                            done = true;
                        else {
                            fprintf(stderr, "That is not a valid option. Enter only [Y/n]: ");
                            tries++;
                        }
                        break;
                }
            } while(!done);
            
        } else if(new_update_res == UPDATE_ERR) {
            fprintf(stderr, "%s\n", curl_error);
        } else if(new_update_res == UPDATE_NO) {
            fprintf(stderr, "No update found.\n");
        }
        fprintf(stderr, "\n");
    }
    
    // ok, that was all the update process
    // all those stupid vars have finally been destroyed
    // next, we start getting links one by one...
    fprintf(stderr, "Ready for downloading. Waiting for input...\n");
    
    char input_buffer[URL_PATH_SIZE]; // buffer for each line of url
    char line_buffer[URL_PATH_SIZE];
    int nread;
    FILE *fp = NULL;
    
    // even though we already checked it, the file might have changed
    // so check it again!
    if(options.llist) {
        if(!(fp = fopen(options.llist, "r"))) {
            fprintf(stderr, "Error opening link list for reading.\n");
            return 0;
        }
    }
    
    // before reading, we might want to flush stdin
    fflush(stdin);
    int i, j = 0;
    do {
        //int i, j = 0; this causes a bug... j can be re-initialized ONLY when the string is capped.
        
        if(options.llist)
            nread = fread(input_buffer, sizeof(char), URL_PATH_SIZE, fp);
        else
            nread = fread(input_buffer, sizeof(char), URL_PATH_SIZE, stdin);
        
        for(i = 0; i < nread; i++) {
            for(; input_buffer[i] != '\n' && i < nread; i++, j++)
                line_buffer[j] = input_buffer[i];
            
            // if we haven't finished reading the line, but the buffer filled up...
            if(i == nread && nread == URL_PATH_SIZE) continue;
            
            // we cap the line, and reinit j
            line_buffer[j] = '\0';
            j = 0;
            
            // until here, we have successfully gotten all the information
            // into line buffer, now entering actual download phase
            {
                FILE *out_file = NULL;
                char video_link[URL_PATH_SIZE];
                char title[URL_PATH_SIZE];
                
                // added in 0.2.0a, found a bug in which the download_path would only be catted
                // oh and make errors continue!
                char out_file_path[PATH_MAX];
                memset(out_file_path, '\0', PATH_MAX);
                strncpy(out_file_path, download_path, PATH_MAX - 1);
                
                fprintf(stderr, "Looking for title...\n");
                if(!(video_title(curl, TITLE_LOOKUP_URL, line_buffer, title, curl_error))) {
                    fprintf(stderr, "\n\nError: %s.\n", curl_error);
                    fprintf(stderr, "Skipping this video..\n\n");
                    continue;
                }
                
                fprintf(stderr, "Title is \"%s\".\n", title);
                fprintf(stderr, "Looking for a direct link to the video...\n");
                
                if(!(generate_link(curl, LINK_LOOKUP_URL, line_buffer, video_link, curl_error))) {
                    fprintf(stderr, "\n\nError: %s.\n", curl_error);
                    fprintf(stderr, "Skipping this video...\n\n");
                    continue;
                }
                
                strncat(out_file_path, title, MAX_PATH - strlen(out_file_path) - 1);
                if(!(out_file = fopen(out_file_path, "wb"))) {
                    fprintf(stderr, "Error opening video file for writing!\n");
                    fprintf(stderr, "Skipping this video...\n\n");
                    continue;
                }
                
                fprintf(stderr, "Downloading...\n");
                if(!(download_vid(curl, video_link, out_file, curl_error)))
                    fprintf(stderr, "\n\nError: %s.\n", curl_error);
                else
                    fprintf(stderr, "\nVideo Sucessfully Downloaded!!!\n\n");
                
                fclose(out_file);
            }
        }
    } while(nread == URL_PATH_SIZE);
    
    return 0;
}

