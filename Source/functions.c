/********************************************************\
 * FUNCTIONS.C - Bunga game source code                 *
 * Compiled using Borland C++ 5..2                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

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
 * Pause Routine - Better one that does not require delay
\********************************************************/
void Pause(int tim){
   int i=1;
   int sec=1;
	for(; ;) { i++;
       if(i==100) { sec++, i=1; } // Estimate secounds
          delay(10);
       if(sec==tim) { break; }   // If sec = number then break
       if(kbhit()) { bioskey(0); break; }
	}
}

