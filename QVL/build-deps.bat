@echo off

set cmakeGppArg=
set gppArg=

set argCount=0
for %%x in (%*) do (
   set /A argCount+=1
	if "%%~x" == "g++" (
		echo --- 'g++' option passed. Build with g++ compiler
		set cmakeGppArg= -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
		set gppArg= g++
	)
)

echo --- Build dependencies

cd ..\Core
python build.py only-lib%gppArg% 
	if errorlevel == 0 (
		echo --- Core library successfully built
		cd ..\Client

	) else (
		echo --- Error while building Core library. Error code: %errorlevel%
		cd ..\Client
		goto end
	)
:end
