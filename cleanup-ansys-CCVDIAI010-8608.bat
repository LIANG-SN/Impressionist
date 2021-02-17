@echo off
set LOCALHOST=%COMPUTERNAME%
if /i "%LOCALHOST%"=="CCVDIAI010" (taskkill /f /pid 11436)
if /i "%LOCALHOST%"=="CCVDIAI010" (taskkill /f /pid 8608)

del /F cleanup-ansys-CCVDIAI010-8608.bat
