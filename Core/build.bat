@echo off

set build=Build-cmake
set deps=dependencies\
set buildConfig=Debug
set buildFolderPrefix=Build
set cmakeTestsArg= -DDMB_TESTS=ON
set cmakeLogOnArg= -DLOG_ON=ON
set cmakeGppArg=
set onlyLibArg=
set gppArg=
set vlBranch=dev

set argCount=0
for %%x in (%*) do (
   set /A argCount+=1

   if "%%~x"=="only-lib" (
	   echo --- 'only-lib' option passed. Build only library without tests
	   set onlyLibArg= only-lib
	   set cmakeTestsArg=
	) else if "%%~x" == "g++" (
		echo --- 'g++' option passed. Build with g++ compiler
		set cmakeGppArg= -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
		set gppArg= g++
		set buildFolderPrefix=Build-g++
	) else if "%%~x" == "no-log" (
		echo --- 'no-log' option passed. Turn off LOG_ON compile definition
		set cmakeLogOnArg=
	) else if "%%~x" == "release" (
		echo --- 'release' option passed. Set Release build type
		set buildConfig=Release
	)
)

IF not exist %deps% ( mkdir %deps% )
cd %deps%
	IF not exist VL (
		echo --- Clone VL library from GitHub
		git clone https://github.com/skalexey/VL.git --branch %vlBranch%
	)
	
	cd VL\JSONConverter
		echo --- Build JSONConverter library
		setlocal
		call build.bat %* only-deps
		endlocal
		if not errorlevel 0 (
			echo --- JSONConverter preparation failed. Error code: %errorlevel%
			goto end
		)
	cd ..\..
cd ..
if not exist %build% (
	mkdir %build%
	if %errorlevel% == 0 ( echo --- '%build%' directory created
	) else ( echo --- Error while creating '%build%' directory. Exit 
		goto end )
) else ( echo --- '%build%' directory already exists )

cd %build%

cmake ..%cmakeLogOnArg%%cmakeGppArg%%cmakeTestsArg%

if %errorlevel% neq 0 (
	echo --- CMake generation error: %errorlevel%
	goto end
)

echo --- Build DMBCore with CMake

cmake --build . --config=%buildConfig%

if %errorlevel% neq 0 (
	echo --- DMBCore CMake build error: %errorlevel%
	goto end
) else (
	echo --- DMBCore CMake build successfully completed
)
cd ..

:end
