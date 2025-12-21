#include "Program.hpp"

#include <cstdio>
#include <string>
#include <vector>
#include <filesystem>

#include "HandlePipeOutput.hpp"
#include "ErrorInfo.hpp"
#include "Utils.hpp"
#include "FFExecute.hpp"
#include "ListMaker.hpp"
#include "enums/SkipAction.hpp"
#include "WinConsoleHandler.hpp"
#include "FFmpegCommand.hpp"

int Program::run(int argc, const char **argv)
{
    printf("\n%s", versionLine);
    HandlePipeOutput::setVersionToSave(versionLine);

    fs::path inDirectory;
    vstr extensions;
    SkipAction skipAction;
    void* arguments[] = {&inDirectory, &extensions, &skipAction};
    if( !Utils::handleArgs(argc, argv, arguments) ) // messages are handle in 'handleArgs' function
        return 1;

    if(!WinConsoleHandler::installConsoleHandler()) // messages are handle in 'installConsoleHandler' method
        return 1;


    printf("Selected directory: %ls\n", inDirectory.wstring().c_str());
    printf("FFmpeg core: \"%S\"\n", FFmpegCommand::getCore()); // FFmpeg Command

    // create list of files
    vpath listOfFiles = ListMaker::listOfFiles(inDirectory, extensions); // listOfFiles method also prints files
    
    if(listOfFiles.empty())
    {
        printf("List of files to compress is " COLOR_RED "empty" COLOR_RESET "!\n");
        return 1;
    }

    fs::path outDirectory, OFCDirectory;
    if(skipAction != SkipAction::Test)
    {
        outDirectory = Utils::createOutputDirectory(inDirectory, IN_DEBUG);
        if(outDirectory == fs::path()) // messages are handle in 'createOutputDirectory' function
            return 1;

        OFCDirectory = Utils::createOCFDirectory(inDirectory, IN_DEBUG);
        if(OFCDirectory == fs::path()) // messages are handle in 'createOCFDirectory' function
            return 1;
            
        HandlePipeOutput::setFFOFileDirectory(outDirectory);
    }
    else
    {
        HandlePipeOutput::setFFOFileDirectory(inDirectory);
    }

    fs::path parentRelativeDirectory = inDirectory.parent_path();

    FFExecute::setTotalFFmpegsToPerform(listOfFiles.size());
    FFExecute::setSkipAction(skipAction);
    
    Utils::printStatusInfo(skipAction);
    if(skipAction != SkipAction::Test)
    {
        str filesProgress = FFExecute::makeFileProgressPostfix();
        printf("Status: %s\n\n", filesProgress.c_str()); 
    }
    bool loopBroken = false;
    for(const auto &inFile : listOfFiles)
    {
        // all files in list are valid at this point
        fs::path outFile = Utils::createOutputFile(inFile, inDirectory, outDirectory);
        fs::path OFCFile = Utils::createOFCFile(inFile, inDirectory, OFCDirectory);
        
        FFExecute::runFFmpeg(inFile, outFile, OFCFile, parentRelativeDirectory);
        if(WinConsoleHandler::combinationCtrlCPressed())
        {
            printf("files loop terminated due to Ctrl+C was pressed\n");
            loopBroken = true;
            break;
        }
        
        // handle case, when when the drive disconnect or something
        if(fs::exists(inDirectory))
            continue;

        printf("inDirectory" COLOR_RED " no longer exist" COLOR_RESET "!\n");
        loopBroken = true;
        break;
    }

    Utils::deleteDirectoryIfEmpty(outDirectory);
    Utils::deleteDirectoryIfEmpty(OFCDirectory);

    if(!loopBroken)
    {
        int correctlyPerformedFFmpegs = FFExecute::getCorrectlyPerformedFFmpegs();
        double correctlyPerformedRatio = 
            static_cast<double>(correctlyPerformedFFmpegs) / static_cast<double>(listOfFiles.size());

        printf(COLOR_WHITE "In directory: %S\n", inDirectory.wstring().c_str());

        if(correctlyPerformedRatio == 1.0)
            printf(COLOR_GREEN "Finished all FFmpegs" COLOR_RESET "!\n");
        else
            printf(COLOR_WHITE "Finished " COLOR_RED "%g%%" COLOR_WHITE " of FFmpegs" COLOR_RESET "!\n", 
                correctlyPerformedRatio * 100);
    }
    else
    {
        // printf(COLOR_RED "Program was stopped" COLOR_RESET "!\n");
        printf("Program was stopped!\n");
    }

    ErrorInfo::printErrors();
    return 0;
}