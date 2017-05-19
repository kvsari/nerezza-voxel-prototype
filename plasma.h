/* Stephan Luther
 * 28-01-2009
 */

/* Why plasma? To understand voxels I've really had to jump all the way back to basics. I've noticed
 * that all the tutorials use a fractal map generator/plasma so I'll start my journey there. So to
 * make sure the landscape generator actually works I have to display it don't I?
 */

#ifndef _PLASMA_H_
#define _PLASMA_H_

#include <math.h>

#include "SDL.h"

#include "splash.h"

/* Will create a 100*100 plasma and won't adjust it */
void simplePlasma1(SDL_Surface *screen);

void simplePlasma2(SDL_Surface *screen);


/* Relies on the period to increment the plasma. Will calculate plasma with the supplied
 * period then increment it by one returning the result
 */
double movingPlasma1(SDL_Surface *screen, double period);

/* Same as above but asks for the size of the plasma. Doesn't do any clipping */
double movingPlasma2(SDL_Surface *screen, unsigned int lx, unsigned int ly, double period);

/* Instead of drawing the plasma into the screen it will put it on the sent memory buffer.
 * The buffer must already be allocated as it will use the length x and length y to determine the 
 * size
 */
double etchPlasma1(Uint8 *canvas, unsigned int lx, unsigned int ly, double period);

#endif

