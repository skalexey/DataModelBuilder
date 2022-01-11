@echo off

set deps=dependencies\
set build=Build\

IF not exist %deps% ( mkdir %deps% )

IF not exist %deps%VL (
	cd %deps%
		git clone https://github.com/skalexey/VL.git --branch v0.1
		cd VL\JSONConverter
			call build.bat
			if not errorlevel 0 (
				echo "JSONConverter preparation failed. Error code: %errorlevel%"
				goto end
			)
		cd ..\..
	cd ..
)

IF not exist %build% (
	mkdir %build%
)

cd %build%
cmake .. -DDMB_TESTS=ON

IF errorlevel 1 (
	echo "CMake generation error: %errorlevel%"
	goto end
)

cmake --build .
cd ..

:end
