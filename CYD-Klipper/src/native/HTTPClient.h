#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#include "Arduino.h"

class HTTPClient {
public:
    // Set HTTP version to 1.0 (true) or 1.1 (false); stream enables forced HTTP/1.0
    void useHTTP10(bool enable);
    HTTPClient();
    ~HTTPClient();

    // Start a new request
    bool begin(const std::string& url);
    void end() {}

    // Add a header to the request
    void addHeader(const std::string& name, const std::string& value);

    // Set overall timeout (ms)
    void setTimeout(uint32_t timeout_ms);
    // Set connection timeout (ms)
    void setConnectTimeout(uint32_t timeout_ms);

    // GET request
    int GET();
    // POST request
    int POST(const std::string& payload);

    // Get response stream (raw body)
    StringStream getStream() const;

    // Get HTTP status code
    int getStatusCode() const;
    // Get response body as string
    std::string getString() const;

    int getSize() const { return _response.size(); }

private:
    std::string _url;
    std::string _base_url;
    std::string _path;
    std::map<std::string, std::string> _headers;
    uint32_t _timeout_ms;
    uint32_t _connection_timeout_ms;
    int _status_code;
    std::string _response;
    bool _http10;
};

#endif // HTTP_CLIENT_H