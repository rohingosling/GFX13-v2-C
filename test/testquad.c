//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Program: Test Quad
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Test program for Quad and FillQuad functions.
//
//   - Tests Quad with outline quadrilaterals of various shapes including
//     rectangles, parallelograms, trapezoids, and diamonds.
//
//   - Tests FillQuad with filled versions of the same shapes, plus
//     overlapping, edge cases, and clipping at screen edges.
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
	// Clear the screen and print the project header.

	clrscr ();
	PrintProjectHeader ();

	// Print the function tests and test cases.

	printf ( "Function Tests:\n" );
	printf ( "- Quad\n" );
	printf ( "- FillQuad\n\n" );
	printf ( "Test Cases:\n" );
	printf ( "- Screen 1: Quad - outline quads (various shapes)\n" );
	printf ( "- Screen 2: FillQuad - filled quads\n" );
	printf ( "- Screen 3: FillQuad - more shapes and edge cases\n" );
	printf ( "- Screen 4: FillQuad - clipping at screen edges\n\n" );
	printf ( "Press any key to begin...\n" );

	// Wait for user input before starting the tests.

	getch ();

	// Set video mode 13h.

	SetMode13 ();

	//------------------------------------------------------------------------
	// Screen 1: Outline quads
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	Quad ( 10,  10,  80,  10,  80,  60,  10,  60,  BLUE,    VGA );   // Axis-aligned rectangle.
	Quad ( 110, 10,  190, 10,  170, 60,  90,  60,  GREEN,   VGA );   // Parallelogram.
	Quad ( 210, 10,  310, 10,  290, 60,  230, 60,  CYAN,    VGA );   // Trapezoid.
	Quad ( 50,  100, 90,  140, 50,  180, 10,  140, RED,     VGA );   // Diamond.
	Quad ( 120, 100, 200, 110, 180, 180, 130, 170, MAGENTA, VGA );   // Irregular convex.
	Quad ( 230, 100, 240, 100, 245, 190, 225, 190, BROWN,   VGA );   // Narrow tall.
	Quad ( 270, 100, 310, 100, 310, 101, 290, 180, YELLOW,  VGA );   // Nearly a triangle.

	getch ();

	//------------------------------------------------------------------------
	// Screen 2: Filled quads
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	FillQuad ( 10,  10,  90,  10,  90,  60,  10,  60,  BLUE,    VGA );   // Filled rectangle.
	FillQuad ( 120, 10,  200, 10,  180, 60,  100, 60,  GREEN,   VGA );   // Filled parallelogram.
	FillQuad ( 220, 10,  310, 10,  290, 60,  240, 60,  CYAN,    VGA );   // Filled trapezoid.
	FillQuad ( 50,  100, 90,  140, 50,  180, 10,  140, RED,     VGA );   // Filled diamond.
	FillQuad ( 120, 100, 210, 110, 190, 185, 130, 170, MAGENTA, VGA );   // Filled irregular.
	FillQuad ( 240, 100, 250, 100, 255, 190, 235, 190, YELLOW,  VGA );   // Filled narrow tall.

	getch ();

	//------------------------------------------------------------------------
	// Screen 3: More filled quads and edge cases
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	FillQuad ( 10,  10,  310, 20,  300, 190, 20,  180, BLUE,   VGA );   // Large.
	FillQuad ( 140, 80,  180, 80,  180, 120, 140, 120, YELLOW, VGA );   // Small, on top.
	FillQuad ( 50,  50,  150, 40,  160, 120, 40,  130, GREEN,  VGA );   // Overlapping A.
	FillQuad ( 100, 60,  200, 70,  190, 150, 110, 140, RED,    VGA );   // Overlapping B.

	getch ();

	//------------------------------------------------------------------------
	// Screen 4: Clipping at screen edges
	//------------------------------------------------------------------------

	ClearScreen ( BLACK, VGA );

	// Quads extending beyond each screen edge.

	FillQuad ( 100, -40, 220, -40, 220, 60,  100, 60,  RED,     VGA );   // Off top.
	FillQuad ( 100, 140, 220, 140, 220, 240, 100, 240, GREEN,   VGA );   // Off bottom.
	FillQuad ( -60, 40,  40,  40,  40,  160, -60, 160, BLUE,    VGA );   // Off left.
	FillQuad ( 280, 40,  380, 40,  380, 160, 280, 160, CYAN,    VGA );   // Off right.

	// Diamonds clipped at corners.

	FillQuad ( -30, -30, 30,  0,   0,   30,  -30, 0,   MAGENTA, VGA );   // Top-left corner.
	FillQuad ( 290, 0,   350, -30, 350, 0,   320, 30,  YELLOW,  VGA );   // Top-right corner.
	FillQuad ( 0,   170, -30, 200, -30, 230, 30,  200, BROWN,   VGA );   // Bottom-left corner.
	FillQuad ( 320, 170, 290, 200, 350, 200, 350, 230, WHITE,   VGA );   // Bottom-right corner.

	// Large quad covering well beyond all edges.

	FillQuad ( -60, -40, 380, -40, 380, 240, -60, 240, DARK_GRAY, VGA );

	getch ();

	//--------------------------------------------------------------------
	// Done.
	//--------------------------------------------------------------------

	SetTextMode ( TEXT_MODE_25_ROWS );

	printf ( "Test complete.\n" );

	return 0;
}
