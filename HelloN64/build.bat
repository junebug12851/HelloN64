@echo off

:: Ultra folder (NO QUOTATIONS)
set ROOT=C:\n64\n64sdk\ultra

:: Shouldn't need to change
set gccdir=%ROOT%\gcc
set PATH=%ROOT%\gcc\mipse\bin;%ROOT%\usr\sbin
set n64align=on
set GCC_CELF=ON

:: Clean old files
@mkdir build_ntsc 2>nul
cd build_ntsc
del /q *
@mkdir artifacts 2>nul
cd artifacts
del /q *
cd..
cd..

@mkdir build_pal 2>nul
cd build_pal
del /q *
@mkdir artifacts 2>nul
cd artifacts
del /q *
cd..
cd..

@mkdir build_mpal 2>nul
cd build_mpal
del /q *
@mkdir artifacts 2>nul
cd artifacts
del /q *
cd..
cd..

:: 1st Build: NTSC
set TVFORMAT=NTSC
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -D%TVFORMAT%_TV
make

:: Move Build Artifacts
move /-y *.o "build_ntsc\artifacts\" >nul
move /-y *.out "build_ntsc\artifacts\" >nul
move /-y *.n64 "build_ntsc\" >nul

:: 2nd Build: PAL
set TVFORMAT=PAL
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -D%TVFORMAT%_TV
make

:: Move Build Artifacts
move /-y *.o "build_pal\artifacts\" >nul
move /-y *.out "build_pal\artifacts\" >nul
move /-y *.n64 "build_pal\" >nul

:: 3rd Build: MPAL
set TVFORMAT=MPAL
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -D%TVFORMAT%_TV
make

:: Move Build Artifacts
move /-y *.o "build_mpal\artifacts\" >nul
move /-y *.out "build_mpal\artifacts\" >nul
move /-y *.n64 "build_mpal\" >nul
