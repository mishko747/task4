/**
* @file task4.cpp
* @brief The source file project of task4
*/

#include "task4.h"

#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

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

	printf("%c", pRoot->name);
	
	if( *(pRoot->pL->data) == '|' || *(pRoot->pL->data) == '-' )
	{
		if( !BuildingMainWindow( pRoot->pL ) )
		{
			return NULL;
		}
	}

	if( *(pRoot->pR->data) == '|' || *(pRoot->pR->data) == '-' )
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
			pRoot->pL->h > pRoot->pR->h ? pRoot->h = pRoot->pL->h : pRoot->h = pRoot->pR->h;
			pRoot->w = pRoot->pL->w + pRoot->pR->w - 1U;

			unsigned short sizeArr = pRoot->w * pRoot->h; // Size of the array to store window

			// Creating an array and filling it with spaces
			if(	( pRoot->data = new UCHAR[pRoot->w * pRoot->h] ) == NULL ||
				memset(pRoot->data, 'x', sizeArr * SIZE_OF_UCHAR) == NULL )
			{
				printf("Memory allocation failed!");
				return NULL;
			}
	
			UCHAR i = 0;
			for(i = 0U; i < pRoot->pL->h; ++i)
			{
				memcpy(pRoot->data + i * pRoot->w, pRoot->pL->data + i * pRoot->pL->w, pRoot->pL->w * SIZE_OF_UCHAR);
			}

			PrintWindow( pRoot );
/*			while(i < pW->h - 1U)
			{
				memcpy(pW->data + i * pW->w, pW1->data + (pW1->h - 1U) * pW1->w, pW1->w * sizeof(UCHAR));
				i++;
			}
			memcpy(pW->data + --i * pW->w, pW1->data + pW1->h * pW1->w, pW1->w * sizeof(UCHAR));
		*/
			break;
		}	// End of case '|'

		case '-':
		{

			// ...
			break;
		}	// End of case '-'

	}	// End of switch(type)
	return pRoot;
}

// This function displays a window
void PrintWindow ( window *pRoot )
{
	printf("\n----------------------------------------------------------\n");

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
		//pStr = (UCHAR *)("|-|-ABC-D|E-FG-P-|Q|RST");
		pStr = (UCHAR *)("|AB");

		printf("Str : %s\n", pStr);

		pTree = CreateTree( pStr );
		
		pTree = BuildingMainWindow( pTree );



		PrintWindow( pTree );
		
	}
	while (CONTINUE && _getch() != CODE_ESC );	// while not pressed ESC

	return;
}

