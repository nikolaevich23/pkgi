@echo off
set col=nhcolor.exe
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
echo Test SIZE = http://zeus.dl.playstation.net/cdn/JP9000/NPJA90165_00/slssGPWCSVfJVAaktnCrykcYbmqICStbWqFmHRdyMqJymTICudtDSgEKrqxYDebAnhylnCYWjZNeWLmxqctXjDTupXnhzIporaSEF.pkg|nhcolor 06
set /p link=Для получения размера введите ссылку: 
echo.
echo Scanning Size for: %link%|nhcolor 0A
echo.
%wget% --spider --server-response %link%
pause