#include "Utils.hpp"

#include <cstdio>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm> // stringTolower
#include <cctype> // stringTolower

#include <unistd.h>
#include <fcntl.h>

#include "enums/SkipAction.hpp"
#include "BetterConversion.hpp"
#include "FFmpegCommand.hpp"

wstr lastError;
const char possibleSeparators[] = {',', '/', '\\', /*'|', */';', '+', '?'};

vstr Utils::splitStringByChar(cstr str, char separator) 
{
    // by Gemini
    vstr splited;
    size_t start = 0;
    size_t end = str.find(separator);

    while (end != str::npos) {
        splited.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(separator, start);
    }

    splited.push_back(str.substr(start, str::npos));

    return splited;
}

vstr Utils::splitExtensionsInput(str input)
{
    char usedSeparator;
    for(int i=0; i<sizeof(possibleSeparators); i++)
    {
        char testedSeparator = possibleSeparators[i];
        if(input.find_first_of(testedSeparator) != str::npos)
        {
            usedSeparator = testedSeparator;
            break;
        }
    }
    return splitStringByChar(input, usedSeparator);
}

void Utils::stringTolower(str &string)
{
    std::transform(string.begin(), string.end(), string.begin(), 
        [](unsigned char c){ return std::tolower(c); });
}

SkipAction Utils::handleInputSkipAction(str input)
{
    stringTolower(input);
    
    if(input == "skip") return SkipAction::Skip;
    else if(input == "move") return SkipAction::Move;
    else if(input == "copy") return SkipAction::Copy;
    else if(input == "test") return SkipAction::Test;
    else if(input == "force") return SkipAction::Force;

    // fprintf(stderr, "Unrecognized '%s', available options are skip/move/copy\n");
    return SkipAction::None;
}

bool Utils::handleArgs(int argc, const char **argv, void *arguments[])
{
    // very dangerous function... but, i kinda like this risk :)
    
    fs::path *directory = static_cast<fs::path *>(arguments[0]);
    vstr *extensions = static_cast<vstr *>(arguments[1]);
    SkipAction *skipAction = static_cast<SkipAction *>(arguments[2]);

    bool exitValue;

    exitValue = argsValid(argc, argv, directory, extensions, skipAction);
    if(!exitValue)
    {
        fprintf(stderr, COLOR_RESET "Arguments are not valid:" COLOR_RED " %S\n" COLOR_RESET, lastError.c_str());
        fprintf(stderr, "Expected at least two arguments!\n");
        printf("\n\n");
        printf("Usage:\n");
        printf("ffmpegRec <path> <extensions> [action] [command]\n");
        printf("\n");
        printf("Arguments:\n");
        printf("\t""<path>          The root directory containing files to process.\n");
        printf("\t\n");
        printf("\t""<extensions>    File extensions to look for. Multiple extensions can be separated by ,/\\?;+" "\n"
               "\t""                Example: mk4,mkv,avi\n");
        printf("\t\n");
        printf("\t""[action]        (Optional) Behavior when a file is already in the target codec (H.265).\n"
               "\t""                Options: skip, copy, move, test, force\n"
               "\t""                Default: %s\n", skipActionString[DEFAULT_SKIP_ACTION]);
        printf("\t\n");
        printf("\t""[command]       (Optional) The FFmpeg arguments to apply. Use quotes \"\" around the command.\n"
               "\t""                Default: \"%S\"\n", FFmpegCommand::getCore());
        printf("\t\n");

    }
    return exitValue;
}

bool Utils::argsValid(int argc, const char **argv, fs::path *const directory, vstr *const extensions, SkipAction *const skipAction)
{
    FUNC_START
    if(argc < 3)
    {
        lastError = L"To few arguments!";
        return false;
    }


    fs::path givenDirectory = argv[1];
    vstr givenExtensions = splitExtensionsInput( str(argv[2]) );
    
    if(!fs::exists( givenDirectory ))
    {
        lastError = L"File " + givenDirectory.wstring() + L" not exist!";
        return false;
    }

    if(!fs::is_directory( givenDirectory ))
    {
        lastError = L"File " + givenDirectory.wstring() + L" is not a directory!";
        return false;
    }


    SkipAction givenSkipAction;
    if(argc < 4) // SkipAction was not given
        givenSkipAction = DEFAULT_SKIP_ACTION;
    else // something was given as the fourth argument
        givenSkipAction = handleInputSkipAction( str(argv[3]) ); 

    if(givenSkipAction == SkipAction::None)
    {
        // don't use default to not force user to stop algorithm (for example if he spell type wrong)
        lastError = L"Given argument '" + BetterConversion::toWideString( argv[3] ) + L"' not match possible options!";
        return false;
    }


    if(argc >= 5)
    {
        FFmpegCommand::setCore( BetterConversion::toWideString(argv[4]) );
    }
    
    if(directory != nullptr)
        *directory = fs::absolute( givenDirectory );

    if(extensions != nullptr)
        *extensions = givenExtensions;

    if(skipAction != nullptr)
        *skipAction = givenSkipAction;

    return true;
}


bool Utils::isDirectoryEmpty(fs::path directory)
{
    // should be an directory at this point, then no checking

    for(const auto &file : fs::recursive_directory_iterator(directory))
    {
        if(file.is_directory())
            continue;

        // if not directory, then must be a file => is not empty
        return false;
    }
    return true;
}

bool Utils::createDirectoryIfValid(fs::path outDirectory)
{
    FUNC_START

    if(fs::exists( outDirectory ))
    {
        // "ffmpeg-h.265" exist, now check if is directory
        if(!fs::is_directory( outDirectory ))
        {
            lastError = L"Output directory already exist, but is not a directory: " + outDirectory.wstring();
            return false;
        }

        if(!isDirectoryEmpty( outDirectory ))
        {
            // this will help to avoid multiple program execution in the same directory
            lastError = L"Output directory already exist, and is not empty: " + outDirectory.wstring();
            return false;
        }

        // if is empty (not contains files) delete it anyway, it might contains empty folders, that are not exist in inputDirectory
        // directory contains only empty folders at this point
        fs::remove_all( outDirectory );

    }

    // "ffmpeg-h.265" not exist, now try to create it
    if(!fs::create_directory( outDirectory ))
    {
        lastError = L"Failed while creating output directory: " + outDirectory.wstring();
        return false;
    }

    return true;
}

bool Utils::copyStructureOfFolders(fs::path sourceDir, fs::path targetDir)
{
    FUNC_START

    wstr sourceStr = sourceDir.wstring();
    wstr targetStr = targetDir.wstring();

    for(const auto &file : fs::recursive_directory_iterator(sourceDir))
    {
        if(!file.is_directory())
            continue;

        wstr directoryStr = fs::absolute(file.path()).wstring();
        if(directoryStr == sourceStr)
            continue;
        
        size_t startPos = directoryStr.find(sourceStr);
        if(startPos == wstr::npos)
        {
            lastError = L"Error while looking for string '" + sourceStr + L"' in '" + directoryStr + L"'\n";
            return false;
        }
        directoryStr.erase(startPos, sourceStr.size()+1);
        
        fs::path directoryToCreate = targetDir / directoryStr;
        if(fs::exists(directoryToCreate))
            continue;
        
        try{
            fs::create_directories(directoryToCreate);
        }
        catch(std::filesystem::filesystem_error &e)
        {
            lastError = L"Error while creating directory: " + directoryToCreate.wstring() + L"! Error: " + BetterConversion::toWideString(e.what());
            return false;
        }
    }

    printf("Structure of folders created in %s\n", targetDir.filename().string().c_str());

    return true;
}

fs::path Utils::createOutputDirectory(cpath inputDirectory, bool removeDirIfExist)
{
    fs::path outDirectory( inputDirectory.wstring() + L"-ffmpeg-h.265" );

    if(removeDirIfExist)
    {
        printf(COLOR_RED "DEBUG ONLY!" COLOR_RESET "\n");
        rm_all(outDirectory);
    }
    
    if(!createDirectoryIfValid( outDirectory ))
    {
        fprintf(stderr, 
            COLOR_RESET "Failed while creating output directory:" 
            COLOR_RED " %ls" COLOR_RESET, lastError.c_str());
        return fs::path();
    }

    if(!copyStructureOfFolders(inputDirectory, outDirectory))
    {
        fprintf(stderr, 
            COLOR_RESET "Failed while creating structure of folders in output directory:" 
            COLOR_RED " %ls" COLOR_RESET, lastError.c_str());
        return fs::path();
    }
    return outDirectory;
}

fs::path Utils::createOCFDirectory(cpath inputDirectory, bool removeDirIfExist) // OCFDirectory is OutputCompletedFilesDirectory
{
    fs::path OFCDirectory( inputDirectory.wstring() + L"-finished_source_files");

    if(removeDirIfExist)
    {
        printf(COLOR_RED "DEBUG ONLY!" COLOR_RESET "\n");
        rm_all(OFCDirectory);
    }

    if(!createDirectoryIfValid( OFCDirectory ))
    {
        fprintf(stderr, 
            COLOR_RESET "Failed while creating output completed files directory:" 
            COLOR_RED " %ls" COLOR_RESET, lastError.c_str());
        return fs::path();
    }

    if(!copyStructureOfFolders(inputDirectory, OFCDirectory))
    {
        fprintf(stderr, 
            COLOR_RESET "Failed while creating structure of folders in output completed files directory:" 
            COLOR_RED " %ls" COLOR_RESET, lastError.c_str());
        return fs::path();
    }
    return OFCDirectory;
}

void Utils::printStatusInfo(SkipAction skipAction)
{
    str output = "\n[ ";

    if(skipAction == SkipAction::Test)
        output += COLOR_WHITE "couldBePerformed" COLOR_RESET;
    else
        output += COLOR_WHITE "correctlyPerformed" COLOR_RESET;

    output += " / ";
    output += COLOR_WHITE "performed" COLOR_RESET;
    output += " / ";
    output += COLOR_WHITE "totalToPerform" COLOR_RESET;
    output += "   ";
    output += COLOR_RED "failed" COLOR_RESET;
    
    if(skipAction != SkipAction::Force)
        output += " / ";

    if(skipAction == SkipAction::Skip)
        output += COLOR_YELLOW "skipped" COLOR_RESET;
    else if(skipAction == SkipAction::Copy) 
        output += COLOR_YELLOW "copied" COLOR_RESET;
    else if(skipAction == SkipAction::Move) 
        output += COLOR_YELLOW "moved" COLOR_RESET;
    else if(skipAction == SkipAction::Test) 
        output += COLOR_YELLOW "already H265" COLOR_RESET;

    output += " ]\n";

    printf("%s\n", output.c_str());
}

fs::path Utils::createOutputFile(cpath inFile, cpath inDirectory, cpath outDirectory)
{
    fs::path inFileWithMP4(inFile);
    inFileWithMP4.replace_extension(L"mp4");
    fs::path relativeInFileWithMP4 = fs::relative(inFileWithMP4, inDirectory);
    return outDirectory / relativeInFileWithMP4;
    // I underestimated the possibilities of filesystem lib
}

fs::path Utils::createOFCFile(cpath inFile, cpath inDirectory, cpath OFCDirectory)
{
    fs::path relativeInFile = fs::relative(inFile, inDirectory);
    return OFCDirectory / relativeInFile;
}

void Utils::deleteDirectoryIfEmpty(fs::path outDirectory)
{
    if(fs::exists( outDirectory ))
    {
        // "ffmpeg-h.265" exist, now check if is directory
        if(!fs::is_directory( outDirectory ))
            return;

        if(isDirectoryEmpty( outDirectory ))
        {
            // directory contains only empty folders at this point
            fs::remove_all( outDirectory );
        }
    }
}

bool Utils::rm_all(const fs::path& path) 
{
    try {
        fs::remove_all(path);
        return true;
    } catch (const fs::filesystem_error& e) {
        fprintf(stderr, "Error while deleting structure: %s\n", e.what());
        return false;
    }
}