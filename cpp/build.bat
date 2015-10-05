REM Build script for Visual Studio 2015
REM
REM To get 'cl' program in your terminal, run
REM "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

@echo off

set CompilerFlags=-Od -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -wd4101 -FC -Z7 -D_CRT_SECURE_NO_WARNINGS=1
set LinkerFlags= -incremental:no

IF NOT EXIST build mkdir build
pushd build

cl %CompilerFlags% ..\muha_slon.cpp /link %LinkerFlags%
popd
