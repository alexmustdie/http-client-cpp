#ifndef HTTP_CLIENT_HPP_INCLUDED
#define HTTP_CLIENT_HPP_INCLUDED

#include <string>
#include <map>
#include <curl/curl.h>

namespace http_client
{
  class HttpClient
  {
  public:
    HttpClient(const std::map<std::string, std::string> &headers = {});
    ~HttpClient();
    std::string makeRequest(const std::string &url) const;
    std::string buildQuery(const std::map<std::string, std::string> &params) const noexcept;
    std::string encodeUrl(const std::string &s) const noexcept;

  private:
    curl_slist *headers_;
  };
}

#endif
