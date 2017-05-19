/* Stephan Luther
	 2008-12-14 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL.h"

#include "splash.h"
#include "plasma.h"
#include "voxel.h"

void plotPixel1(SDL_Surface *surface, int x, int y, Uint32 colour);

void plotRectangle(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 colour);

void plotRandomLine(SDL_Surface *surface, Uint32 colour);

void lookupPlot(SDL_Surface *surface, Uint8 *map, int mX, int mY);

int main(int argc, char **argv) {

	SDL_Surface *screen;
	spl_Bitmap *height, *texture;
	SDL_Event event;
	Uint32 yellow, green, red, blue;
	Uint8 *canvas;
	unsigned int cX, cY, sX, sY;
	int i,quit = 0;
	double period = 0;
	int movementX, movementY,lookAngle;
	SDL_Rect blitPos;

	srand(time(NULL));

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("unable to initialize video: %s\n",SDL_GetError());
		exit(1);
	}

	height = loadBitmap("HEIGHT.BMP");
	texture = loadBitmap("GROUND.BMP");

	sX = 320;
	sY = 200;
	screen = SDL_SetVideoMode(sX,sY,16,SDL_DOUBLEBUF);//|SDL_FULLSCREEN);
	//screen = SDL_SetVideoMode(sX,sY,16,SDL_DOUBLEBUF|SDL_FULLSCREEN);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetClipRect(screen,NULL);

	yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
	red = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
	green = SDL_MapRGB(screen->format, 0x00, 150, 0x00);
	blue = SDL_MapRGB(screen->format, 0xff, 0xff, 150);

	if(setClipRectangle(0,0,319,199) < 0)
		quit = 1;

	/* Create plasma image*/
	cX = 1024;
	cY = 1024;
	canvas = malloc(1 * cX * cY);
	etchPlasma1(canvas,cY,cX,period);
	/*SDL_Surface *mapView = SDL_CreateRGBSurfaceFrom(canvas,cX,cY,16,100,0,0,0,0);
	if (mapView == NULL) {
		exit(1);
	}*/
	blitPos.x = cX;
	blitPos.y = 0;
	blitPos.w = cX;
	blitPos.h = cY;

	/* Load in heigthmap from file */
	/*SDL_Surface *heightmap = SDL_LoadBMP("HEIGHT.BMP");
	if(heightmap == NULL) {
		printf("Couldn't load heightmap! \n");
		exit(1);
	}*/


	/* Start voxel stuff */
	vInitialize();
	/*voxelMap = canvas;
	voxelMapX = cX;
	voxelMapY = cY;*/
	voxelMap = height->image;
	voxelMapX = height->info->width;
	voxelMapY = height->info->height;
	mapTexture = texture;

	//SDL_FillRect(mapView,NULL,green);
	SDL_FillRect(screen,NULL,yellow);
	
	movementX = movementY = 0;
	while(!quit) {

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT: 	movementY =  1;break;
						case SDLK_RIGHT:	movementY = -1;break;
						case SDLK_UP:			movementX =  1;break;
						case SDLK_DOWN:		movementX = -1;break;
						case SDLK_a:			lookAngle = -1;break;
						case SDLK_d:			lookAngle =  1;break;
						case SDLK_q:			quit = 1;			 break;
						default:break;
					} break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT: movementY = movementX > 0 ? 0 : movementX; break;
						case SDLK_RIGHT:movementY = movementX < 0 ? 0 : movementX; break;
						case SDLK_UP:		movementX = movementY > 0 ? 0 : movementY; break;
						case SDLK_DOWN: movementX = movementY < 0 ? 0 : movementY; break;
						//case SDLK_a:		lookAngle = lookAngle < 0 ? 0 : lookAngle; break;
						//case SDLK_d:		lookAngle = lookAngle > 0 ? 0 : lookAngle; break;
						case SDLK_a: lookAngle = 0; break;
						case SDLK_b: lookAngle = 0; break;
						default:break;
					} break;
				case SDL_QUIT: quit = 1;
				default:break;
			}
		}

		player.x += movementX;
		player.y += movementY;
		//player.yAngle += lookAngle;

		SDL_FillRect(screen,NULL,blue);
		
		SDL_LockSurface(screen);
		voxelRender(screen);
		SDL_UnlockSurface(screen);

		//SDL_BlitSurface(heightmap,NULL,screen,&blitPos);
		SDL_Flip(screen);
		//quit = 1;
	}

	unloadBitmap(height);
	unloadBitmap(texture);


	/*SDL_FreeSurface(mapView);*/
	SDL_FreeSurface(screen);
	//SDL_FreeSurface(heightmap);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	free(canvas); 

	SDL_Quit(); /* Crashes if this is called? */

	return 0;
}

void lookupPlot(SDL_Surface *surface, Uint8 *map, int mX, int mY) {
	int x,y;
	Uint8 *cursor = map;
	Uint32 colour;
	for(x = 0; x < mX; ++x) {
		cursor = map;
		cursor += x;
		for(y = 0; y < mY; ++y) {
			cursor += mX;
			colour = SDL_MapRGB(surface->format,0x00,0x00,*cursor);
			plotPixel(surface,x,y,colour);
		}
	}
}

void plotRectangle(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 colour) {
	plotLine(surface,x0,y0,x1,y0,colour);
	plotLine(surface,x0,y0,x0,y1,colour);
	plotLine(surface,x0,y1,x1,y1,colour);
	plotLine(surface,x1,y1,x1,y0,colour);
}

/* Since there is no clipping yet, random lines will be within the screen bounds */
void plotRandomLine(SDL_Surface *surface, Uint32 colour) {
	int x0,y0,x1,y1;

	x0 = -1000 + rand() % 4000;
	x1 = -1000 + rand() % 4000;
	y0 = -1000 + rand() % 4000;
	y1 = -1000 + rand() % 4000;

	plotLine(surface,x0,y0,x1,y1,colour);
}

void plotPixel1(SDL_Surface *surface, int x, int y, Uint32 colour) {
	int bpp = surface->format->BytesPerPixel;

	Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	*(Uint16 *)pixel = colour;
}

