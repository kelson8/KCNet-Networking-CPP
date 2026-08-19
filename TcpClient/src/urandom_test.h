#pragma once

#ifdef __linux__

#include <iostream>

#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>

#include <cstdint> // include this header for uint64_t

// #include <fcntl.h>
#include <unistd.h>
// #include <stdexcept>
// #include <string>

#include <vector>

#endif // __linux__

class Urandom
{
public:
    Urandom()
    {
        fd_ = open("/dev/urandom", O_RDONLY);
        if (fd_ < 0)
            throw std::runtime_error("open(/dev/urandom) failed");
    }
    ~Urandom()
    {
        if (fd_ >= 0)
            close(fd_);
    }

    void read_exact(void *dst, size_t n)
    {
        unsigned char *p = static_cast<unsigned char *>(dst);
        size_t done = 0;
        while (done < n)
        {
            ssize_t r = read(fd_, p + done, n - done);
            if (r <= 0)
                throw std::runtime_error("read(/dev/urandom) failed");
            done += static_cast<size_t>(r);
        }
    }

    std::string GenerateRandom(size_t numNumbers, size_t numLetters, size_t numSymbols);
    void Run();

private:
    std::string random_password_counts(Urandom &rng,
                                       size_t numNumbers,
                                       size_t numLetters,
                                       size_t numSymbols);

    std::string random_from_alphabet(Urandom& rng, size_t count, std::string_view alphabet);
    uint64_t urandom_u64();
    uint64_t urandom_bounded(uint64_t maxExclusive);
    std::string urandom_string(size_t length, std::string_view alphabet);


    int fd_{-1};
};

#endif // __linux__