#include "gba.h"
#include "logo.h"

////////////////////////////////////////////////////////////
// Display fading logo screen
////////////////////////////////////////////////////////////
void LogoDisplayFade(unsigned char coloradjust) {
	u16 data,buffer;
	int x, y;
	
   	for(y = 0; y < 160; y++) {
		for(x = 0; x < 120; x++) {
			data = 0x0000;
			buffer = logoData[(y*240)+(2*x+1)];
			if(buffer > coloradjust)
				data |= (buffer - coloradjust);
			data = data << 8;
			
			buffer = logoData[(y*240)+(2*x)];
			if(buffer > coloradjust)
				data |= (buffer - coloradjust);

			PlotPixel(x,y,data);
		}
	}
	WaitForVblank();
	Flip();
}


////////////////////////////////////////////////////////////
// Main Function for displaying logo screen
////////////////////////////////////////////////////////////
void LogoDisplay(void)
{
	unsigned char i;
		
	SetPalette(logoPalette);

	// Display fade-in,out logo screen	
	for(i=32; i > 0; i-=2)
		LogoDisplayFade(i);

	for(i=1; i < 5; i++)
		LogoDisplayFade(0);		
			
	for(i=0; i < 32; i+=2)
		LogoDisplayFade(i);
	
}
