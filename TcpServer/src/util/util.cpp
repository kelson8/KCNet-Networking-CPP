#include "util.h"

#include <string>
#include "defines.h"

#include <random>
#include <openssl/rand.h>

// Linux specific headers, I may use this Util file on other systems.
// This should also work between these other systems too.
#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/utsname.h>
#endif

Util::Util()
{
    isServerRunning = false;
}

/**
 * Get the program server status.
 */
const bool Util::GetServerRunning() const 
{
    return isServerRunning;
}

/**
 * Set the TCP Server status, if this is false it stops the program loop.
 */
void Util::SetServerRunning(bool status)
{
    isServerRunning = status;
}

/**
 * Generate random numbers with a range.
 * 
 * https://www.geeksforgeeks.org/cpp/how-to-generate-random-number-in-range-in-cpp/
 * 
 * @param min The minimum number to generate.
 * @param max The maximum number to generate.
 */
const int Util::GenerateRandomNumber(int min, int max)
{
    // Initialize a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
    // return rand() % max;
}

/**
 * Generate random number with OpenSSL
 * https://stackoverflow.com/questions/39635053/getting-random-number-using-openssl
 * 
 * TODO Fix this to work.
 * 
 * @param amountOfNumbers
 */
// void random_number_ssl(int amountOfNumbers)
// {
//     std::vector<unsigned char> buf(amountOfNumbers);
//     int ret = RAND_bytes(buf.data(), buf.size());
//     if (ret != 1) {
//         std::cerr << "RAND_bytes failed!" << std::endl;
//         ERR_print_errors_fp(stderr);
//         return;
//     }

//     // std::cout << std::hex << std::setw(2) << std::setfill('0');
    
//     for (const auto& c : buf) {
//         std::cout << (unsigned) c;
//     }
//     std::cout << std::endl;
// }

#ifdef __linux__

/**
 * Get the PID of the current program
 * https://www.systutorials.com/how-to-get-the-running-process-pid-in-c-cpp/
 * 
 */
int Util::GetProgramPid()
{
    // Get the current PID
    pid_t pid = getpid();
    return pid;
}

#endif // __linux__

// TODO Look into this for getting Windows current version and other info
// https://github.com/0x73616D/Get-System-Info

/**
 * Basic test for viewing the kernel and other info in my own function.
 * 
 * TODO Try to make this also work for getting the Windows OS info if _WIN32 is defined.
 * 
 * https://lindevs.com/get-linux-kernel-version-using-cpp
 * 
 * @return The kernel info from the specified OsInfo enum value.
 */
#ifdef __linux__
std::string Util::GetOsInfo(OsInfo kernelInfo)
#else
std::string Util::GetOsInfo()
#endif // __linux__
{
#ifdef __linux__
    struct utsname osInfo{};
    uname(&osInfo);

    switch(kernelInfo)
    {
        // The name of the system, such as Linux.
        case OsInfo::SYSNAME:
            return osInfo.sysname;

        // System hostname
        case OsInfo::HOSTNAME:
            return osInfo.nodename;

        // Kernel version
        case OsInfo::KERNEL_VERSION:
            return osInfo.release;

        // Seems to print the date and other stuff that normally gets print in the console when starting it up.
        case OsInfo::VERSION:
            return osInfo.version;
        
        // Arch type, such as x86_64.    
        case OsInfo::ARCH_TYPE:
            return osInfo.machine;

        default:
            return "Invalid option provided";
    }

    return "Nothing set";

#else
    return "This only works with Linux!";
#endif // __linux__
}


