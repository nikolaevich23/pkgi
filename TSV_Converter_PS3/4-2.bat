@echo off
set col=nhcolor.exe
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
echo �ਬ�� ����᪠: 4-2.bat http://zeus.dl.playstation.net/cdn/UP0017/NPUA30003_00/ioK4HQEme7gxPUvRL2GdkKcQ22m8NSbH18wyykhfRciOKfcS3wRbg8wDi5mV55RqLthom20FQkQoMj2bdmlYMABmssDfnsbDMVP9s.pkg|nhcolor 06
echo.
set link=%1 
echo.
echo ���稢���� ����: %Link%        please wait...|nhcolor 09
echo.
set of=%Link:~42,9%.pkg
%wget% -c --no-cookies --user-agent="Mozilla/5.0 (PLAYSTATION 3 4.88) AppleWebKit/531.22.8 (KHTML, like Gecko)" -O %of% %Link%
