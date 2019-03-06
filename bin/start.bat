set BDADDR="127.0.0.1"
set BDUSER="UserName"
set BDPASS="Password"

@:myParser.exe -h

myParser.exe -bda %BDADDR% -bdn rsi -bdu %BDUSER% -bdp %BDPASS% -clearTables
myParser.exe -u SZF-00000000 -p PASSWORD -bda %BDADDR% -bdn rsi -bdu %BDUSER% -bdp %BDPASS%
myParser.exe -u SZF-00000000 -p PASSWORD -bda %BDADDR% -bdn rsi -bdu %BDUSER% -bdp %BDPASS%
