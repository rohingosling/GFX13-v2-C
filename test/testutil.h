//****************************************************************************
// Project: GFX-13 (Graphics Mode 13h Library)
// Module:  Test Utilities
// Version: 2.1
// Date:    1991-10-03
// Author:  Rohin Gosling
//
// Description:
//
//   Shared utility functions for GFX-13 test programs.
//
//   - PrintProjectHeader draws a CP437 box-drawing border around the
//     project name, version, date, and author.
//
//   - All functions are declared static so this header can be included
//     in multiple test programs without linker conflicts.
//
//****************************************************************************

#ifndef TESTUTIL_H
#define TESTUTIL_H

#include <stdio.h>
#include <string.h>

// CP437 box-drawing characters.

#define BOX_TOP_LEFT       '\xDA'
#define BOX_TOP_RIGHT      '\xBF'
#define BOX_BOTTOM_LEFT    '\xC0'
#define BOX_BOTTOM_RIGHT   '\xD9'
#define BOX_HORIZONTAL     '\xC4'
#define BOX_VERTICAL       '\xB3'

#define BOX_WIDTH          79

//----------------------------------------------------------------------------
// PrintBoxBorder
//
// Prints a horizontal border line using CP437 box-drawing characters.
//
// Parameters:
//
//   left (char):
//   - Left corner character  (BOX_TOP_LEFT or BOX_BOTTOM_LEFT).
//
//   right (char):
//   - Right corner character (BOX_TOP_RIGHT or BOX_BOTTOM_RIGHT).
//
//----------------------------------------------------------------------------

static void PrintBoxBorder ( char left, char right )
{
	// Print the left corner character.
	
	putchar ( left );

	// Print the horizontal line characters to fill the box width.

	int i;
	for ( i = 0; i < BOX_WIDTH - 2; i++ )
	{
		putchar ( BOX_HORIZONTAL );
	}

	// Print the right corner character and a newline.

	putchar ( right );
	putchar ( '\n' );
}

//----------------------------------------------------------------------------
// PrintBoxLine
//
// Prints a line of text inside the box, padded with spaces to fill the
// box width.
//
// Parameters:
//
//   text (const char *):
//   - The text to display inside the box.
//
//----------------------------------------------------------------------------

static void PrintBoxLine ( const char *text )
{
	// Initialize local variables.

	int length;							// Length of text string.
	int padding;						// Number of trailing spaces needed.
	int i;								// Loop index.

	// Calculate the length of the text and the padding needed to fill the

	length  = strlen ( text );
	padding = BOX_WIDTH - 2 - 1 - length;

	// Print the line with vertical borders and padding.

	putchar  ( BOX_VERTICAL );
	putchar  ( ' ' );
	printf   ( "%s", text );

	// Print the padding spaces to fill the box width.

	for ( i = 0; i < padding; i++ )
	{
		putchar ( ' ' );
	}

	// Print the right vertical border and a newline.

	putchar ( BOX_VERTICAL );
	putchar ( '\n' );
}

//----------------------------------------------------------------------------
// PrintProjectHeader
//
// Prints the GFX-13 project header inside a CP437 box-drawing border.
//----------------------------------------------------------------------------

static void PrintProjectHeader ( void )
{
	// Print the project header inside a box.

	PrintBoxBorder ( BOX_TOP_LEFT,    BOX_TOP_RIGHT    );
	PrintBoxLine   ( "Project: GFX-13 (Graphics Mode 13h Library)" );
	PrintBoxLine   ( "Version: 2.1" );
	PrintBoxLine   ( "Date:    1991" );
	PrintBoxLine   ( "Author:  Rohin Gosling" );
	PrintBoxBorder ( BOX_BOTTOM_LEFT, BOX_BOTTOM_RIGHT );
	printf         ( "\n" );
}

#endif
