/* Stephan Luther
 * 2009/01/02
 */
 
#include "voxel.h"

void vInitialize() {
	vInitializePlayer();
	vInitializeLookupTables();
}

void vInitializePlayer() {
	player.x = 0;
	player.y = 0;
	player.vpDistance = 60;
	player.viewDepth = 500; /* Don't yet know a good number to put here */ 
	player.height = 150;
	player.yAngle = 0;
}

void vInitializeLookupTables() {
	int i;
	float angle;

	for(i = 0; i < ANGULAR_SLICES; ++i) {

		angle = i * ANGULAR_INCREMENT * (M_PI / 180.0);
		cosLookup[i] = cos(angle);
		sinLookup[i] = sin(angle);
	}

}

void voxelRender(SDL_Surface *screen) {
	castColumns(screen);
}

void castColumns(SDL_Surface *screen) {
	unsigned int column;
	int angle;

	player.yAngle %= ANGULAR_SLICES;
	
	angle = player.yAngle - 160;
	angle %= ANGULAR_SLICES;
	angle = angle < 0 ? (angle + ANGULAR_SLICES) : angle;

	for(column = 0; column < 320; ++column, ++angle) {
		angle %= ANGULAR_SLICES;
		castRays(screen,angle,column);
	}

}

void castRays(SDL_Surface *screen, int angle, unsigned int column) {

	unsigned int row; /* For looping through each row*/
	unsigned int ray_length, ray_x, ray_y, map_x, map_y;
	unsigned int vpDelta; /* viewing plane delta relative to the camera */
	unsigned int top, bottom, viewScale;
	Uint8 index, textureIndex;
	float scale;

	/* Increment through each row in the column. Starting position of the rows shall be
	 * half way?
	 */
	for(row = 150; row < 200; ++row) {

		/* Calculate the ray length using similar triangles */
		vpDelta = row - 100; /* camera is aligned in the middle of view plane */
		scale = (float)player.height / (float)vpDelta;
		ray_length = player.vpDistance * scale;

		/* Rotate ray into position */
		ray_x = (int)((float)player.x + ray_length * cosLookup[angle]);
		ray_y = (int)((float)player.y - ray_length * sinLookup[angle]);

		/* Get location in map */
		map_x = ray_x % voxelMapX;
		map_y = ray_y % voxelMapY;
		index = voxelMap[map_x + (map_y * voxelMapY)];
		textureIndex = mapTexture->image[map_x + (map_y * voxelMapY)]; /* assuming alignment */

		/* Calculate length and position of voxel strip */
		viewScale = VOXEL_HEIGHT * index / (ray_length + 1); 
		top = 30 + row - viewScale;
		bottom = top + viewScale;

		/* Pass coordinates off to be rendered */
		//plotGroundValue(screen,column,row,index);
		//plotVoxelStrip(screen,top,bottom,column,index);
		plotVoxelStripWithTexture(screen,top,bottom,column,textureIndex);
	}
}

void plotGroundValue(SDL_Surface *screen, unsigned int sX, unsigned int sY, Uint8 colour) {
	Uint32 rgbColour = SDL_MapRGB(screen->format,0x00,colour,0x00);
	plotPixel(screen,sX,sY,rgbColour);
}

void plotVoxelStrip(SDL_Surface *screen, unsigned int top, unsigned int bottom, 
										unsigned int column, Uint8 colour) {
	int i;
	Uint32 rgbColour = SDL_MapRGB(screen->format,0x00,colour,0x00);

	/* Swap things around */
	/*if(top > bottom) {
		bottom = top - bottom;
		top = top - bottom;
		bottom = top + bottom;
	}*/

	for(i = 0; i <= bottom - top; ++i) {
		if((i + top) < 200) { // basic clipping
			plotPixel(screen,column,i+top,rgbColour);
		}
	}
}

void plotVoxelStripWithTexture(SDL_Surface *screen, unsigned int top, unsigned int bottom, 
										unsigned int column, Uint8 index) {
	int i;
	Uint32 rgbColour = SDL_MapRGB(screen->format,mapTexture->palette[index].red,
																							mapTexture->palette[index].green,
																							mapTexture->palette[index].blue);

	/* Swap things around */
	/*if(top > bottom) {
		bottom = top - bottom;
		top = top - bottom;
		bottom = top + bottom;
	}*/

	for(i = 0; i <= bottom - top; ++i) {
		if((i + top) < 200) {
			plotPixel(screen,column,i+top,rgbColour);
		}
	}
}

