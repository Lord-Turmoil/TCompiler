call converter\IncludeConverter.exe -a tomic\3rd-party\mioc -i include -s include src
call converter\IncludeConverter.exe -a tomic\3rd-party\twio -i include -s include src
call converter\IncludeConverter.exe -a tomic -i include -s include src
call converter\IncludeConverter.exe -a tomic -i 3rd-party\mioc\include 3rd-party\twio\include -s include src
call converter\IncludeConverter.exe -a -i tomic\include -s src