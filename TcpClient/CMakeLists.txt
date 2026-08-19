cmake_minimum_required (VERSION 3.22)

set(PROJECT_NAME "tcp-client")

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

project(${PROJECT_NAME}
  LANGUAGES CXX
  VERSION 1.0.0
)

# https://github.com/ArthurSonzogni/ftxui-starter/blob/master/CMakeLists.txt

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    )

# Fix warnings for now
# I got this working.
# https://stackoverflow.com/questions/67297878/how-to-remove-a-specific-compiler-warning-in-cmake

include(CheckCCompilerFlag)
check_c_compiler_flag(-Wdeprecated-declarations HAVE_DEPRECATED_DECLARATIONS)

# Disable -Wdeprecated-declarations for RSA warnings for now.
if (HAVE_DEPRECATED_DECLARATIONS)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-declarations)
endif (HAVE_DEPRECATED_DECLARATIONS)

# add_subdirectory(lib/base64)

# For removing all warnings, this is probably not a good idea.
# target_compile_options(${PROJECT_NAME} PRIVATE -w)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    src/util
    # lib/base64
    )

# Add OpenSSL for project.
# https://stackoverflow.com/questions/45548088/include-openssl-in-a-cmakelist-txt-file
if(UNIX)
    find_package(OpenSSL REQUIRED)
    # find_package(TinyRSA REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL
        base64
        # RSA
        # TODO Fix this library.
        # TinyRSA
        # socket
    )
endif()

# target_link_libraries(${PROJECT_NAME}
    
# )
