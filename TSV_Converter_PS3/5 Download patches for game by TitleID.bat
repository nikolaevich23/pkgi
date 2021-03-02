@echo off
set col=nhcolor.exe
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
if exist "*.xml" del /q "*.xml"
echo Тестовый TitleID = BLES00073|%col% 06
echo.
set /p title=Для скачивания обновлений введите TitleID: 
echo.
echo Поиск обновлений для игры: %title%|%col% 0A
echo.
%wget% -c --no-cookies --user-agent="Mozilla/5.0 (PLAYSTATION 3 4.87) AppleWebKit/531.22.8 (KHTML, like Gecko)" -O %title%.xml https://a0.ww.np.dl.playstation.net/tpl/np/%title%/%title%-ver.xml
find "File not found.""" < %title%.xml && goto not_found || for %%a in ("%title%.xml") do if %%~za==0 (goto not_update) else (echo Найден патч для игры: %title%|%col% 0A)
echo.
set "pth=%cd%"
for /f "usebackq delims=" %%B in (`powershell -ex bypass .\dwn.ps1 '%pth%'`) do (
echo Скачивание патча: %%~nxB        пожалуйста подождите...|%col% 09
echo.
if not exist "UPDATES_%title%" md "UPDATES_%title%"
%wget% -c --no-cookies --user-agent="Mozilla/5.0 (PLAYSTATION 3 4.87) AppleWebKit/531.22.8 (KHTML, like Gecko)" -O "UPDATES_%title%"\%%~nxB %%B
)
if exist "%title%.xml" del /q "%title%.xml"
pause&exit /b
:not_found
echo File not found.^" - Обновление для %title% отсутствует^^!|%col% 0C
echo.
pause&exit /b
:not_update
echo Файл нулевого размера - Обновление для %title% отсутствует^^!|%col% 0C
echo.
pause&exit /b
