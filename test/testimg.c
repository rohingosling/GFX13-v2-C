//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Image
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for PutImage, GetImage, ScaleImage, and FlipScreen
//   functions.
//
//   - Draws a 32x32 checkerboard pattern and captures it with GetImage.
//
//   - Tests PutImage at multiple positions with and without transparency.
//
//   - Tests ScaleImage with various scale factors.
//
//   - Tests FlipScreen with offscreen buffer copy and restore.
//
//   - Press any key to advance through each test screen.
//
//   - After the final screen, the program returns to text mode and prints
//     "Test complete."
//
//****************************************************************************

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "../src/gfx13.h"
#include "testutil.h"

#define VGA            0xA000

#define BLACK          0
#define BLUE           1
#define GREEN          2
#define CYAN           3
#define RED            4
#define MAGENTA        5
#define BROWN          6
#define LIGHT_GRAY     7
#define DARK_GRAY      8
#define LIGHT_BLUE     9
#define LIGHT_GREEN    10
#define LIGHT_CYAN     11
#define LIGHT_RED      12
#define LIGHT_MAGENTA  13
#define YELLOW         14
#define WHITE          15

#define FALSE		   0
#define TRUE		   1

#define SPRITE_WIDTH   32
#define SPRITE_HEIGHT  32
#define SPRITE_SIZE    1024   // 32 * 32

static BYTE spriteBuffer[SPRITE_SIZE];

int main ( void )
{
	// Initialize local variables.

	int  x;								// Pixel x coordinate.
	int  y;								// Pixel y coordinate.
	WORD spriteSegment;					// Segment address of sprite buffer.
	WORD spriteOffset;					// Offset address of sprite buffer.
	WORD offscreenSegment;				// Segment address of offscreen buffer.
	int  offscreenAllocated;			// Flag: 1 if offscreen buffer was allocated.

	// Clear the screen and print the project header and test plan.

	clrscr ();
	PrintProjectHeader ();

	// Print the function being tested and the test cases.

	printf ( "Function Tests:\n" );
	printf ( "- PutImage\n" );
	printf ( "- GetImage\n" );
	printf ( "- ScaleImage\n" );
	printf ( "- FlipScreen\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Draw 32x32 pattern, capture with GetImage\n" );
	printf ( "- Screen 2: PutImage - paste at multiple positions\n" );
	printf ( "- Screen 3: PutImage - transparency mask demos\n" );
	printf ( "- Screen 4: ScaleImage - scaled up and down\n" );
	printf ( "- Screen 5: FlipScreen - offscreen to VGA copy\n\n" );
	printf ( "Press any key to begin...\n" );

	// Wait for key press before starting tests.

	getch  ();

	// Build a 32x32 gradient sprite.

	spriteSegment = FP_SEG ( (BYTE far *) spriteBuffer );
	spriteOffset  = FP_OFF ( (BYTE far *) spriteBuffer );

	// 

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screen 1: Draw pattern and capture
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Draw a 32x32 checkerboard with a colored border.

	for ( y = 0; y < SPRITE_HEIGHT; y++ )
	{
		for ( x = 0; x < SPRITE_WIDTH; x++ )
		{
			BYTE color;

			if ( x == 0 || x == 31 || y == 0 || y == 31 )
				color = YELLOW;   // Yellow border.
			else if ( ( ( x / 4 ) + ( y / 4 ) ) & 1 )
				color = WHITE;    // White.
			else
				color = RED;      // Red.

			PutPixel ( x, y, color, TRUE, VGA );
		}
	}

	// Capture the 32x32 region into spriteBuffer.

	GetImage ( 0, 0, 31, 31, VGA, spriteSegment, spriteOffset );

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: PutImage at multiple positions
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Row of sprites, no transparency (mask = 255, unused color).

	PutImage ( 10, 10, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	PutImage ( 60, 10, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	PutImage ( 110, 10, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	PutImage ( 160, 10, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	// Tightly packed row.

	PutImage ( 10, 60, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	PutImage ( 42, 60, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	PutImage ( 74, 60, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: Transparency mask demos
	//------------------------------------------------------------------------

	ClearScreen ( BLUE, VGA );

	// No transparency (mask = 255) - full sprite visible.

	PutImage ( 20, 20, SPRITE_WIDTH, SPRITE_SIZE, 255, spriteSegment, spriteOffset, VGA );

	// Red (4) transparent - checkerboard holes show blue.

	PutImage ( 20, 80, SPRITE_WIDTH, SPRITE_SIZE, 4, spriteSegment, spriteOffset, VGA );

	// White (15) transparent - inverse pattern.

	PutImage ( 20, 140, SPRITE_WIDTH, SPRITE_SIZE, 15, spriteSegment, spriteOffset, VGA );

	// Yellow border (14) transparent.

	PutImage ( 100, 80, SPRITE_WIDTH, SPRITE_SIZE, 14, spriteSegment, spriteOffset, VGA );

	getch ();

	//------------------------------------------------------------------------
	// Screen 4: ScaleImage
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Scale down: 32x32 -> 16x16.

	ScaleImage ( 10, 10, 25, 25, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	// Original size: 32x32 -> 32x32.

	ScaleImage ( 40, 10, 71, 41, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	// Scale up 2x: 32x32 -> 64x64.

	ScaleImage ( 10, 60, 73, 123, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	// Scale up wide: 32x32 -> 128x64.

	ScaleImage ( 90, 10, 217, 73, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	// Scale up with transparency (mask = 4, red).

	ScaleImage ( 90, 90, 217, 189, SPRITE_WIDTH, SPRITE_HEIGHT, 4, spriteSegment, spriteOffset, VGA );

	// Scale up large: 32x32 -> 96x96.

	ScaleImage ( 224, 10, 315, 105, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	getch ();

	//------------------------------------------------------------------------
	// Screen 5: FlipScreen
	//------------------------------------------------------------------------

	offscreenAllocated = 0;

	if ( allocmem ( 4000, &offscreenSegment ) == -1 )
	{
		offscreenAllocated = 1;

		// Draw to offscreen buffer (invisible).

		ClearScreen     ( BLACK, offscreenSegment );

		FillRectangle   ( 40,  20,  280, 180, GREEN,  offscreenSegment );
		Rectangle       ( 40,  20,  280, 180, WHITE,  offscreenSegment );

		// X pattern across the rectangle.

		Line ( 40,  20,  280, 180, YELLOW, TRUE, offscreenSegment );
		Line ( 280, 20,  40,  180, YELLOW, TRUE, offscreenSegment );

		// Colored rectangle on top.

		FillRectangle   ( 130, 80,  190, 120, RED,   offscreenSegment );
		Rectangle       ( 130, 80,  190, 120, WHITE, offscreenSegment );

		// Copy offscreen buffer to VGA.

		FlipScreen ( offscreenSegment, VGA );

		getch ();

		// Now copy VGA back to offscreen, modify VGA, then restore.

		FlipScreen  ( VGA, offscreenSegment );

		ClearScreen ( MAGENTA, VGA );   // Overwrite VGA with magenta.

		getch ();

		// Restore original from offscreen.

		FlipScreen ( offscreenSegment, VGA );

		getch ();

		freemem ( offscreenSegment );
	}

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	if ( !offscreenAllocated )
		printf ( "FlipScreen test SKIPPED (not enough memory).\n" );

	printf ( "Test complete.\n" );

	return 0;
}
