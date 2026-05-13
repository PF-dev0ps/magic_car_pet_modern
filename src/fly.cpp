/*
   Main unit for Magic Car Pet 3d-project
   Copyright (c) 1996 Aatu Koskensilta (Zaphod.B)
--

   VER:    0.2 - MAY2026
   BY:    https://github.com/PF-dev0ps.
*/

#include "mcpet.h"
#include "fixed.h"
#include "keyboard.h"
#include "grp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

byte fadetbl[64*256];

int main(int argc,char**argv)
{
   gmode(0x13);
   FILE*f=fopen("assets/fade.dat","rb");
   f = fopen("assets/fade.dat", "rb");
   if (!f)  {
      perror("assets/fade.dat");
      return 1;
            }
   fread((char*)&fadetbl,256*64,1,f);
   fclose(f);
   keyboard_init();
   keyboard_unchain();
   init_engine();
   texture[0]=load_pic_from_file("assets/t1.pcc");
   texture[1]=load_pic_from_file("assets/t2.pcc");
   texture[2]=load_pic_from_file("assets/t3.pcc");
   texture[3]=load_pic_from_file("assets/t4.pcc");
   texture[4]=load_pic_from_file("assets/t5.pcc");
   texture[5]=load_pic_from_file("assets/t6.pcc");
   RGBcolor *pal=load_pal_from_file("assets/t1.pcc");
   setpal(pal);
   free (pal);
   int x=128<<(block_size+8),z=128<<(block_size+8),y=0;
   int angle=0;
   int angle2=0;
   int angle3=0;
   int yy=512;
   int speed=0;
   char *hname=(char*)"h256.pcx";
   if (argc>1) hname=argv[1];
   landscape=load_pic_from_file(hname);
   ground=load_pic_from_file("assets/g256.pcx");
   do
   {
      
      if (keyb[SCAN_Q])
      {
         y-=2;
      };

      if (keyb[SCAN_PGDN])
      {
         angle3++;
         if (angle3>NUM_O_ANGLES) angle3=0;
      };
      if (keyb[SCAN_PGUP])
      {
         angle3--;
         if (angle3<0) angle3=NUM_O_ANGLES;
      };

      if (keyb[SCAN_U])
      {
         angle2++;
         if (angle2>NUM_O_ANGLES) angle2=0;
      };

      if (keyb[SCAN_J])
      {
         angle2--;
         if (angle2<0) angle2=NUM_O_ANGLES;
      };


      if (keyb[SCAN_A])
      {
         y+=2;
      };


      if (keyb[SCAN_LEFT])
      {
         angle++;
         angle%=NUM_O_ANGLES;
      }
      if (keyb[SCAN_RIGHT])
      {
         angle--;
         if (angle<0) angle+=NUM_O_ANGLES;
      }
      if (keyb[SCAN_UP])
      {
         speed++; 
         if (speed>64) speed=64;
      };



      if (keyb[SCAN_DOWN])
      {
         speed--;
         if (speed<-64) speed=-64;
      };

      if (!keyb[SCAN_DOWN] && !keyb[SCAN_UP])
      {
         if (speed>0) speed--;
         if (speed<0) speed++;
      };

      if (speed)
      {
         x+=cosine[angle]*speed;
         z+=sine[angle]*speed;
      };

      int loffset=((((x+(cosine[angle]<<8))>>(10+block_size))&255)+((((z+(sine[angle]<<8))>>(10+block_size))&255)<<8))&65535;

      int yy1=(((int)255-landscape[loffset]))<<HEIGHT_SHIFT;
      if (yy<yy1)
      {
         yy+=2;
         if (yy>yy1) yy=yy1;
      };
      if (yy>yy1) 
      {
         yy-=(abs(speed)*(yy-yy1))>>8;
         if (yy<yy1) yy=yy1;
      };
      render_landscape(x>>2,yy+y,z>>2,angle,angle2,angle3);
      update_screen();
      memset(dbuffer,0,320*200);
   }while(!keyb[SCAN_ESC]);
   keyboard_close();
   gmode(3);

}
