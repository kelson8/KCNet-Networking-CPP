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

# This turns on the CURL_TEST preprocessor and enables testing with Curl with TcpClient.
# Curl can be a pain to get working on Windows for me, so by default this is disabled.
option(ENABLE_CURL "Enable the curl testing" OFF)

# TODO Move this option into here, this breaks my build for some reason.
# This will toggle the WxWidgets GUI for TcpClient, for Docker and CLI use this should be disabled.
# option(ENABLE_GUI "Enable the WxWidgets GUI for TcpClient" ON)

# Enable curl test
# https://stackoverflow.com/questions/34937671/cmake-how-to-check-if-preprocessor-is-defined
# https://stackoverflow.com/questions/9017573/define-a-preprocessor-macro-through-cmake
if(ENABLE_CURL)
    add_compile_definitions(CURL_TEST=1)
endif()

# Enable the GUI
# if(ENABLE_GUI)
#     add_compile_definitions(WXWIDGETS_GUI=1)
# endif()

# TODO Make this not overwrite files
# https://www.studyplan.dev/cmake/cmake-working-with-the-file-system

## Copying files into build folder
# file(GLOB EXTRA_FILES
    # "${CMAKE_CURRENT_SOURCE_DIR}/images/KCNet-Networking-CPP.png"
    # )

# foreach(EXTRA_FILE ${EXTRA_FILES})
    # file(COPY ${EXTRA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# endforeach()

# Copy logo into build folder
file(COPY ${CMAKE_SOURCE_DIR}/images/KCNet-Networking-CPP.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources)

###

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

# https://forum.cocosengine.org/t/v4-0-pc-resources-folder/48625/11
# This should allow me to just symlink things like images to the build folder.
# TODO Figure this out
# if(LINUX OR WINDOWS)
#     add_custom_command(
#         TARGET ${PROJECT_NAME}
#         PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
#         create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/Resources)
# endif()

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


# if(ENABLE_GUI)
    # Added wxwidgets, this is cross platform so I'll include it in all systems.
    # https://docs.wxwidgets.org/latest/overview_cmake.html
    find_package(wxWidgets REQUIRED COMPONENTS net core base)
    if(wxWidgets_USE_FILE) # not defined in CONFIG mode
        include(${wxWidgets_USE_FILE})
    endif()
# endif()

# Add curl for testing
# https://stackoverflow.com/questions/15657931/linking-curl-in-a-project-using-cmake
if(ENABLE_CURL)
    find_package(CURL REQUIRED)
endif()

# Add OpenSSL for project.
# https://stackoverflow.com/questions/45548088/include-openssl-in-a-cmakelist-txt-file
if(UNIX)
    find_package(OpenSSL REQUIRED)
endif()
    # find_package(TinyRSA REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL
        base64
        # RSA
        # TODO Fix this library.
        # TinyRSA
        # socket
        fmt::fmt
        # Cpp simple uri
        # cppsimpleuri

        # Tiny AES
        # tiny-aes
    )

if(WIN32)
	find_package(OpenSSL REQUIRED COMPONENTS Crypto)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
	OpenSSL::Crypto)
endif()

# Extra libraries

# if(ENABLE_GUI)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${wxWidgets_LIBRARIES}
        )
# endif()

if(ENABLE_CURL)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        CURL::libcurl
    )
endif()
