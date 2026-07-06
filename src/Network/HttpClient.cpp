#include "HttpClient.hpp"
#include <curl/curl.h>
#include <iostream>

namespace Network {

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    void HttpClient::Initialize() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    void HttpClient::Terminate() {
        curl_global_cleanup();
    }

    std::string HttpClient::Get(const std::string& url) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Para simplificar CA en PS3
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        }
        return readBuffer;
    }

} // namespace Network
