@echo off
rem =============================================
rem  GFX-13 Test Suite - Build Script
rem  Compiler: Borland C++ 3.1 (C++ mode)
rem =============================================
rem
rem  Usage: Run this batch file from the test
rem         directory. BCC must be on the PATH.
rem
rem  All output is logged to build.log.
rem
rem  Memory model: small (default).
rem =============================================

echo ============================================ >build.log
echo  GFX-13 Test Suite - Build Script >>build.log
echo  Compiler: Borland C++ 3.1 (C++ mode) >>build.log
echo ============================================ >>build.log
echo. >>build.log

echo [1/9] Compiling gfx13.c... >>build.log
bcc -1 -P -I..\src -c ..\src\gfx13.c >>build.log
if errorlevel 1 goto fail

echo [2/9] testpix.c... >>build.log
bcc -1 -P -I..\src testpix.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [3/9] testline.c... >>build.log
bcc -1 -P -I..\src testline.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [4/9] testrect.c... >>build.log
bcc -1 -P -I..\src testrect.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [5/9] testtri.c... >>build.log
bcc -1 -P -I..\src testtri.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [6/9] testquad.c... >>build.log
bcc -1 -P -I..\src testquad.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [7/9] testimg.c... >>build.log
bcc -1 -P -I..\src testimg.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [8/9] testpal.c... >>build.log
bcc -1 -P -I..\src testpal.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo [9/9] testclip.c... >>build.log
bcc -1 -P -I..\src testclip.c gfx13.obj >>build.log
if errorlevel 1 goto fail

echo. >>build.log
echo ============================================ >>build.log
echo  All 8 tests built successfully. >>build.log
echo ============================================ >>build.log
goto end

:fail
echo. >>build.log
echo *** BUILD FAILED *** >>build.log

:end
type build.log

echo Build finished. See build.log for details.
