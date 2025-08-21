// httpclient.h
#pragma once

#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>
#include <thread>

class HttpClient {
public:
    HttpClient() = default;
    ~HttpClient() = default;

    // GET simple, non-bloquant (thread détaché)
    void getAsync(const std::string& url) {
        std::thread([url]() {
            get(url);
            }).detach();
    }

private:
    static bool parseUrl(const std::string& url, std::string& host, std::string& path, INTERNET_PORT& port) {
        size_t schemePos = url.find("http://");
        size_t start = (schemePos == 0) ? 7 : 0;

        port = 80; // par défaut
        size_t slashPos = url.find('/', start);
        size_t colonPos = url.find(':', start);

        if (colonPos != std::string::npos && colonPos < slashPos) {
            host = url.substr(start, colonPos - start);
            port = static_cast<INTERNET_PORT>(std::stoi(url.substr(colonPos + 1, slashPos - colonPos - 1)));
        }
        else {
            host = url.substr(start, slashPos - start);
        }

        if (slashPos == std::string::npos) {
            path = "/";
        }
        else {
            path = url.substr(slashPos);
        }

        return !host.empty();
    }

    static void get(const std::string& url) {
        std::string host, path;
        INTERNET_PORT port;
        if (!parseUrl(url, host, path, port)) {
            std::cout << "[HttpClient] URL invalide\n";
            return;
        }
        std::cout << "[HttpClient] Host: " << host << " | Port: " << port << " | Path: " << path << "\n";

        HINTERNET hSession = WinHttpOpen(L"HttpClient/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) { std::cout << "[HttpClient] WinHttpOpen failed\n"; return; }

        std::wstring wHost(host.begin(), host.end());
        HINTERNET hConnect = WinHttpConnect(hSession, wHost.c_str(), port, 0);
        if (!hConnect) { std::cout << "[HttpClient] WinHttpConnect failed\n"; WinHttpCloseHandle(hSession); return; }

        std::wstring wPath(path.begin(), path.end());
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wPath.c_str(),
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);
        if (!hRequest) { std::cout << "[HttpClient] WinHttpOpenRequest failed\n"; WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return; }

        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
            std::cout << "[HttpClient] WinHttpSendRequest failed\n";
        }

        if (!WinHttpReceiveResponse(hRequest, NULL)) {
            std::cout << "[HttpClient] WinHttpReceiveResponse failed\n";
        }

        DWORD bytesAvailable = 0;
        while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
            char* buffer = new char[bytesAvailable + 1];
            DWORD bytesRead = 0;
            WinHttpReadData(hRequest, buffer, bytesAvailable, &bytesRead);
            buffer[bytesRead] = 0;
            std::cout << "[HttpClient] Response: " << buffer << "\n";
            delete[] buffer;
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
    }
};
