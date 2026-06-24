//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Triangles
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for Triangle and FillTriangle functions.
//
//   - Displays various triangles on the screen, including degenerate cases
//     and clipping at screen edges, to verify correct rendering.
//
//	 - Press any key to advance through each test screen.
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
	// Clear the screen and print the project header.

	clrscr ();
	PrintProjectHeader ();

	// Print the function tests and test cases.

	printf ( "Function Tests:\n" );
	printf ( "- Triangle\n" );
	printf ( "- FillTriangle\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Triangle - outline triangles (various shapes)\n" );
	printf ( "- Screen 2: Triangle - more outlines (degenerate cases)\n" );
	printf ( "- Screen 3: FillTriangle - filled triangles\n" );
	printf ( "- Screen 4: FillTriangle - flat-top, flat-bottom, large\n" );
	printf ( "- Screen 5: FillTriangle - clipping at screen edges\n\n" );
	printf ( "Press any key to begin...\n" );

	// Wait for user input before starting the tests. 	

	getch ();

	// 

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screen 1: Outline triangles
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	Triangle ( 40,  10,  10,  80,  70,  80,  BLUE,    VGA );   // Equilateral-ish.
	Triangle ( 90,  10,  90,  80,  160, 80,  GREEN,   VGA );   // Right-angled.
	Triangle ( 200, 80,  170, 10,  230, 10,  CYAN,    VGA );   // Inverted.
	Triangle ( 260, 10,  250, 90,  270, 90,  RED,     VGA );   // Thin tall.
	Triangle ( 10,  110, 150, 110, 80,  140, MAGENTA, VGA );   // Wide flat.
	Triangle ( 160, 110, 310, 190, 160, 190, BROWN,   VGA );   // Large, bottom row.

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: Degenerate outlines
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	Triangle ( 20,  20,  80,  20,  50,  80,  YELLOW,        VGA );  // Flat-top.
	Triangle ( 120, 80,  100, 20,  140, 20,  LIGHT_MAGENTA, VGA );  // Flat-bottom.
	Triangle ( 180, 20,  182, 80,  178, 80,  LIGHT_RED,     VGA );  // Very thin.
	Triangle ( 200, 50,  250, 50,  300, 50,  WHITE,         VGA );  // Collinear (degenerate).
	Triangle ( 50,  120, 52,  125, 48,  125, LIGHT_CYAN,    VGA );  // Very small.
	Triangle ( 0,   0,   319, 0,   160, 199, LIGHT_BLUE,    VGA );  // Near-screen-edge.

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: Filled triangles
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	FillTriangle ( 40,  10,  10,  90,  70,  90,  BLUE,    VGA );  // Standard.
	FillTriangle ( 120, 10,  80,  90,  160, 90,  GREEN,   VGA );  // Right-angled.
	FillTriangle ( 230, 90,  170, 10,  290, 10,  RED,     VGA );  // Inverted.
	FillTriangle ( 10,  110, 70,  110, 40,  190, MAGENTA, VGA );  // Bottom-left.
	FillTriangle ( 160, 190, 100, 110, 220, 150, BROWN,   VGA );  // Bottom-center.
	FillTriangle ( 250, 110, 310, 110, 280, 190, YELLOW,  VGA );  // Bottom-right.

	getch ();

	//------------------------------------------------------------------------
	// Screen 4: Flat-top, flat-bottom, large
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	FillTriangle ( 20,  20,  100, 20,  60,  90,  BLUE,    VGA );  // Flat-top.
	FillTriangle ( 60,  10,  120, 90,  20,  90,  GREEN,   VGA );  // Flat-bottom.
	FillTriangle ( 160, 5,   310, 190, 160, 190, CYAN,    VGA );  // Large.
	FillTriangle ( 130, 50,  230, 50,  180, 150, RED,     VGA );  // Overlapping A.
	FillTriangle ( 180, 30,  280, 130, 80,  130, MAGENTA, VGA );  // Overlapping B.
	FillTriangle ( 20,  150, 25,  160, 15,  160, YELLOW,  VGA );  // Very small.

	getch ();

	//------------------------------------------------------------------------
	// Screen 5: Clipping at screen edges
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Triangles extending beyond each screen edge.

	FillTriangle ( 160, -40, 220, 60,  100, 60,  RED,     VGA );   // Off top.
	FillTriangle ( 160, 240, 100, 140, 220, 140, GREEN,   VGA );   // Off bottom.
	FillTriangle ( -40, 100, 60,  40,  60,  160, BLUE,    VGA );   // Off left.
	FillTriangle ( 360, 100, 260, 40,  260, 160, CYAN,    VGA );   // Off right.

	// Triangles extending beyond two corners.

	FillTriangle ( -20, -20, 80,  -20, -20, 80,  MAGENTA, VGA );   // Top-left corner.
	FillTriangle ( 340, -20, 340, 80,  240, -20, YELLOW,  VGA );   // Top-right corner.
	FillTriangle ( -20, 220, -20, 140, 80,  220, BROWN,   VGA );   // Bottom-left corner.
	FillTriangle ( 340, 220, 240, 220, 340, 140, WHITE,   VGA );   // Bottom-right corner.

	// Large triangle covering well beyond all edges.

	FillTriangle ( 160, -100, 400, 300, -80, 300, DARK_GRAY, VGA );

	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "Test complete.\n" );

	return 0;
}
