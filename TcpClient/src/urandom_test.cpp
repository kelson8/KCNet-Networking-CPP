#include "urandom_test.h"

#include <iostream>
#include <cstdint> // include this header for uint64_t
#include <sstream>
#include <iomanip>

#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <string>

#include <vector>

#endif // __linux__

// Base64 library
// https://github.com/tobiaslocker/base64

#include "base64.hpp"
// #include "../lib/base64/base64.hpp"

// https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
// https://www.geeksforgeeks.org/cpp/stringstream-c-applications/
template <typename T>
std::string ValueToHex(T i)
{
    std::stringstream stream;
    stream << "0x"
           //  << std::setfill ('0') << std::setw(sizeof(T)*2)
           << std::setfill('0') << std::setw(2)
           << std::hex << i;
    return stream.str();
}

#ifdef __linux__

//--------
// New Urandom generation.
// Generated with AI
//--------

/**
 * Get a random string from the alphabet
 */
std::string Urandom::random_from_alphabet(Urandom& rng, size_t count, std::string_view alphabet) 
{
    if (alphabet.empty()) throw std::invalid_argument("alphabet must not be empty");
    std::string out(count, '\0');

    std::vector<unsigned char> buf(count);
    rng.read_exact(buf.data(), buf.size());

    for (size_t i = 0; i < count; i++) {
        out[i] = alphabet[buf[i] % alphabet.size()];
    }
    return out;
}

/**
 * Generate random passwords with the set amout of numbers, uppercase letters and symbols
 * 
 * @param rng The Urandom instance.
 * @param numNumbers The amount of numbers for the password. 
 * @param numLetters The number of letters for the password.
 * @param numSymbols The amount of symbols for the password. 
 */
std::string Urandom::random_password_counts(Urandom& rng,
                                    size_t numNumbers,
                                    size_t numLetters,
                                    size_t numSymbols)
{
    const std::string digits   = "0123456789";
    const std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::string symbols  = "!@#$%^&*()-_=+[]{};:,.?";

    std::string a;
    a += random_from_alphabet(rng, numNumbers, digits);
    a += random_from_alphabet(rng, numLetters, letters);
    a += random_from_alphabet(rng, numSymbols, symbols);

    // Shuffle characters so categories are mixed
    // We'll generate indices from urandom and do Fisher-Yates.
    for (size_t i = a.size(); i > 1; --i) {
        uint64_t x;
        rng.read_exact(&x, sizeof(x));
        size_t j = static_cast<size_t>(x % i); // 0..i-1
        std::swap(a[i - 1], a[j]);
    }
    return a;
}

//


// Urandom generation
uint64_t Urandom::urandom_u64()
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) throw std::runtime_error("open(/dev/urandom) failed");

    uint64_t x;
    ssize_t got = read(fd, &x, sizeof(x));
    close(fd);

    if (got != (ssize_t)sizeof(x)) throw std::runtime_error("read(/dev/urandom) failed");
    return x;
}

/**
 * Urandom number generator.
 */
uint64_t Urandom::urandom_bounded(uint64_t maxExclusive)
{
    if (maxExclusive == 0) throw std::invalid_argument("maxExclusive must be > 0");

    // Simple approach: use 64-bit random, then modulo.
    // Good enough for many non-cryptographic uses; if you need strict uniformity,
    // use rejection sampling.
    return urandom_u64() % maxExclusive;
}

/**
 * Generate a basic string with Urandom.
 */
std::string Urandom::urandom_string(size_t length, std::string_view alphabet)
{
    if (alphabet.empty()) return {};

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) throw std::runtime_error("open(/dev/urandom) failed");

    std::string out;
    out.resize(length);

    std::vector<unsigned char> buf(length);
    ssize_t got = read(fd, buf.data(), buf.size());
    close(fd);

    if (got != (ssize_t)buf.size()) throw std::runtime_error("read(/dev/urandom) failed");

    for (size_t i = 0; i < length; i++) {
        out[i] = alphabet[buf[i] % alphabet.size()];
    }
    return out;
}

//--------
// End Generated with AI
//--------

//
#define RANDOM_BYTES 16

/**
 * Urandom Test, TODO Fix this.
 * https://en.ittrip.xyz/c-language/secure-random-c-linux
 * Well this is using some C code for now.
 * @param rng The Urandom instance.
 * @param numNumbers The amount of numbers for the password. 
 * @param numLetters The number of letters for the password.
 * @param numSymbols The amount of symbols for the password. 
 */
std::string Urandom::GenerateRandom(size_t numNumbers, size_t numLetters, size_t numSymbols)
{
    int fd;
    unsigned char buffer[RANDOM_BYTES];

    // Open /dev/urandom
    fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open /dev/urandom");
        return "Failed to open";
        // return EXIT_FAILURE;
    }

    // Read random bytes
    if (read(fd, buffer, RANDOM_BYTES) != RANDOM_BYTES) {
        perror("Failed to read random bytes");
        close(fd);
        return "Failed to read random bytes";
        // return EXIT_FAILURE;
    }

    // const char* s;
    // strcpy(buffer, s);

    auto encoded = base64::to_base64(buffer, RANDOM_BYTES);

    // std::cout << encoded << std::endl;  

    // Random number generator
    // uint64_t x = urandom_bounded(10);  // 0..9
    // std::cout << "Random number: " << x << std::endl;

    // Random text generator
    // TODO Make this generate a set amount of letters for this
    // TODO Make this only generate the specified amount of symbols, numbers, and uppercase letters.
    auto lower = urandom_string(12, "abcdefghijklmnopqrstuvwxyz");
    auto upper = urandom_string(12, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    auto digits = urandom_string(12, "0123456789");

    auto allnum = urandom_string(16, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    auto allsymbol = urandom_string(16, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-={}[]|\'';,./<>?");

    // std::cout << "Random text: " << allsymbol << std::endl;

    Urandom rng;

    // Example: 4 digits, 3 letters, 2 symbols (total length = 9)
    // std::string s = random_password_counts(rng, 4, 3, 2);
    // std::string s = random_password_counts(rng, 8, 16, 4);
    std::string s = random_password_counts(rng, numNumbers, numLetters, numSymbols);
    return s;
    // std::cout << s << "\n";

    // auto decoded = base64::from_base64("SGVsbG8sIFdvcmxkIQ==");  
    // std::cout << decoded << std::endl; // Hello, World!  

    // Print random bytes in hexadecimal
    // printf("Random Bytes: ");
    // for (int i = 0; i < RANDOM_BYTES; i++) {
    //     // printf("%02x ", buffer[i]);
    //     printf("%02x ", buffer[i]);
    // }
    // printf("\n");

    // Close the file descriptor
    close(fd);

    // TODO Test this
    // Code shouldn't reach here.
    return "";

    // return 0;
}

void Urandom::Run()
{

    // Urandom urandom;
    // https://stackoverflow.com/questions/2572366/how-to-use-dev-random-or-urandom-in-c
    // char myRandomData[50];
    // arc4random_buf(myRandomData, sizeof myRandomData); // done!
    // std::cout << "Random data: " << std::hex << ValueToHex(myRandomData)  << std::endl;
    // std::cout << "Random data: " << ValueToHex(myRandomData) << std::endl;
    // std::cout << "Random data: " << GenerateRandom() << std::endl;
    // urandom.GenerateRandom();
    // GenerateRandom();

    // ValueToHex(myRandomData);



}

#endif // __linux__
