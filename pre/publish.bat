:: Used to gather all test files together.

@ECHO OFF

SET srcDir=.\tests
SET dstDir=.\publish

::Not sure if this is needed
::It guarantees you have a canonical path (standard form)
FOR %%F IN (%dstDir%) DO SET dstDir="%%~fF"
IF EXIST "%dstDir%" RMDIR /S /Q "%dstDir%"
MKDIR "%dstDir%"

FOR /r %srcDir% %%F IN (.) DO IF "%%~fF" NEQ %dstDir% ROBOCOPY "%%F" %dstDir% *.txt
