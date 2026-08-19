#pragma once

#include <iostream>
#include <string>

class TomlHandler {
public:
    static TomlHandler &getInstance()
    {
        static TomlHandler instance; // Guaranteed to be destroyed.
        return instance;
    }

    void TestTomlFile();


private:
    TomlHandler();
    // ~Commands(); // Optional deconstructor
    TomlHandler(const TomlHandler &) = delete;            // Prevent copy-construction
    TomlHandler &operator=(const TomlHandler &) = delete; // Prevent assignment

};