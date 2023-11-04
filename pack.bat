:: Used to gather all test files together.

@ECHO OFF

:: Remove old publish folder and create a new one.
rmdir /S /Q publish
mkdir publish

:: Copy all files to the publish folder.
robocopy src publish/src /E
robocopy tomic publish/tomic /E

:: Copy project files
robocopy .idea publish/.idea /E
copy /y CMakeLists.txt publish
copy /y pack.bat publish

:: Change header files
:: mioc -> mioc
call converter\IncludeConverter.exe -a publish\tomic\3rd-party\mioc -i include -s include src
:: twio -> twio
call converter\IncludeConverter.exe -a publish\tomic\3rd-party\twio -i include -s include src
:: tomic -> tomic
call converter\IncludeConverter.exe -a publish\tomic -i include -s include src
:: mioc twio -> tomic
call converter\IncludeConverter.exe -a publish\tomic -i 3rd-party\mioc\include 3rd-party\twio\include -s include src
:: tomic -> src
call converter\IncludeConverter.exe -a publish -i tomic\include -s src

cd publish
jar -cfM "../../publish.zip" src tomic .idea CMakeLists.txt pack.bat
cd ..
