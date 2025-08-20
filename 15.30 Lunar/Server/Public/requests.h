#include "../curl/curl.h"
#include <string>
#include <thread>

namespace Requests
{
    inline size_t WriteCallbackRequests(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    inline size_t returnData(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    inline void Get(const std::string& url)
    {
        std::thread([url = url]()
            {
                curl_global_init(CURL_GLOBAL_ALL);
                CURL* curl = curl_easy_init();
                if (!curl) {
                    curl_global_cleanup();
                    return;
                }

                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackRequests);

                std::string response_body;
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

                curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                curl_global_cleanup();
            }
        ).detach();
    }
}