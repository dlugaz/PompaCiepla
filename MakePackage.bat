@echo off

rem The first parameter is the output package file, the second parameter is the compiler/tools directory
rem Usage example for Code::Blocks: MakePackage.bat $exe_output "${CODEBLOCKS}\NVMTools\bin"

rem NOTE: use single quote, to avoid conflict with double quote used in parameters
if NOT '%2' == '' set path=%2

set autodesc_opt=
set multilang_opt=
set include_pk=

rem To enable only custom font
rem use compiler option -CustomFont and edit the resource.xml file to configure languages and associated fonts
rem set multilang_opt=-cf Font0.def
if NOT "%multilang_opt%" == "" set include_pk=-add 5FFF.01:font.bin


set autodesc_opt=-autodesc


:images
rem Compile images if image file exist
rem To add an image library for the application, create the images.xml file and create a subdirectory images containing all the images used
if NOT exist images\images.xml goto no_images
echo Compiling images...
nvmresc -image -d -p images\ -c images\images.xml -o graph.bin
if errorlevel 1 goto err
:no_images


:multilanguage
rem Compile text files if any exist
if NOT exist AGF_text_*.rc goto no_multilanguage
set multilang_opt=-cf Font_WE.def

rem no autodesc because we have to compile the desc file manually
set autodesc_opt=
:no_multilanguage


:compile
rem standard compile...
nvmcc %multilang_opt% -g -c main.c -o main.o
if errorlevel 1 goto err
if exist nvmcc_headers.h copy nvmcc_headers.h nvmcc_headers_Algo.h
nvmcc %multilang_opt% -g -c useri.c -o useri.o
if errorlevel 1 goto err


:package
if NOT "%autodesc_opt%" == "" npackage -pk app.pk %autodesc_opt% %include_pk% -add 5FFF.21:useri.o -add 5FFF.06:graph.bin -add 5FFF.fe:desc.bin -add 1F50.02:main.o
if     "%autodesc_opt%" == "" nvmresc -pk -c resources.xml -o app.pk

goto out

:err
echo unknown.c:0: error: Error


:out
