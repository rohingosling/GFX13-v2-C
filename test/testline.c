//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Line
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for Line function.
//
//   - Tests Line with starburst patterns, axis-aligned, diagonal, and
//     multi-directional lines.
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

#define FALSE		   0
#define TRUE		   1

int main ( void )
{
	// Initialize local variables.

	int screen_width  = 320;			// Mode 13h screen width in pixels.
	int screen_height = 200;			// Mode 13h screen height in pixels.
	int i;								// Loop index.
	int x0, y0, x1, y1;					// Line endpoints.
	int color;							// Line color.

	// Clear the screen and print the project header and test plan.

	clrscr ();
	PrintProjectHeader ();

	// Print the function being tested and the test cases.

	printf ( "Function Tests:\n" );
	printf ( "- Line\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Line - starburst from center\n" );
	printf ( "- Screen 2: Line - horizontal, vertical, and diagonal\n" );
	printf ( "- Screen 3: Line - clipping tests\n\n" );
	printf ( "Press any key to begin...\n" );

	// Wait for key press before starting tests.

	getch ();

	// Set graphics mode 13h.

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screen 1: Starburst from center
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Lines from center to top and bottom edges.

	for ( i = 0; i < screen_width; i += 8 )
	{
		x0    = 160;
		y0    = 100;
		x1    = i;
		y1    = 0;
		color = (BYTE) ( i / 8 + 1 );

		Line ( x0, y0, x1, y1, color, TRUE, VGA );

		x0	  = 160;
		y0	  = 100;
		x1	  = i;
		y1	  = screen_height - 1;
		color = (BYTE) ( i / 8 + 41 );

		Line ( x0, y0, x1, y1, color, TRUE, VGA );
	}

	// Lines from center to left and right edges.

	for ( i = 0; i < screen_height; i += 8 )
	{
		x0    = 160;
		y0    = 100;
		x1    = 0;
		y1    = i;
		color = (BYTE) ( i / 8 + 81 );

		Line ( x0, y0, x1, y1, color, TRUE, VGA );

		x0    = 160;
		y0    = 100;
		x1    = screen_width - 1;
		y1    = i;
		color = (BYTE) ( i / 8 + 106 );

		Line ( x0, y0, x1, y1, color, TRUE, VGA );
	}

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: Axis-aligned and diagonal lines
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	Line ( 11,  10,  310, 10,  WHITE,      TRUE, VGA );   	// Horizontal.
	Line ( 10,  11,  10,  190, WHITE,      TRUE, VGA );   	// Vertical.
	Line ( 12,  12,  190, 190, WHITE,      TRUE, VGA );   	// 45-degree diagonal.
	Line ( 170, 20,  171, 180, LIGHT_BLUE, TRUE, VGA );   	// Steep diagonal.
	Line ( 20,  110, 300, 111, LIGHT_RED,  TRUE, VGA );   	// Shallow diagonal.
	Line ( 160, 100, 160, 100, WHITE,      TRUE, VGA );   	// Same-point (single pixel).

	// Boarders lines.

	Line ( 0,   0,   319, 0,   LIGHT_GRAY, TRUE, VGA );		// Top
	Line ( 0,   199, 319, 199, DARK_GRAY,  TRUE, VGA ); 	// Bottom
	Line ( 0,   0,   0,   199, LIGHT_GRAY, TRUE, VGA );		// Left
	Line ( 319, 0,   319, 199, DARK_GRAY,  TRUE, VGA ); 	// Right

	// Lines in all four directions.

	Line ( 240, 100, 280, 60,  LIGHT_RED,   TRUE, VGA );	// Up-right.
	Line ( 240, 100, 280, 140, LIGHT_GREEN, TRUE, VGA );  	// Down-right.
	Line ( 240, 100, 200, 60,  LIGHT_BLUE,  TRUE, VGA );  	// Up-left.
	Line ( 240, 100, 200, 140, YELLOW,      TRUE, VGA );  	// Down-left.

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: Clipping tests
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Edges

	Line ( 50,  -10,  50,    20, RED,        TRUE,  VGA );			// Top
	Line ( 270,  20,  270,  -10, LIGHT_RED,  TRUE,  VGA );			//

	Line ( -10,  50,   20,  50,  GREEN,       TRUE, VGA );			// Left
	Line (  20,  150, -10,  150, LIGHT_GREEN, TRUE, VGA );			//

	Line ( 50,  210,  50,   180, BLUE,        TRUE, VGA );			// Bottom
	Line ( 270, 180,  270,  210, LIGHT_BLUE,  TRUE, VGA );			//

	Line ( 330,  50,  300,  50,  DARK_GRAY,   TRUE, VGA );			// Right
	Line ( 300, 150,  330,  150, LIGHT_GRAY,  TRUE, VGA );			//


	// Fully off-screen lines (should not draw anything).

	Line (  50, -20,   50, -10, YELLOW, TRUE, VGA );			// Top
	Line ( -20,  50,  -10,  50, YELLOW, TRUE, VGA );			// Left
	Line (  50, 220,   50, 210, YELLOW, TRUE, VGA );			// Bottom
	Line ( 340,  50,  320,  50, YELLOW, TRUE, VGA );			// Right

	// Full screen width and height lines.

	Line ( 150,  -10, 150,  210, RED,        TRUE,  VGA );		// Top to Bottom
	Line ( 170,  210, 170,  -10, RED,        TRUE,  VGA );		//
	Line ( -10,   90, 330,   90, GREEN,       TRUE, VGA );		// Left to Right
	Line ( 330,  110, -10,  110, LIGHT_GREEN, TRUE, VGA );		//

	// Full screen diagonal lines.

	Line ( -40, -25, 360, 225, BLUE, TRUE, VGA );				// Top-Left to Bottom-Right
	Line ( -40, 225, 360, -25, BLUE, TRUE, VGA );				// Bottom-Left to Top-Right
	
	getch ();

	Line ( 360, 225, -40, -25, LIGHT_BLUE, TRUE, VGA );			// Top-Left to Bottom-Right
	Line ( 360, -25, -40, 225, LIGHT_BLUE, TRUE, VGA );			// Bottom-Left to Top-Right

	


	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "Test complete.\n" );

	return 0;
}
