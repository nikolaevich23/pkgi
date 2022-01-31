@echo off
setlocal EnableDelayedExpansion
set col=nhcolor.exe
if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)
echo �ਬ�� ��אַ� ��뫪� �� CDN-�ࢥ� Sony PlayStation:|!col! 09
echo http://zeus.dl.playstation.net/cdn/UP0017/NPUA30003_00/ioK4HQEme7gxPUvRL2GdkKcQ22m8NSbH18wyykhfRciOKfcS3wRbg8wDi5mV55RqLthom20FQkQoMj2bdmlYMABmssDfnsbDMVP9s.pkg|!col! 06
echo.
echo ��� ᪠稢���� ���� ��⠢�� ࠭�� ᪮��஢����� ��뫪� �ࠢ�� �������:|!col! 09
set /p link=
echo.
:: ������ ����஫��� �㬬� ᪠稢������ PKG
set start=!TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!
!wget! --spider -S --output-file=spider.txt !link!
for /f "tokens=1,2 delims= " %%A in (spider.txt) do if [%%A]==[X-Agile-Checksum:] set hshg=%%B && set hshg=!hshg: =!
del /q "spider.txt"
for %%C in (!link!) do set gnm=%%~nxC
echo ���稢���� PKG ����:|!col! 0A
echo !gnm!
echo �����ন������ ����窠, �� � �� ������ ����� ������� �� ���� � �த������ ᭮��.|!col! 09
echo ��������, ��������...|!col! 09
echo.
!wget! -c -q --no-cookies --show-progress --user-agent="Mozilla/5.0 (PLAYSTATION 3 4.88) AppleWebKit/531.22.8 (KHTML, like Gecko)" !link!
echo.
:: �஢��塞 ����஫��� �㬬�
echo �஢������ ����஫쭠� �㬬� SHA256 ᪠砭��� ����, ��������, ��������... !TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!|!col! 0A
echo.
if exist "!gnm!" (
	CertUtil -hashfile "!gnm!" SHA256 > "SHA256.txt"
	for %%D in ("!gnm!") do set size=%%~zD
) else (
	echo �� ������ ᪠砭�� PKG ���� � �⮩ �����:|!col! EC
	echo "!gnm!"|!col! 0E
	pause & exit
)
for /f "tokens=* delims=" %%E in (SHA256.txt) do (
	set /a ng+=1
	set hash=%%E
	if !ng!==2 set SHA256=!hash: =!
)
if "!SHA256!"=="!hshg!" (
	echo ����஫쭠� �㬬� SHA256 ᮢ������: !SHA256!|!col! 0B
	echo ������ ᪠砭���� PKG ᮮ⢥����: !size! ����|!col! 0B
	echo.
	echo ��⮢�^^! PKG ���� ᪠砭 �ᯥ譮 � �஢�७ �� ������ �� ����஫쭮� �㬬�... !TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!|!col! 0A
) else (
	echo ����஫쭠� �㬬� SHA256 �� ���������. ��������, ��४�砩� ����...|!col! 0C
	echo ����஫쭠� �㬬� SHA256 �� ���������: !SHA256! !TIME:~0,2!:!TIME:~3,2!:!TIME:~6,2!|!col! 0C
	echo !SHA256!
	echo !hshg!
	pause & exit
)
del /q "SHA256.txt"
echo.
pause
