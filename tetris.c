/*************************************************************************
 * TETRIS for GBA.
 * Author: Ryoga a.k.a. JDURANMASTER
 * First step, on GBA programming. This module is a sample tetris-clone.
 *
 **************************************************************************
 *    - Testing GBA mode 4, double buffering, keyboard controlling.
 *    - No sound implemented yet
 *
 *************************************************************************/  
#include <stdio.h>
#include <stdlib.h>
#include "gfx_tetris.h"
#include "gba.h"
#include "keypad.h"
#include "Rlogo.h"
#include "HKlogo.h"
#include "saludIntro.h"

unsigned short* ScreenBuffer = (unsigned short*)0x6000000;
unsigned short* ScreenPal = (unsigned short*)0x5000000;

#define TETRIS_MAP_WIDTH	10
#define TETRIS_MAP_HEIGHT	16
#define FREE_BLOCK		-99

volatile u32* KEYS = (volatile u32*)0x04000130;
int tetris_map[16][10];

typedef struct TetrisShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
	int x4;
	int y4;
	int shape;
	int form;
} tetris_shape;
tetris_shape current_block;


///////////////////////////////////////////////////////////////
// Plot blue block image on tetris table
///////////////////////////////////////////////////////////////
void PlotBlueBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, blueBlock[j+(i*4)]);
}

///////////////////////////////////////////////////////////////
// Plot cyan block image on tetris table
///////////////////////////////////////////////////////////////
void PlotCyanBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, cyanBlock[j+(i*4)]);
}

///////////////////////////////////////////////////////////////
// Plot grey block image on tetris table
///////////////////////////////////////////////////////////////
void PlotGreyBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, greyBlock[j+(i*4)]);
}

///////////////////////////////////////////////////////////////
// Plot magenta block image on tetris table
///////////////////////////////////////////////////////////////
void PlotMagentaBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, magentaBlock[j+(i*4)]);
}

///////////////////////////////////////////////////////////////
// Plot yellow block image on tetris table
///////////////////////////////////////////////////////////////
void PlotYellowBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, yellowBlock[j+(i*4)]);
}

///////////////////////////////////////////////////////////////
// Plot red block image on tetris table
///////////////////////////////////////////////////////////////
void PlotRedBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, redBlock[j+(i*4)]);
}

///////////////////////////////////////////////////////////////
// Plot green block image on tetris table
///////////////////////////////////////////////////////////////
void PlotGreenBlock(int x, int y) {
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 4; j++)
			PlotPixel((x*4)+j+10, (y*8)+i+16, greenBlock[j+(i*4)]);
}


///////////////////////////////////////////////////////////////
// Initialize Block information by its shape
///////////////////////////////////////////////////////////////
void tetris_blockinfo(tetris_shape *tetris, int shape)
{
	switch(shape) {
		case 0 : tetris->x1 = 4;
			 tetris->y1 = 0;
			 tetris->x2 = 5;
			 tetris->y2 = 0;
			 tetris->x3 = 6;
			 tetris->y3 = 0;
			 tetris->x4 = 5;
			 tetris->y4 = -1;
			 tetris->form = 0;
			 break;
		case 1 : tetris->x1 = 4;
			 tetris->y1 = 0;
			 tetris->x2 = 5;
			 tetris->y2 = 0;
			 tetris->x3 = 4;
			 tetris->y3 = -1;
			 tetris->x4 = 4;
			 tetris->y4 = -2;
			 tetris->form = 0;
			 break;
		case 2 : tetris->x1 = 4;
			 tetris->y1 = 0;
			 tetris->x2 = 5;
			 tetris->y2 = 0;
			 tetris->x3 = 5;
			 tetris->y3 = -1;
			 tetris->x4 = 5;
			 tetris->y4 = -2;
			 tetris->form = 0;
			 break;
		case 3 : tetris->x1 = 4;
			 tetris->y1 = 0;
			 tetris->x2 = 4;
			 tetris->y2 = -1;
			 tetris->x3 = 5;
			 tetris->y3 = -1;
			 tetris->x4 = 5;
			 tetris->y4 = -2;
			 tetris->form = 0;
			 break;
		case 4 : tetris->x1 = 4;
			 tetris->y1 = -2;
			 tetris->x2 = 4;
			 tetris->y2 = -1;
			 tetris->x3 = 5;
			 tetris->y3 = -1;
			 tetris->x4 = 5;
			 tetris->y4 = 0;
			 tetris->form = 0;
			 break;
		case 5 : tetris->x1 = 4;
			 tetris->y1 = 0;
			 tetris->x2 = 4;
			 tetris->y2 = -1;
			 tetris->x3 = 4;
			 tetris->y3 = -2;
			 tetris->x4 = 4;
			 tetris->y4 = -3;
			 tetris->form = 0;
			 break;
		case 6 : tetris->x1 = 4;
			 tetris->y1 = 0;
			 tetris->x2 = 5;
			 tetris->y2 = 0;
			 tetris->x3 = 4;
			 tetris->y3 = -1;
			 tetris->x4 = 5;
			 tetris->y4 = -1;
			 tetris->form = 0;
			 break;
	}
}

///////////////////////////////////////////////////////////////
// Draw block in tetris table
///////////////////////////////////////////////////////////////
void tetris_drawtable() 
{
	int x, y;
	
	for(y=0; y < TETRIS_MAP_HEIGHT; y++) {
		for(x=0; x < TETRIS_MAP_WIDTH; x++) {
			switch(tetris_map[y][x]) {
				case 0 :  PlotYellowBlock(x, y);  break;
				case 1 :  PlotBlueBlock(x, y);    break;
				case 2 :  PlotCyanBlock(x, y);    break;
				case 3 :  PlotRedBlock(x, y);     break;
				case 4 :  PlotMagentaBlock(x, y); break;
				case 5 :  PlotGreenBlock(x, y);   break;
				case 6 :  PlotGreyBlock(x, y);	  break;
				default : break;
			}
		}
	}
}


///////////////////////////////////////////////////////////////
// Draw current block in tetris table
///////////////////////////////////////////////////////////////
void tetris_drawcurrentblock()
{	
	switch(current_block.shape) {
		case 0 : 
	 		if(current_block.y1 >= 0) 
				PlotYellowBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotYellowBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotYellowBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotYellowBlock(current_block.x4, current_block.y4);
			break;

		case 1 : 
	 		if(current_block.y1 >= 0) 
				PlotBlueBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotBlueBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotBlueBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotBlueBlock(current_block.x4, current_block.y4);
			break;

		case 2 : 
	 		if(current_block.y1 >= 0) 
				PlotCyanBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotCyanBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotCyanBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotCyanBlock(current_block.x4, current_block.y4);
			break;

		case 3 :
	 		if(current_block.y1 >= 0) 
				PlotRedBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotRedBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotRedBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotRedBlock(current_block.x4, current_block.y4);
			break;
		
		case 4 : 
	 		if(current_block.y1 >= 0) 
				PlotMagentaBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotMagentaBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotMagentaBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotMagentaBlock(current_block.x4, current_block.y4);
			break;
			
		case 5 :
	 		if(current_block.y1 >= 0) 
				PlotGreenBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotGreenBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotGreenBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotGreenBlock(current_block.x4, current_block.y4);
			break;
		
		case 6 : 
	 		if(current_block.y1 >= 0) 
				PlotGreyBlock(current_block.x1, current_block.y1);
			if(current_block.y2 >= 0)
				PlotGreyBlock(current_block.x2, current_block.y2);
			if(current_block.y3 >= 0)
				PlotGreyBlock(current_block.x3, current_block.y3);
			if(current_block.y4 >= 0)
				PlotGreyBlock(current_block.x4, current_block.y4);
			break;
	}
}


///////////////////////////////////////////////////////////////
// Check status while current block moving down
// return : 0 = Can't move down more
//          1 = Game Over
//	    2 = Block can move down more
///////////////////////////////////////////////////////////////
int current_blockmovedown()
{	
	// If current block position is on the same with non-free block
	// occur when current block just drop to tetris table
	// and that position is already occupy by other blocks
	///////////////////////////////////////////////////////////////////
	if((current_block.y1 >= 0 && tetris_map[current_block.y1][current_block.x1] != FREE_BLOCK) ||
	   (current_block.y2 >= 0 && tetris_map[current_block.y2][current_block.x2] != FREE_BLOCK) ||
	   (current_block.y3 >= 0 && tetris_map[current_block.y3][current_block.x3] != FREE_BLOCK) ||
	   (current_block.y4 >= 0 && tetris_map[current_block.y4][current_block.x4] != FREE_BLOCK))
	   	return 1;	//gameover
	   	
	// If current block is at the bottom of tetris table
	///////////////////////////////////////////////////////////////////
      	if(current_block.y1 == TETRIS_MAP_HEIGHT-1 || current_block.y2 == TETRIS_MAP_HEIGHT-1 ||
      	   current_block.y3 == TETRIS_MAP_HEIGHT-1 || current_block.y4 == TETRIS_MAP_HEIGHT-1)
      	   	return 0;	// Can't move more
      
      
      	// If 1 more cell down of current block is not free
      	// the block can't drop down more
      	///////////////////////////////////////////////////////////////////
      	if((current_block.y1 >= 0 && tetris_map[current_block.y1+1][current_block.x1] != FREE_BLOCK) || 
      	   (current_block.y2 >= 0 && tetris_map[current_block.y2+1][current_block.x2] != FREE_BLOCK) ||
       	   (current_block.y3 >= 0 && tetris_map[current_block.y3+1][current_block.x3] != FREE_BLOCK) ||
      	   (current_block.y4 >= 0 && tetris_map[current_block.y4+1][current_block.x4] != FREE_BLOCK)) {
      	   	
      	   	//Check Game Over
      	   	if((current_block.y1 < 0) || (current_block.y2 < 0) || 
      	   	   (current_block.y3 < 0) || (current_block.y4 < 0)) 
      	   		return 1;
      	   	
		return 0; //not over yet
	}

		
	//If it can move down just move down.
	//////////////////////////////////////
	current_block.y1++;
	current_block.y2++;
	current_block.y3++;
	current_block.y4++;
	
	return 2;
}


///////////////////////////////////////////////////////////////
// Draw Background
///////////////////////////////////////////////////////////////
void draw_background()
{
	int x, y;
		
   	for(y = 0; y < 160; y++) {
		for(x = 0; x < 120; x++) {
			PlotPixel(x,y,tetrisBodyBGData[(y*120)+x]);
		}
	}
}


///////////////////////////////////////////////////////////////
// Check and Move current block to the left
///////////////////////////////////////////////////////////////
void tetris_moveleft() {
	
	// Can't move because of border of tetris table
	if(current_block.x1 == 0 || current_block.x2 == 0 || 
	   current_block.x3 == 0 || current_block.x4 == 0)
	   return;
	   
	// Can't move because of others blocks
	if((current_block.y1 >= 0 && tetris_map[current_block.y1][current_block.x1 - 1] != FREE_BLOCK) ||
	   (current_block.y2 >= 0 && tetris_map[current_block.y2][current_block.x2 - 1] != FREE_BLOCK) ||	
  	   (current_block.y3 >= 0 && tetris_map[current_block.y3][current_block.x3 - 1] != FREE_BLOCK) ||
	   (current_block.y4 >= 0 && tetris_map[current_block.y4][current_block.x4 - 1] != FREE_BLOCK))
	   return;
	  
	current_block.x1--;
	current_block.x2--;
	current_block.x3--;
	current_block.x4--;
}


///////////////////////////////////////////////////////////////
// Check and Move current block to the right
///////////////////////////////////////////////////////////////
void tetris_moveright()
{
	// Can't move because of border of tetris table
	if(current_block.x1 == TETRIS_MAP_WIDTH-1 || current_block.x2 == TETRIS_MAP_WIDTH-1 || 
	   current_block.x3 == TETRIS_MAP_WIDTH-1 || current_block.x4 == TETRIS_MAP_WIDTH-1)
	   return;
	   
	// Can't move because of others blocks	   
	if((current_block.y1 >= 0 && tetris_map[current_block.y1][current_block.x1 + 1] != FREE_BLOCK) ||
	   (current_block.y2 >= 0 && tetris_map[current_block.y2][current_block.x2 + 1] != FREE_BLOCK) ||	
  	   (current_block.y3 >= 0 && tetris_map[current_block.y3][current_block.x3 + 1] != FREE_BLOCK) ||
	   (current_block.y4 >= 0 && tetris_map[current_block.y4][current_block.x4 + 1] != FREE_BLOCK))
	   return;
	  
	current_block.x1++;
	current_block.x2++;
	current_block.x3++;
	current_block.x4++;
}


///////////////////////////////////////////////////////////////
// Change current block form by Clock wise
///////////////////////////////////////////////////////////////
void ChangeFormClockWise() 
{
	switch(current_block.shape) {
		case 0 :
			switch(current_block.form) {
				case 0 : //Check if can change form
					if((current_block.y2 + 1 < 16) && 
					   (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2 - 1;
						current_block.x1 = current_block.x2;
						
						current_block.y3 = current_block.y2 + 1;
						current_block.x3 = current_block.x2;
						
						current_block.y4 = current_block.y2;
						current_block.x4 = current_block.x2 + 1;
						
						current_block.form = 1;
					}
					break;
				case 1 : //Check if can change form
					if((current_block.x2 - 1 >= 0) && 				
					   (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2;
						current_block.x1 = current_block.x2 + 1;
						
						current_block.y3 = current_block.y2;
						current_block.x3 = current_block.x2 - 1;
						
						current_block.y4 = current_block.y2 + 1;
						current_block.x4 = current_block.x2;
						
						current_block.form = 2;
					}
					break;
				case 2 : //Check if can change form
					if((tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2 + 1;
						current_block.x1 = current_block.x2;
						
						current_block.y3 = current_block.y2 - 1;
						current_block.x3 = current_block.x2;
						
						current_block.y4 = current_block.y2;
						current_block.x4 = current_block.x2 - 1;
						
						current_block.form = 3;
					}
					break;
																		
				case 3 : //Check if can change form
					if((current_block.x2 + 1 < 10) &&
					   (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2;
						current_block.x1 = current_block.x2 - 1;
						
						current_block.y3 = current_block.y2;
						current_block.x3 = current_block.x2 + 1;
						
						current_block.y4 = current_block.y2 - 1;
						current_block.x4 = current_block.x2;
						
						current_block.form = 0;										
					}
					break;
			}
		break;									
		
		case 1 :
			switch(current_block.form) {
				case 0 : if((current_block.x3 - 1 >= 0) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 - 1;
				         	current_block.y1 = current_block.y3;
				         	
   					        current_block.x2 = current_block.x3 - 1;
				         	current_block.y2 = current_block.y3 + 1;
				         	   	
     					        current_block.x4 = current_block.x3 + 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 1;
				         }
				         break;
				             
				case 1 : if((tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3;
				         	current_block.y1 = current_block.y3 - 1;
				         	
   					        current_block.x2 = current_block.x3 - 1;
				         	current_block.y2 = current_block.y3 - 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 + 1; 	
		
						current_block.form = 2;
				         }
				         break;
				         
				case 2 : if((current_block.x3+1 < 10) &&
					    (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 + 1;
				         	current_block.y1 = current_block.y3;
				         	
   					        current_block.x2 = current_block.x3 + 1;
				         	current_block.y2 = current_block.y3 - 1;
				         	   	
     					        current_block.x4 = current_block.x3 - 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 3;
				         }
				         break;
				case 3 : if((current_block.y3+1 < 16) &&
					    (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3;
				         	current_block.y1 = current_block.y3 + 1;
				         	
   					        current_block.x2 = current_block.x3 + 1;
				         	current_block.y2 = current_block.y3 + 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 - 1; 	
		
						current_block.form = 0;
				         }
				         break;
			}
		break;
		
		case 2 :
			switch(current_block.form) {
				case 0 : if((current_block.x3 + 1 < 10) &&
				            (tetris_map[current_block.y3-1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 - 1;
				         	current_block.y1 = current_block.y3 - 1;
				         	
   					        current_block.x2 = current_block.x3 - 1;
				         	current_block.y2 = current_block.y3;
				         	   	
     					        current_block.x4 = current_block.x3 + 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 1;
				         }
				         break;
				             
				case 1 : if((current_block.y3 + 1 < 16) &&
				            (tetris_map[current_block.y3-1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 + 1;
				         	current_block.y1 = current_block.y3 - 1;
				         	
   					        current_block.x2 = current_block.x3;
				         	current_block.y2 = current_block.y3 - 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 + 1; 	
		
						current_block.form = 2;
				         }
				         break;
				         
				case 2 : if((current_block.x3 - 1 >= 0) &&
					    (tetris_map[current_block.y3+1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 + 1;
				         	current_block.y1 = current_block.y3 + 1;
				         	
   					        current_block.x2 = current_block.x3 + 1;
				         	current_block.y2 = current_block.y3;
				         	   	
     					        current_block.x4 = current_block.x3 - 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 3;
				         }
				         break;
				case 3 : if((tetris_map[current_block.y3+1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 - 1;
				         	current_block.y1 = current_block.y3 + 1;
				         	
   					        current_block.x2 = current_block.x3;
				         	current_block.y2 = current_block.y3 + 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 - 1; 	
		
						current_block.form = 0;
				         }
				         break;
			}											
			break;
			
		case 3 :
			switch(current_block.form) {
				case 0 : if((current_block.x2 - 1 >= 0) &&
				            (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2+1][current_block.x2+1] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2;
				            	current_block.x1 = current_block.x2 - 1;
				            	
				         	current_block.y3 = current_block.y2+1;
   					        current_block.x3 = current_block.x2;
   					        
				         	current_block.y4 = current_block.y2 + 1; 					         	   	
     					        current_block.x4 = current_block.x2 + 1;
		
						current_block.form = 1;
				         }
				         break;
				         
				case 1 : if((tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2-1][current_block.x2+1] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2 + 1;
				            	current_block.x1 = current_block.x2;
				            	
				         	current_block.y3 = current_block.y2;
   					        current_block.x3 = current_block.x2 + 1;
   					        
				         	current_block.y4 = current_block.y2 - 1; 					         	   	
     					        current_block.x4 = current_block.x2 + 1;
		
						current_block.form = 0;
				         }
				         break;
			}		        
			break;
			
		case 4 :
			switch(current_block.form) {
				case 0 : if((current_block.x3 + 1 < 10) &&
				            (tetris_map[current_block.y3-1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y3 - 1;
				            	current_block.x1 = current_block.x3 + 1;
				            	
				         	current_block.y2 = current_block.y3 - 1;
   					        current_block.x2 = current_block.x3;
   					        
				         	current_block.y4 = current_block.y3; 					         	   	
     					        current_block.x4 = current_block.x3 - 1;
		
						current_block.form = 1;
				         }
				         break;
				         
				case 1 : if((current_block.y3 + 1 < 16) &&
				            (tetris_map[current_block.y3-1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y3 - 1;
				            	current_block.x1 = current_block.x3 - 1;
				            	
				         	current_block.y2 = current_block.y3;
   					        current_block.x2 = current_block.x3 - 1;
   					        
				         	current_block.y4 = current_block.y3 + 1; 					         	   	
     					        current_block.x4 = current_block.x3;
		
						current_block.form = 0;
				         }
				         break;				         				         
			}		        
			break;



		case 5 :
			switch(current_block.form) {
				case 0 : if((current_block.x2 - 1 >= 0) &&
					    (current_block.x2 + 2 < 10) &&
				            (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2][current_block.x2+2] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2;
				            	current_block.x1 = current_block.x2 - 1;
				            	
				         	current_block.y3 = current_block.y2;
   					        current_block.x3 = current_block.x2 + 1;
   					        
				         	current_block.y4 = current_block.y2; 					         	   	
     					        current_block.x4 = current_block.x2 + 2;
		
						current_block.form = 1;
				         }
				         break;
				         
				case 1 : if((current_block.y2 + 1 < 16) &&
				            (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2-2][current_block.x2] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2 + 1;
				            	current_block.x1 = current_block.x2;
				            	
				         	current_block.y3 = current_block.y2 - 1;
   					        current_block.x3 = current_block.x2;
   					        
				         	current_block.y4 = current_block.y2 - 2; 					         	   	
     					        current_block.x4 = current_block.x2;
						current_block.form = 0;
				         }
				         break;
			}
			break;
	}	
}


///////////////////////////////////////////////////////////////
// Change current block form by Back Clock wise
///////////////////////////////////////////////////////////////
void ChangeFormBackClockWise() 
{
	switch(current_block.shape) {
		case 0 :
			switch(current_block.form) {
				case 0 : //Check if can change form
					if((current_block.y2 + 1 < 16) && 
					   (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2 + 1;
						current_block.x1 = current_block.x2;
						
						current_block.y3 = current_block.y2 - 1;
						current_block.x3 = current_block.x2;
						
						current_block.y4 = current_block.y2;
						current_block.x4 = current_block.x2 - 1;
						
						current_block.form = 3;
					}
					break;
				case 3 : //Check if can change form
					if((current_block.x2 + 1 < 10) && 				
					   (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2;
						current_block.x1 = current_block.x2 + 1;
						
						current_block.y3 = current_block.y2;
						current_block.x3 = current_block.x2 - 1;
						
						current_block.y4 = current_block.y2 + 1;
						current_block.x4 = current_block.x2;
						
						current_block.form = 2;
					}
					break;
				case 2 : //Check if can change form
					if((tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2 - 1;
						current_block.x1 = current_block.x2;
						
						current_block.y3 = current_block.y2 + 1;
						current_block.x3 = current_block.x2;
						
						current_block.y4 = current_block.y2;
						current_block.x4 = current_block.x2 + 1;
						
						current_block.form = 1;
					}
					break;
																		
				case 1 : //Check if can change form
					if((current_block.x2 - 1 >= 0) &&
					   (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
   					   (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
					   (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK)) {

						current_block.y1 = current_block.y2;
						current_block.x1 = current_block.x2 - 1;
						
						current_block.y3 = current_block.y2;
						current_block.x3 = current_block.x2 + 1;
						
						current_block.y4 = current_block.y2 - 1;
						current_block.x4 = current_block.x2;
						
						current_block.form = 0;										
					}
					break;
			}
		break;									
		
		case 1 :
			switch(current_block.form) {
				case 0 : if((current_block.x3 - 1 >= 0) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK)) {
				            	
				         	current_block.y1 = current_block.y3;
				         	current_block.x1 = current_block.x3 + 1;
				         	
				         	current_block.y2 = current_block.y3 - 1;				         	
   					        current_block.x2 = current_block.x3 + 1;

				         	current_block.y4 = current_block.y3; 					         	   	
     					        current_block.x4 = current_block.x3 - 1;
		
						current_block.form = 3;
				         }
				         break;
				             
				case 3 : if((current_block.y3+1 < 16) &&
					    (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3;
				         	current_block.y1 = current_block.y3 - 1;
				         	
   					        current_block.x2 = current_block.x3 - 1;
				         	current_block.y2 = current_block.y3 - 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 + 1; 	
		
						current_block.form = 2;
				         }
				         break;
				         
				case 2 : if((current_block.x3+1 < 10) &&
					    (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 - 1;
				         	current_block.y1 = current_block.y3;
				         	
   					        current_block.x2 = current_block.x3 - 1;
				         	current_block.y2 = current_block.y3 + 1;
				         	   	
     					        current_block.x4 = current_block.x3 + 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 1;
				         }
				         break;
				case 1 : if((tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3;
				         	current_block.y1 = current_block.y3 + 1;
				         	
   					        current_block.x2 = current_block.x3 + 1;
				         	current_block.y2 = current_block.y3 + 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 - 1; 	
		
						current_block.form = 0;
				         }
				         break;
			}
		break;
		
		case 2 :
			switch(current_block.form) {
				case 0 : if((current_block.x3 + 1 < 10) &&
				            (tetris_map[current_block.y3+1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 + 1;
				         	current_block.y1 = current_block.y3 + 1;
				         	
   					        current_block.x2 = current_block.x3 + 1;
				         	current_block.y2 = current_block.y3;
				         	   	
     					        current_block.x4 = current_block.x3 - 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 3;
				         }
				         break;
				             
				case 3 : if((tetris_map[current_block.y3-1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 + 1;
				         	current_block.y1 = current_block.y3 - 1;
				         	
   					        current_block.x2 = current_block.x3;
				         	current_block.y2 = current_block.y3 - 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 + 1; 	
		
						current_block.form = 2;
				         }
				         break;
				         
				case 2 : if((current_block.x3 - 1 >= 0) &&
					    (tetris_map[current_block.y3-1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3+1] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 - 1;
				         	current_block.y1 = current_block.y3 - 1;
				         	
   					        current_block.x2 = current_block.x3 - 1;
				         	current_block.y2 = current_block.y3;
				         	   	
     					        current_block.x4 = current_block.x3 + 1;
				         	current_block.y4 = current_block.y3; 	
		
						current_block.form = 1;
				         }
				         break;
				case 1 : if((current_block.y3 + 1 < 16) &&				
					    (tetris_map[current_block.y3+1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK)) {
				            	
				         	current_block.x1 = current_block.x3 - 1;
				         	current_block.y1 = current_block.y3 + 1;
				         	
   					        current_block.x2 = current_block.x3;
				         	current_block.y2 = current_block.y3 + 1;
				         	   	
     					        current_block.x4 = current_block.x3;
				         	current_block.y4 = current_block.y3 - 1; 	
		
						current_block.form = 0;
				         }
				         break;
			}											
			break;
			
		case 3 :
			switch(current_block.form) {
				case 0 : if((current_block.x2 - 1 >= 0) &&
				            (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2+1][current_block.x2+1] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2;
				            	current_block.x1 = current_block.x2 - 1;
				            	
				         	current_block.y3 = current_block.y2+1;
   					        current_block.x3 = current_block.x2;
   					        
				         	current_block.y4 = current_block.y2 + 1; 					         	   	
     					        current_block.x4 = current_block.x2 + 1;
		
						current_block.form = 1;
				         }
				         break;
				         
				case 1 : if((tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2-1][current_block.x2+1] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2 + 1;
				            	current_block.x1 = current_block.x2;
				            	
				         	current_block.y3 = current_block.y2;
   					        current_block.x3 = current_block.x2 + 1;
   					        
				         	current_block.y4 = current_block.y2 - 1; 					         	   	
     					        current_block.x4 = current_block.x2 + 1;
		
						current_block.form = 0;
				         }
				         break;
			}		        
			break;
			
		case 4 :
			switch(current_block.form) {
				case 0 : if((current_block.x3 + 1 < 10) &&
				            (tetris_map[current_block.y3-1][current_block.x3+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3-1][current_block.x3] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y3 - 1;
				            	current_block.x1 = current_block.x3 + 1;
				            	
				         	current_block.y2 = current_block.y3 - 1;
   					        current_block.x2 = current_block.x3;
   					        
				         	current_block.y4 = current_block.y3; 					         	   	
     					        current_block.x4 = current_block.x3 - 1;
		
						current_block.form = 1;
				         }
				         break;
				         
				case 1 : if((current_block.y3 + 1 < 16) &&
				            (tetris_map[current_block.y3-1][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3][current_block.x3-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y3+1][current_block.x3] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y3 - 1;
				            	current_block.x1 = current_block.x3 - 1;
				            	
				         	current_block.y2 = current_block.y3;
   					        current_block.x2 = current_block.x3 - 1;
   					        
				         	current_block.y4 = current_block.y3 + 1; 					         	   	
     					        current_block.x4 = current_block.x3;
		
						current_block.form = 0;
				         }
				         break;				         				         
			}		        
			break;



		case 5 :
			switch(current_block.form) {
				case 0 : if((current_block.x2 - 1 >= 0) &&
					    (current_block.x2 + 2 < 10) &&
				            (tetris_map[current_block.y2][current_block.x2-1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2][current_block.x2+1] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2][current_block.x2+2] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2;
				            	current_block.x1 = current_block.x2 - 1;
				            	
				         	current_block.y3 = current_block.y2;
   					        current_block.x3 = current_block.x2 + 1;
   					        
				         	current_block.y4 = current_block.y2; 					         	   	
     					        current_block.x4 = current_block.x2 + 2;
		
						current_block.form = 1;
				         }
				         break;
				         
				case 1 : if((current_block.y2 + 1 < 16) &&
				            (tetris_map[current_block.y2+1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2-1][current_block.x2] == FREE_BLOCK) &&
				            (tetris_map[current_block.y2-2][current_block.x2] == FREE_BLOCK)) {

				         	current_block.y1 = current_block.y2 + 1;
				            	current_block.x1 = current_block.x2;
				            	
				         	current_block.y3 = current_block.y2 - 1;
   					        current_block.x3 = current_block.x2;
   					        
				         	current_block.y4 = current_block.y2 - 2; 					         	   	
     					        current_block.x4 = current_block.x2;
						current_block.form = 0;
				         }
				         break;
			}
			break;
	}	
}


///////////////////////////////////////////////////////////////
// Clear tetris table
///////////////////////////////////////////////////////////////
void clearTetrisTable() {
	int x, y;
	
	for(y = 16; y < 144; y++) {
		for(x = 10; x < 50; x++) {
			PlotPixel(x, y, 0x0000);		
		}
	}			
}


///////////////////////////////////////////////////////////////
// Check if the line is completed
// return : number of completed line
///////////////////////////////////////////////////////////////
int checkMakeLine()
{
	
	int LineEffect[4];
	int KeepEffectData[4][10];
	int numLineMade;
	char lineMade;
	int x, y;	
	int temp, loop;
	
	numLineMade = 0;	
	
	for(y = 0; y < 16; y++) {
		lineMade = 1;
		for(x = 0; x < 10; x++) {
			if(tetris_map[y][x] == FREE_BLOCK) {
				lineMade = 0;
				break;
			}
		}		
		if(lineMade == 1) {
			LineEffect[numLineMade] = y;
			numLineMade++;	
		}
	}
	
	//Display Effect
	///////////////////////////////
	if(numLineMade > 0) {
		
		//Store old Data
		for(y = 0; y < numLineMade; y++) {
			for(x = 0; x < 10; x++) {
				KeepEffectData[y][x] = tetris_map[LineEffect[y]][x];	
			}	
		}
	
		//Display Effect 3 times
		for(loop = 0; loop < 2; loop++) {
			for(y = 0; y < numLineMade; y++) {
				for(x = 0; x < 10; x++) {
					tetris_map[LineEffect[y]][x] = FREE_BLOCK;
				}
			}
			clearTetrisTable();
			tetris_drawtable();
			WaitForVblank();
			Flip();
		
			Sleep(8);
		
		
			for(y = 0; y < numLineMade; y++) {
				for(x = 0; x < 10; x++) {
					tetris_map[LineEffect[y]][x] = KeepEffectData[y][x];
				}
			}
			
			clearTetrisTable();
			tetris_drawtable();
			WaitForVblank();
			Flip();		
			Sleep(8);	
		}
	
		//Rearrange line
		loop = 0;
		while(loop != numLineMade) {
			temp = LineEffect[loop];
			if(temp != 0) {	
				for(y = temp - 1; y >= 0 ; y--) {
					for(x = 0; x < 10; x++)	{
						tetris_map[y+1][x] = tetris_map[y][x];
					}
				}
			}
	
			for(x = 0; x < 10; x++) {
				tetris_map[0][x] = FREE_BLOCK;
			}
			loop++;
		}
	}
	return numLineMade;
}


///////////////////////////////////////////////////////////////
// Display Pause Image (when gamer press start for pausing)
///////////////////////////////////////////////////////////////
void DisplayPauseImage() 
{
	int x, y;

	WaitForVblank();		
	for(y = 0; y < PAUSEDATA_HEIGHT; y++) {
		for(x = 0; x < PAUSEDATA_WIDTH/2; x++) {
			PlotPixel(x+16, y+70, pauseData[(y * PAUSEDATA_WIDTH/2) + x]);		
		}
	}		
	WaitForVblank();
	Flip();	
}


///////////////////////////////////////////////////////////////
// Display Next Block on Screen
///////////////////////////////////////////////////////////////
void DisplayNextBlock(int shape)
{
	int x, y;
	u16 buffer;
	int x_center, y_center;
	
	switch(shape) {
		case 0 : 
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = yellowBlock[x + (y * 4)];
					x_center = 68;
					y_center = 40;
					PlotPixel(x+x_center,    y + y_center, buffer);
					PlotPixel(x-4+ x_center, y+8+y_center, buffer);
					PlotPixel(x+4+ x_center, y+8+y_center, buffer);
					PlotPixel(x+x_center,    y+8+y_center, buffer);								
				}
			}
		break;
	
		case 1 : 
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = blueBlock[x + (y * 4)];
					x_center = 67;
					y_center = 34;
					PlotPixel(x+x_center,    y + y_center, buffer);
					PlotPixel(x+ x_center,   y+8+y_center, buffer);
					PlotPixel(x+ x_center,   y+16+y_center, buffer);
					PlotPixel(x+4+x_center,  y+16+y_center, buffer);								
				}
			}
		break;
				
		case 2 : 		
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = cyanBlock[x + (y * 4)];
					x_center = 66;
					y_center = 34;
					PlotPixel(x+4+x_center,    y + y_center, buffer);
					PlotPixel(x+4+ x_center,   y+8+y_center, buffer);
					PlotPixel(x+ x_center,   y+16+y_center, buffer);
					PlotPixel(x+4+x_center,  y+16+y_center, buffer);								
				}
			}
		break;
		
		case 3 : 
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = redBlock[x + (y * 4)];
					x_center = 66;
					y_center = 50;
					PlotPixel(x+x_center,    y + y_center, buffer);
					PlotPixel(x+x_center,    y-8+y_center, buffer);
					PlotPixel(x+4+x_center,  y-8+y_center, buffer);
					PlotPixel(x+4+x_center,  y-16+y_center, buffer);								
				}
			}
		break;
		
		case 4 :		
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = magentaBlock[x + (y * 4)];
					x_center = 66;
					y_center = 50;
					PlotPixel(x+4+x_center,    y + y_center, buffer);
					PlotPixel(x+4+x_center,    y-8+y_center, buffer);
					PlotPixel(x+x_center,      y-8+y_center, buffer);
					PlotPixel(x+x_center,    y-16+y_center, buffer);								
				}
			}
		break;
		
		case 5 :
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = greenBlock[x + (y * 4)];
					x_center = 68;
					y_center = 30;
					PlotPixel(x+x_center,    y + y_center, buffer);
					PlotPixel(x+x_center,    y+8+y_center, buffer);
					PlotPixel(x+x_center,    y+16+y_center, buffer);
					PlotPixel(x+x_center,    y+24+y_center, buffer);								
				}
			}		
		break;
		
		case 6 :
			for(y = 0; y < 8; y++) {
				for(x = 0; x < 4; x++) {
					buffer = greyBlock[x + (y * 4)];
					x_center = 66;
					y_center = 40;
					PlotPixel(x+x_center,    y + y_center, buffer);
					PlotPixel(x+4+x_center,  y+8+y_center, buffer);
					PlotPixel(x+x_center,    y+8+y_center, buffer);
					PlotPixel(x+4+x_center,  y+y_center, buffer);								
				}
			}		
		break;
	}	
}


///////////////////////////////////////////////////////////////
// Clear Screen in next block window
///////////////////////////////////////////////////////////////
void ClearNextBlockScreen() 
{
	int x, y;
	for(y = 28; y < 65; y++) {
		for(x = 63; x < 77; x++) {
			PlotPixel(x,y,0x0000);
		}
	}	
}


///////////////////////////////////////////////////////////////
// Display digit on screen (0 - 9)
///////////////////////////////////////////////////////////////
void DisplayDigit(int screen_x, int screen_y, int number, int height_adjust)
{
	int x, y,width, height;
	
	height = DIGITDATA_HEIGHT + height_adjust;
	width  = DIGITDATA_WIDTH/2;
	switch(number) {	
		case 0 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, zeroData[(y*width)+x]);
				}
			}
		break;
		case 1 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, oneData[(y*width)+x]);
				}
			}
		break;
		case 2 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, twoData[(y*width)+x]);
				}
			}
		break;
		case 3 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, threeData[(y*width)+x]);
				}
			}
		break;
		case 4 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, fourData[(y*width)+x]);
				}
			}
		break;
		case 5 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, fiveData[(y*width)+x]);
				}
			}
		break;
		case 6 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, sixData[(y*width)+x]);
				}
			}
		break;
		case 7 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, sevenData[(y*width)+x]);
				}
			}
		break;
		case 8 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, eightData[(y*width)+x]);
				}
			}
		break;
		case 9 :
			for(y = 0; y < height; y++) {
				for(x = 0; x < width; x++) {
					PlotPixel(screen_x+x, screen_y+y, nineData[(y*width)+x]);
				}
			}
		break;
	}
}


///////////////////////////////////////////////////////////////
// Get array of digit for number
///////////////////////////////////////////////////////////////
void GetDigit(int in_num,int *number, int *digit)
{
	int i = 0;
	int temp;
	
	*digit = 0;
	temp = in_num;
	printf("Input numbere = %d\n", temp);
	while(1) 
	{
		number[i] = temp % 10;
		(*digit)++;	
		
		temp -= number[i];
		temp /= 10;
		if(temp == 0)
			break;
		i++;
	}
}


///////////////////////////////////////////////////////////////
// Display Level on Screen
///////////////////////////////////////////////////////////////
void DisplayLevel(int level)
{
	int number[2];
	int i, digit;
	int y = 17;	
	int x = 110;
	
	GetDigit(level, number,&digit);
	for(i = 0; i < digit; i++) {
		DisplayDigit(x,y,number[i],0);	
		x -= DIGITDATA_WIDTH/2;
	}
}


///////////////////////////////////////////////////////////////
// Display number of line made on Screen
///////////////////////////////////////////////////////////////
void DisplayLine(int line)
{
	int number[3];
	int i, digit;
	int y = 41;	
	int x = 110;
	
	GetDigit(line, number,&digit);
	for(i = 0; i < digit; i++) {
		DisplayDigit(x,y,number[i],0);	
		x -= DIGITDATA_WIDTH/2;
	}	
}


///////////////////////////////////////////////////////////////
// Display score on screen
///////////////////////////////////////////////////////////////
void DisplayScore(int score)
{
	int number[8];
	int i, digit;
	int y = 135;	
	int x = 110;
	
	GetDigit(score, number,&digit);
	for(i = 0; i < digit; i++) {
		DisplayDigit(x,y,number[i], -1);	
		x -= DIGITDATA_WIDTH/2;
	}	
	
}


///////////////////////////////////////////////////////////////
// Display game over window 
///////////////////////////////////////////////////////////////
void DisplayGameOver()
{
	int x, y;

	for(y = 0; y < GAMEOVERDATA_HEIGHT; y++) {
		for(x = 0; x < GAMEOVERDATA_WIDTH/2; x++) {
			PlotPixel(x+15, y+50, gameoverData[(y * GAMEOVERDATA_WIDTH/2) + x]);		
		}
	}			
}


////////////////////////////////////////////////////////////
// MAIN PROGRAM of my first GBA program !!!
////////////////////////////////////////////////////////////
int main(void)
{
	int i, j;
	int seednumber = 0;
	int loop;
	int level = 1;
	int score = 0;
	int subscore = 0;
	int wait_counter = 0;
	int nextblock;
	int current_status, downkey_loop = 0;
	
	int sublineMade, lineMade = 0;
	
	char right_flag, left_flag, a_flag, b_flag;
	//int frame_counter = 0;



	///////////////////////////////////////////////////////
	// Initialize graphics mode
	///////////////////////////////////////////////////////
	REG_DISPCNT = MODE_4 | BG2_ENABLE; 	// Enable mode 4
	
	EraseScreen();
	
	// logo - saludIntro
	for(loop=0;loop<256;loop++) {
      	ScreenPal[loop] = saludIntroPalette[loop];     
   	}

   	for(loop=0;loop<19200;loop++) {
      	ScreenBuffer[loop] = saludIntroData[loop];
   	}
	
    WaitForVblank();
	Flip();
	Sleep(2000);
	EraseScreen();
   	
	// logo Hammer Keyboard Studios.
	for(loop=0;loop<256;loop++) {
      	ScreenPal[loop] = HKlogoDataPalette[loop];     
   	}

   	for(loop=0;loop<19200;loop++) {
      	ScreenBuffer[loop] = HKlogoDatadata[loop];
   	}
	
   	WaitForVblank();
	Flip();
	Sleep(2000);
	EraseScreen();
   	
	// logo - Rlogo
	for(loop=0;loop<256;loop++) {
      	ScreenPal[loop] = RlogoPalette[loop];     
   	}

   	for(loop=0;loop<19200;loop++) {
      	ScreenBuffer[loop] = Rlogodata[loop];
   	}
	
   	WaitForVblank();
	Flip();
	Sleep(2000);
	EraseScreen();
	
	//Display Tetris Start Screen
	seednumber = TitleDisplay();	
	srand(seednumber);


	///////////////////////////////////////////////////////
	// Initialize tetris map table
	///////////////////////////////////////////////////////
RESTART:for(i = 0; i < TETRIS_MAP_HEIGHT; i++) {
    		for(j = 0; j < TETRIS_MAP_WIDTH; j++) {
			tetris_map[i][j] = FREE_BLOCK;
		}
	}
	right_flag = left_flag = 10;
	a_flag = b_flag = 1;
	level = 1;
	score = subscore = wait_counter = downkey_loop = 0;

	current_block.shape	= rand()%7;
	tetris_blockinfo(&current_block, current_block.shape);
	nextblock		= rand()%7;
	
	
	//Tetris game
	////////////////////////
	SetPalette(gfx_tetris_Palette);
	
	//Prepare BackGround on Both frame buffer screen
	// For performance so we will draw background
	// only 1 time
	////////////////////////////////////////////////////
	draw_background();
	DisplayNextBlock(nextblock);
	WaitForVblank();
	Flip();
	
	draw_background();
	DisplayNextBlock(nextblock);	
	WaitForVblank();
	Flip();
		
	////////////////////////////////////////
	// 		GAME LOOP
	///////////////////////////////////////
	while (1) 
	{		
		//Check Key Down
		///////////////////
		if(!((*KEYS) & KEY_RIGHT)) {
			right_flag++;
		}
		if(!((*KEYS) & KEY_LEFT)) {
			left_flag++;				
		}
		if(right_flag > 2) {
			tetris_moveright();	
			right_flag = 1;
		}
		if(left_flag > 2) {
			tetris_moveleft();
			left_flag = 1;	
		}				
		if ((a_flag) && ! ((*KEYS) & KEY_A))    {
			ChangeFormClockWise();					
			a_flag = 0;
		}
		if ((b_flag) && ! ((*KEYS) & KEY_B))    {
			ChangeFormBackClockWise();					
			b_flag = 0;
		}
		if (! ((*KEYS) & KEY_START)) {
				
				while(1) {
					if((*KEYS) & KEY_START) //Key up
						break;
				}	
				//Pause status wait until press start again	
				
				DisplayPauseImage();
				
				
				
				while(1) {
					if(!((*KEYS) & KEY_START)) 
						break;	
				}
				while(1) {
					if((*KEYS) & KEY_START)
						break;	
				}							
		}					
		if (! ((*KEYS) & KEY_DOWN)) {		
			wait_counter = 99;
			if(downkey_loop++ > 3) {
				subscore+=2;
				downkey_loop = 0;
			}
		}
		
		
		//Check Key UP 
		////////////////////
		if((*KEYS) & KEY_A)	a_flag = 1;
		if((*KEYS) & KEY_B)	b_flag = 1;
		
				
		//Moving down block (level is a key for slow down block dropping speed
		/////////////////////////////////////////////////////////////////////////
		if(wait_counter++ > (26 - ((2*level) + 1))) {
		
			current_status = current_blockmovedown();
			switch(current_status) {
			
				case 0 : 
					// Can't move down more
					tetris_map[current_block.y1][current_block.x1] = current_block.shape;
					tetris_map[current_block.y2][current_block.x2] = current_block.shape;
					tetris_map[current_block.y3][current_block.x3] = current_block.shape;
					tetris_map[current_block.y4][current_block.x4] = current_block.shape;

					current_block.shape = nextblock;
					tetris_blockinfo(&current_block, current_block.shape);
					nextblock = rand()%7;

					//Delete Block
					ClearNextBlockScreen();
					DisplayNextBlock(nextblock);	
					WaitForVblank();
					Flip();
					
					ClearNextBlockScreen();
					DisplayNextBlock(nextblock);	
					WaitForVblank();
					Flip();
					
					//Check Make Line and do effect
					sublineMade = checkMakeLine();
					lineMade += sublineMade;
					// Add Score
					switch(sublineMade) {
						case 1 : score += 100;  break;
						case 2 : score += 300; break;
						case 3 : score += 600; break;
						case 4 : score += 1200; break;												
					}
					score += subscore;  	// add subscore from using key down
					subscore = 0;	    	// reset subscore
					level = (lineMade/10)+1;// check current level
				break;
					
				case 1 : 
					// Game Over
					WaitForVblank();
					Flip();
					DisplayGameOver();
					WaitForVblank();
					Flip();
					
					// Press start to continue
					while(1) {
						if (! ((*KEYS) & KEY_START)) break;
					}
					while(1) {
						if((*KEYS) & KEY_START) break;							
					}
				
					// Simply way to restart from infinite loop :)
					goto RESTART;		
				break;
			}						
		   	wait_counter = 0;
		}
		
		DisplayLevel(level);
		DisplayLine(lineMade);
		DisplayScore(score);
		clearTetrisTable();				
		tetris_drawtable();	
		tetris_drawcurrentblock();
		WaitForVblank();
		Flip();
		
	}
	return 0;
}
