/********************************************************\
 * BUNGA.C - Bunga game source code                     *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

#ifndef __LARGE__
#error Must use Large memory model for this game to run properly.
#endif

/********************************************************\
 * Include Compiler Headers
\********************************************************/
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <mem.h>
#include <time.h>

/********************************************************\
 * Include GameLib Headers
\********************************************************/
#include "header\keyboard.h"
#include "header\svga.h"
#include "header\pcx.h"
#include "header\timer.h"
#include "header\functions.h"
#include "header\bunga.h"

void main(int argc, char **argv) {

   printf("********************************************************\n\r");
   printf("* BUNGA - The Kidnapped Puddy Cat                      *\n\r");
   printf("* Copyright (c) BrainScanMedia.com, Inc., 1999         *\n\r");
   printf("* Developed & Programmed By Thomas J. Allen            *\n\r");
   printf("* Compiled using Borland C++ 5.02                      *\n\r");
   printf("* Req: DOS, 640KB RAM, 1MB HD, Sound Optional          *\n\r");
   printf("********************************************************\n\r");
   printf("BETA VERSION - Still in development...\n\n\r");
   printf("Report Any Bugs To info@brainscanmedia.com\n\n\r");

   printf("KEYS:\n\r");
   printf("      l = Drop Lives to 0        LEFTARROW = Pan Board Left\n\r");
   printf("      L = Increases Lives       RIGHTARROW = Pan Board Right\n\r");
   printf("      s = Increases Score               F1 = Help\n\r");
   printf("    ESC = Exits\n\n\r");

   printf("Hit Any Key!");
   //Pause(5);
   //PCX_FX_ENDING(0);                       /* Fade out the screen with FX */

   DetectVGA();
   Set_Graphics_Mode(GRAPHICS_MODE13);     /* Set the graphics mode to mode 13h */

   //Show_Intro();                           /* Show Game Intro */
   //Show_Menu();                            /* Start Menu Routine */

   Level1();  /* Play Level 1 */

   Set_Graphics_Mode(TEXT_MODE);

}
