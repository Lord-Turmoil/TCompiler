:: Used to gather all test files together.

@ECHO OFF

:: Remove old publish folder and create a new one.
rmdir /S /Q publish
mkdir publish

:: Copy all files to the publish folder.
robocopy src publish/src /E
robocopy tomic publish/tomic /E
robocopy 3rd-party publish/3rd-party /E

:: Copy project files
robocopy .idea publish/.idea /E
copy /y CMakeLists.txt publish
copy /y pack.bat publish

:: Change header files
call converter\IncludeConverter.exe -a publish\3rd-party\mioc -i include -s include src
call converter\IncludeConverter.exe -a publish\3rd-party\twio -i include -s include src
call converter\IncludeConverter.exe -a publish\tomic -i include -s include src
call converter\IncludeConverter.exe -a publish -i include 3rd-party\mioc\include 3rd-party\twio\include -s tomic\include tomic\src
call converter\IncludeConverter.exe -a publish -i tomic\include -s src

cd publish
jar -cfM "../../publish.zip" src tomic 3rd-party .idea CMakeLists.txt pack.bat
cd ..
