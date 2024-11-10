@echo off
setlocal enabledelayedexpansion

set col=nhcolor
if exist temp.txt del /q temp.txt
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
echo Test SIZE = http://zeus.dl.playstation.net/cdn/JP9000/NPJA90165_00/slssGPWCSVfJVAaktnCrykcYbmqICStbWqFmHRdyMqJymTICudtDSgEKrqxYDebAnhylnCYWjZNeWLmxqctXjDTupXnhzIporaSEF.pkg|!col! 06
set link=%1
if not defined link set /p link=Для получения размера введите ссылку: 
echo.
echo Scanning Size for: %link%|!col! 0A
echo.
%wget% --spider --server-response %link% -o temp.txt
for /f "tokens=2 delims=:" %%A in ('type temp.txt ^|findstr /i /c:"X-Agile-Checksum:"') do set sha=%%A
for /f "tokens=2 delims=:" %%B in ('type temp.txt ^|findstr /i /c:"Content-Length:"') do set size=%%B
type temp.txt |more
echo Length: !size! |!col! 0B
if defined sha (
echo SHA256: !sha! |!col! 0B
)
if not defined sha (
echo SHA256:  Отсутствует контрольная сумма SHA256 на сервере.|!col! 0C
)
echo.
pause