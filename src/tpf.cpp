/*
   NAME:    TP.CC
   DESC:    Texturing unit for a magic car pet 3d-project
    VER:    0.1
     BY:    Aatu Koskensilta (Zaphod.B)
   Copyright (c) 1996 Aatu Koskensilta (Zaphod.B)
*/
#include "fixed2.h" // 16.16 fixed p
#include "grp.h" // Graphics functions
#include "tp.h" // Texturing
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define POSITIVE  0
#define NEGATIVE  1

int pixels_drawn; // Not used

void tmap(tpoly*tp)
{
   byte*texture;
   tpoint *p[3];
   tpoint *tmp;
   p[0]=&tp->pnts[0];
   p[1]=&tp->pnts[1];
   p[2]=&tp->pnts[2];
   char sign;
   // Sort points so that 1 = topmost, 2 = middle, 3 = lowmost
   if (p[1]->y<p[0]->y)
   {
      tmp=p[0];
      p[0]=p[1];
      p[1]=tmp;

   };
   if (p[2]->y<p[0]->y) 
   {
      tmp=p[0];
      p[0]=p[2];
      p[2]=tmp;

   };

   if (p[2]->y<p[1]->y) 
   {
      tmp=p[1];
      p[1]=p[2];
      p[2]=tmp;

   };

   // If triangle has no height, return
   if (p[2]->y==p[0]->y) return;

   // Calculate a multiplier
   fixed16 temp=((p[1]->y-p[0]->y)<<16)/(p[2]->y-p[0]->y);
   texture=tp->texture;
   // Calc width of widest scanline
   int width=(temp * (p[2]->x - p[0]->x) + ((p[0]->x - p[1]->x) << 16 ));
   // If polygon has no width, return
   if (width==0) return;
   if (width<0)
   {
            
      if (width>-FIXED_ONE) return;
   }
   else
   {
      if (width<FIXED_ONE) return;
   };
   // Calculate dx and dv (du & dv) and dfade for widest scanline. They stay constant
   // over whole triangle.
   int dx=fixdiv16((( (temp * (p[2]->tx - p[0]->tx))  ) + ((p[0]->tx - p[1]->tx ) <<16)) , width)>>8;
   int dy=fixdiv16((( (temp * (p[2]->ty - p[0]->ty))  ) + ((p[0]->ty - p[1]->ty ) <<16)) , width)>>8;
   int df=fixdiv16((( (temp * (p[2]->fd - p[0]->fd))  ) + ((p[0]->fd - p[1]->fd ) <<16)) , width)>>8;

   // Which side is right, and which is left 
   if (width>0) sign=NEGATIVE; else sign=POSITIVE;
   width=abs(width);

   int plus; // Used for clipping

   // Set up stuff
   int rex=p[0]->x<<8;
   int lex=p[0]->x<<8;
   int etx=p[0]->tx<<8;
   int ety=p[0]->ty<<8;
   int fade=p[0]->fd<<8;
   int drex;
   int dlex;   
   int detx;
   int dety;
   int dfade;


   int u,v,fd;
   if (sign==POSITIVE)
   {
      dlex=((p[2]->x-p[0]->x)<<8)/(p[2]->y-p[0]->y);
      if (p[1]->y!=p[0]->y) drex=((p[1]->x-p[0]->x)<<8)/(p[1]->y-p[0]->y);
      else
      drex=((p[1]->x-p[2]->x)<<8)/(p[1]->y-p[2]->y);

      detx=((p[2]->tx-p[0]->tx)<<8)/(p[2]->y-p[0]->y);
      dety=((p[2]->ty-p[0]->ty)<<8)/(p[2]->y-p[0]->y);
      dfade=((p[2]->fd-p[0]->fd)<<8)/(p[2]->y-p[0]->y);
   }
   else
   {
      drex=((p[2]->x-p[0]->x)<<8)/(p[2]->y-p[0]->y);
      if (p[1]->y!=p[0]->y)
      {
         dlex=((p[1]->x-p[0]->x)<<8)/(p[1]->y-p[0]->y);
         detx=((p[1]->tx-p[0]->tx)<<8)/(p[1]->y-p[0]->y);
         dety=((p[1]->ty-p[0]->ty)<<8)/(p[1]->y-p[0]->y);
         dfade=((p[1]->fd-p[0]->fd)<<8)/(p[1]->y-p[0]->y);
      }
      else
      {
         dlex=((p[1]->x-p[2]->x)<<8)/(p[1]->y-p[2]->y);
         detx=((p[1]->tx-p[2]->tx)<<8)/(p[1]->y-p[2]->y);
         dety=((p[1]->ty-p[2]->ty)<<8)/(p[1]->y-p[2]->y);
         dfade=((p[1]->fd-p[2]->fd)<<8)/(p[1]->y-p[2]->y);
      };
   };

   int yoffs=(p[0]->y<<6)+(p[0]->y<<8);
   // Draw polygon
   for (int yy=p[0]->y;yy<p[2]->y;yy++)
   {
      if (yy>199) return;
      if (sign==POSITIVE)
      {
         if (yy==p[1]->y && p[1]->y!=p[2]->y)
         {
            rex=p[1]->x<<8;
            drex=((p[2]->x-p[1]->x)<<8)/(p[2]->y-p[1]->y);
         };
      }
      else
      {
         if (yy==p[1]->y && p[1]->y!=p[2]->y)
         {

            lex=p[1]->x<<8;
            dlex=((p[2]->x-p[1]->x)<<8)/(p[2]->y-p[1]->y);
            etx=p[1]->tx<<8;
            ety=p[1]->ty<<8;
            fade=p[1]->fd<<8;
            detx=((p[2]->tx-p[1]->tx)<<8)/(p[2]->y-p[1]->y);
            dety=((p[2]->ty-p[1]->ty)<<8)/(p[2]->y-p[1]->y);
            dfade=((p[2]->fd-p[1]->fd)<<8)/(p[2]->y-p[1]->y);
         };
      };

      plus=0;
      int begx,endx;
      begx=lex>>8;
      endx=rex>>8;
      if (endx>319) endx=319;
      if (begx<0)
      {
         plus=-begx;
         begx=0;
      };
      int sa=yoffs+(begx);

      int sw=(endx-begx);
      register byte*ptr=&dbuffer[sa];;

      u=etx+plus*dx;
      v=ety+plus*dy;
      fd=fade+plus*df;
      if (yy>=0 && sw>=1)
         while(sw--)
         {
            // If no pixel is drawn to this point, draw a pixel
            if ((*ptr)==0) {(*ptr)=fadetbl[((((fd>>8))<<8)+texture[(((u>>8))+(((v>>8))<<5))&1023])];};
            u+=dx;
            v+=dy;
            fd+=df;
            ptr++;

         };
   
      yoffs+=320;
      rex+=drex;
      lex+=dlex;
      etx+=detx;
      ety+=dety;
      fade+=dfade;
   };


}
