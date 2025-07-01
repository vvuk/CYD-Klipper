#include "HTTPClient.h"
#include "httplib.h"
#include <sstream>

void HTTPClient::useHTTP10(bool enable) {
    _http10 = enable;
}

HTTPClient::HTTPClient()
    : _timeout_ms(5000), _connection_timeout_ms(5000), _status_code(-1), _http10(false) {}

HTTPClient::~HTTPClient() {}

bool HTTPClient::begin(const std::string& url) {
    _url = url;
    _headers.clear();
    _status_code = -1;
    _response.clear();
    return true;
}

void HTTPClient::addHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void HTTPClient::setTimeout(uint32_t timeout_ms) {
    _timeout_ms = timeout_ms;
}

void HTTPClient::setConnectTimeout(uint32_t timeout_ms) {
    _connection_timeout_ms = timeout_ms;
}

int HTTPClient::GET() {
    if (_url.empty()) return -1;
    httplib::Client cli(_url.c_str());
    cli.set_connection_timeout(_connection_timeout_ms / 1000, (_connection_timeout_ms % 1000) * 1000);
    cli.set_read_timeout(_timeout_ms / 1000, (_timeout_ms % 1000) * 1000);
    if (_http10) {
        cli.set_keep_alive(false);
        cli.set_default_headers({{"Connection", "close"}});
    }
    httplib::Headers headers;
    for (const auto& h : _headers) headers.emplace(h.first, h.second);
    auto res = cli.Get("/", headers);
    if (res) {
        _status_code = res->status;
        _response = res->body;
    } else {
        _status_code = -1;
        _response.clear();
    }
    return _status_code;
}

int HTTPClient::POST(const std::string& payload) {
    if (_url.empty()) return -1;
    httplib::Client cli(_url.c_str());
    cli.set_connection_timeout(_connection_timeout_ms / 1000, (_connection_timeout_ms % 1000) * 1000);
    cli.set_read_timeout(_timeout_ms / 1000, (_timeout_ms % 1000) * 1000);
    if (_http10) {
        cli.set_keep_alive(false);
        cli.set_default_headers({{"Connection", "close"}});
    }
    httplib::Headers headers;
    for (const auto& h : _headers) headers.emplace(h.first, h.second);
    auto res = cli.Post("/", headers, payload, "application/x-www-form-urlencoded");
    if (res) {
        _status_code = res->status;
        _response = res->body;
    } else {
        _status_code = -1;
        _response.clear();
    }
    return _status_code;
}

StringStream HTTPClient::getStream() const {
    return StringStream(_response);
}

int HTTPClient::getStatusCode() const {
    return _status_code;
}

std::string HTTPClient::getString() const {
    return _response;
}
