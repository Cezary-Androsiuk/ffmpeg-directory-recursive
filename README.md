# FFmpegDirectoryRecursive v1.5.0

## About
Program for multi-file compression. Could perform FFmpeg command for each file in the given directory keeping creation and modification date untouched

It is a twin project to [FFmpegDirectory](https://github.com/Cezary-Androsiuk/FFmpegDirectory), but upgraded to handle recursive directories.
Unlike FFmpegDirectory, this algorithm creates a parallel directory that will contain an identical structure to the one specified by the user but with re-encoded files.
Check out [FFmpegDirectory](https://github.com/Cezary-Androsiuk/FFmpegDirectory) for more screenshots!

## Installation
Requires installed FFmpeg - added to system environment variables. 
1:
    download ```ffmpegRec.exe``` 
    add file's directory to ystem environment variables
2: 
    download project
    create ```bin/``` folder
    execute ```compile.bat```
    add file's directory to ystem environment variables

## Usage
```
>ffmpegRec :1 :2 :3 :4
    :1 - path to execute ffmpeg in it
    :2 - extensions to look for, can be separated by ,/\\?;+
    :3 - action when file is already H265 [skip/copy/move/test/force] (optional)
         default: Force
    :4 - ffmpeg core command (optional)
         default: "-c:v libx265 -vtag hvc1"
         NOTE: use brackets "" with this argument
```

## Input Files:
![inFiles](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/inFiles.png "inFiles") 

## Command Prompt Output
![display1](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/display1.png "display1") 

## Output Files:
![outFiles](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/outFiles.png "outFiles")


## Invalid arguments
![display2](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/display2.png "display2") 

## Catalog Occupied
![display3](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/display3.png "display3") 

<!-- The algorithm is armed with many more tools to protect and facilitate file compression. As soon as I find time to do so, I will describe the operation of both algorithms. -->