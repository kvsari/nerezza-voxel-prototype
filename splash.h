/* Stephan Luther
 * 2008-12-15
 */

#include <math.h>
#include <stdio.h>
#include <errno.h>

#include "SDL.h"

#ifndef _SPLASH_H_
#define _SPLASH_H_

/* Expects SDL_INIT_VIDEO to already have run */


typedef struct {
	int minX;
	int minY;
	int maxX;
	int maxY;
} spl_Rectangle;

typedef struct {
	char blue;
	char green;
	char red;
	char reserved;
} spl_RGBQuad;

typedef struct {
	Uint16		filetype;
	Uint32 		filesize; /* In bytes */
	Uint16		reserved1;
	Uint16		reserved2;
	Uint32		offset;
} spl_BitmapHeader;

/* Bitmap Infor header for V3 Windows BitMap format. */
typedef struct {
	Uint32	headerSize;
	Sint32	width;
	Sint32 	height;
	Uint16	colourPanes;
	Uint16	bpp;
	Uint32	compression;
	Uint32	imageSize;
	Uint32	hRes;
	Uint32	vRes;
	Uint32	colours;
	Uint32	importantColours;
} spl_BitmapInfo;

typedef struct {
	spl_BitmapHeader 	*header;
	spl_BitmapInfo		*info;
	spl_RGBQuad				*palette;
	char *image;
} spl_Bitmap;

/* Cheesy global variable */
spl_Rectangle clipRectangle;

/* Will clip the pixel then plot it */
void plotPixel(SDL_Surface *suface, int x, int y, Uint32 colour);

/* You'd want to do this at the screen resolution,
 * will return -1 if the bounds are back to front
 * and will leave the clip struct as is. Otherwise 0
 */
int setClipRectangle(int minX, int minY, int maxX, int maxY);

/* Will clip the line according the clip rectangle.
 * If the clip rectangle hasn't been set yet it'll
 * probably crash
 */
void plotLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 colour);

/* Expects the supplied line to already be within the clip rectangle bounds.
 * Usually the rectangle bounds are set at the screen limits.
 */
void plotClippedLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 colour);

/* Opens a bitmap image and loads it into memory. Returns a pointer to the bitmap held in
 * memory. Returns zero if failed to load 
 * This function expects V3 windows bitmap files. This is the most common format */
spl_Bitmap *loadBitmap(char *filename);

void unloadBitmap(spl_Bitmap *bitmap);

/* Will plot the bitmap to the sent SDL surface
 *  Will lock the sent surface for direct pixel access and unlock before it exits.
 * 	WARNING, this is just a test function. The surface has to be larger than the bitmap
 * 	as it doesn't do any bounds checking. Also, expects the bitmap to be at 8bit colour
 * 	depth*/
void plotBitmap(SDL_Surface *surface, spl_Bitmap *bmp);

#endif

