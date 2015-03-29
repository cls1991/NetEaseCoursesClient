#include <curl/curl.h>

#include "httpClient.h"

HttpClient::HttpClient() {
}

HttpClient::~HttpClient() {
}


static size_t OnWriteData(void *buffer, size_t size, size_t nmemb, void *lpvoid) {
    std::string *str = dynamic_cast<std::string *>((std::string *)lpvoid);
    if (NULL == str || NULL == buffer) return -1;
    char *pData = (char *)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

int HttpClient::get(const std::string & strCurl, std::string & strResponse) {
    CURLcode res;
    CURL *curl = curl_easy_init();
    if (NULL == curl) return CURLE_FAILED_INIT;
    curl_easy_setopt(curl, CURLOPT_URL, strCurl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return res;
}
