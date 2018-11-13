@echo off

cd bin

nupload -comauto -ubaud 57600 ../MykFiles/App.pk


if errorlevel 1 goto err
goto out

:err
echo ******************* ERROR

:out
pause
