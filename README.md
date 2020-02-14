# HamX16
To compile, use whatever C++ compiler you prefer. This hasn't been tested on other platforms besides Windows and it was compiled with MinGW64 using Eclipse. HamX16 also requires SDL2: https://www.libsdl.org/download-2.0.php

To assemble .b16 files, B16A (https://github.com/turtleStew/B16A) is required. Follow its instructions to produce a rom.bin file to be placed in the same folder as the emulator's executable so it can boot from it

# Todo
 - Add documentation
 - Improve desciption
 - Implement remaining devices:
   - keyboard input
   - gpu
   - disk drive
   - hard drive
   - simple audio output?
 - Thread support?
 - Multiple emulated cores?
 - Reduce gpu update call frequency
 - Optimize stuff
 - Find and fix bugs
 - Implement logging for the core and device modules

# Upcoming
After I have written working versions of this and B16A (currently ~90% complete), I will write a compiler for a higher level language which compiles to b16 code. This language will either be C-like or similar to BASIC with support for writing b16 instructions directly into the source code, probably with each instruction following a symbol or enclosed in a pair of braces. After I finish the compiler, I will write a BIOS and operating system for HamX16 and eventually an assembler which can run in the emulator.

# Releases
At first, HamX16 will be compiled for Windows and it will be packaged with the latest compiled version of B16A. All required binaries and the source code for a "hello world" rom will be included. If a Linux version is requested, I'll compile one

^^Linux is supported
