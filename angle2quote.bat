@echo off

call converter\IncludeConverter.exe --silent --to-quote tomic\3rd-party\mioc -i include -s include src
call converter\IncludeConverter.exe --silent --to-quote tomic\3rd-party\twio -i include -s include src
call converter\IncludeConverter.exe --silent --to-quote tomic -i include -s include src
call converter\IncludeConverter.exe --silent --to-quote tomic -i 3rd-party\mioc\include 3rd-party\twio\include -s include src
call converter\IncludeConverter.exe --silent --to-quote -i tomic\include -s src