@echo off

set PATH=%PATH%;../build/cmake/windows/bin;
cd ..
mkdir tmp
cd tmp
cmake .. -G"Visual Studio 16 2019" -A Win32 -DCMAKE_GENERATOR_TOOLSET=v142 -DCMAKE_CONFIGURATION_TYPES=Debug;Release
pause
@echo on
