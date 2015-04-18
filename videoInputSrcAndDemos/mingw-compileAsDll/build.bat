SET CFLAGS=-DBUILD_DLL -c -O3 -I..\libs\videoInput -I..\libs\videoInput-c-api\include -I..\libs\DShow\Include
SET LIBS=..\libs\DShow\lib\ddraw.lib ..\libs\DShow\lib\dxguid.lib ..\libs\DShow\lib\ole32.lib ..\libs\DShow\lib\oleaut32.lib ..\libs\DShow\lib\strmbasd.lib ..\libs\DShow\lib\strmbase.lib ..\libs\DShow\lib\strmiids.lib ..\libs\DShow\lib\uuid.lib
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

g++ -shared -o %OUT_DIR%\videoinput_dll.dll ..\libs\videoInput-c-api\src\video_input.o ..\libs\videoInput\videoInput.o %LIBS% -Wl,--out-implib,..\..\compiledLib\mingw\libvideoinput_dll.a
@echo off
if %errorlevel% neq 0 goto error

echo. 1>&2
echo ************************************ 1>&2
echo BUILD SUCCESSFUL 1>&2
echo ************************************ 1>&2
echo. 1>&2
goto cleanup

:error

echo. 1>&2
echo ************************************ 1>&2
echo BUILD FAILED 1>&2
echo ************************************ 1>&2
echo. 1>&2

:cleanup

@echo on

del ..\libs\videoInput\videoInput.o ..\libs\videoInput-c-api\src\video_input.o >nul 2>&1
