# DirInfo

## Retrieves information about files and directory structure

Operating System: Windows

```
Usage: dirinfo.exe "path to folder or file"
```

Download the latest version from github: https://github.com/chipmunk-sm/DirInfo/releases


[![CMake](https://github.com/chipmunk-sm/DirInfo/actions/workflows/build.yml/badge.svg)](https://github.com/chipmunk-sm/DirInfo/actions/workflows/build.yml)


## Build

open "x64 Native Tools Command Prompt for VS 2019" or set build environment in preferred way.
```
CD "working folder" 
git clone https://github.com/chipmunk-sm/DirInfo.git
CD DirInfo
MKDIR build
CD build
cmake ../
cmake --build . --config Release
```
Copy exe and runtime libraries to "install" folder
```
cmake --install . --prefix ./install
```

Create package using ZIP
``` 
cpack -G ZIP -C Release
```
