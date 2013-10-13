#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <iomanip>
#include "curl/curl.h"

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::streamsize;
using std::setprecision;

void print_progress(double total, double now)
{
    int perc = 0;
    double i = 0;
    
    fprintf(stderr, "\r");
    
    // calculate percentage only if total is known
    if(total > 0) {
        perc = (int)((now / total) * 100);
        
        // draw progress bar
        fprintf(stderr, "%3d%%",perc);
        fprintf(stderr, "|");
        for (i = 0; i < 100 / 3; i++) {
            if (i == perc / 3)
                fprintf(stderr, "*");
            else
                fprintf(stderr, "-");
        }
        fprintf(stderr, "|");
    }
    
    // now, prepare a nice string
    if(now / 1024 < 1024)
        fprintf(stderr, " %.0f KB", now / 1024);
    else if(now / (1024 * 1024) < 1024)
        fprintf(stderr, " %.2f MB", now / (1024*1024));
    else
        fprintf(stderr, " %.2f GB", now / (1024*1024*1024));
    
    if(total > 0) {
        fprintf(stderr, " /");
        
        if(total / 1024 < 1024)
            fprintf(stderr, " %.0f KB", total / 1024);
        else if(total / (1024 * 1024) < 1024)
            fprintf(stderr, " %.2f MB", total / (1024*1024));
        else
            fprintf(stderr, " %.2f GB", total / (1024*1024*1024));
    }
    
    return;
}

int progress_callback(void *clientp,
                        double dltotal, double dlnow,
                        double ultotal, double ulnow)
{
    
    // determine wether we are uploading or downloading
    if(dltotal > 0 || dlnow > 0)
        print_progress(dltotal, dlnow);
    else if (ultotal > 0 || ulnow > 0)
        print_progress(ultotal, ulnow);
    else
        fprintf(stderr, "\rUnknown Progress...");
    
    return 0;
}

int main() {
    CURL *curl;
    CURLcode curl_result;
    FILE *fp = NULL;
    string url = "http://www.example.com/test.mp3";
    double dlspeed = 0;
    
    cout << "Initializing..." << endl;
    curl = curl_easy_init();
    
    if(curl) {
        // Open file
        cout << "Opening file for writing..." << endl;
        fp = fopen("test.mp3", "wb");
        if(fp == NULL)
            cout << "File cannot be opened!!!" << endl;
        
        // set opts
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // for HTTP/1.1 303 See Other
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // for debugging
       
        // Grab file
        cout << "Downloading file..." << endl;
        curl_result = curl_easy_perform(curl);
        fprintf(stderr, "\n\n");
        if(curl_result) {
            fprintf(stderr, "Cannot grab the file!\n");
        } else {
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &dlspeed);
            // write the average down speed
            streamsize prec = cout.precision();
            cout << "Finished!!! Average Download Speed: " << setprecision(3)
            << dlspeed / 1024 << " KB/s" << setprecision(prec) << endl;
        }
    }
    
    // Clean up the resources
    curl_easy_cleanup(curl);
    
    // Close the file
    fclose(fp);
    
    system("PAUSE > NULL");
    return 0;
}


