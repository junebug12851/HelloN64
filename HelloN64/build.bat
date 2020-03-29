@echo off

:: Ultra folder (NO QUOTATIONS)
set ROOT=C:\n64\n64sdk\ultra

:: Shouldn't need to change
set gccdir=%ROOT%\gcc
set PATH=%ROOT%\gcc\mipse\bin;%ROOT%\usr\sbin
set n64align=on
set GCC_CELF=ON

:: Clean old files
@mkdir build 2>nul
cd build
del /q *
@mkdir artifacts 2>nul
cd artifacts
del /q *
cd..
cd..

:: Build
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__
make

:: Move Build Artifacts
move /-y *.o "build\artifacts\" >nul
move /-y *.out "build\artifacts\" >nul
move /-y *.n64 "build\" >nul
