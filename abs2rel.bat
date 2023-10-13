call converter\IncludeConverter.exe 3rd-party\mioc -i include -s include src
call converter\IncludeConverter.exe 3rd-party\twio -i include -s include src
call converter\IncludeConverter.exe tomic -i include -s include src
call converter\IncludeConverter.exe -i include 3rd-party\mioc\include 3rd-party\twio\include -s tomic\include tomic\src
call converter\IncludeConverter.exe -i tomic\include -s src