# FFmpegDirectoryRecursive v1.5.0

## About
**FFmpegRecursive** is a CLI tool for multi-file video compression and conversion. It automates FFmpeg execution for every file in a specified directory, recursively handling subfolders.

**Key Features:**
* **Recursive Processing:** Handles complex directory trees.
* **Metadata Preservation:** Keeps original creation and modification dates intact.
* **Non-destructive:** Creates a parallel output folder structure instead of overwriting.
* **Smart Detection:** Can skip or handle files that are already encoded (e.g., H.265).

### Purpose
Downloaded and recorded videos are often encoded in **H.264**. By modern standards, these files consume significantly more disk space than necessary.

While you could manually re-encode a single video to the more efficient **H.265** format using:
`ffmpeg -i video.mp4 -c:v libx265 -vtag hvc1 smaller_size_video.mp4`

...doing this one by one is tedious and time-consuming.

**ffmpegRec** was created to automate this workflow. It simplifies the task to a single command:
`ffmpegRec video_folder mp4`
allowing you to batch-convert thousands of files residing in that folder and its subdirectories efficiently.

### More
This is an advanced successor to [FFmpegDirectory](https://github.com/Cezary-Androsiuk/FFmpegDirectory). Unlike its predecessor, this tool handles **recursive directory structures**. It creates a parallel output directory that mirrors the input structure, filling it with processed files while keeping the original files untouched.
Check out [FFmpegDirectory](https://github.com/Cezary-Androsiuk/FFmpegDirectory) for more screenshots!


## Prerequisites
* [**FFmpeg:**](https://www.ffmpeg.org/) Must be installed and added to your system's `PATH` environment variable.



## Installation (Windows)

### Option 1: Pre-built Binary (Recommended)
1. Download `ffmpegRec.exe` from the releases section.
2. Add the directory containing the `.exe` file to your system environment variables.

### Option 2: Build from Source
**Prerequisites:**
* A MinGW compiler (e.g., MinGW-w64) installed and added to PATH.

**Steps:**
1. Clone or download this repository.
2. Create a `build\` folder in the project root.
3. Run `g++ source\*.cpp -o build\ffmpegRec.exe` to build the application.
4. Add the output directory to your system environment variables.

### Option 3: Build from Source (CMake)
**Prerequisites:**
* [CMake](https://cmake.org/download/) installed.
* A MinGW compiler (e.g., MinGW-w64) installed and added to PATH.

**Steps:**
1. Clone or download this repository.
2. Open a terminal in the project folder.
3. Create a build directory and enter it:
   ```cmd
   mkdir build
   cd build
   ```
4. Generate build files specifically for MinGW (to avoid MSVC conflicts):
   ```cmd
   cmake -G "MinGW Makefiles" ..
   ```
5. Compile the application:
   ```cmd
   cmake --build .
   ```
6. The executable will be created (check build/ or bin/). Add its directory to your system environment variables.



## Usage
```cmd
ffmpegRec <path> <extensions> [action] [command]
```
### Arguments
| Argument | Description |
| :--- | :--- |
| **`<path>`** | The root directory containing files to process. |
| **`<extensions>`** | File extensions to look for. Multiple extensions can be separated by `,` `/` `\` `?` `;` `+`. <br> *Example:* `mp4,mkv,avi`|
| **`[action]`** | *(Optional)* Behavior when a file is already in the target codec (H.265). <br> **Options:** `skip`, `copy`, `move`, `test`, `force` <br> **Default:** `force` |
| **`[command]`** | *(Optional)* The FFmpeg arguments to apply. Use quotes  around the command. <br> **Default:** `"-c:v libx265 -vtag hvc1"` |

### Examples

**Basic usage (convert all mp4 and mkv in `old_format/` directory to H.265):**

You can provide a full path to the directory:
```cmd
cd %userprofile%\Videos
ffmpegRec "old_format" mp4+mkv
```

Or, simply run it inside the current directory using `.` (dot):
```cmd
cd %userprofile%\Videos\old_format
ffmpegRec . mp4+mkv
```
Both commands work identically, but the second one is often faster to type if you open the terminal directly in the target folder.

**Result:**
Two new folders will be created:
1. `old_format-ffmpeg-h.265` – containing the compressed files.
2. `old_format-finished_source_files` – containing the original source files (moved here).
*The second folder is for comparison and recovery purposes. If the conversion is successful, you can delete it to save space.*

---

**Custom FFmpeg settings (e.g., set CRF to 24):**

You can adjust the compression quality by modifying the FFmpeg command.
```cmd
ffmpegRec . mp4 skip "-c:v libx265 -crf 24 -vtag hvc1"
```
The `skip` action tells the program to ignore files that are already encoded in H.265 (treating them as if they had a different extension).

---

**Custom FFmpeg settings (e.g., change resolution):**

By changing the FFmpeg command, you can alter the program's purpose entirely, for example, to resize videos.
```cmd
ffmpegRec . mp4 force "-vf scale=1920:1080"
```
In this example, all files in the directory will be forced to scale to 1920x1080 resolution.

---

## Screenshots

### Input Files:
<table>
  <tr>
    <td align="center">
      <img src="https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/inFiles.png" width="290px" alt="inFiles">
    </td>
  </tr>
</table>

### Command Prompt Output
![display1](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/display1.png "display1") 

### Output Files:
<table>
  <tr>
    <td align="center">
      <img src="https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/outFiles1.png" width="190px" alt="outFiles 1">
    </td>
    <td align="center">
      <img src="https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/outFiles2.png" width="290px" alt="outFiles 2">
    </td>
    <td align="center">
      <img src="https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/outFiles3.png" width="290px" alt="outFiles 3">
    </td>
  </tr>
</table>


### Invalid arguments
![display2](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/display2.png "display2") 

### Catalog Occupied
![display3](https://github.com/Cezary-Androsiuk/FFmpegDirectoryRecursive/blob/master/images/display3.png "display3") 

<!-- The algorithm is armed with many more tools to protect and facilitate file compression. As soon as I find time to do so, I will describe the operation of both algorithms. -->