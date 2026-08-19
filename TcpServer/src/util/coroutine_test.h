#pragma once
#include <coroutine>
#include <iostream>

#include <thread>

struct MyCoroutine {
    struct promise_type {
        MyCoroutine get_return_object() { 
            return MyCoroutine(std::coroutine_handle<promise_type>::from_promise(*this)); 
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { 
            std::terminate(); 
        }
    };

    std::coroutine_handle<promise_type> handle;

    MyCoroutine(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~MyCoroutine() { 
        if (handle) handle.destroy(); 
    }

    void resume() { 
        if (handle) handle.resume(); 
    }
};

// Coroutine awaitable

class CustomAwaitable {
public:
    bool await_ready() const noexcept { 
        return false; // Always suspend
    }

    void await_suspend(std::coroutine_handle<> h) const noexcept {
        // Schedule the coroutine to be resumed later
        std::thread([h]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            h.resume();
        }).detach();
    }

    void await_resume() const noexcept {}

    void TestCoroutine();
};
