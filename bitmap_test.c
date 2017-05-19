/* Stephan Luther
 * 2009/03/12
 */

/* Test file for bitmap loading code to keep main.c clutter free
 */

#include <stdio.h>

#include "SDL.h"

#include "splash.h"

int main(int argc, char **argv) {

	char *filename;
	char *translate;
	spl_Bitmap *bmp;
	int i;
	int quit = 0;

	SDL_Surface *screen;
	SDL_Event 	event;
	
	if(argc < 2) {
		printf("No file specified\n");
		return 0;
	}

	filename = argv[1];

	printf("Attempting to open file: %s\n",filename);

	bmp = loadBitmap(filename);
	if(bmp == 0) {
		printf("Failed to open file\n");
		return 0;
	}


	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("unable to initialize video: %s\n",SDL_GetError());
		exit(1);
	}

	screen = SDL_SetVideoMode(640,480,16,SDL_DOUBLEBUF);
		
	translate = (char *) &(bmp->header->filetype);

	printf("Opened bitmap\n");
	printf("Header values -- \n");
	printf("Filetype is: %d -- stands for : %s\n",bmp->header->filetype,translate);
	printf("Filesize is: %d\n",bmp->header->filesize);
	printf("Reserved 1 & 2: %d %d\n",bmp->header->reserved1,bmp->header->reserved2);
	printf("Offset: %d\n",bmp->header->offset);

	printf("Info values -- \n");
	printf("Header size: %d\n",bmp->info->headerSize);
	printf("Width: %d\n",bmp->info->width);
	printf("Height: %d\n",bmp->info->height);
	printf("ColourPanes: %d\n",bmp->info->colourPanes);
	printf("Bits Per Pixel: %d\n",bmp->info->bpp);
	printf("Compression: %d\n",bmp->info->compression);
	printf("Image Size: %d\n",bmp->info->imageSize);
	printf("Horizonal Resolution: %d\n",bmp->info->hRes);
	printf("Vertical Resolution: %d\n",bmp->info->vRes);
	printf("Colours: %d\n",bmp->info->colours);
	printf("Important Colours: %d\n",bmp->info->importantColours);

	while(!quit) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN : quit = 1;break;
				default : break;
			}
		}

		SDL_LockSurface(screen);
		plotBitmap(screen,bmp);
		SDL_UnlockSurface(screen);
		SDL_Flip(screen);
	}

	SDL_FreeSurface(screen);
	SDL_Quit();


	unloadBitmap(bmp);

	return 0;
}
