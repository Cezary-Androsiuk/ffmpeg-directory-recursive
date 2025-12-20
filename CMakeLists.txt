cmake_minimum_required(VERSION 3.10)

project(FFmpegDirectoryRecursive VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCES
    source/main.cpp
    source/Program.cpp
    source/BetterConversion.cpp
    source/ChangeFileDate.cpp
    source/FFExecute.cpp
    source/FFmpegCommand.cpp
    source/FFTester.cpp
    source/HandlePipeOutput.cpp
    source/ListMaker.cpp
    source/Utils.cpp
    source/ErrorInfo.cpp
    source/TestPipe.cpp
    source/WinConsoleHandler.cpp
    source/CalculatorETA.cpp
)

set(HEADERS
    source/Program.hpp
    source/BetterConversion.hpp
    source/ChangeFileDate.hpp
    source/FFExecute.hpp
    source/FFmpegCommand.hpp
    source/FFTester.hpp
    source/HandlePipeOutput.hpp
    source/ListMaker.hpp
    source/Utils.hpp
    source/ErrorInfo.hpp
    source/TestPipe.hpp
    source/WinConsoleHandler.hpp
    source/enums/SkipAction.hpp
    source/CalculatorETA.hpp
)

add_executable(ffmpegRec ${SOURCES} ${HEADERS})

target_include_directories(ffmpegRec PRIVATE 
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/enums"
)
