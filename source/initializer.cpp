#include "log.h"
#include "types.h"
#include "dynlibs/os/functions.h"
#include "dynlibs/gx2/functions.h"
#include "curl/curl.h"

/*
    ______
   /_  __/______  _________  __
    / / / ___/ / / / ___/ / / /
   / / (___ ) /_/ / /  / /_/ /
  /_/ /____/\__,_/_/   \__,_/

      _              __             _                       _           _
     | |__ _  _     / /  __ ____ _ (_) __  __ ____ __   ___| |_    __ _| |
     | '_ \ || |   / /__/ // /  ' \/ / _ \/ // /\ \ /  / -_)  _|  / _` | |_
     |_.__/\_, |  /____/\_,_/_/_/_/_/_//_/\_, //_\_\   \___|\__|  \__,_|_(_)
            |__/                         /___/
*/

// staticInit array
extern "C" funcPtr _ctors[];

// RPL loading dependencies
OsSpecifics osSpecifics;
extern u32 BLOSDynLoad_Acquire;
extern u32 BOSDynLoad_FindExport;

char curl_data[0xFFFF];

static size_t curl_cb(char* buf, size_t scale, size_t bufsize, void* userdata) {
	size_t* bytes_written = (size_t*)userdata;
	size_t realsize = scale * bufsize;
    PRINT("curl_cb: Received ", *bytes_written, " + ", realsize, " bytes = ", buf, "\0");

    //void* newbuf = realloc(curl_data, *bytes_written + realsize);
    //if (!newbuf) {
    //    PRINT("realloc failed");
    //    return 0;
    //}

    //curl_data = (char*)newbuf;
    OSBlockMove(curl_data + *bytes_written, buf, realsize, 0);

	*bytes_written += realsize;
	return realsize;
}

void initialize() {
    // Duplicate call check
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    // Call staticInit functions from _ctors array
    for (s32 i = 0; _ctors[i]; i++)
        (*_ctors[i])();

    // Set OSDynLoad_Acquire and OSDynLoad_FindExport
    OS_SPECIFICS->addr_OSDynLoad_Acquire    = (u32)(BLOSDynLoad_Acquire   & 0x03FFFFFC);
    OS_SPECIFICS->addr_OSDynLoad_FindExport = (u32)(BOSDynLoad_FindExport & 0x03FFFFFC);

    if (!(BLOSDynLoad_Acquire & 2))
        OS_SPECIFICS->addr_OSDynLoad_Acquire    += (u32)&BLOSDynLoad_Acquire;
    if (!(BOSDynLoad_FindExport & 2))
        OS_SPECIFICS->addr_OSDynLoad_FindExport += (u32)&BOSDynLoad_FindExport;

    // Init RPL libraries
    InitOSFunctionPointers();
    InitGX2FunctionPointers();

    uint32_t nlibcurl_handle = 0;
    OSDynLoad_Acquire("nlibcurl.rpl", &nlibcurl_handle);

    //curl_version* curl_version;
    //curl_version_info* curl_version_info;
    //OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_version", &curl_version);
    //OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_version_info", &curl_version_info);

    curl_easy_init* curl_easy_init;
    curl_easy_cleanup* curl_easy_cleanup;
    curl_easy_setopt* curl_easy_setopt;
    curl_easy_perform* curl_easy_perform;
    curl_easy_getinfo* curl_easy_getinfo;
    curl_easy_strerror* curl_easy_strerror;
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_init", &curl_easy_init);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_cleanup", &curl_easy_cleanup);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_setopt", &curl_easy_setopt);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_perform", &curl_easy_perform);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_getinfo", &curl_easy_getinfo);
    OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_strerror", &curl_easy_strerror);

    CURL* curl_handle = curl_easy_init();
    if (!curl_handle) PRINT("Failed to initialize curl");
    else {
        size_t bytes_written = 0;
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://google.com");
        //curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &curl_cb);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &bytes_written);

        CURLcode status = curl_easy_perform(curl_handle);
        if (status != CURLE_OK) {
            PRINT("curl failed: ", curl_easy_strerror(status)); //! currently failing here
        } else {
            long response_code = 404;
            curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &response_code);
            if (response_code != 200) PRINT("curl http not ok: ", response_code);
            else {
                curl_easy_cleanup(curl_handle);
                PRINT("curl success: (", bytes_written, ") = ", curl_data, "\0");
            }
        }
    }

    PRINT("OSDynLoad_Acquire address: ", LogColor::Yellow, fmt::hex, OS_SPECIFICS->addr_OSDynLoad_Acquire);
    PRINT("OSDynLoad_FindExport address: ", LogColor::Yellow, fmt::hex, OS_SPECIFICS->addr_OSDynLoad_FindExport);

    PRINT(LogColor::Green, "Custom code initialization complete!");
}
