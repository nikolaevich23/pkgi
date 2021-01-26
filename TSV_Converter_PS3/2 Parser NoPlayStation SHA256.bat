@echo off
setlocal EnableDelayedExpansion
TITLE Database Parser NoPlayStation - http://nopaystation.com
mode con:cols=172
set col=nhcolor.exe
chcp 65001 > nul

if not exist PS3_DLCS md PS3_DLCS
if not exist PS3_GAMES md PS3_GAMES
if not exist PS3_DEMOS md PS3_DEMOS
if not exist PS3_THEMES md PS3_THEMES
if not exist PS3_AVATARS md PS3_AVATARS

if exist "%ProgramFiles(x86)%" (set wget=wget64.exe) else (set wget=wget32.exe)

for /f %%X in ('dir /b /a:d') do (
!wget! --quiet -O %%X\%%X.tsv http://nopaystation.com/tsv/%%X.tsv
TITLE %%X.tsv

if %%X==PS3_DLCS set pkgi=pkgi_dlcs
if %%X==PS3_GAMES set pkgi=pkgi_games
if %%X==PS3_DEMOS set pkgi=pkgi_demos
if %%X==PS3_THEMES set pkgi=pkgi_themes
if %%X==PS3_AVATARS set pkgi=pkgi_avatars

if %%X==PS3_DLCS set Y=2
if %%X==PS3_GAMES set Y=1
if %%X==PS3_DEMOS set Y=5
if %%X==PS3_THEMES set Y=3
if %%X==PS3_AVATARS set Y=4

if exist %%X\!pkgi!_MISSING_URL.txt del \q %%X\!pkgi!_MISSING_URL.txt
if exist %%X\!pkgi!_MISSING_RAP.txt del \q %%X\!pkgi!_MISSING_RAP.txt
if exist %%X\!pkgi!_MISSING_CID.txt del \q %%X\!pkgi!_MISSING_CID.txt
if exist %%X\!pkgi!_MISSING_RAP_NOT16BYTE.txt del \q %%X\!pkgi!_MISSING_RAP_NOT16BYTE.txt
if exist %%X\!pkgi!.txt del \q %%X\!pkgi!.txt

set n=0
for /f "tokens=1,2,3,4,5,6,7,8 delims=	" %%A in (%%X\%%X.tsv) do (
set /a n+=1
set TID=%%A
set REG=%%B
set NAM=%%C
set NAM=!NAM:,=!
set URL=%%D
set RAP=%%E
set CID=%%F
set DATE=%%G
set SIZE=%%H

echo TitleID  : !TID!                              Line number: !n!
echo Region   : !REG!
echo Name     : !NAM!
if "!URL:~-4!"==".pkg" (
		if "!URL:~0,5!"=="http:" (
			echo URL      : !URL!
		) else (
			echo URL Error: !URL!|!col! 0C
			echo Line !n!: !CID!,!Y!,!NAM!,COMMENT,!RAP!,!URL!,!SIZE!,>>%%X\!pkgi!_MISSING_URL.txt
			set URL=
		)
	) else (
		echo URL Error: !URL!|!col! 0C
		echo Line !n!: !CID!,!Y!,!NAM!,COMMENT,!RAP!,!URL!,!SIZE!,>>%%X\!pkgi!_MISSING_URL.txt
		set URL=
	)
set i=1
for /l %%I in (1, 1, 36) do (
		set count=!RAP:~%%I!
		if defined count set /a i+=1
	)
if !i!==32 (
		for %%J in (a b c d e f) do set RAP=!RAP:%%J=%%J!
		echo RAP      : !RAP!
	) else (
		if "!RAP!"=="NOT REQUIRED" (
			set RAP=0
		)
		if "!RAP!"=="UNLOCK/LICENSE BY DLC" (
			set RAP=0
		)
		if "!RAP!"=="MISSING" (
			echo RAP      : !RAP!|!col! 0C
			echo Line !n!: !CID!,!Y!,!NAM!,COMMENT,!RAP! RAP,!URL!,!SIZE!,>>%%X\!pkgi!_MISSING_RAP.txt
			set RAP=
		)
		if not "!RAP!"=="0" (
			if defined RAP (
				echo RAP NOT32: !RAP!|!col! 0C
				echo Line !n!: !CID!,!Y!,!NAM!,COMMENT,!RAP!,!URL!,!SIZE!,>>%%X\!pkgi!_MISSING_RAP_NOT16BYTE.txt
				set RAP=
			)
		)
	)
set i=1
for /l %%K in (1, 1, 36) do (
		set count=!CID:~%%K!
		if defined count set /a i+=1
	)
if !i!==36 (
		echo ContentID: !CID!
	) else (
		echo ContentID: !CID! - Incorrect|!col! 0C
		echo Line !n!: !CID!,!Y!,!NAM!,COMMENT,!RAP!,!URL!,!SIZE!,>>%%X\!pkgi!_MISSING_CID.txt
		set RAP=
	)
if "!DATE:~4,1!!DATE:~7,1!"=="--" (
		echo Date     : !DATE!
	) else (
		echo Date     : Not Defined
		if not defined SIZE set SIZE=!DATE!
		if not defined SIZE set SIZE=0
	)
if defined URL (
		if defined RAP (
			!wget! --quiet --spider --server-response -o SHA256.txt !URL! > nul
			for /f "tokens=1,2 delims= " %%M in (SHA256.txt) do (
				if [%%M]==[X-Agile-Checksum:] set SHA256=%%N
				if [%%M]==[Content-Length:] set SIZE=%%N
			)
			if defined SIZE echo Size     : !SIZE!
			if defined SHA256 echo SHA256   : !SHA256!
			echo !CID!,!Y!,!NAM!,TitleID: !CID:~7,9!; Region: !REG!,!RAP!,!URL!,!SIZE!,!SHA256!>>%%X\!pkgi!.txt
		)
	)
echo.
)
if exist SHA256.txt del \q SHA256.txt
)
pause