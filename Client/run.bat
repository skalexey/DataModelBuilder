@echo off
set dir=%CD%
set buildDir=%CD%\Build-cmake\Release
cd %buildDir%
call DataModelBuilder.exe
cd %dir%
echo Finished

