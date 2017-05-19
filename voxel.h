/* Stephan Luther
 * 2009/01/02
 */

#ifndef _VOXEL_H_
#define _VOXEL_H_

#include <math.h>

#include "SDL.h"

#include "splash.h"

/* Here goes, my first foray into voxels
 */


/* Basic voxel definition */
typedef struct {
	int x;
	int y;
	int z;
	Uint32 colour;
} VOXEL;

/* Define struct for a uniform 3D grid consisting of cuboid cells
 */
typedef struct {
 Uint32 *colour;
 int x;
 int y;
 int z;
} CUBE;

typedef struct {
	int x;
	int y;
	int vpDistance; /* Viewing plane distance */
	int viewDepth; /* maximum ray length */
	int height; /* Height of the camera off the ground */
	int yAngle;
} CAMERA;

/* Define some constants */

/* We are assuming 360*200 display with a viewing angle of 60 degress
 * Therefore, the angular increments will total 1920. 360/60 = 6. Six degrees per
 * vertical screen slice. 6 * 320 = 1920. Total number of degress.
 */
#define ANGULAR_SLICES 1920

/* Angular increment is the total degrees(360) by our computed total increments(1920) */
#define ANGULAR_INCREMENT ((float)0.1875)

/* Height of each voxel */
#define VOXEL_HEIGHT 40




/* Define some globals, yes globals are bad but I need to understand this voxel stuff before I can 
 * refactor into classes
 */


/* Sin and cos lookup tables */
float cosLookup[ANGULAR_SLICES];
float sinLookup[ANGULAR_SLICES];

/* The world player */
CAMERA player;

/* Data for map */
Uint8 *voxelMap;
unsigned int voxelMapX, voxelMapY;

/* data for the texture */
spl_Bitmap *mapTexture;


/* Functions for very basic voxel rendering */

void vInitialize();

/* Sets up the players staring position */
void vInitializePlayer();

/* Initialize lookup tables*/
void vInitializeLookupTables();

/* Function that wraps the more technical sounding stuff
 */
void voxelRender(SDL_Surface *screen);

/* Will loop through all 320 columns of the viewing plance. On each column will
 * calculate the angle that the rays are to be later rotated
 */
void castColumns(SDL_Surface *screen);

/* Will cast rays for each row in the desired column
 */
void castRays(SDL_Surface *screen, int angle, unsigned int column);

/* Will just draw the colour value found on the ground. Will not render a texel strip
 */
void plotGroundValue(SDL_Surface *screen, unsigned int sX, unsigned int sY, Uint8 colour);

/* Basic voxel strip. Should really be named a texel strip */
void plotVoxelStrip(SDL_Surface *screen, unsigned int top, unsigned int bottom, 
										unsigned int column, Uint8 colour);

/* Assumes presence of bitmap. Bitmap has to align with the world or it will look funny. Expects
 * the bitmap to be 256 colour (8 bit).
 */
void plotVoxelStripWithTexture(SDL_Surface *screen, unsigned int top, unsigned int bottom,
										unsigned int column, Uint8 index);

#endif

