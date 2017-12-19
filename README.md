# ImageDuplicateFinder
Search for near-duplicates in large image collections

# Usage

## Graphical user interface

1. Add some directories in which the application will search for image duplicates
2. Click on the Scan button
3. The duplicate groups are in the table on the right, click on a line to display the images

## Console

```bash
$ ./ImageDuplicateFinder.exe --recursive --progress --output <output file> <input_dir_1> <input_dir_2> ...
```

# Download

A Windows version is available on GitHub:
1. Go to the [releases page](https://github.com/mgaillard/ImageDuplicateFinder/releases) of this project
2. Download *ImageDuplicateFinder.zip* from the latest release


# Build

## Dependencies

- Qt 5.9
- OpenCV 3.3 

## Windows

1. Open the project in Qt Creator and select the Microsoft Visual C++ 141 compiler
2. Optionally change the OPENCV_PATH variable in ImageDuplicateFinder.pro
3. Compile in Release mode
4. Copy OpenCV dlls : opencv_world331.dll
5. Use windeployqt.exe on the binary file to retrieve the required runtime files

## Linux

```bash
mkdir build && cd build
qmake -config release ../src
make
```
