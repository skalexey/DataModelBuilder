@echo off
set dir=%CD%
set buildDir=%CD%\Build-cmake\Release
set homeDir=%CD%
cd C:\Qt\6.2.1\msvc2019_64\bin
call windeployqt.exe %buildDir%\DataModelBuilder.exe --qmldir %homeDir%\UI
cd %dir%
echo All is done

