#pragma once

#include "defines.h"
#include <iostream>
#include <string.h>

class CurlTest{
public:
    static CurlTest &getInstance()
    {
        static CurlTest instance; // Guaranteed to be destroyed.
        return instance;
    }

#ifdef CURL_TEST
    void TestUrlRequest(std::string url);
#endif // CURL_TEST

private:
    CurlTest();
    // ~Commands(); // Optional deconstructor
    CurlTest(const CurlTest &) = delete;            // Prevent copy-construction
    CurlTest &operator=(const CurlTest &) = delete; // Prevent assignment
};
