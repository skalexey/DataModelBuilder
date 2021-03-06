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
set homeDir=%CD%

cd C:\Qt\6.2.1\msvc2019_64\bin

call windeployqt.exe %buildDir%\DataModelBuilder.exe --qmldir %homeDir%\UI
cd %dir%
echo All is done

