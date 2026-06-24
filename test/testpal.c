//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Palette
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for SetPalette and WaitRetrace (palette animation)
//   functions.
//
//   - Displays the default 256-color palette as a 16x16 grid.
//
//   - Applies a custom rainbow gradient palette.
//
//   - Demonstrates smooth palette rotation animation using WaitRetrace.
//
//   - Applies a fire-like palette (black to red to yellow to white).
//
//   - Demonstrates 3-second random grayscale palette noise animation.
//
//   - Press any key to advance through each test screen.
//
//   - After the final screen, the program returns to text mode and prints
//     "Test complete."
//
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
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

// 256 colors x 3 components (R, G, B) = 768 bytes.
// VGA DAC uses 6-bit values: 0-63 per component.

static BYTE paletteData[768];

int main ( void )
{
	int screen_width       = 320;		// Mode 13h screen width in pixels.
	int screen_height      = 200;		// Mode 13h screen height in pixels.
	int row;							// Grid row index.
	int column;							// Grid column index.
	int x, y;							// Pixel coordinates for fill loops.
	int i;								// Loop index.
	int frame;							// Animation frame counter.
	int x0, y0, x1, y1;				// Rectangle coordinates.
	BYTE colorIndex;					// Current palette color index (0-255).
	BYTE pixelColor;					// Pixel color for PutPixel fill.
	BYTE grayscaleValue;				// Random grayscale value for palette animation.
	BYTE tempR;							// Temporary red component for palette rotation.
	BYTE tempG;							// Temporary green component for palette rotation.
	BYTE tempB;							// Temporary blue component for palette rotation.
	WORD paletteSegment;				// Segment address of palette data.
	WORD paletteOffset;					// Offset address of palette data.
	unsigned int animationDuration;		// Animation duration in milliseconds.
	clock_t startTime;					// Animation start time in clock ticks.
	clock_t durationTicks;				// Animation duration in clock ticks.

	clrscr ();

	PrintProjectHeader ();

	printf ( "Function Tests:\n" );
	printf ( "- SetPalette\n" );
	printf ( "- WaitRetrace\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Default palette - 16x16 color grid\n" );
	printf ( "- Screen 2: Custom gradient palette applied\n" );
	printf ( "- Screen 3: Palette rotation animation (press key to skip)\n" );
	printf ( "- Screen 4: Fire palette demonstration\n" );
	printf ( "- Screen 5: Grayscale gradient + 3-second palette noise\n\n" );
	printf ( "Press any key to begin...\n" );

	getch  ();

	paletteSegment = FP_SEG ( (BYTE far *) paletteData );
	paletteOffset  = FP_OFF ( (BYTE far *) paletteData );

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screen 1: Default palette color grid
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// 16x16 grid showing all 256 colors (20x12 cells).

	for ( row = 0; row < 16; row++ )
	{
		for ( column = 0; column < 16; column++ )
		{
			colorIndex = (BYTE) ( row * 16 + column );

			x0 = column * 20;
			y0 = row * 12;
			x1 = column * 20 + 19;
			y1 = row * 12 + 11;

			FillRectangle ( x0, y0, x1, y1, colorIndex, VGA );
		}
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: Custom gradient palette
	//------------------------------------------------------------------------

	// Build a smooth red -> yellow -> green -> cyan -> blue -> magenta cycle.

	for ( i = 0; i < 43; i++ )
	{
		// Colors 0-42: black to red.

		paletteData[i * 3 + 0] = (BYTE) ( i * 63 / 42 );
		paletteData[i * 3 + 1] = 0;
		paletteData[i * 3 + 2] = 0;
	}

	for ( i = 0; i < 43; i++ )
	{
		// Colors 43-85: red to yellow.

		paletteData[( 43 + i ) * 3 + 0] = 63;
		paletteData[( 43 + i ) * 3 + 1] = (BYTE) ( i * 63 / 42 );
		paletteData[( 43 + i ) * 3 + 2] = 0;
	}

	for ( i = 0; i < 42; i++ )
	{
		// Colors 86-127: yellow to green.

		paletteData[( 86 + i ) * 3 + 0] = (BYTE) ( 63 - i * 63 / 41 );
		paletteData[( 86 + i ) * 3 + 1] = 63;
		paletteData[( 86 + i ) * 3 + 2] = 0;
	}

	for ( i = 0; i < 43; i++ )
	{
		// Colors 128-170: green to cyan.

		paletteData[( 128 + i ) * 3 + 0] = 0;
		paletteData[( 128 + i ) * 3 + 1] = 63;
		paletteData[( 128 + i ) * 3 + 2] = (BYTE) ( i * 63 / 42 );
	}

	for ( i = 0; i < 43; i++ )
	{
		// Colors 171-213: cyan to blue.

		paletteData[( 171 + i ) * 3 + 0] = 0;
		paletteData[( 171 + i ) * 3 + 1] = (BYTE) ( 63 - i * 63 / 42 );
		paletteData[( 171 + i ) * 3 + 2] = 63;
	}

	for ( i = 0; i < 42; i++ )
	{
		// Colors 214-255: blue to magenta.

		paletteData[( 214 + i ) * 3 + 0] = (BYTE) ( i * 63 / 41 );
		paletteData[( 214 + i ) * 3 + 1] = 0;
		paletteData[( 214 + i ) * 3 + 2] = 63;
	}

	// Color 0 stays black.

	paletteData[0] = 0;
	paletteData[1] = 0;
	paletteData[2] = 0;

	SetPalette ( 0, 256, paletteSegment, paletteOffset );

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: Palette rotation animation
	//------------------------------------------------------------------------

	for ( frame = 0; frame < 512; frame++ )
	{
		if ( kbhit () ) break;

		WaitRetrace ();

		// Rotate palette entries 1-255, leave 0 as black.

		tempR = paletteData[1 * 3 + 0];
		tempG = paletteData[1 * 3 + 1];
		tempB = paletteData[1 * 3 + 2];

		for ( i = 1; i < 255; i++ )
		{
			paletteData[i * 3 + 0] = paletteData[( i + 1 ) * 3 + 0];
			paletteData[i * 3 + 1] = paletteData[( i + 1 ) * 3 + 1];
			paletteData[i * 3 + 2] = paletteData[( i + 1 ) * 3 + 2];
		}

		paletteData[255 * 3 + 0] = tempR;
		paletteData[255 * 3 + 1] = tempG;
		paletteData[255 * 3 + 2] = tempB;

		SetPalette ( 0, 256, paletteSegment, paletteOffset );
	}

	// Flush any key pressed during animation.

	while ( kbhit () ) getch ();
	getch ();

	//------------------------------------------------------------------------
	// Screen 4: Fire palette
	//------------------------------------------------------------------------

	// Build a fire-like palette: black -> red -> orange -> yellow -> white.

	for ( i = 0; i < 64; i++ )
	{
		// 0-63: black to red.

		paletteData[i * 3 + 0] = (BYTE) i;
		paletteData[i * 3 + 1] = 0;
		paletteData[i * 3 + 2] = 0;
	}

	for ( i = 0; i < 64; i++ )
	{
		// 64-127: red to orange/yellow.

		paletteData[( 64 + i ) * 3 + 0] = 63;
		paletteData[( 64 + i ) * 3 + 1] = (BYTE) i;
		paletteData[( 64 + i ) * 3 + 2] = 0;
	}

	for ( i = 0; i < 64; i++ )
	{
		// 128-191: yellow to white.

		paletteData[( 128 + i ) * 3 + 0] = 63;
		paletteData[( 128 + i ) * 3 + 1] = 63;
		paletteData[( 128 + i ) * 3 + 2] = (BYTE) i;
	}

	for ( i = 0; i < 64; i++ )
	{
		// 192-255: white.

		paletteData[( 192 + i ) * 3 + 0] = 63;
		paletteData[( 192 + i ) * 3 + 1] = 63;
		paletteData[( 192 + i ) * 3 + 2] = 63;
	}

	paletteData[0] = 0;
	paletteData[1] = 0;
	paletteData[2] = 0;

	SetPalette ( 0, 256, paletteSegment, paletteOffset );

	// Redraw the color grid with the new palette.

	ClearScreen ( BLACK, VGA );

	for ( row = 0; row < 16; row++ )
	{
		for ( column = 0; column < 16; column++ )
		{
			colorIndex = (BYTE) ( row * 16 + column );

			x0 = column * 20;
			y0 = row * 12;
			x1 = column * 20 + 19;
			y1 = row * 12 + 11;

			FillRectangle ( x0, y0, x1, y1, colorIndex, VGA );
		}
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 5: Grayscale palette noise animation
	//------------------------------------------------------------------------

	// Set palette indices 16-79 to a 64-level grayscale gradient.
	// Index 16 = RGB(0,0,0), ..., Index 79 = RGB(63,63,63).

	for ( i = 0; i < 64; i++ )
	{
		paletteData[i * 3 + 0] = (BYTE) i;
		paletteData[i * 3 + 1] = (BYTE) i;
		paletteData[i * 3 + 2] = (BYTE) i;
	}

	SetPalette ( 16, 64, paletteSegment, paletteOffset );

	// Fill the screen with random pixels using palette indices 16-79.

	srand ( (unsigned int) time ( NULL ) );

	ClearScreen ( BLACK, VGA );

	for ( y = 0; y < screen_height; y++ )
	{
		for ( x = 0; x < screen_width; x++ )
		{
			pixelColor = (BYTE) ( rand () % 64 + 16 );

			PutPixel ( x, y, pixelColor, FALSE, VGA );
		}
	}

	getch ();

	// Animate palette for 3 seconds with random grayscale values.

	animationDuration = 3000;
	durationTicks     = (clock_t) ( (long) animationDuration * 182L / 10000L );
	startTime         = clock ();

	do
	{
		WaitRetrace ();

		for ( i = 0; i < 64; i++ )
		{
			grayscaleValue = (BYTE) ( rand () % 64 );

			paletteData[i * 3 + 0] = grayscaleValue;
			paletteData[i * 3 + 1] = grayscaleValue;
			paletteData[i * 3 + 2] = grayscaleValue;
		}

		SetPalette ( 16, 64, paletteSegment, paletteOffset );

	} while ( ( clock () - startTime ) < durationTicks );

	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "Test complete.\n" );

	return 0;
}
