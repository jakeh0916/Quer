@echo off
@setlocal

rem xx-xx-2023 Jake Huseman
rem 06-23-2023 Aidan Sommer - added execution time
rem 07-06-2023 Jake Huseman - added 'quit' and better display

rem TODO:
rem
rem * Allow command line flags
rem
rem * --h (-help)
rem 	shows help menu
rem * --v (-version)
rem	shows the version of quer
rem * --c (-clean)
rem 	removes all quer scripts from %APPDATA%\quer_scripts\
rem * --u (-update)
rem	updates the quer version from GitHub
rem * -where
rem 	shows installation path and where scripts are (^)
rem * --w {SCRIPT_NAME} (-write {SCRIPT_NAME})
rem 	runs quer in WRITE mode where the user can write scripts
rem 	saves the script to %APPDATA%\quer_scripts\{SCRIPT_NAME}.querscript
rem * --r {SCRIPT_NAME} (-run {SCRIPT_NAME})
rem     runs quer in RUN mode from a script file (from %APPDATA%\quer_scripts\{SCRIPT_NAME}.querscript)
rem * --q (-quiet)
rem	runs quer like normal but doesn't print anything except for "Next command:"
rem
rem * No command args
rem	runs quer in RUN mode where user can input commands to queue
rem   
rem QUER CORE FEATURES
rem 
rem 'done'
rem	RUN MODE: completes the queue and starts it
rem	WRITE MODE: saves the queue to %APPDATA%\quer_scripts\{SCRIPT_NAME}.querscript
rem 'quit'
rem 	ALL MODES: quits out and doesn't run/write anything
rem 'undo {{INDEX}}'
rem 	ALL MODES: removes the queued command at INDEX. Or, if INDEX invalid (or not present) removes last queued command
rem
rem Execution Time - Shows up after RUN MODE completes.

echo [101;93m==== QUER =============================[0m
echo [101;93m= Queue some commands                 =[0m
echo [101;93m= Type 'done' to finish the queue     =[0m
echo [101;93m= Type 'quit' to quit immediately     =[0m
echo [101;93m=======================================[0m
set done_command=done
set cmd_list=

:LOOP
set "next_command="
set /P "next_command=Next command: "
if not defined next_command goto LOOP
setlocal EnableDelayedExpansion
if /I "!next_command!" == "done" endlocal & goto DONE
if /I "!next_command!" == "quit" endlocal & goto QUIT
endlocal

set cmd_list=%cmd_list%;"%next_command%"
goto LOOP

:DONE

set start=%time%
echo [101;93m========= Built command queue =========[0m
for %%a in (%cmd_list%) do echo %%~a
echo [101;93m======= Executing command queue =======[0m
for %%a in (%cmd_list%) do %%~a

rem !! For timing purposes the following is used. not sure what happens but was ripped from a stackoverflow. -Aidan
set end=%time%
set options="tokens=1-4 delims=:.,"
for /f %options% %%a in ("%start%") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100
for /f %options% %%a in ("%end%") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100

set /a hours=%end_h%-%start_h%
set /a mins=%end_m%-%start_m%
set /a secs=%end_s%-%start_s%
set /a ms=%end_ms%-%start_ms%
if %ms% lss 0 set /a secs = %secs% - 1 & set /a ms = 100%ms%
if %secs% lss 0 set /a mins = %mins% - 1 & set /a secs = 60%secs%
if %mins% lss 0 set /a hours = %hours% - 1 & set /a mins = 60%mins%
if %hours% lss 0 set /a hours = 24%hours%
if 1%ms% lss 100 set ms=0%ms%

:: Mission accomplished
set /a totalsecs = %hours%*3600 + %mins%*60 + %secs%
echo [101;93m============ Execution Time ===========[0m
echo Execution Time:: %hours%:%mins%:%secs%.%ms% (%totalsecs%.%ms%s total)

:QUIT