/* Stephan Luther
 * 2008-12-15
 */

#include "splash.h"

void plotPixel(SDL_Surface *surface, int x, int y, Uint32 colour) {
	int bpp = surface->format->BytesPerPixel;

	/*if((x < clipRectangle.minX) || (x > clipRectangle.maxX) ||
		(y < clipRectangle.minY) || (y > clipRectangle.minY))
		return;*/

	Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	*(Uint16 *)pixel = colour;
}

int setClipRectangle(int minX, int minY, int maxX, int maxY) {
	if(minX >= maxX) {
		return -1;
	}
	if(minY >= maxY) {
		return -1;
	}

	clipRectangle.minX = minX;
	clipRectangle.minY = minY;
	clipRectangle.maxX = maxX;
	clipRectangle.maxY = maxY;
	return 0;
}

void plotLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 colour) {
	
	float t0 = 0;
	float t1 = 1;

	float  rx0, rx1, ry0, ry1;

	int dx0, dx1;
	int dy0, dy1;
	int qx0, qx1;
	int qy0, qy1;
	int sx, sy, ex, ey;

	/* Calculate all delta's */
	dx0 = -(x1 - x0);
	dx1 = (x1 - x0);
	dy0 = -(y1 - y0);
	dy1 = (y1 - y0);

	qx0 = x0 - clipRectangle.minX;
	qx1 = clipRectangle.maxX - x0;
	qy0 = y0 - clipRectangle.minY;
	qy1 = clipRectangle.maxY - y0;

	/* Test to see if line is outside the bounding box, if so we just leave */
	if(((x0 <= clipRectangle.minX) && (x1 <= clipRectangle.minX)) ||
		((x0 >= clipRectangle.maxX) && (x1 >= clipRectangle.maxX)) ||
		((y0 <= clipRectangle.minY) && (y1 <= clipRectangle.minY)) ||
		((y0 >= clipRectangle.maxY) && (y1 >= clipRectangle.maxY))) {
		return;
	}

	/* Test to see if line is outside the bounding box, if so we just leave 
	if(((dx0 == 0) && (qx0 < 0)) ||
		((dx1 == 0) && (qx1 < 0)) ||
		((dy0 == 0) && (qy0 < 0)) ||
		((dy1 == 0) && (qy1 < 0))) {
		return;
	}*/

	/* Left edge check */
	if(dx0 != 0) {
		rx0 = (float) qx0 / dx0;
		if(dx0 < 0)
			t0 = rx0 > t0 ? rx0 : t0;
		else
			t1 = rx0 < t1 ? rx0 : t1;
	}

	/* Top edge check */ 
	if(dy0 != 0) {
		ry0 = (float) qy0 / dy0;
		if(dy0 < 0)
			t0 = ry0 > t0 ? ry0 : t0;
		else
			t1 = ry0 < t1 ? ry0 : t1;
	}

	/* Right edge check */
	if(dx1 != 0) {
		rx1 = (float) qx1 / dx1;
		if(dx1 < 0)
			t0 = rx1 > t0 ? rx1 : t0;
		else
			t1 = rx1 < t1 ? rx1 : t1;
	}

	/* Bottom edge check */
	if(dy1 != 0) {
		ry1 = (float) qy1 / dy1;
		if(dy1 < 0)
			t0 = ry1 > t0 ? ry1 : t0;
		else
			t1 = ry1 < t1 ? ry1 : t1;
	}

	/* Line falls outside of the clipping box */
	if(t0 > t1)
		return;

	sx = x0 + round(t0 * (x1 - x0));
	sy = y0 + round(t0 * (y1 - y0));
	ex = x0 + round(t1 * (x1 - x0));
	ey = y0 + round(t1 * (y1 - y0));

	plotClippedLine(surface,sx,sy,ex,ey,colour);	
}

void plotClippedLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 colour) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int stepx0, stepx1, stepy0, stepy1;
	int denominator, numerator, numeratorAdd, pixels, currentPixel;

	int bpp = surface->format->BytesPerPixel;
	Uint8 *pixel = (Uint8 *)surface->pixels + y0 * surface->pitch + x0 * bpp;

	if (x1 >= x0) {stepx0 = stepx1 = bpp;} else {stepx0 = stepx1 = -1 * bpp;}
	if (y1 >= y0) {stepy0 = stepy1 = surface->pitch;} else {stepy0 = stepy1 = -1 * surface->pitch;}

	if(dx >= dy) {
		stepx0 = 0;
		stepy1 = 0;
		denominator = dx;
		numerator = dx / 2;
		numeratorAdd = dy;
		pixels = dx;
	}
	else {
		stepx1 = 0;
		stepy0 = 0;
		denominator = dy;
		numerator = dy / 2;
		numeratorAdd = dx;
		pixels = dy;
	}

	for(currentPixel = 0; currentPixel <= pixels; ++currentPixel) {
		*(Uint16 *)pixel = colour;
		numerator += numeratorAdd;
		if(numerator >= denominator) {
			numerator -= denominator;
			pixel += stepx0;
			pixel += stepy0;
		}
		pixel += stepx1;
		pixel += stepy1;
	}
}

spl_Bitmap *loadBitmap(char *filename) {
	
	int counter = 0;
	FILE *file;

	spl_Bitmap *bitmap = malloc(sizeof(spl_Bitmap));
	if(bitmap == NULL)
		return 0;

	bitmap->header = malloc(sizeof(spl_BitmapHeader));
	if(bitmap->header == NULL)
		return 0;

	bitmap->info = malloc(sizeof(spl_BitmapInfo));
	if(bitmap->info == NULL)
		return 0;
	
	file = fopen(filename,"r");
	if(file == NULL) {
		return 0; /* We won't worry about logging until I rewrite all this in OOP */
	}

	/* Will need to read in the header entry by entry due to GCC mangling
	 * the size of a structure thanks to padding
	 */

	counter = fread(&bitmap->header->filetype,2,1,file);
	if(counter != 1) {
		unloadBitmap(bitmap);
		fclose(file);
		return 0;
	}

	counter = fread(&bitmap->header->filesize,4,1,file);
	if(counter != 1) {
		unloadBitmap(bitmap);
		fclose(file);
		return 0;
	}
	counter = fread(&bitmap->header->reserved1,2,1,file);
	if(counter != 1) {
		unloadBitmap(bitmap);
		fclose(file);
		return 0;
	}
	counter = fread(&bitmap->header->reserved2,2,1,file);
	if(counter != 1) {
		unloadBitmap(bitmap);
		fclose(file);
		return 0;
	}

	counter = fread(&bitmap->header->offset,4,1,file);
	if(counter != 1) {
		unloadBitmap(bitmap);
		fclose(file);
		return 0;
	}

	counter = fread(bitmap->info,sizeof(spl_BitmapInfo),1,file);
	if(counter != 1) {
		unloadBitmap(bitmap);
		fclose(file);
		return 0;
	}

	bitmap->palette = NULL;
	if(bitmap->info->bpp == 8) { /* load in palette */
		bitmap->palette = malloc(sizeof(spl_RGBQuad) * 256);
		if(bitmap->palette == NULL) {
			fclose(file);
			unloadBitmap(bitmap);
			return 0;
		}
		counter = fread(bitmap->palette,sizeof(spl_RGBQuad),256,file);
		if(counter != 256) {
			fclose(file);
			unloadBitmap(bitmap);
			return 0;
		}
	}

	bitmap->image = malloc(bitmap->info->imageSize);
	if(bitmap->image == NULL) {
		fclose(file);
		unloadBitmap(bitmap);
		return 0;
	}

	printf("Allocated space for bitmap\n");

	counter = fread(bitmap->image,1,bitmap->info->imageSize,file);
	if(counter != bitmap->info->imageSize) {
		fclose(file);
		unloadBitmap(bitmap);
		return 0;
	}
	
	fclose(file);
	return bitmap;
}

void unloadBitmap(spl_Bitmap *bitmap) {
	if(bitmap == NULL)
		return;

	if(bitmap->header != NULL)
		free(bitmap->header);

	if(bitmap->info != NULL)
		free(bitmap->info);

	if(bitmap->palette != NULL)
		free(bitmap->palette);

	if(bitmap->image != NULL)
		free(bitmap->image);
	
	free(bitmap);
}

/* Will plot the bitmap starting from the top left corner of
 * the sent surface
 */
void plotBitmap(SDL_Surface *surface, spl_Bitmap *bmp) {

	int i,j,w,h,index;
	Uint32 colour;

	h = bmp->info->height;
	w = bmp->info->width;
	index = 0;

	SDL_LockSurface(surface);

	for(i = 0; i < h; ++i) {
		for(j = 0; j < w; ++j) {
			colour = SDL_MapRGB(surface->format,bmp->palette[bmp->image[index]].red,
																					bmp->palette[bmp->image[index]].green,
																					bmp->palette[bmp->image[index]].blue);
			/*colour = SDL_MapRGB(surface->format,0x00,0xff,0x00);*/
		
			plotPixel(surface,i,j,colour);
			index++;
		}
	}

	SDL_UnlockSurface(surface);

}

