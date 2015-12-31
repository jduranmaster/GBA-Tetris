# GBA-TETRIS
A repository to storage the source code of a homebrew Tetris Game Engine for Game Boy Advance.
The Tetris Game Engine has been developed in C programming language and DEVKITADV develpment kit.

TETRIS for GBA.
Author: Ryoga a.k.a. JDURANMASTER
First step, on GBA programming. This module is a sample tetris-clone.

- Testing GBA mode 4, double buffering, keyboard controlling.
- No sound implemented yet

Change the following compilation script if you need to do it in order to compile the source
code in your local system.

SET PATH=D:\devkitadv\bin

gcc -c -O3 -mthumb -mthumb-interwork tetris.c
gcc -c -O3 -mthumb -mthumb-interwork logo.c
gcc -c -O3 -mthumb -mthumb-interwork util.c
gcc -c -O3 -mthumb -mthumb-interwork tetris_title.c

gcc -mthumb -mthumb-interwork -o tetris.elf tetris.o logo.o util.o tetris_title.o

objcopy -O binary tetris.elf tetris.gba

del *.o
del *.elf

The project also includes the pcx2gba tool.