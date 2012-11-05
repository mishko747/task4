/**
* @file task4.cpp
* @brief The source file project of task4
*/

#include "task4.h"

#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>


// This function creates a simple window size of 1 character
window* CreateSimpleWindow ( UCHAR name )
{
	window *pWin = new window;
	if(!pWin)	// if not allocated memory under the window
	{
		return NULL;
	}
	
	pWin->name = name;
	pWin->h = MIN_SIZE_WINDOW;
	pWin->w = MIN_SIZE_WINDOW;

	// If not allocated memory for the matrix that display window
	if(	( pWin->data = new UCHAR[MIN_SIZE_WINDOW * MIN_SIZE_WINDOW] ) == NULL ||
		memset(pWin->data, ' ', MIN_SIZE_WINDOW * MIN_SIZE_WINDOW * SIZE_OF_UCHAR) == NULL )
	{
		delete pWin;
		return NULL;
	}

	//	Construction window
	UCHAR *pEndData = pWin->data + MIN_SIZE_WINDOW * MIN_SIZE_WINDOW;
	for(UCHAR i = 1U; i < MIN_SIZE_WINDOW - 1U; ++i)
	{
		*(pWin->data + i) = '-';	// Drawing top border
		*(pEndData - i - 1U) = '-';		// Drawing bottom border
		*( &( *( pWin->data + i * MIN_SIZE_WINDOW ) = '|' ) + MIN_SIZE_WINDOW - 1U ) = '|'; // Drawing left and right border
	}

	*pWin->data = pWin->name;

	*( pWin->data + MIN_SIZE_WINDOW - 1U ) = '*';	// top right angle
	*( pEndData - 1U ) = '*';						// bottom right angle
	*( pEndData - MIN_SIZE_WINDOW ) = '*';		// bottom left angle

	pWin->pL = NULL;
	pWin->pR = NULL;

	return pWin;
}

// This function changes the height of the window to the specified size
bool VerticalResize ( window *pRoot, UCHAR height )
{
	UCHAR *pData;	// pointer to an array of new windows
	if( !pRoot || !(pData = new UCHAR[pRoot->w * height]) )
	{
		return false;
	}

	UCHAR sizeOfRow = pRoot->w * SIZE_OF_UCHAR;
	
	// Copy array without the last row
	memcpy(pData, pRoot->data, sizeOfRow * pRoot->h - pRoot->w);	
	
	// Copy the last line of an array in the last line of the new array
	memcpy(pData + (height - 1U) * pRoot->w, pRoot->data + (pRoot->h - 1U) * pRoot->w, sizeOfRow);
	
	// Fill in the missing lines
	UCHAR *pLast = pData + ( pRoot->h - 2U) * pRoot->w;	// Pointer to the last but one row
	for(UCHAR i = pRoot->h - 1U ; i < height - 1U; ++i)
	{
		memcpy(pData + i * pRoot->w, pLast, sizeOfRow);
	}

	pRoot->h = height;
	delete pRoot->data;
	pRoot->data = pData;
	return true;
}

// This function changes the width of the window to the specified size
bool HorizontalResize ( window *pRoot, UCHAR width )
{
	UCHAR *pData;	// pointer to an array of new windows
	if( !pRoot || !(pData = new UCHAR[pRoot->h * width]) )
	{
		return false;
	}

	UCHAR  sizeOfRow = pRoot->w * SIZE_OF_UCHAR - 1U;

	for(UCHAR i = 0; i < pRoot->h; ++i)
	{
		*( pData + i * width + width - 1U ) = *( pRoot->data + i * pRoot->w + pRoot->w - 1U );	// Copying the last column
		memcpy(pData + i * width, pRoot->data + i * pRoot->w, sizeOfRow);	// Copying lines
		memset(pData + i * width + pRoot->w - 1U, *( pData + i * width + pRoot->w - 2U ) , (width - pRoot->w) * SIZE_OF_UCHAR);
	}
	
	pRoot->w = width;
	delete pRoot->data;
	pRoot->data = pData;
	return true;
}

// This function create tree
window* CreateTree ( UCHAR *&pStr )
{
	if(*pStr != '|' && *pStr != '-')	// If the letter then return pointer to the simple window
	{
		return CreateSimpleWindow( *pStr );	// Create and return a simple window
	}
	
	window *root = new window;
	if( !root )
	{
		return NULL;
	}

	root->name = *pStr;

	root->pL = CreateTree( ++pStr );	// Create a the left subtree
	if( !(root->pL) )
	{
		delete root;
		return NULL;
	}

	root->pR = CreateTree( ++pStr ); // Create a the right subtree
	if( !(root->pR) )
	{
		delete root;
		return NULL;
	}
	
	return root;
}

// This function builds the main window
window* BuildingMainWindow ( window *pRoot )
{
	if( !pRoot || !pRoot->pL || !pRoot->pR )
	{
		return NULL;
	}
	
	if( pRoot->pL->name == '|' || pRoot->pL->name == '-' )
	{
		if( !BuildingMainWindow( pRoot->pL ) )
		{
			return NULL;
		}
	}

	if( pRoot->pR->name == '|' || pRoot->pR->name == '-' )
	{
		if( !BuildingMainWindow( pRoot->pR ) )
		{
			return NULL;
		}
	}

	return BuildingWindow ( pRoot );
}

// This function unites two windows
window* BuildingWindow ( window* pRoot )
{

	
	if( !pRoot->pL || !pRoot->pR )	// If the windows not exist
	{
		return NULL;
	}

	switch( pRoot->name )
	{
		case '|':
		{
			// determination the size of the window
			if( pRoot->pL->h == pRoot->pR->h )
			{
				pRoot->h = pRoot->pL->h;
			}
			else
			{
				if( pRoot->pL->h > pRoot->pR->h )
				{
					pRoot->h = pRoot->pL->h;
					if( VerticalResize(pRoot->pR, pRoot->h) == false )
					{
						return NULL;
					}
				}
				else
				{
					pRoot->h = pRoot->pR->h;
					if( VerticalResize(pRoot->pL, pRoot->h) == false )
					{
						return NULL;
					}
				}	// End if(pRoot->pL->h > pRoot->pR->h)

			}	// End if(pRoot->pL->h == pRoot->pR->h)
			pRoot->w = pRoot->pL->w + pRoot->pR->w - 1U;

			unsigned short sizeArr = pRoot->w * pRoot->h; // Size of the array to store window

			// Creating an array and filling it with spaces
			if(	( pRoot->data = new UCHAR[sizeArr] ) == NULL ||
				memset(pRoot->data, ' ', sizeArr * SIZE_OF_UCHAR) == NULL )
			{
				printf("Memory allocation failed!");
				return NULL;
			}
	
			// Building a united window
			UCHAR i = 0;
			UCHAR LWidth = ( pRoot->pL->w ) * SIZE_OF_UCHAR;
			UCHAR RWidth = ( pRoot->pR->w - 1U ) * SIZE_OF_UCHAR;
			for(i = 0U; i < pRoot->h; ++i)
			{
				memcpy(pRoot->data + i * pRoot->w, pRoot->pL->data + i * pRoot->pL->w, LWidth);
				if( *( pRoot->pR->data + i * pRoot->pR->w ) != '|' )
				{
					*(pRoot->data + i * pRoot->w + pRoot->pL->w - 1U) = *( pRoot->pR->data + i * pRoot->pR->w );
				}
				memcpy(pRoot->data + i * pRoot->w + LWidth, pRoot->pR->data + i * pRoot->pR->w + 1U, RWidth);
			}

			break;
		}	// End of case '|'

		case '-':
		{
			// determination the size of the window
			if( pRoot->pL->w == pRoot->pR->w )
			{
				pRoot->w = pRoot->pL->w;
			}
			else
			{
				if( pRoot->pL->w > pRoot->pR->w )
				{
					pRoot->w = pRoot->pL->w;
					if( HorizontalResize(pRoot->pR, pRoot->w) == false )
					{
						return NULL;
					}
				}
				else
				{
					pRoot->w = pRoot->pR->w;
					if( HorizontalResize( pRoot->pL, pRoot->w) == false )
					{
						return NULL;
					}
				}
			}	// End if( pRoot->pL->w == pRoot->pR->w )
			pRoot->h = pRoot->pL->h + pRoot->pR->h - 1U;
			unsigned short sizeArr = pRoot->w * pRoot->h; // Size of the array to store window

			// Creating an array and filling it with spaces
			if(	( pRoot->data = new UCHAR[sizeArr] ) == NULL ||
				memset(pRoot->data, ' ', sizeArr * SIZE_OF_UCHAR) == NULL )
			{
				printf("Memory allocation failed!");
				return NULL;
			}
	
			// Building a united window
			UCHAR i = 0;
			UCHAR SizeOfLRow = pRoot->pL->w * SIZE_OF_UCHAR;
			UCHAR SizeOfRRow = pRoot->pR->w * SIZE_OF_UCHAR;
			for(i = 0U; i < pRoot->pL->h; ++i)
			{
				memcpy(pRoot->data + i * pRoot->w, pRoot->pL->data + i * pRoot->pL->w, SizeOfLRow);
			}

			i--;
			for(UCHAR n = 0U; n < pRoot->w; ++n)
			{
				if( *(pRoot->pR->data + n) != '-' )
				{
					*( pRoot->data + i * pRoot->w + n ) = *(pRoot->pR->data + n);
				}
			}

			for(UCHAR j = 1U; j < pRoot->pR->h; ++j)
			{
				memcpy(pRoot->data + (i + j) * pRoot->w, pRoot->pR->data + j * pRoot->pR->w, SizeOfRRow);
			}

			break;
		}	// End of case '-'

	}	// End of switch(type)

	PrintWindow( pRoot );

	return pRoot;
}

// This function displays a window
void PrintWindow ( window *pRoot )
{

	printf("\nWindow: %c %c %c\n", *(pRoot->pL->data), pRoot->name, *(pRoot->pR->data) );
	printf("----------------------------------------------------------\n");

	if( !pRoot || !(pRoot->data) )
	{
		printf("No data for output.\n");
		return;
	}

	UCHAR *tmp;

	for(UCHAR i = 0U; i < pRoot->h; ++i)
	{
		tmp = pRoot->data + i * pRoot->w;
		for(UCHAR j = 0U; j < pRoot->w; ++j)
		{
			printf("%c ", *(tmp + j));
		}
		printf("\n");
	}	// End for i

	printf("----------------------------------------------------------\n");
	return; 
}



void main ( void )
{
	UCHAR *pStr;	// down traversal of the tree
	window *pTree;	// a pointer to the root of the tree

	do
	{
		system("cls");
		pStr = (UCHAR *)("|-|-ABC-D|E-FG-P-|Q|RST");
	//	pStr = (UCHAR *)("-|-|ABC|D-E|FG|P|-Q-RST");

		printf("Str : %s\n", pStr);

		pTree = CreateTree( pStr );
		
		if( !pTree )
		{
			printf("Impossible to build a tree!\n");
			continue;
		}
		BuildingMainWindow( pTree );
	}
	while (CONTINUE && _getch() != CODE_ESC );	// while not pressed ESC

	return;
}

