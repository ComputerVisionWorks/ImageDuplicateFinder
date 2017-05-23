# ImageDuplicateFinder
Search for near-duplicates in large image collections

# Usage

1. Add some directories in which the application will search for image duplicates
2. Click on the Scan button
3. The duplicate groups are in the table on the right, click on a line to display the images

# Build

## Dependencies

- Qt 5.8
- OpenCV 3.2 

## Windows

1. Open the project in Qt Creator and select the Microsoft Visual C++ 14 compiler
2. Optionally change the OPENCV_PATH variable in ImageDuplicateFinder.pro
3. Compile in Release mode
4. Use windeployqt.exe on the binary file to retrieve the required runtime files

## Linux

```bash
mkdir build && cd build
qmake -config release ../src
make
```