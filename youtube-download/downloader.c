/* YouTube Downloader's Actual Downloader */

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "youtube_downloader.h"

int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

static void *myrealloc(void *ptr, size_t size)
{
    // There might be a realloc() out there that doesn't like reallocing
    // NULL pointers, so we take care of it here 
    
    if(ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

static size_t write_mem_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    
    // reallocate memory every time...
    mem->memory = myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        // copy data from ptr to mem starting at the size of mem
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        // now we add the size we just wrote...
        mem->size += realsize;
        mem->memory[mem->size] = 0; // and set the last byte to 0
    }
    return realsize;
}

int video_title(CURL *curl, char *lookup_url, char *watch_url, char *name, char *err_buff)
{
    struct MemoryStruct data;
    char url[URL_PATH_SIZE];
    CURLcode res;
    
    data.memory = NULL; // we expect realloc(NULL, size) to work 
    data.size = 0;    // no data at this point
    
    strncpy(url, lookup_url, URL_PATH_SIZE);
    strncat(url, watch_url, URL_PATH_SIZE - strlen(url) - 1);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, YD_USERAGENT);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err_buff);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // on 404 and shit
    res = curl_easy_perform(curl);
    curl_easy_reset(curl);
    
    if(res)
        return DOWNLOAD_ERR;
    
    strncpy(name, data.memory, URL_PATH_SIZE);
    strncat(name, ".mp4", URL_PATH_SIZE - strlen(name) - 1);
    free(data.memory);

    return DOWNLOAD_OK;
}

int generate_link(CURL *curl, char *lookup_url, char *watch_url, char *direct_url, char *err_buff)
{
    struct MemoryStruct data;
    char url[URL_PATH_SIZE];
    CURLcode res;
    
    data.memory = NULL; // we expect realloc(NULL, size) to work 
    data.size = 0;    // no data at this point
    
    strncpy(url, lookup_url, URL_PATH_SIZE);
    strncat(url, watch_url, URL_PATH_SIZE - strlen(url) - 1);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, YD_USERAGENT);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err_buff);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // on 404 and shit
    res = curl_easy_perform(curl);
    curl_easy_reset(curl);
    
    if(res)
        return DOWNLOAD_ERR;
    
    strncpy(direct_url, data.memory, URL_PATH_SIZE);
    free(data.memory);
    
    return DOWNLOAD_OK;
}

int download_vid(CURL *curl, char *url, FILE *fp, char *err_buff)
{
    CURLcode res;
    
    if(fp) {
        if(url) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err_buff);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0); // show progress
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // for HTTP/1.1 303 See Other
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // on 404 and shit
            res = curl_easy_perform(curl);
            curl_easy_reset(curl);
            
            if(res)
                return UPDATE_ERR;
            
        } else {
            strncpy(err_buff, "Invalid URL.", CURL_ERROR_SIZE);
            return DOWNLOAD_ERR;
        }
    } else {
        strncpy(err_buff, "Invalid File.", CURL_ERROR_SIZE);
        return DOWNLOAD_ERR;
    }
    
    return DOWNLOAD_OK;

}
