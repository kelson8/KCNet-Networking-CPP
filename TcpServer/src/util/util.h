#pragma once

#include <iostream>

#ifdef __linux__
#include <unistd.h>
#endif // __linux__

class Util
{
public:
    static Util &getInstance()
    {
        static Util instance; // Guaranteed to be destroyed.
        return instance;
    }

#ifdef __linux__
    enum class OsInfo {
        SYSNAME,
        HOSTNAME,
        KERNEL_VERSION,
        VERSION,
        ARCH_TYPE
    };
#endif // __linux__

#ifdef __linux__
    std::string GetOsInfo(OsInfo kernelInfo);
#else
    std::string GetOsInfo();
#endif // __linux__

    const bool GetServerRunning() const;
    void SetServerRunning(bool status);
    const int GenerateRandomNumber(int min, int max);

private:
    Util();
    // ~Util(); // Optional deconstructor
    Util(const Util &) = delete;            // Prevent copy-construction
    Util &operator=(const Util &) = delete; // Prevent assignment

    bool isServerRunning;

#ifdef __linux__
    static int GetProgramPid();
#endif // __linux__
};
