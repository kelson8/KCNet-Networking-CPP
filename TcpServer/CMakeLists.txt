cmake_minimum_required (VERSION 3.22)

set(PROJECT_NAME "tcp-server")

# Get all source files
file(GLOB SOURCES
    "src/*.cpp"
    "src/util/*.cpp"
    )

# Get all headers
file(GLOB HEADERS
    "src/*.h"
    "src/util/*.h"
    )

# Copy config.toml into build folder
# TODO Why doesn't this work? The messages have the correct folders..
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.toml
    #  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# message(${CMAKE_CURRENT_SOURCE_DIR}/config.toml)
# message("Copied to ${CMAKE_CURRENT_BINARY_DIR}")

project(${PROJECT_NAME}
  LANGUAGES CXX
  VERSION 1.0.0
)

# https://github.com/ArthurSonzogni/ftxui-starter/blob/master/CMakeLists.txt

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-------------
# Cryptopp-modern
# TODO Fix this here
# https://github.com/cryptopp-modern/cryptopp-modern
#-------------

# include(FetchContent)

# FetchContent_Declare(cryptopp-modern
#   GIT_REPOSITORY https://github.com/cryptopp-modern/cryptopp-modern.git
#   GIT_TAG        2026.8.1
#   GIT_PROGRESS   TRUE
#   GIT_SHALLOW    TRUE
# )

# FetchContent_MakeAvailable(cryptopp-modern)

#-------------
# spdlog
# https://github.com/gabime/spdlog
#-------------

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    )

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    src/util
    )

# Add OpenSSL for project.
# https://stackoverflow.com/questions/45548088/include-openssl-in-a-cmakelist-txt-file
if(UNIX)
    find_package(OpenSSL REQUIRED)
    # find_package(cryptopp-modern REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        OpenSSL::SSL
        # cryptopp-modern
        base64
        tomlplusplus::tomlplusplus
        # Logging
        spdlog::spdlog
        # Formatting
        fmt::fmt
    )
endif()

# target_link_libraries(${PROJECT_NAME}
    
# )
