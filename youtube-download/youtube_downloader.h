/* Common... */

#ifndef _YOUTUBE_DOWNLOADER_H_
#define _YOUTUBE_DOWNLOADER_H_

// program version package, defines user-agent
#define PROGRAM_VERSION "0.2.0a"
#define YD_USERAGENT "YouTube-Downloader/"PROGRAM_VERSION

// stuff for the updater
#define YD_UPDATECHECKLINK "http://www.example.com/youtube/downloader/curr_ver.cgi"
#define YD_UPDATELINK "http://www.example.com/youtube/downloader/update_link.cgi"
#define UPDATE_YES 1
#define UPDATE_NO -1
#define UPDATE_ERR 0
#define UPDATE_OK 1

// stuff for downloader
#define DOWNLOAD_OK 1
#define DOWNLOAD_ERR 0
#define TITLE_LOOKUP_URL "http://www.example.com/youtube/downloader/video_title.cgi?url="
#define LINK_LOOKUP_URL "http://www.example.com/youtube/downloader/video_link.cgi?url="

// very important limits
#define URL_PATH_SIZE 512
#define UPDATE_VER_SIZE 16
#define UPDATE_BASENAME_SIZE 128

// memory struct
struct MemoryStruct {
    char *memory;
    size_t size;
};

#endif
