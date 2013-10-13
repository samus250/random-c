/* YouTube Downloader's Updater */

#include <stdlib.h>
#include <string.h>
#include <libgen.h> // for basename
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

int new_update(CURL *curl, char *url, char *curr_ver, int curr_ver_sz, char *err_buff)
{
    struct MemoryStruct data;
    CURLcode res;
    
    data.memory = NULL; // we expect realloc(NULL, size) to work 
    data.size = 0;    // no data at this point
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, YD_USERAGENT);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err_buff);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // on 404 and shit
    res = curl_easy_perform(curl);
    curl_easy_reset(curl);
    
    if(res)
        return UPDATE_ERR;
    
    if(data.memory) {
        strncpy(curr_ver, data.memory, curr_ver_sz);
        if(strcmp(data.memory, PROGRAM_VERSION) <= 0) {
            free(data.memory);
            return UPDATE_NO;
        } else {
            free(data.memory);
            return UPDATE_YES;
        }
    }
    
    return 0;
}

char *update_url(CURL *curl, char *url, char *up_basename, size_t up_basename_sz, char *err_buff)
{
    struct MemoryStruct data;
    int i;
    CURLcode res;
    
    data.memory = NULL;
    data.size = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, YD_USERAGENT);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // on 404 and shit
    res = curl_easy_perform(curl);
    curl_easy_reset(curl);
    
    if(res)
        return NULL;
    
    strncpy(up_basename, basename(data.memory), up_basename_sz);
    
    // MUST REMOVE ILLEGAL CHARACTERS FROM BASENAME, like trailing \n
    for(i = 0; i < up_basename_sz; i++) {
        if(up_basename[i] == '\n')
            up_basename[i] = '\0';
    }
    
    // the url is returned just like it was gotten,
    // does not surfer from lengthy paths, but free outside please
    return data.memory;
}

int perform_update(CURL *curl, char *url, FILE *fp, char *err_buff)
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
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // on 404 and shit
            res = curl_easy_perform(curl);
            curl_easy_reset(curl);
            
            if(res)
                return UPDATE_ERR;
            
        } else {
            strncpy(err_buff, "Invalid URL.", CURL_ERROR_SIZE);
            return UPDATE_ERR;
        }
    } else {
        strncpy(err_buff, "Invalid File.", CURL_ERROR_SIZE);
        return UPDATE_ERR;
    }
    
    return UPDATE_OK;
}
