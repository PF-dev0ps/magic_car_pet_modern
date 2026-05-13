#ifndef _MCPET_H_
#define _MCPET_H_

#define WATER     0 // Dont care about these
#define SHORE     1
#define GROUND    2
#define GGROUND   3
#define GRASS     4

#define NUM_O_ANGLES 256 // Number of angles

#define HEIGHT_SHIFT 2 

#include "fixed.h"
#include "grp.h"

struct point
{
   int x,y,z;
   int sx,sy;
};

// Rotate and project a point
int rotate_and_project(point*pnt,int,int,int);

extern byte*texture[6];
extern byte*landscape;
extern byte*ground;
// Sinuses & cosinuses of angles
extern fixed cosine[NUM_O_ANGLES];
extern fixed sine[NUM_O_ANGLES];

// Size of block: 0 = 1, 1 = 2, 2 = 4, 3 = 8, 4 = 16, 5 = 32, 6 = 64 and so forth
extern short block_size;

// Render landscape
void render_landscape(int,int,int,int,int,int);

// Initialize engine
void init_engine();

#endif
