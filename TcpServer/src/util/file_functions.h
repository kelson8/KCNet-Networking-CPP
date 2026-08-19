#pragma once

#include <iostream>
#include <string>

class FileFunctions {
public:
    static FileFunctions &getInstance()
    {
        static FileFunctions instance; // Guaranteed to be destroyed.
        return instance;
    }

    bool FileExists(const std::string &fileName);

    std::string GetWorkingDirectory();

private:
    FileFunctions();
    // ~Commands(); // Optional deconstructor
    FileFunctions(const FileFunctions &) = delete;            // Prevent copy-construction
    FileFunctions &operator=(const FileFunctions &) = delete; // Prevent assignment

};