/*
   NAME:    TP.H
   DESC:    Header for texturing unit for a magic car pet 3d-project
    VER:    0.1
     BY:    Aatu Koskensilta (Zaphod.B)
*/

#ifndef _TP_H
#define _TP_H

#include "fixed.h"
#include "grp.h"

extern byte fadetbl[256*64]; // Nearest palette entries for different shades
                             // of each palette color.
// Texture point
struct tpoint
{
   int tx,ty; // Texture coordinates (u&v)
   int x,y; // Screen coordinates
   int fd; // Intensity level
}; 

struct tpoly
{
   tpoint pnts[3]; // Texture points for a triangle
   byte*texture; // Texture for triangle
};



// Texture triangle. In C++ at this point, since I'm not very familiar
// with this horrible AT&T asm syntax
void tmap(tpoly*);

// Pixels drawn. Not used
extern int pixels_drawn;
#endif
