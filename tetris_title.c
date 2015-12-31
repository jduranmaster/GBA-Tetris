#include "gba.h"
#include "keypad.h"
#include "tetris_title.h"

volatile u32* KEYS_TITLE = (volatile u32*)0x04000130;

////////////////////////////////////////////////////////////
// Draw Tetris Title 
////////////////////////////////////////////////////////////
void DrawTitleDown(int y_cur)
{
	int x, y;
	int y_start_in_image;
	int y_start_in_screen = 0;
	int width, y_temp; 	// For performance
	
	
	y_start_in_image = TETRIS_TITLE_HEIGHT  - y_cur;
	if(y_start_in_image < 0) {
		y_start_in_screen = 0-y_start_in_image;
		y_start_in_image = 0;
	} 
	
	EraseScreenNoFlip();
	width = TETRIS_TITLE_WIDTH / 2;
	
   	for(y = y_start_in_image; y < TETRIS_TITLE_HEIGHT; y++) {
   		y_temp = (y - y_start_in_image) + y_start_in_screen;
		for(x = 0; x < width; x++) {
			PlotPixel(x+10,y_temp, tetrisTitleData[(y*width)+x]);
		}
	}
		
	WaitForVblank();
	Flip();
}

////////////////////////////////////////////////////////////
// Draw PRESS START image to screen
////////////////////////////////////////////////////////////
void DrawPressStart()
{
	int x, y;
	
	for(y = 0; y < TETRIS_TITLE_PRESS_START_HEIGHT; y++) {
		for(x = 0; x < TETRIS_TITLE_PRESS_START_WIDTH/2; x++) {
			PlotPixel(x+38,y+115,tetrisTitlePressStartData[(y*TETRIS_TITLE_PRESS_START_WIDTH/2)+x]);
		}
	}
	
	WaitForVblank();
	Flip();
}

////////////////////////////////////////////////////////////
// Clear PRESS START image from screen
////////////////////////////////////////////////////////////
void ClearPressStart()
{
	int x, y;
	for(y = 0; y < TETRIS_TITLE_PRESS_START_HEIGHT; y++) {
		for(x = 0; x < TETRIS_TITLE_PRESS_START_WIDTH/2; x++) {			
			PlotPixel(x+38,y+115,0x00);
		}
	}
	
	WaitForVblank();
	Flip();	
}

////////////////////////////////////////////////////////////
// Main Function for displaying title screen
// return : seednumber (used for random function)
////////////////////////////////////////////////////////////
int TitleDisplay()
{
	int x, y;
	int seed_return;
	
	SetPalette(tetrisTitlePalette);
	
	// Moving down Title
	for(y = 1; y < 90; y+=2) {
		DrawTitleDown(y);	
	}
	
	// Blink press start image
	while(1) {
		if(seed_return < 130) 
			DrawPressStart();
		else if(seed_return < 200)
			ClearPressStart();
		else 
			seed_return = 0;	
		
		seed_return++;
		if (! ((*KEYS_TITLE) & KEY_START)) break;
	
	}
			
	return seed_return;
}
