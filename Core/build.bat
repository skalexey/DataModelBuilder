@echo off

set deps=dependencies\
set build=Build\
set dmbTestsArg= -DDMB_TESTS=ON
set onlyLibArg=
set gppArg=
set cmakeGppArg=

set argCount=0
for %%x in (%*) do (
   set /A argCount+=1

   if "%%~x"=="only-lib" (
	   echo --- 'only-lib' option passed. Build only library without tests
	   set onlyLibArg= only-lib
	   set dmbTestsArg=
	) else if "%%~x" == "g++" (
		echo --- 'g++' option passed. Build with g++ compiler
		set cmakeGppArg= -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
		set gppArg= g++
		set build=Build-g++\
	)
)

IF not exist %deps% ( mkdir %deps% )
cd %deps%

IF not exist VL (
	echo --- Clone VL library from GitHub
	git clone https://github.com/skalexey/VL.git --branch v0.1
)

cd VL\JSONConverter
	echo --- Build JSONConverter library
	call build.bat only-deps
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

echo --- Configure DMBCore with CMake

cmake ..%cmakeGppArg%%dmbTestsArg%

if %errorlevel% neq 0 (
	echo --- CMake generation error: %errorlevel%
	goto end
)

echo --- Build DMBCore with CMake

cmake --build .

if %errorlevel% neq 0 (
	echo --- CMake build error: %errorlevel%
	goto end
) else (
	echo --- CMake build successfully completed
)
cd ..

:end
