//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Rectangle
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for Rectangle and FillRectangle functions.
//
//   - Tests Rectangle with nested outlines and edge cases (small, 1-pixel,
//     reversed coordinates).
//
//   - Tests FillRectangle with overlapping fills, small sizes, and
//     full-screen coverage.
//
//   - Press any key to advance through each test screen.
//
//   - After the final screen, the program returns to text mode and prints
//     "Test complete."
//
//****************************************************************************

#include <stdio.h>
#include <conio.h>
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

int main ( void )
{
	// Initialize local variables.

	int screen_width  = 320;			// Mode 13h screen width in pixels.
	int screen_height = 200;			// Mode 13h screen height in pixels.
	int i;								// Loop index.
	int x0, y0, x1, y1;				// Rectangle coordinates.
	BYTE color;							// Rectangle color.

	// Variables for GetPixel verification in testpix.c.

	clrscr ();
	PrintProjectHeader ();

	// Print the function tests and test cases.

	printf ( "Function Tests:\n" );
	printf ( "- Rectangle\n" );
	printf ( "- FillRectangle\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Rectangle - nested outlines\n" );
	printf ( "- Screen 2: Rectangle - edge cases (small, 1-px, reversed)\n" );
	printf ( "- Screen 3: FillRectangle - overlapping colored fills\n" );
	printf ( "- Screen 4: FillRectangle - small sizes and full screen\n\n" );
	printf ( "Press any key to begin...\n" );

	// Wait for user input before starting the tests.	

	getch  ();

	// 

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screen 1: Nested outline rectangles
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	for ( i = 0; i < 12; i++ )
	{
		x0    = 5 + i * 10;
		y0    = 3 + i * 7;
		x1    = 314 - i * 10;
		y1    = 196 - i * 7;
		color = (BYTE) ( i + 1 );

		Rectangle ( x0, y0, x1, y1, color, VGA );
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: Edge cases
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	Rectangle ( 10,  10,  80,  50,  WHITE,         VGA );   // Normal rectangle.
	Rectangle ( 100, 10,  101, 11,  YELLOW,        VGA );   // Tiny 2x2.
	Rectangle ( 120, 10,  120, 50,  LIGHT_MAGENTA, VGA );   // 1-pixel-wide (vertical line).
	Rectangle ( 130, 10,  200, 10,  LIGHT_RED,     VGA );   // 1-pixel-tall (horizontal line).
	Rectangle ( 200, 50,  140, 20,  LIGHT_CYAN,    VGA );   // Reversed coordinates.

	x1 = screen_width  - 1;
	y1 = screen_height - 1;

	Rectangle ( 0, 0, x1, y1, RED, VGA );   // Full-screen rectangle.

	// Squares of decreasing size.

	for ( i = 0; i < 8; i++ )
	{
		x0    = 10 + i * 3;
		y0    = 70 + i * 3;
		x1    = 90 - i * 3;
		y1    = 150 - i * 3;
		color = (BYTE) ( i + 1 );

		Rectangle ( x0, y0, x1, y1, color, VGA );
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: Overlapping filled rectangles
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	FillRectangle ( 20,  20,  150, 100, BLUE,    VGA );   // Blue.
	FillRectangle ( 50,  40,  180, 120, GREEN,   VGA );   // Green.
	FillRectangle ( 80,  60,  210, 140, RED,     VGA );   // Red.
	FillRectangle ( 110, 80,  240, 160, MAGENTA, VGA );   // Magenta.
	FillRectangle ( 140, 30,  300, 90,  YELLOW,  VGA );   // Yellow.
	FillRectangle ( 10,  150, 310, 190, CYAN,    VGA );   // Cyan.

	getch ();

	//------------------------------------------------------------------------
	// Screen 4: Small sizes and full screen
	//------------------------------------------------------------------------

	// Full-screen fill.

	x1 = screen_width  - 1;
	y1 = screen_height - 1;

	FillRectangle ( 0, 0, x1, y1, DARK_GRAY, VGA );

	// 1-pixel fill.

	FillRectangle ( 160, 100, 160, 100, WHITE, VGA );

	// Thin vertical stripe.

	FillRectangle ( 50, 10, 50, 190, YELLOW, VGA );

	// Thin horizontal stripe.

	FillRectangle ( 10, 100, 310, 100, LIGHT_MAGENTA, VGA );

	// Small filled squares.

	for ( i = 0; i < 10; i++ )
	{
		x0    = 100 + i * 15;
		x1    = 110 + i * 15;
		color = (BYTE) ( i + 1 );

		FillRectangle ( x0, 40, x1, 60, color, VGA );
	}

	// Reversed coordinates.

	FillRectangle ( 250, 150, 200, 120, BROWN, VGA );

	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "Test complete.\n" );

	return 0;
}
