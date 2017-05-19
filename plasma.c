/* Stephan Luther
 * 28-01-2009
 */

#include "plasma.h"

void simplePlasma1(SDL_Surface *screen) {

	int x, y;
	Uint8 colour;
	Uint32 green;

	for(x = 0; x < 100; ++x) {
		for(y = 0; y < 100; ++y) {
			colour = (128.0 + (128.0 * sin((x+y)/8.0)));
			green = SDL_MapRGB(screen->format,0x00,colour,0x00);
			plotPixel(screen,x,y,green);
		}
	}
}

void simplePlasma2(SDL_Surface *screen) {
	int x, y;
	Uint8 colour;
	double xResult, yResult, result;
	Uint32 green;

	for(x = 0; x < 100; ++x) {
		for(y = 0; y < 100; ++y) {
			xResult = 128.0 + (128.0 * sin(x/8.0));
			yResult = 128.0 + (128.0 * sin(y/8.0));
			result = (xResult + yResult) / 2;
			colour = result;
			green = SDL_MapRGB(screen->format,0x00,colour,0x00);
			plotPixel(screen,x,y,green);
		}
	}
}

double movingPlasma1(SDL_Surface *screen, double period) {
	int x, y;
	Uint8 colour;
	double xResult, yResult, result;
	Uint32 green;

	for(x = 0; x < 100; ++x) {
		for(y = 0; y < 100; ++y) {

			xResult = 128.0 + (128.0 * sin((x + period)/8.0));
			yResult = 128.0 + (128.0 * sin((y + period)/8.0));
			result = (xResult + yResult) / 2;
			colour = result;
			green = SDL_MapRGB(screen->format,0x00,colour,0x00);
			plotPixel(screen,x,y,green);
		}
	}
	return period + 1.0;
}

double movingPlasma2(SDL_Surface *screen, unsigned int lx, unsigned int ly, double period) {
	int x, y;
	Uint8 colour;
	double xResult, yResult, result;
	Uint32 green;

	for(x = 0; x < lx; ++x) {
		for(y = 0; y < ly; ++y) {

			xResult = 128.0 + (128.0 * sin((x + period)/8.0));
			yResult = 128.0 + (128.0 * sin((y + period)/8.0));
			result = (xResult + yResult) / 2;
			colour = result;
			green = SDL_MapRGB(screen->format,0x00,colour,0x00);
			plotPixel(screen,x,y,green);
		}
	}
	return period + 1.0;
}

double etchPlasma1(Uint8 *canvas, unsigned int lx, unsigned int ly, double period) {
	int x, y;
	Uint8 colour, *cursor;
	double xResult, yResult, result;

	cursor = canvas;

	for(y = 0; y < ly; ++y) {
		for(x = 0; x < lx; ++x) {

			xResult = 128.0 + (128.0 * sin((x + period)/8.0));
			yResult = 128.0 + (128.0 * sin((y + period)/8.0));
			result = (xResult + yResult) / 2;
			colour = result;
			*cursor = colour;
			cursor++;
		}
	}
	return period + 1.0;
}

