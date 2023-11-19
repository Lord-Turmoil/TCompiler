@echo off

call converter\IncludeConverter.exe --silent tomic\3rd-party\mioc -i include -s include src
call converter\IncludeConverter.exe --silent tomic\3rd-party\twio -i include -s include src
call converter\IncludeConverter.exe --silent tomic -i include -s include src
call converter\IncludeConverter.exe --silent tomic -i 3rd-party\mioc\include 3rd-party\twio\include -s include src
call converter\IncludeConverter.exe --silent -i tomic\include -s src