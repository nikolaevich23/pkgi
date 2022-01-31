@echo off
setlocal EnableDelayedExpansion
set col=nhcolor.exe
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
echo Пример прямой ссылки на CDN-сервер Sony PlayStation:|!col! 09
echo http://zeus.dl.playstation.net/cdn/UP0017/NPUA30003_00/ioK4HQEme7gxPUvRL2GdkKcQ22m8NSbH18wyykhfRciOKfcS3wRbg8wDi5mV55RqLthom20FQkQoMj2bdmlYMABmssDfnsbDMVP9s.pkg|!col! 06
echo.
echo Для скачивания игры вставьте ранее скопированную ссылку правой кнопкой:|!col! 09
set /p link=
echo.
:: Дампим контрольную сумму скачиваемого PKG
set start=!TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!
!wget! --spider -S --output-file=spider.txt !link!
for /f "tokens=1,2 delims= " %%A in (spider.txt) do if [%%A]==[X-Agile-Checksum:] set hshg=%%B && set hshg=!hshg: =!
del /q "spider.txt"
for %%C in (!link!) do set gnm=%%~nxC
echo Скачивание PKG игры:|!col! 0A
echo !gnm!
echo Поддерживается докачка, вы в любой момент можете закрыть это окно и продолжить снова.|!col! 09
echo Пожалуйста, подождите...|!col! 09
echo.
!wget! -c -q --no-cookies --show-progress --user-agent="Mozilla/5.0 (PLAYSTATION 3 4.88) AppleWebKit/531.22.8 (KHTML, like Gecko)" !link!
echo.
:: Проверяем контрольную сумму
echo Проверяется контрольная сумма SHA256 скачанной игры, пожалуйста, подождите... !TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!|!col! 0A
echo.
if exist "!gnm!" (
	CertUtil -hashfile "!gnm!" SHA256 > "SHA256.txt"
	for %%D in ("!gnm!") do set size=%%~zD
) else (
	echo Не найден скачанный PKG игры в этой папке:|!col! EC
	echo "!gnm!"|!col! 0E
	pause & exit
)
for /f "tokens=* delims=" %%E in (SHA256.txt) do (
	set /a ng+=1
	set hash=%%E
	if !ng!==2 set SHA256=!hash: =!
)
if "!SHA256!"=="!hshg!" (
	echo Контрольная сумма SHA256 совпадает: !SHA256!|!col! 0B
	echo Размер скачанного PKG соответсвует: !size! байт|!col! 0B
	echo.
	echo Готово^^! PKG игры скачан успешно и проверен на битость по контрольной сумме... !TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!|!col! 0A
) else (
	echo Контрольная сумма SHA256 НЕ СОВПАДАЕТ. Пожалуйста, перекачайте игру...|!col! 0C
	echo Контрольная сумма SHA256 НЕ СОВПАДАЕТ: !SHA256! !TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!|!col! 0C
	echo !SHA256!
	echo !hshg!
	pause & exit
)
del /q "SHA256.txt"
echo.
pause
