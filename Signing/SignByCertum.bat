@echo off
setlocal

:: -------------------------------------------------------
:: Configuration
set "SIGNTOOL=%ProgramFiles(x86)%\Windows Kits\10\bin\10.0.26100.0\x64\signtool.exe"
set "SUBJECT=Open Source Developer, Volkan Orhan"
set "TIMESTAMP=http://time.certum.pl"
set "FD=SHA256"
:: -------------------------------------------------------

:: Setting the target directory as the given parameter
if "%~1"=="" (
  set "TARGET_DIR=%CD%"
) else (
  set "TARGET_DIR=%~1"
)

echo.
echo === Signing all .exe and .dll in: "%TARGET_DIR%"
echo Using signtool: "%SIGNTOOL%"
echo.

pushd "%TARGET_DIR%" || (echo Target folder couldn't be found & goto :end)

for /R %%F in (*.exe *.dll) do (
  echo -------------------------------------------------
  echo File: "%%~fF"

  "%SIGNTOOL%" verify /pa "%%~fF" >nul 2>&1
  if errorlevel 1 (
    echo [!] Not signed — signing...
    "%SIGNTOOL%" sign /n "%SUBJECT%" /t %TIMESTAMP% /fd %FD% /v "%%~fF"
    if errorlevel 1 (
      echo [X] ERROR: Signing is unsuccessfull "%%~fF"
    ) else (
      echo [+] Successfully signed.
    )
  ) else (
    echo [=] Already signed — skipped.
  )
)

popd

:end
endlocal
echo.
echo Completed.
pause
