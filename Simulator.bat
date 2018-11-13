@echo off
del .\SimFiles\app.pk
copy .\app.pk .\SimFiles
cd SimFiles\bin\
start NSimulator.exe -start -load ..\app.pk
