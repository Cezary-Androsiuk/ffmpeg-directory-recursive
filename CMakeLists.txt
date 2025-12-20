cmake_minimum_required(VERSION 3.10)

project(FFmpegDirectoryRecursive VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCES
    source/main.cpp
    source/BetterConversion.cpp
    source/ChangeFileDate.cpp
    source/FFExecute.cpp
    source/FFmpegCommand.cpp
    source/FFTester.cpp
    source/HandlePipeOutput.cpp
    source/ListMaker.cpp
    source/MainMethods.cpp
    source/OtherError.cpp
    source/TestPipe.cpp
    source/WinConsoleHandler.cpp
)

set(HEADERS
    source/BetterConversion.hpp
    source/ChangeFileDate.hpp
    source/FFExecute.hpp
    source/FFmpegCommand.hpp
    source/FFTester.hpp
    source/HandlePipeOutput.hpp
    source/ListMaker.hpp
    source/MainMethods.hpp
    source/OtherError.hpp
    source/TestPipe.hpp
    source/WinConsoleHandler.hpp
    source/enums/SkipAction.hpp
)

add_executable(ffmpegRec ${SOURCES} ${HEADERS})

target_include_directories(ffmpegRec PRIVATE 
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/enums"
)
