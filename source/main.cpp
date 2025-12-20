#include "Program.hpp"

// locate files recursive

// test commands:
// xcopy .\tests\backupenv\test\ .\tests\test\ /E /H /Q
// ffmpegRec .\tests\test avi+mkv+mp4+ move
// rd /S /Q .\tests\test
// rd /S /Q .\tests\test-ffmpeg-h.265
// rd /S /Q .\tests\test-finished_source_files

// prepare test env
// rd /S /Q .\tests\test && rd /S /Q .\tests\test-ffmpeg-h.265 && rd /S /Q .\tests\test-finished_source_files && xcopy .\tests\backupenv\test\ .\tests\test\ /E /H /Q


// assuming that ffmpeg is already installed
// instalation(add ffmpegRec.exe to PATH environment):
// 1 in windows search type "Edit the system environment variables" 
// 2 press "Environment Variables..." button
// 3 in bottom part (System variables) find variable named "Path" and double click on it
// 4 press on the right site the "New" button and type path to directory, where executable file (created after compilation) is located
// in example path to this executable (for now) is "D:\vscode\c++\projects\FFmpegDirectoryRecursive(github)"
// now, you can open cmd in any directory and just in command prompt type "ffmpegRec . mkv+mp4 move"

/// compilation
// cd build
// cmake -G "MinGW Makefiles" .. 
// cmake --build .


int main(int argc, const char **argv)
{
    return Program::run(argc, argv);
}