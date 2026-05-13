/*
   Engine-code for Magic Car Pet 3d-project.
   Copyright (c) 1996 Aatu Koskensilta (Zaphod.B)
--

   VER:    0.2 - MAY2026
   BY:    https://github.com/PF-dev0ps.
*/

#include "tp.h"
#include "grp.h"
#include "fixed.h"
#include "mcpet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath> // #include <math.h> Reemplazada en líneas 74, 75 por std::sin/cos

int sangle=0;

// Pointers
byte*landscape;
byte*texture[6];
byte*ground;

byte*check;
fixed sine[NUM_O_ANGLES];
fixed cosine[NUM_O_ANGLES];
short block_size;


#define PERSPECTIVE_SHIFT  8

#define OK        0
#define Z_TOO_LOW 1

inline int rotate_and_project(point*pnt,int angle,int angle3,int angle2)
{
   int xx,zz,yy,pz;

   xx=((fixmul(pnt->x,sine[angle]))-(fixmul(pnt->z,cosine[angle])));
   zz=((fixmul(pnt->x,cosine[angle]))+(fixmul(pnt->z,sine[angle])));
   pnt->z=zz;
   pnt->x=xx;
   pz=zz>>8;

   zz=(fixmul(pnt->y<<8,sine[angle2]))+(fixmul(pnt->z,cosine[angle2]));
   yy=((fixmul(pnt->y<<8,cosine[angle2]))-(fixmul(pnt->z,sine[angle2])));
   pnt->y=yy;
   pnt->z=zz;

   xx=((fixmul(pnt->y,sine[angle3]))+(fixmul(pnt->x,cosine[angle3])));
   yy=((fixmul(pnt->y,cosine[angle3]))-(fixmul(pnt->x,sine[angle3])));
   pnt->y=yy;
   pnt->x=xx;  

   zz=pnt->z>>8;

   if (zz<3) {pnt->y>>=8;return Z_TOO_LOW;};
   pnt->sx=160+((xx)/(zz));
   pnt->sy=100+((yy)/(zz));

   pnt->z=pz;
   pnt->y>>=8;  
   return OK;
}

void init_engine()
{
   block_size=5;
   // Calculate sine & cosine look-uptables   
   for (int angle=0;angle<NUM_O_ANGLES;angle++)
   {
      sine[angle]=(int)((std::sin((3.14159265/(NUM_O_ANGLES/2))*(float)angle))*256.0);
      cosine[angle]=(int)((std::cos((3.14159265/(NUM_O_ANGLES/2))*(float)angle))*256.0);
   }
   landscape=new byte[256*256];
   // Creates some kind of language
   check=new byte[256*256];
   ground=new byte[256*256];
   int i=0,ex;
   for (int j=0;j<256;j++)
   {
      for (int xx=0;xx<256;xx++)
      {
         ex=(rand()%2)*127;
         landscape[i]=ex;

         i++;
      };

   };

   int clr;
   for (i=0;i<256*256;i++)
   {
      clr=landscape[i];
      clr+=landscape[i-256];
      clr+=landscape[i+256];
      clr+=landscape[i-1];
      clr+=landscape[i+1];
      clr+=landscape[i-255];
      clr+=landscape[i+255];
      clr+=landscape[i-257];
      clr+=landscape[i+257];

      landscape[i]=clr/9;
      
   };


}

void render_landscape(int x,int y,int z,int angle,int angle2,int angle3)
{
   int result;

   int ray_angle=angle-47;
   int f[4];
   if (ray_angle<0) ray_angle+=NUM_O_ANGLES;
   // Ray xdelta, ydelta and position
   fixed rcos,rsin;
   fixed rx,rz;

   point p[4];

   // Coordinates in height-map
   int realx,realz;

   // Temporary polygon
   tpoly poly;
   byte tnum;
   int maxx=-65536;
   int maxy=-65536;
   int minx=65536;
   int miny=65536;
   // Not very efficient, but...   
   memset(check,0,256*256);

   for (int i=1;i<17;i++)
   {
      ray_angle=angle-28;
      if (ray_angle<0) ray_angle+=NUM_O_ANGLES;

      for (int dist=0;dist<28*2;dist++)
      {
         rcos=sine[ray_angle];
         rsin=cosine[ray_angle];
         rx=(INT_TO_FIXED(x>>(block_size+8))-cosine[angle])+(rsin*i);
         rz=(INT_TO_FIXED(z>>(block_size+8))-sine[angle])+(rcos*i);

         realx=(rx>>8)<<8;
         realz=(rz>>8)<<8;
         
         int loffset=(((realx>>8)&255)+(((realz>>8)&255)<<8));
         int loffset2=(( (( (realx) >>8) +1) &255)+(( ((realz>>8)) &255)<<8));
         int loffset3=(( (( (realx)>>8)+1) &255)+(( ((realz>>8)+1) &255)<<8));
         int loffset4=(( (( (realx)>>8)) &255)+(( ((realz>>8)+1) &255)<<8));
         if (check[loffset]==1) goto end;
         check[loffset]=1;


         tnum=(ground[loffset]-1)%6;
         poly.texture=texture[tnum];

         p[0].z=(realz<<block_size)-z;
         p[0].x=(realx<<block_size)-x;
         p[0].y=(((int)255-landscape[loffset])<<HEIGHT_SHIFT)-y;

         p[1].z=(realz<<block_size)-z;
         p[1].x=((realx+FIXED_ONE)<<block_size)-x;
         p[1].y=(((int)255-landscape[loffset2])<<HEIGHT_SHIFT)-y;

         p[2].z=((realz+FIXED_ONE)<<block_size)-z;
         p[2].x=((realx+FIXED_ONE)<<block_size)-x;
         p[2].y=(((int)255-landscape[loffset3])<<HEIGHT_SHIFT)-y;

         p[3].z=((realz+FIXED_ONE)<<block_size)-z;
         p[3].x=(realx<<block_size)-x;
         p[3].y=(((int)255-landscape[loffset4])<<HEIGHT_SHIFT)-y;;



         result=rotate_and_project(&p[0],angle,angle2,angle3);
         result+=rotate_and_project(&p[1],angle,angle2,angle3);
         result+=rotate_and_project(&p[2],angle,angle2,angle3);
         result+=rotate_and_project(&p[3],angle,angle2,angle3);

         if (result!=OK) goto end;

         if (p[0].z>256) f[0]=63-((p[0].z-256)>>2);
         else
         f[0]=63;

         if (p[1].z>256) f[1]=63-((p[1].z-256)>>2);
         else
         f[1]=63;


         if (p[2].z>256) f[2]=63-((p[2].z-256)>>2);
         else
         f[2]=63;


         if (p[3].z>256) f[3]=63-((p[3].z-256)>>2);
         else
         f[3]=63;



         maxx=-65536;
         maxy=-65536;
         minx=65536;
         miny=65536;
         for (int o=0;o<4;o++)
         {
               if (p[o].sx>maxx) maxx=p[o].sx;
               if (p[o].sy>maxy) maxy=p[o].sy;
               if (p[o].sx<minx) minx=p[o].sx;
               if (p[o].sy<miny) miny=p[o].sy;
         };

         if (maxx<0&&minx<0) goto end;
         if (minx>320&&maxx>320) goto end;
         if (maxy<0&&maxy<0) goto end;
         if (miny>200&&miny>200) goto end;
         poly.pnts[0].x=p[0].sx;
         poly.pnts[0].y=p[0].sy;
         poly.pnts[0].tx=0;
         poly.pnts[0].ty=0;
         poly.pnts[0].fd=f[0];
         if (poly.pnts[0].fd<1) poly.pnts[0].fd=1;

         poly.pnts[1].x=p[1].sx;
         poly.pnts[1].y=p[1].sy;
         poly.pnts[1].tx=32;
         poly.pnts[1].ty=0;
         poly.pnts[1].fd=f[1];
         if (poly.pnts[1].fd<1) poly.pnts[1].fd=1;

         poly.pnts[2].x=p[3].sx;
         poly.pnts[2].y=p[3].sy;
         poly.pnts[2].tx=0;
         poly.pnts[2].ty=32;
         poly.pnts[2].fd=f[3];
         if (poly.pnts[2].fd<1) poly.pnts[2].fd=1;

         tmap(&poly);


         poly.pnts[0].x=p[2].sx;
         poly.pnts[0].y=p[2].sy;
         poly.pnts[0].tx=32;
         poly.pnts[0].ty=32;
         poly.pnts[0].fd=f[2];

         if (poly.pnts[0].fd<1) poly.pnts[0].fd=1;
         tmap(&poly);
         end:
         ray_angle++;
         ray_angle&=NUM_O_ANGLES-1;
      };
      
   };

}
