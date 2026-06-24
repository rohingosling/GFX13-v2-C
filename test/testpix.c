//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Pixel
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for SetMode13, SetTextMode, ClearScreen, WaitRetrace,
//   PutPixel, and GetPixel functions.
//
//   - Tests ClearScreen with solid color fills.
//
//   - Tests PutPixel with color gradients and dot grid patterns.
//
//   - Verifies GetPixel reads back correct color values.
//
//   - Tests WaitRetrace with a smooth moving pixel.
//
//   - Tests white noise animation with a custom grayscale palette.
//
//   - Press any key to advance through each test screen.
//
//   - After the final screen, the program returns to text mode and prints
//     GetPixel verification results and "Test complete."
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

int main ( void )
{
	// Initialize local variables.

	int          screen_width  = 320;		// Mode 13h screen width in pixels.
	int          screen_height = 200;    	// Mode 13h screen height in pixels.
	int          x;							// Pixel x coordinate.
	int          y;							// Pixel y coordinate.
	int          previous_x;				// Previous pixel x coordinate (for erase).
	BYTE         pixelColor;				// Used to set the color of pixels and read back colors with GetPixel for verification.
	BYTE         cornerTopLeft;				// _
	BYTE         cornerTopRight;			//  |_ USed to store the colors of the corner pixels for verification.
	BYTE         cornerBottomLeft;			// _| 
	BYTE         cornerBottomRight;			//
	BYTE         clip;						// Clipping flag for PutPixel (TRUE to enable clipping, FALSE to disable).
	unsigned int frameDelay;				// Delay between white noise frames in milliseconds.
	unsigned int totalDuration;				// Total white noise duration in milliseconds.
	clock_t      startTime;					// White noise animation start time in clock ticks.
	clock_t      durationTicks;				// White noise animation duration in clock ticks.
	BYTE         noiseColor;				// Random color for white noise pixel.
	BYTE         grayscalePalette[195];		// Grayscale ramp palette data (65 entries x 3 RGB bytes).
	WORD         paletteSegment;			// Segment address of palette data.
	WORD         paletteOffset;				// Offset address of palette data.

	// Clear the screen and print the project header and test plan.

	clrscr ();
	PrintProjectHeader ();

	printf ( "Function Tests:\n" );
	printf ( "- SetMode13\n" );
	printf ( "- SetTextMode\n" );
	printf ( "- ClearScreen\n" );
	printf ( "- WaitRetrace\n" );
	printf ( "- PutPixel\n" );
	printf ( "- GetPixel\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1, 2, 3: ClearScreen - Red fill, GReen fill, blue fill\n" );	
	printf ( "- Screen 5:       PutPixel - (x + y) color gradient\n" );
	printf ( "- Screen 6:       PutPixel - colored dot grid + GetPixel verify\n" );
	printf ( "- Screen 7:       PutPixel - screen corner pixels + GetPixel verify\n" );
	printf ( "- Screen 8:       WaitRetrace - smooth moving pixel\n" );
	printf ( "- Screen 9:       White noise - 5 second grayscale noise animation\n\n" );
	printf ( "Press any key to begin...\n" );
	
	getch  ();

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screenss 1, 2, and 3: ClearScreen with red, green, and blue fills.
	//------------------------------------------------------------------------

	ClearScreen ( RED, VGA );
	getch       ();

	ClearScreen ( GREEN, VGA );
	getch       ();

	ClearScreen ( BLUE, VGA );
	getch       ();

	//------------------------------------------------------------------------
	// Screen 5: PutPixel gradient
	//------------------------------------------------------------------------

	for ( y = 0; y < screen_height; y++ )
	{
		for ( x = 0; x < screen_width; x++ )
		{
			// Color cycles through 0-255 as x and y increase, 
			// creating a gradient effect.
			// - We "AND" by 0xFF to ensure the color wraps around at 255.

			pixelColor = (BYTE) ( ( x + y ) & 0xFF );

			PutPixel ( x, y, pixelColor, TRUE, VGA );
		}
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 6: Dot grid + GetPixel
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	for ( y = 5; y < screen_height; y += 10 )
	{
		for ( x = 5; x < screen_width; x += 10 )
		{
			// Color is determined by the grid position, creating a 
			// repeating pattern.
			// - The color cycles through 1-15 (EGA colors) based on the 
			//   integer division of x and y by 10.

			pixelColor = (BYTE) ( ( ( x / 10 ) + ( y / 10 ) ) % 15 + 1 );
			clip       = TRUE;

			PutPixel ( x, y, pixelColor, TRUE, VGA );
		}
	}

	// (5,5): color = (0 + 0) % 15 + 1 = 1

	pixelColor = GetPixel ( 5, 5, TRUE, VGA );

	getch ();

	//------------------------------------------------------------------------
	// Screen 5: Corner pixels
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	x = screen_width  - 1;
	y = screen_height - 1;

	PutPixel ( 0, 0, WHITE, TRUE, VGA );
	PutPixel ( x, 0, WHITE, TRUE, VGA );
	PutPixel ( 0, y, WHITE, TRUE, VGA );
	PutPixel ( x, y, WHITE, TRUE, VGA );

	cornerTopLeft     = GetPixel ( 0, 0, TRUE, VGA );
	cornerTopRight    = GetPixel ( x, 0, TRUE, VGA );
	cornerBottomLeft  = GetPixel ( 0, y, TRUE, VGA );
	cornerBottomRight = GetPixel ( x, y, TRUE, VGA );

	getch ();

	//------------------------------------------------------------------------
	// Screen 6: WaitRetrace - moving dot
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	for ( x = 1; x < 319; x++ )
	{
		previous_x = x - 1;

		WaitRetrace ();
		PutPixel ( previous_x, 100, BLACK, TRUE, VGA );
		PutPixel ( x,          100, WHITE, TRUE, VGA );
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 9: White noise animation
	//------------------------------------------------------------------------

	// Seed the random number generator.

	srand ( (unsigned int) time ( NULL ) );

	// Configure white noise animation parameters.

	frameDelay    = 0;		// Delay between frames in milliseconds.
	totalDuration = 1000;	// Total animation duration in milliseconds.

	// Set up grayscale palette ramp for indices 16-80.
	// Index 16 = RGB(0,0,0), Index 17 = RGB(1,1,1), ... Index 80 = RGB(64,64,64).

	for ( x = 0; x < 65; x++ )
	{
		grayscalePalette[x * 3 + 0] = (BYTE) x;
		grayscalePalette[x * 3 + 1] = (BYTE) x;
		grayscalePalette[x * 3 + 2] = (BYTE) x;
	}

	paletteSegment = FP_SEG ( (BYTE far *) grayscalePalette );
	paletteOffset  = FP_OFF ( (BYTE far *) grayscalePalette );

	SetPalette ( 16, 65, paletteSegment, paletteOffset );

	// Render white noise animation.
	// - Each frame fills the screen with random palette indices from 16 to 80.
	// - Use clock() for real-time measurement so the total duration is accurate
	//   regardless of how long each frame takes to render.
	// - BIOS timer ticks at 18.2 Hz: ticks = milliseconds * 182 / 10000.

	durationTicks = (clock_t) ( (long) totalDuration * 182L / 10000L );
	startTime     = clock ();

	// Loop until the total duration has elapsed.

	do
	{
		// Wait for vertical retrace to minimize tearing during animation.

		WaitRetrace ();

		// Fill the screen with random grayscale colors from the palette ramp.

		for ( y = 0; y < screen_height; y++ )
		{
			for ( x = 0; x < screen_width; x++ )
			{
				noiseColor = (BYTE) ( ( rand () % 65 ) + 16 );

				PutPixel ( x, y, noiseColor, TRUE, VGA );
			}
		}

		// Optional delay between frames to control animation speed.

		if ( frameDelay > 0 )
		{
			delay ( frameDelay );
		}
	} 
	while ( ( clock () - startTime ) < durationTicks );

	// Wait for final key press before exiting.

	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "GetPixel Results:\n\n" );

	printf ( "  Grid (5,5):  returned %3d, expected   1 - %s\n", (int) pixelColor,        pixelColor        == 1  ? "PASS" : "FAIL" );
	printf ( "  Corner TL:   returned %3d, expected  10 - %s\n", (int) cornerTopLeft,     cornerTopLeft     == 10 ? "PASS" : "FAIL" );
	printf ( "  Corner TR:   returned %3d, expected  20 - %s\n", (int) cornerTopRight,    cornerTopRight    == 20 ? "PASS" : "FAIL" );
	printf ( "  Corner BL:   returned %3d, expected  30 - %s\n", (int) cornerBottomLeft,  cornerBottomLeft  == 30 ? "PASS" : "FAIL" );
	printf ( "  Corner BR:   returned %3d, expected  40 - %s\n", (int) cornerBottomRight, cornerBottomRight == 40 ? "PASS" : "FAIL" );

	printf ( "\nTest complete.\n" );

	return 0;
}
