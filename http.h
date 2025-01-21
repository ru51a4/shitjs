#include <iostream>
#include <string>
#include <curl/curl.h>
namespace _http
{
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

    static std::string get(std::string _url)
    {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            return readBuffer;
        }
        return "";
    }
}