#include <string>


class HttpClient {
public:
    HttpClient();
    ~HttpClient();

public:
    int get(const std::string & stdUrl, std::string & strResponse);
};
