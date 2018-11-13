@echo off

rem The first parameter is the output package file, the second parameter is the compiler/tools directory
rem Usage example for Code::Blocks: MakePackage.bat $exe_output "${CODEBLOCKS}\NVMTools\bin"

rem NOTE: use single quote, to avoid conflict with double quote used in parameters
if NOT '%2' == '' set path=%2

rem include MCXShape autogen environment variables
call AGF_SetEnv.bat

set autodesc_opt=
set multilang_opt=
set font_opt=
set include_pk=
set nvmcc_opt=
if NOT '%1' == '' set nvmcc_opt=-g

rem To enable only custom font
rem use compiler option -CustomFont and edit the resource.xml file to configure languages and associated fonts
rem set multilang_opt=-cf Font0.def
if NOT "%multilang_opt%" == "" set include_pk=-add 5FFF.01:Font.bin

set autodesc_opt=-autodesc

rem Copy standard font 8pixel and 16pixel ( similar to Font_WE but unknown)
if exist StdFonts\*.def (
echo load standard fonts...
copy StdFonts\*.def
copy StdFonts\*.bin
)
cls

:images
rem Compile images if image file exist
rem To add an image library for the application, create the images.xml file and create a subdirectory images containing all the images used
if NOT exist images\images.xml copy images\StdImages\images.xml images\images.xml
if NOT exist images\images.xml goto no_images
echo Compiling images...
nvmresc -image -d -p images\ -c images\images.xml -o graph.bin
if errorlevel 1 goto err
:no_images

:multilanguage
rem Compile text files if any exist
if "%MULTILANGUAGE%" == ""  goto no_multilanguage
if "%UsedModel%" == "LCX06C" goto no_multilanguage
if "%UsedModel%" == "LCX06D" goto no_multilanguage

echo Multilanguage...
set multilang_opt=-D MULTILANGUAGE=%MULTILANGUAGE%
set font_opt=-cf Font_WE.def
rem no autodesc because we have to compile the desc file manually
set autodesc_opt=
:no_multilanguage

:compile_custom_font
rem Comment GOTO to compile a custom font
if "%LANGUAGE_CN%" == "" goto no_compile_chinese_font
echo Compiling Chinese font ...
rem load basic font (number and special char)
copy Font_CN.def font_CN_used.def
rem append to the file font_CN_used.def the character founded in the AGF_text_CN.rc
nvmresc -text -c AGF_text_CN.rc -o AGF_text_CN.bin -updcf -cf font_CN_used.def
rem append to the file font_CN_used.def the character founded in the useri.c
nvmcc %multilang_opt% -c useri.c -o useri.o -updcf -cf font_CN_used.def
if errorlevel 1 goto err
rem Compiling Chinese font...
nvmresc -font -c AGF_Package.rc -o Font_CN.bin
if errorlevel 1 goto err
:no_compile_chinese_font

:compile
rem standard compile...
nvmcc %multilang_opt% %font_opt% %nvmcc_opt% -c main.c -o main.o
if errorlevel 1 goto err
if exist nvmcc_headers.h copy nvmcc_headers.h nvmcc_headers_Algo.h
nvmcc %multilang_opt% %font_opt% %nvmcc_opt% -c useri.c -o useri.o
if errorlevel 1 goto err

:package
if NOT "%autodesc_opt%" == "" npackage -pk app.pk %autodesc_opt% %include_pk% -add 5FFF.21:useri.o -add 5FFF.06:graph.bin -add 5FFF.fe:desc.bin -add 1F50.02:main.o
if     "%autodesc_opt%" == "" nvmresc -pk -c AGF_Package.rc -o app.pk

rem copy package for MMIMYK and other tools
COPY *.pk ..\mykfiles\

rem display dimension for a quick reference
dir *.pk
dir *.o

rem check size tool
if NOT exist ..\CheckSize\CheckSize.bat goto skip_check_size
cd ..
echo Check dimension and memory usage...
call CheckSize\CheckSize.bat .\bin
:skip_check_size

goto out

:err
echo -----------------------
echo ---    E R R O R    ---
echo -----------------------

:out
