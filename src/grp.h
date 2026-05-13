/*
   NAME:    GRP.H
   DESC:    Graphics header for true 3d-project
    VER:    0.1
     BY:    Aaru Koskensilta (Zaphod.B)
    VER:    0.2 - MAY2026
     BY:    https://github.com/PF-dev0ps.
*/

#ifndef _GRP_H_
#define _GRP_H_

/* Data defines */
using byte = unsigned char; //#define byte   unsigned char *Cambios realizados
#define word   unsigned short
#define dword  unsigned long


/* Double buffer */
extern byte dbuffer[64000];

/* RGB-colour structure */
struct RGBcolor
{
   byte r,g,b;
};

/* Current palette */
extern RGBcolor current_palette[256]; 

/* Load PCX-pic from file */
byte * load_pic_from_file(char*);

/* Set graphics mode */
void gmode(short);

/* Load palette from PCX-file */
RGBcolor * load_pal_from_file(char*);

/* Set palette */
void setpal(RGBcolor*);

/* Plot a pixel */
void plot(short,short,byte);

/* Copy doublebuffer to screen */
void update_screen();

/* Draw a line */
void line(int,int,int,int,char);

/* PCX-header */
struct PCXheader
{
   char mfacturer;
   char version;
   char encoding;
   char bit_per_pixel;
   short Xmin;
   short Ymin;
   short Xmax;
   short Ymax;
   short Hres;
   short Vres;
   char hpalette[48];
   char reserved;
   char planes;
   short bpline;
   short headerpal;
   char filler[58];
};


#endif
