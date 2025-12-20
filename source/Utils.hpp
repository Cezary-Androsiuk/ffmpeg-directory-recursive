#ifndef MAIN_METHODS_HPP
#define MAIN_METHODS_HPP

#include <string>
#include <vector>
#include <filesystem>

#include "enums/SkipAction.hpp"

namespace fs = std::filesystem;
typedef std::string str;
typedef std::wstring wstr;
typedef const str &cstr;
typedef const wstr &cwstr;
typedef std::vector<str> vstr;
typedef const fs::path &cpath;
typedef std::vector<fs::path> vpath;


extern wstr lastError;
extern const char possibleSeparators[];

#define FUNC_START {lastError.clear();}

#define DEFAULT_PATH fs::current_path()
#define DEFAULT_EXTENSIONS {"mkv", "mp4"}
#define DEFAULT_SKIP_ACTION SkipAction::Force

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define TEXT_BOLD     "\033[1m"

class Utils{
public:
    static vstr splitStringByChar(cstr str, char separator);
    static vstr splitExtensionsInput(str input);
    static void stringTolower(str &string);
    static SkipAction handleInputSkipAction(str input);
    static bool handleArgs(int argc, const char **argv, void *arguments[]);
    static bool argsValid(int argc, const char **argv, fs::path *const directory, vstr *const extensions, SkipAction *const skipAction);
    
    static bool isDirectoryEmpty(fs::path directory);
    static bool createDirectoryIfValid(fs::path outDirectory);
    static bool copyStructureOfFolders(fs::path sourceDir, fs::path targetDir);
    
    static fs::path createOutputDirectory(cpath inputDirectory, bool removeDirIfExist = false);
    static fs::path createOCFDirectory(cpath inputDirectory, bool removeDirIfExist = false); // OCFDirectory is OutputCompletedFilesDirectory
    
    static void printStatusInfo(SkipAction skipAction);
    
    static fs::path createOutputFile(cpath inFile, cpath inDirectory, cpath outDirectory);
    static fs::path createOFCFile(cpath inFile, cpath inDirectory, cpath OFCDirectory);
    
    static void deleteDirectoryIfEmpty(fs::path outDirectory);
    static bool rm_all(const fs::path& path);
};



#endif // MAIN_METHODS_HPP