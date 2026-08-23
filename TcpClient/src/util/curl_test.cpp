#include "curl_test.h"

#include <fstream>

#ifdef CURL_TEST

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

// https://github.com/hectorlin/cmake_sample/blob/master/src/curl_wrapper.cpp

// https://dev.to/secure_daily/using-libcurl-in-c-c-application-4668

// Mostly taken from this guide here.
// https://terminalroot.com/using-curl-with-cpp/

CurlTest::CurlTest()
{
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

/**
 * Write text to a text file.
 *
 * @param textFile The file to write into.
 * @param text The text to write into the file.
 */
void WriteToTextFile(std::string &textFile, std::string &text)
{
    // This works as a basic way to write to a text file in C++.
    // Hmm, maybe I should make this into a function.
    // https://cplusplus.com/doc/tutorial/files/
    std::ofstream file;
    file.open(textFile);
    file << text;
    file.close();
}

/**
 * Check if a URL is online
 * 
 * TODO Fix this to work.
 * https://stackoverflow.com/questions/20773634/curl-checking-connection
 * https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
 */
void IsUrlOnline(std::string url)
{
    CURL *curl;
    CURLcode res;
    // std::string readBuffer;

    // TODO Try to fix this to ping a website to check if its online.
    // If I can get this working, I'll try to use it to check if the local server is online.
    // If TcpServer is offline, the send server message options should be disabled in the UI.
    if (curl)
    {
        std::cout << "Checking if " << url << " is online...." << std::endl;
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_perform(curl);

        // OK, now we are connected (if nothing bad happened),
        // but it would be nice to communicate with the server:
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 0L);
        std::cout << "Url " << url << " is online" << std::endl;
    }
}

/**
 * This works for outputting the HTML contents of a webpage into an HTML file.
 */
void CurlTest::TestUrlRequest(std::string url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    // The html file to write to.
    std::string htmlFile = "test.html";

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // Print the html file output.
        // std::cout << readBuffer << std::endl;

        // Write the text from curl into the file.
        WriteToTextFile(htmlFile, readBuffer);

        // IsUrlOnline("https://google.com");
    }
}

#endif // CURL_TEST
