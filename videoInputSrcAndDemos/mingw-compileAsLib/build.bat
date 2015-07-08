SET CFLAGS=-c -g -I..\libs\videoInput -I..\libs\videoInput-c-api\include -I..\libs\DShow\Include
SET OUT_DIR=..\..\compiledLib\mingw

g++ %CFLAGS% -o ..\libs\videoInput\videoInput.o ..\libs\videoInput\videoInput.cpp
@echo off
if %errorlevel% neq 0 goto error
@echo on

g++ %CFLAGS% -o ..\libs\videoInput-c-api\src\video_input.o ..\libs\videoInput-c-api\src\video_input.cpp
@echo off
if %errorlevel% neq 0 goto error
@echo on

if not exist %OUT_DIR% mkdir %OUT_DIR%
@echo off
if %errorlevel% neq 0 goto error
@echo on

ar rcs %OUT_DIR%\libvideoinput.a ..\libs\videoInput-c-api\src\video_input.o ..\libs\videoInput\videoInput.o
@echo off
if %errorlevel% neq 0 goto error

echo.
echo ************************************
echo BUILD SUCCESSFUL
echo ************************************
echo.
goto cleanup

:error

echo.
echo ************************************
echo BUILD FAILED
echo ************************************
echo.

:cleanup

@echo on
del ..\libs\videoInput\videoInput.o ..\libs\videoInput-c-api\src\video_input.o >nul 2>&1

@echo off
echo.
pause
