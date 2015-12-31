#include "gba.h"

u16 *videoBuffer = (u16*)0x6000000;
u16 *paletteMem  = (u16*)0x5000000;

// some memory location defines
u16* FrontBuffer = (u16*)0x6000000;
u16* BackBuffer = (u16*)0x600A000;
volatile u16* ScanlineCounter = (volatile u16*)0x4000006;

/////////////////////////////////////////////////////
//  Sleep function to slow down sometimes
/////////////////////////////////////////////////////
void Sleep(int i)
{
	int x, y;
	int c;
	for (y = 0; y < i; y++)
	{
		for (x = 0; x < 30000; x++)
			c = c + 2; // do something to slow things down
	}
}

/////////////////////////////////////////////////////
//  Set Palette of screen
/////////////////////////////////////////////////////
void SetPalette(u16 *p) {
	int i;
	for(i = 0; i < 256; i++)	
		paletteMem[i] = p[i];	
}


/////////////////////////////////////////////////////
//  Plot 1 pixel on screen
/////////////////////////////////////////////////////
void PlotPixel(int x,int y, unsigned short int c) 
{
	videoBuffer[(y) *120 + (x)] = (c);
}


/////////////////////////////////////////////////////
//  Fliping between framebuffers
/////////////////////////////////////////////////////
void Flip(void)			
{
	if(REG_DISPCNT & BACKBUFFER) { 
		//back buffer is the current buffer so we need to switch it to the font buffer
		REG_DISPCNT &= ~BACKBUFFER; //flip active buffer to front buffer by clearing back buffer bit
		videoBuffer = BackBuffer;   //now we point our drawing buffer to the back buffer
    	} else {
    		//front buffer is active so switch it to backbuffer
    		REG_DISPCNT |= BACKBUFFER; //flip active buffer to back buffer by setting back buffer bit
		videoBuffer = FrontBuffer; //now we point our drawing buffer to the front buffer
	}
}


/////////////////////////////////////////////////////
//  Wait for V-Blank
/////////////////////////////////////////////////////
void WaitForVblank(void)
{
	while(*ScanlineCounter < 160);
}


//////////////////////////////////////////////////////
//  Clear Screen with Fliping
/////////////////////////////////////////////////////
void EraseScreen(void)
{
	int x,y;
	
	for(y = 0; y < 160; y++) {
		for(x = 0; x < 120; x++) {
			PlotPixel(x,y,0x0000);
		}
	}
	
	WaitForVblank();
	Flip();
}


//////////////////////////////////////////////////////
//  Clear Screen without Fliping
/////////////////////////////////////////////////////
void EraseScreenNoFlip(void)
{
	int x,y;
	
	for(y = 0; y < 160; y++) {
		for(x = 0; x < 120; x++) {
			PlotPixel(x,y,0x0000);
		}
	}
}
