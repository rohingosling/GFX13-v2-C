//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Clipping
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for SetClipping function.
//
//   - Tests clipping behaviour of FillRectangle, FillTriangle, FillQuad,
//     and ScaleImage.
//
//   - A gray dashed border marks the clip boundary for visual reference.
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

#define CLIP_X1        80
#define CLIP_Y1        50
#define CLIP_X2        240
#define CLIP_Y2        150

#define SPRITE_WIDTH   16
#define SPRITE_HEIGHT  16
#define SPRITE_SIZE    256   // 16 * 16

static BYTE spriteBuffer[SPRITE_SIZE];

//----------------------------------------------------------------------------
// drawClipBorder
//
// Draw a gray dashed border showing the clip region.
// Must be called BEFORE SetClipping (Rectangle does not clip).
//----------------------------------------------------------------------------

static void drawClipBorder ( void )
{
	int i;								// Loop index.
	int border_left   = CLIP_X1 - 1;	// One pixel outside clip region.
	int border_right  = CLIP_X2 + 1;
	int border_top    = CLIP_Y1 - 1;
	int border_bottom = CLIP_Y2 + 1;

	// Draw dashed border just outside the clip region.

	for ( i = border_left; i <= border_right; i += 2 )
	{
		PutPixel ( i, border_top,    DARK_GRAY, TRUE, VGA );
		PutPixel ( i, border_bottom, DARK_GRAY, TRUE, VGA );
	}

	for ( i = border_top; i <= border_bottom; i += 2 )
	{
		PutPixel ( border_left,  i, DARK_GRAY, TRUE, VGA );
		PutPixel ( border_right, i, DARK_GRAY, TRUE, VGA );
	}
}

int main ( void )
{
	// Initialize local variables.

	int screen_width  = 320;			// Mode 13h screen width in pixels.
	int screen_height = 200;			// Mode 13h screen height in pixels.
	int i;								// Loop index.
	int x1, y1;						// Computed coordinates.
	WORD spriteSegment;					// Segment address of sprite buffer.
	WORD spriteOffset;					// Offset address of sprite buffer.

	// Clear the screen and print the project header and test plan.

	clrscr ();
	PrintProjectHeader ();

	// Print the function being tested and the test cases.

	printf ( "Function Tests:\n" );
	printf ( "- SetClipping\n\n" );
	printf ( "Clip region: (%d,%d)-(%d,%d)\n", CLIP_X1, CLIP_Y1, CLIP_X2, CLIP_Y2 );
	printf ( "Gray dashed border marks the clip boundary.\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Clipped FillRectangle\n" );
	printf ( "- Screen 2: Clipped FillTriangle\n" );
	printf ( "- Screen 3: Clipped FillQuad\n" );
	printf ( "- Screen 4: Clipped ScaleImage\n" );
	printf ( "- Screen 5: Same ScaleImage with full-screen clip (reset)\n\n" );
	printf ( "Press any key to begin...\n" );

	// Wait for key press before starting tests.

	getch  ();

	// Build a 16x16 gradient sprite.

	spriteSegment = FP_SEG ( (BYTE far *) spriteBuffer );
	spriteOffset  = FP_OFF ( (BYTE far *) spriteBuffer );

	for ( i = 0; i < SPRITE_SIZE; i++ )
	{
		spriteBuffer[i] = (BYTE) ( ( i / SPRITE_WIDTH + i % SPRITE_WIDTH ) * 2 + 32 );
	}

	SetMode13 ();

	SetClipping ( CLIP_X1, CLIP_Y1, CLIP_X2, CLIP_Y2 );

	//------------------------------------------------------------------------
	// Screen 1: Clipped FillRectangle
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );
	drawClipBorder ();

	FillRectangle ( 20,  10,  300, 190, GREEN,   VGA );   // Beyond clip on all sides.
	FillRectangle ( 120, 80,  200, 120, RED,     VGA );   // Fully inside clip region.
	FillRectangle ( 40,  60,  100, 90,  BLUE,    VGA );   // Partially outside left.
	FillRectangle ( 220, 130, 310, 160, MAGENTA, VGA );   // Partially outside right.

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: Clipped FillTriangle
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );
	drawClipBorder ();

	FillTriangle ( 160, -20, 350, 180, -30, 180, BLUE,    VGA );   // Beyond all clip edges.
	FillTriangle ( 30,  60,  130, 60,  80,  140, GREEN,   VGA );   // Clipped on the left.
	FillTriangle ( 200, 70,  300, 70,  250, 150, RED,     VGA );   // Clipped on the right.
	FillTriangle ( 120, 20,  200, 20,  160, 80,  MAGENTA, VGA );   // Clipped on top.

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: Clipped FillQuad
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );
	drawClipBorder ();

	FillQuad ( 20,  30,  300, 20,  310, 180, 10,  170, CYAN,   VGA );   // Beyond clip region.
	FillQuad ( 160, 30,  260, 100, 160, 170, 60,  100, BLUE,   VGA );   // Diamond on boundary.
	FillQuad ( 140, 80,  180, 85,  175, 115, 145, 110, YELLOW, VGA );   // Fully inside clip.

	getch ();

	//------------------------------------------------------------------------
	// Screen 4: Clipped ScaleImage
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );
	drawClipBorder ();

	// Scale 16x16 sprite to a very large area exceeding clip bounds.

	ScaleImage ( 20, 10, 300, 190, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	getch ();

	//------------------------------------------------------------------------
	// Screen 5: Reset clip and show unclipped ScaleImage
	//------------------------------------------------------------------------

	x1 = screen_width  - 1;
	y1 = screen_height - 1;

	SetClipping ( 0, 0, x1, y1 );

	ClearScreen ( BLACK, VGA );

	// Same ScaleImage now unclipped for comparison.

	ScaleImage ( 20, 10, 300, 190, SPRITE_WIDTH, SPRITE_HEIGHT, 255, spriteSegment, spriteOffset, VGA );

	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "Test complete.\n" );

	return 0;
}
