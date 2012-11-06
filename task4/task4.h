/**
* @file task4.h
* @brief The header file to the task4
*/

#ifndef TASK4_H_
#define TASK4_H_

typedef unsigned char UCHAR;	

#define MIN_SIZE_WINDOW 3U	// minimal size of the window
#define MAX_SIZE 53U	// number of nodes of the tree
#define CODE_ESC 27U	// ESC key code
#define CONTINUE printf("\nPress any key to continue or ESC to exit...")	// macro output message

struct window	// structure window and node of the tree
{
	UCHAR name;		// name of the window
	UCHAR *data;	// array of characters by which the window is displayed
	UCHAR w;		// width of the window
	UCHAR h;		// height of the window
	window *pL;		// a pointer to the root of the left subtree
	window *pR;		// a pointer to the root of the right subtree
};


/**
* @brief	This function checks whether the string is a bypass tree top-down
* @param	[in]	pStr The pointer to the first character string
* @return	bool
*/
bool IsTopDown ( UCHAR *pStr );

/**
* @brief	This function recreates tree for his bypass top-down
* @param	[in]	pStr The pointer to the first character string
* @post		returns a pointer to the root
* @return	window*
*/
window* CreateTree ( UCHAR *&pStr );

/**
* @brief	This function creates a simple window size of 1 character
* @param	[in]	name The name of the window
* @post		returns a pointer to the struct window
* @return	window
*/
window* CreateSimpleWindow ( UCHAR name );

/**
* @brief	This function changes the height of the window to the specified size
* @param	[in]	pRoot The pointer to the root node of the tree
* @param	[in]	height Needed height windows
* @return	bool
*/
bool VerticalResize ( window *pRoot, UCHAR height );

/**
* @brief	This function changes the width of the window to the specified size
* @param	[in]	pRoot The pointer to the root node of the tree
* @param	[in]	width Needed width windows
* @return	bool
*/
bool HorizontalResize ( window *pRoot, UCHAR width );

/**
* @brief	This function builds the main window
* @param	[in]	pRoot The pointer to the root node of the tree
* @return	window*
*/
window* BuildingMainWindow ( window *pRoot );

/**
* @brief	This function unites two windows
* @param	[in]	pRoot The pointer to the root node of the tree
* @return	window*
*/
window* BuildingWindow ( window *pRoot );

/**
* @brief	This function displays a window
* @param	[in]	pRoot The pointer to the root node of the tree
* @return	void
*/
void PrintWindow ( window *pRoot );

/**
* @brief	This function frees the dynamic memory that was allocated under the tree
* @param	[in]	pRoot The pointer to the root node of the tree
* @return	bool
*/
bool DeleteTree( window *pRoot );

#endif /* TASK4_H_ */