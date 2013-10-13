#include <stdio.h>
#include <curl/curl.h>

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

int main(void)
{
    CURL *image;
    CURLcode imgresult;
    FILE *fp = NULL;
    char url[] = "http://www.example.com/test.mp3";
    double dlspeed = 0;
    
    fprintf(stderr, "Initializing...\n");
    image = curl_easy_init();
    if(image) {
        // Open file
        fprintf(stderr, "Opening file for writing...\n");
        fp = fopen("test.mp3", "wb");
        if(fp == NULL)
            fprintf(stderr, "File cannot be opened!\n");
        
        // set opts
        curl_easy_setopt(image, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(image, CURLOPT_URL, url);
        curl_easy_setopt(image, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(image, CURLOPT_FOLLOWLOCATION, 1); // for HTTP/1.1 303 See Other
       
        // Grab image
        fprintf(stderr, "Downloading...\n\n");
        imgresult = curl_easy_perform(image);
        fprintf(stderr, "\n\n");
        if(imgresult) {
            fprintf(stderr, "Cannot grab the image!\n");
        } else {
            curl_easy_getinfo(image, CURLINFO_SPEED_DOWNLOAD, &dlspeed);
            fprintf(stderr, "Finished!!! Average Download Speed: %.2f KB/s\n", dlspeed / 1024);
        }
    }
    
    // Clean up the resources
    curl_easy_cleanup(image);
    // Close the file
    fclose(fp);
    return 0;
}

