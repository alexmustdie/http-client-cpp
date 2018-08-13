#include <http-client/http-client.hpp>

#include <iostream>
#include <stdexcept>

using namespace http_client;
using namespace std;

HttpClient::HttpClient(const map<string, string> &headers):
  curl_(curl_easy_init()),
  headers_(nullptr)
{
  headers_ = curl_slist_append(headers_, "accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
  headers_ = curl_slist_append(headers_, "content-type: application/x-www-form-urlencoded");
  headers_ = curl_slist_append(headers_, "user-agent: Mozilla/5.0 (X11; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0");

  for (auto header = headers.begin(); header != headers.end(); ++header)
  {
    headers_ = curl_slist_append(headers_, (header->first + ": " + header->second).c_str());
  }
}

HttpClient::~HttpClient()
{
  curl_easy_cleanup(curl_);
  curl_slist_free_all(headers_);
}

size_t writeCurlToString(void *contents, size_t size, size_t nmemb, string *s)
{
  size_t old_length = s->size();
  size_t new_length = size * nmemb;
  
  try
  {
    s->resize(old_length + new_length);
  }
  catch (const bad_alloc &e)
  {
    cerr << e.what() << "\n";
    return 1;
  }

  copy((char*)contents, (char*)contents + new_length, s->begin() + old_length);

  return size * nmemb;
}

string HttpClient::makeRequest(const string &url) const
{
  CURLcode res;
  string data;
  
  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_HEADER, 0);
  curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT, 30);
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeCurlToString);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_);

  res = curl_easy_perform(curl_);

  if (res != CURLE_OK)
  {
    throw runtime_error(curl_easy_strerror(res));
  }

  std::cout << data << std::endl;

  return data;
}

string HttpClient::buildQuery(const map<string, string> &params) const noexcept
{
  string data;

  for (auto param = params.begin(); param != params.end(); ++param)
  {
    data += param->first + "=" + encodeUrl(param->second);

    if (param != --params.end())
    {
      data += "&";
    }
  }

  return data;
}

string HttpClient::encodeUrl(const string &s) const noexcept
{
  char *encoded_str = curl_easy_escape(curl_, s.c_str(), s.size());

  if (encoded_str)
  {
    curl_free(encoded_str);
  }

  return encoded_str;
}
