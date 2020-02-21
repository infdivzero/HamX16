# HamX16
This is an emulator emulating a fictional instruction set and architecture. This system "boots" from a rom.bin file in its working directory and it is built modularly. On its own, HamX16 can perform basic CPU-related tasks and output register contents to the command line. To receive keyboard input and graphical text output without the use of the command line, the keyboard and text renderer extensions can be used. These have their own repository as "stock components."

The operating system used with this emulator is contained in its own repository: (not yet available).

To program the emulator, use a Bacon-16 assembler to create a rom.bin. The only Bacon-16 assembler is B16A which can be found at this [repository](https://github.com/turtleStew/B16A). A potentially faster assembler with a different syntax will eventually be developed.

The "stock components" include:
 - text renderer
 - keyboard
 - hard drive
 - disk drive

# Todo
 - Add documentation
 - Implement remaining devices:
   - keyboard input
   - gpu
   - disk drive
   - hard drive
   - simple audio output?
 - Thread support?
 - Multiple emulated cores?
 - Bus timing?
 - Optimize stuff
 - Find and fix bugs
 - Implement logging for the core and device modules

# Upcoming
After I have written working versions of this and B16A (currently ~90% complete), I will write a compiler for a higher level language which compiles to b16 code. This language will either be C-like or similar to BASIC with support for writing b16 instructions directly into the source code, probably with each instruction following a symbol or enclosed in a pair of braces.

# Releases
After I have created a good working system build, I will put together release packages containing the HamX16 core, tools, and device extensions compiled for Windows and Linux.
