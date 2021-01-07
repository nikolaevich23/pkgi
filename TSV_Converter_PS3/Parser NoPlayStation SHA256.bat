@echo off
setlocal EnableDelayedExpansion
TITLE Database Parser NoPlayStation - http://nopaystation.com
mode con:cols=172
if exist pkgi_games_MISSING_URL.txt del \q pkgi_games_MISSING_URL.txt
if exist pkgi_games_MISSING_RAP.txt del \q pkgi_games_MISSING_RAP.txt
if exist pkgi_games_MISSING_CID.txt del \q pkgi_games_MISSING_CID.txt
if exist pkgi_games.txt del \q pkgi_games.txt

wget -O PS3_GAMES.tsv http://nopaystation.com/tsv/PS3_GAMES.tsv
::wget -O PS3_DLCS.tsv http://nopaystation.com/tsv/PS3_DLCS.tsv
::wget -O PS3_THEMES.tsv http://nopaystation.com/tsv/PS3_THEMES.tsv
::wget -O PS3_AVATARS.tsv http://nopaystation.com/tsv/PS3_AVATARS.tsv
::wget -O PS3_DEMOS.tsv http://nopaystation.com/tsv/PS3_DEMOS.tsv

set n=0
for /f "tokens=1,2,3,4,5,6,7,8 delims=	" %%A in (PS3_GAMES.tsv) do (
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
		echo URL Error: !URL!
		echo !CID!,1,!NAM! - URL Error,TitleID: !CID:~7,9!; Region: !REG!,!RAP!,URL Error - !URL!,!SIZE!,>>pkgi_games_MISSING_URL.txt
		set URL=
	)
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
	echo RAP      : !RAP!
	if "!RAP!"=="NOT REQUIRED" (
		set NAM=!NAM! - RAP !RAP!
		set RAP=0
	)
	if "!RAP!"=="UNLOCK/LICENSE BY DLC" (
		set NAM=!NAM! - RAP !RAP!
		set RAP=0
	)
	if "!RAP!"=="MISSING" (
		echo !CID!,1,!NAM! - !RAP! RAP,TitleID: !CID:~7,9!; Region: !REG!,!RAP! RAP,!URL!,!SIZE!,>>pkgi_games_MISSING_RAP.txt
		set RAP=
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
	echo ContentID: !CID! - Incorrect
	echo !CID! - Incorrect ContentID,1,!NAM!,TitleID: !CID:~7,9!; Region: !REG!,!RAP!,!URL!,!SIZE!,>>pkgi_games_MISSING_CID.txt
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
	wget --spider --server-response -o SHA256.txt !URL! > nul
	for /f "tokens=1,2 delims= " %%M in (SHA256.txt) do (
		if [%%M]==[X-Agile-Checksum:] set SHA256=%%N
		if [%%M]==[Content-Length:] set SIZE=%%N
	)
	if defined SIZE echo Size     : !SIZE!
	if defined SHA256 echo SHA256   : !SHA256!
)
echo.
if defined RAP (
	if defined URL echo !CID!,1,!NAM!,TitleID: !CID:~7,9!; Region: !REG!,!RAP!,!URL!,!SIZE!,!SHA256!>>pkgi_games.txt
	)
)
del \q SHA256.txt
pause