@echo off
set col=nhcolor.exe
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
echo Пример ссылки: http://zeus.dl.playstation.net/cdn/UP0017/NPUA30003_00/ioK4HQEme7gxPUvRL2GdkKcQ22m8NSbH18wyykhfRciOKfcS3wRbg8wDi5mV55RqLthom20FQkQoMj2bdmlYMABmssDfnsbDMVP9s.pkg|nhcolor 06
echo.
echo Начало ссылки парсера dokpub: http://getfile.dokpub.com/yandex/get/|nhcolor 06
echo.
set /p link=Для скачивания введите ссылку: 
echo.
echo Скачивание игры: %Link%        please wait...|nhcolor 09
echo.
%wget% -c --no-cookies --user-agent="Mozilla/5.0 (PLAYSTATION 3 4.87) AppleWebKit/531.22.8 (KHTML, like Gecko)" %Link%
pause
