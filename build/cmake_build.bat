@echo off

set PATH=%PATH%;../build/cmake/windows/bin
cd ..
mkdir tmp
cd tmp
cmake .. -G"Visual Studio 14 2015" -DCMAKE_GENERATOR_TOOLSET=v140_xp -DCMAKE_CONFIGURATION_TYPES=Debug;Release
pause
@echo on
