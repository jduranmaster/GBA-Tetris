#ifndef UTILS_FUNCTIONS
#define UTILS_FUNCTIONS

#include "gba.h"

void PlotPixel(int x,int y, unsigned short int c);
void Flip(void);
void WaitForVblank(void);
void EraseScreen(void);
void SetPalette(u16 *palette);

#endif
