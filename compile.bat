path=D:\devkitadv\bin

gcc -c -O3 -mthumb -mthumb-interwork tetris.c
gcc -c -O3 -mthumb -mthumb-interwork logo.c
gcc -c -O3 -mthumb -mthumb-interwork util.c
gcc -c -O3 -mthumb -mthumb-interwork tetris_title.c

gcc -mthumb -mthumb-interwork -o tetris.elf tetris.o logo.o util.o tetris_title.o

objcopy -O binary tetris.elf tetris.gba

del *.o
del *.elf

pause
