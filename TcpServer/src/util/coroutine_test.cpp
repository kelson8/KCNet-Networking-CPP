#include <coroutine>
#include <iostream>

#include <thread>

#include "coroutine_test.h"

// https://codelucky.com/cpp-coroutines/

// Coroutine promise

// TODO Test this for TcpServer, try to learn how coroutines work in C++20.
// I have mostly moved everything for this into coroutine_test.h to be used.

// Coroutine handle

MyCoroutine coro_function() {
    std::cout << "Coroutine started\n";
    co_await std::suspend_always{};
    std::cout << "Coroutine resumed\n";
    co_await std::suspend_always{};
    std::cout << "Coroutine finished\n";
}

MyCoroutine custom_await_coroutine() {
    std::cout << "Coroutine started\n";
    co_await CustomAwaitable{};
    std::cout << "Coroutine resumed after 1 second\n";
}

void CustomAwaitable::TestCoroutine()
{
    // Using coroutine handle
    auto coro = coro_function();
    std::cout << "Main function\n";
    coro.resume();
    std::cout << "Back in main\n";
    coro.resume();
}

