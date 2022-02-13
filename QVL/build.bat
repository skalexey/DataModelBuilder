@echo off

set build=Build-cmake
set buildConfig=Debug
set buildFolderPrefix=Build
set cmakeGppArg=
set gppArg=
set cmakeLogOnArg= -DLOG_ON=ON

echo --- Build config: %buildConfig% ---
set argCount=0
for %%x in (%*) do (
	set /A argCount+=1
	if "%%~x" == "g++" (
		echo --- 'g++' option passed. Build with g++ compiler
		set cmakeGppArg= -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
		set gppArg= g++
		set buildFolderPrefix=Build-g++
	) else if "%%~x" == "no-log" (
		echo --- 'no-log' option passed. Turn off LOG_ON compile definition
		set cmakeLogOnArg=
	) else if "%%~x" == "release" (
		echo --- 'release' option passed. Use Release build type
		set buildConfig=Release
	)
)

echo --- Build DMBCore

set curDir=%CD%
cd ../Core
setLocal
call build.bat %*
endLocal
if not errorlevel 0 (
	echo --- Core build failed. Error code: %errorlevel%
	goto end
)

echo --- Core build done

cd %curDir%

echo --- Build QVL

if not exist %build% (
	mkdir %build%
	if %errorlevel% == 0 ( echo --- '%build%' directory created
	) else ( echo --- Error while creating '%build%' directory. Exit 
		goto end )
) else ( echo --- '%build%' directory already exists )

cd %build%

cmake -S .. -DCMAKE_BUILD_TYPE:STRING=%buildConfig% -DCMAKE_PREFIX_PATH="C:/Qt/6.2.1/msvc2019_64/lib/cmake"%cmakeGppArg%%cmakeLogOnArg%

if %errorlevel% neq 0 (
	echo --- CMake generation error: %errorlevel%
	goto end
)

echo --- Build QVL with CMake

cmake --build . --config=%buildConfig%

if %errorlevel% neq 0 (
	echo --- QVL CMake build error: %errorlevel%
	goto end
) else (
	echo --- QVL CMake build successfully completed
)
cd ..

:end
