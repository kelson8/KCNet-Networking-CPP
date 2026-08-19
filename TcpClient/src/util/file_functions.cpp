#include "file_functions.h"
#include <filesystem>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#endif // _WIN32

#include <sstream>
#include <iostream>
#include <fstream>

// #include <toml.hpp>
#include <map>

FileFunctions::FileFunctions() 
{

}


/**
 * Check if a file exists
 * 
 * @param fileName The name of the file to check.
 */
bool FileFunctions::FileExists(const std::string &fileName)
{
    return std::filesystem::exists(fileName);
}

/**
 * Read the text from a text file.
 */
std::string FileFunctions::ReadFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::ostringstream content;
    content << file.rdbuf(); // Read file into content
    return content.str();
}

/**
 * Get the current program directory
 * Taken from my local testing projects.
 */
std::string FileFunctions::GetWorkingDirectory()
{
    const size_t size = 1024;
    char buffer1[size];

    // Call _getcwd to get the current working directory and store it in buffer
#ifdef _WIN32
    if (_getcwd(buffer1, size) != NULL)
#else
    if (getcwd(buffer1, size) != NULL)
#endif
    {
        //std::cout << "Current working directory: " << buffer1 << std::endl;
        return std::string(buffer1);
    }
    else
    {
        std::cerr << "Error getting current working directory" << std::endl;
        return "";
    }
}
