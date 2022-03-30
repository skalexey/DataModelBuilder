@echo off

set destinationFolder=Release
set argCount=0
for %%x in (%*) do (
	set /A argCount+=1
	if "%%~x" == "debug" (
		echo --- 'debug' option passed. Use Debug build folder
		set destinationFolder=Debug
	)
)

set dir=%CD%
set buildDir=%CD%\Build-cmake\%destinationFolder%
cd %buildDir%
call DataModelBuilder.exe
cd %dir%
echo Finished

