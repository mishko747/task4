/**
* @file task4.cpp
* @brief The source file project of task4
*/

#include "task4.h"

#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

// This function checks whether the string is a bypass tree top-down
bool IsTopDown ( UCHAR *pStr )
{
	if( *pStr != '|' && *pStr != '-')
	{
		return false;
	}

	UCHAR s = 0U;	// number of letters
	UCHAR op = 0U;	// number of operations
	UCHAR i = 0U;	// number of characters
	UCHAR tmp;

	do
	{
		if( s >= 2U && op > 0U)	// if there are two letters and over zero operations
		{
			s--;	// replace two letters on one
			op--;	// subtract one operation
		}
		else
		{
			tmp = *(pStr + i);
			if( tmp == '|' || tmp == '-' )	// if symbol is not the letter 
			{
				op++;	// add operation
			}
			else
			{
				if(tmp >= 'A' && tmp <= 'Z')
				{
					s++;	// add letter
				}
			}
			i++;	// increment number of characters
		}
	}
	while( op > 0U && i < MAX_SIZE);	// while the number of operations > zero and not end of line

	return op == 0U && s == 1U ? true : false;	// if the number of operations and letters is zero, returns true, otherwise - false
}

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

	unsigned short sizeOfArr = MIN_SIZE_WINDOW * MIN_SIZE_WINDOW;

	// If not allocated memory for the matrix that display window
	if( ( pWin->data = new UCHAR[ sizeOfArr ] ) == NULL )
	{
		return NULL;
	}

	if(	memset(pWin->data, ' ', sizeOfArr) == NULL )
	{
		return NULL;		
	}

	//	Construction window
	UCHAR *pEndData = pWin->data + sizeOfArr;

	for(UCHAR i = 1U; i < MIN_SIZE_WINDOW - 1U; ++i)
	{
		*(pWin->data + i) = '-';	// Drawing top border
		*(pEndData - i - 1U) = '-';		// Drawing bottom border
		*( &( *( pWin->data + i * MIN_SIZE_WINDOW ) = '|' ) + MIN_SIZE_WINDOW - 1U ) = '|'; // Drawing left and right border
	}

	*pWin->data = pWin->name;	// set name of window

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
	
	try
	{
		// Copy array without the last row
		memcpy(pData, pRoot->data, pRoot->w * pRoot->h - pRoot->w);	
	
		// Copy the last line of an array in the last line of the new array
		memcpy(pData + (height - 1U) * pRoot->w, pRoot->data + (pRoot->h - 1U) * pRoot->w, pRoot->w);
	
		// Fill in the missing lines
		UCHAR *pLast = pData + ( pRoot->h - 2U) * pRoot->w;	// Pointer to the last but one row
		for(UCHAR i = pRoot->h - 1U ; i < height - 1U; ++i)
		{
			memcpy(pData + i * pRoot->w, pLast, pRoot->w);
		}
	}
	catch(...)
	{
		delete pData;
		return false;
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

	UCHAR  sizeOfRow = pRoot->w - 1U;
	UCHAR pRootWm1 = pRoot->w - 1U;
	UCHAR wM1 = width - 1U;

	try
	{
		for(UCHAR i = 0; i < pRoot->h; ++i)
		{
			*( pData + i * width + wM1 ) = *( pRoot->data + i * pRoot->w + pRootWm1 );	// Copying the last column
			memcpy(pData + i * width, pRoot->data + i * pRoot->w, sizeOfRow);	// Copying lines
			// filling lines added
			memset(pData + i * width + pRootWm1, *( pData + i * width + pRootWm1 - 1U ) , width - pRoot->w);
		}
	}
	catch(...)
	{
		delete pData;
		return false;
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

			// Size of the array to store window
			unsigned short sizeArr = pRoot->w * pRoot->h; 

			// Creating an array and filling it with spaces
			if(	( pRoot->data = new UCHAR[sizeArr] ) == NULL )
			{
				printf("Memory allocation failed!");
				return NULL;
			}
			if(	memset(pRoot->data, ' ', sizeArr) == NULL )
			{
				delete pRoot->data;
				return NULL;
			}
	
			// Building a united window
			UCHAR LRowSize = pRoot->pL->w;
			UCHAR RRowSize = pRoot->pR->w - 1U;

			try
			{
				UCHAR i = 0;
				for(i = 0U; i < pRoot->h; ++i)
				{
					// copy the left window to the main
					memcpy(pRoot->data + i * pRoot->w, pRoot->pL->data + i * pRoot->pL->w, LRowSize);

					if( *( pRoot->pR->data + i * pRoot->pR->w ) != '|' )	// if the character is not '|'
					{	// overwrite it in the main window
						*(pRoot->data + i * pRoot->w + pRoot->pL->w - 1U) = *( pRoot->pR->data + i * pRoot->pR->w );
					}

					// copy the right window to the main
					memcpy(pRoot->data + i * pRoot->w + LRowSize, pRoot->pR->data + i * pRoot->pR->w + 1U, RRowSize);
				}
			}
			catch(...)
			{
				delete pRoot->data;
				return NULL;
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

			// Size of the array to store window
			unsigned short sizeArr = pRoot->w * pRoot->h; // Size of the array to store window

			// Creating an array and filling it with spaces
			if(	( pRoot->data = new UCHAR[sizeArr] ) == NULL )
			{
				printf("Memory allocation failed!");
				return NULL;
			}
	
			if( memset(pRoot->data, ' ', sizeArr) == NULL )
			{
				delete pRoot->data;
				return NULL;
			}


			// Building a united window
			UCHAR LRowSize = pRoot->pL->w;
			UCHAR RRowSize = pRoot->pR->w;

			try
			{
				UCHAR i = 0;
				// copy the left window to the main
				for(i = 0U; i < pRoot->pL->h; ++i)
				{
					memcpy(pRoot->data + i * pRoot->w, pRoot->pL->data + i * pRoot->pL->w, LRowSize);
				}
	
				i--;
				for(UCHAR n = 0U; n < pRoot->w; ++n)
				{
					if( *(pRoot->pR->data + n) != '-' )	// if the character is not '-'
					{	// overwrite it in the main window
						*( pRoot->data + i * pRoot->w + n ) = *(pRoot->pR->data + n);
					}
				}
				// copy the right window to the main
				for(UCHAR j = 1U; j < pRoot->pR->h; ++j)
				{
					memcpy(pRoot->data + (i + j) * pRoot->w, pRoot->pR->data + j * pRoot->pR->w, RRowSize);
				}
			}
			catch(...)
			{
				delete pRoot->data;
				return NULL;
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
	if( !pRoot || !(pRoot->data) )
	{
		printf("No data for output.\n");
		return;
	}

	printf("\nWindow: %c %c %c\n", *(pRoot->pL->data), pRoot->name, *(pRoot->pR->data) );
	printf("----------------------------------------------------------\n");

	for(UCHAR i = 0U; i < pRoot->h; ++i)
	{
		for(UCHAR j = 0U; j < pRoot->w; ++j)
		{
			printf("%c ", *(pRoot->data + i * pRoot->w + j));
		}
		printf("\n");
	}	// End for i

	printf("----------------------------------------------------------\n");
	return; 
}

// This function frees the dynamic memory that was allocated under the tree
bool DeleteTree( window *pRoot )
{
	if( !pRoot )
	{
		return false;
	}

	if( pRoot->data != NULL)	// if the memory allocated for the array
	{
		delete pRoot->data;	// delete array
	}

	if( pRoot->pL != NULL )	// if there is a left descendant
	{
		if( DeleteTree( pRoot->pL ) == false)	// delete left descendant
		{
			return false;
		}
	}

	if( pRoot->pR != NULL )	// if there is a right descendant
	{
		if( DeleteTree( pRoot->pR ) == false)	// delete right descendant
		{
			return false;
		}
	}

	delete pRoot;

	return true;
}

void main ( void )
{
	UCHAR *pStr;	// down traversal of the tree
	window *pTree;	// a pointer to the root of the tree
	if( !( pStr = new UCHAR[MAX_SIZE] ) )
	{
		printf("Memory allocation failed!\n");
		printf("Press any key to exit...");
		getch();
	}

	do
	{
		system("cls");
		printf("Enter a string that is the bypass binary tree top-down.\n");
		printf("String can contain the characters '|', '-' and letters A-Z.\n");
		printf("Enter string (max %u char): ", MAX_SIZE);
		fflush(stdin);
		if( !scanf_s( "%s", pStr, MAX_SIZE ) || !IsTopDown( pStr ) )
		{
			printf("Incorrectly entered data!\n");
			continue;
		}
		
		//	pStr = (UCHAR *)("|-|-ABC-D|E-FG-P-|Q|RST");
		//	pStr = (UCHAR *)("-|-|ABC|D-E|FG|P|-Q-RST");
		system("cls");
		printf("Input string: %s\n", pStr);

		// creation a tree
		pTree = CreateTree( pStr );
		
		if( !pTree )	// if the tree is not created
		{
			printf("Impossible to build a tree!\n");
			continue;
		}

		if( !BuildingMainWindow( pTree ) )
		{
			printf("Error building windows!\n");
		}

		if( DeleteTree( pTree ) == false )
		{
			printf("Error removing tree!\n");
		}
	}
	while (CONTINUE && _getch() != CODE_ESC );	// while not pressed ESC

	delete pStr;
	return;
}

