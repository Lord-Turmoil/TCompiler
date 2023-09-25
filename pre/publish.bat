:: Used to gather all test files together.

@ECHO OFF

SET curDir=%cd%
SET srcDir=.\tests
SET dstDir=.\publish

:: Not sure if this is needed
:: It guarantees you have a canonical path (standard form)
FOR %%F IN (%srcDir%) DO SET srcDir="%%~fF"
FOR %%F IN (%dstDir%) DO SET dstDir="%%~fF"

IF EXIST "%dstDir%" RMDIR /S /Q "%dstDir%"
MKDIR "%dstDir%"

FOR /r %srcDir% %%F IN (.) DO IF "%%~fF" NEQ %dstDir% ROBOCOPY "%%F" %dstDir% *.txt

:: Compress to zip file
SETLOCAL EnableDelayedExpansion
CD %dstDir%
SET fileList=
FOR /r %%F IN (*.txt) DO SET fileList=!fileList! %%~nxF
jar -cfM "../../../pre-publish.zip" %fileList%

CD %curDir%
