# quer

A simple command-line tool for queueing commands and creating BAT-like scripts on the fly.

# Installation

1. Download/clone the `quer` Visual Studio 2022 solution and build the executable.
2. Place the built `quer.exe` somewhere that's added to your system PATH.
    - (Currently only Windows is supported.)

# Usage

### Running `quer`

Run quer by entering `quer` into a Windows terminal. 

Then queue some commands and type `done` to execute the commands in order.

### Recording and Playing Back

Quer also supports "recording" and "playing back" queues of commands. 

You can run quer in "recording" mode by entering `quer -r <ScriptName>`. Then enter your commands like usual - but they won't be executed. Instead they will be stored locally on a `.bat` file. 

This script can be "played back" using `quer -p <ScriptName>`.

### Cleaning Out Querscripts

If you want to see where your querscripts are being stored, enter `quer -where` which will reveal the location in a Windows File Explorer. 

You can also use `quer -c` to easily clean out all saved querscripts.
