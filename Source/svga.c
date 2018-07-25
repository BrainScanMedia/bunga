/********************************************************\
 * SVGA.C - Bunga game source code                      *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
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

#include "header\svga.h"

unsigned char far *video_buffer = (unsigned char far *)0xA0000000L;
unsigned char far *rom_char_set = (unsigned char far *)0xF000FA6EL;

/********************************************************\
 * Exit Function In case Of Errors                       *
\********************************************************/
void ExitProgram(char *msg) {
  // reset graphics to text mode
  Set_Graphics_Mode(TEXT_MODE);
  clrscr();
  cprintf(msg);
  exit(0);
}

/********************************************************\
 * Moves the cursor position to the indicated row and   *
 * column using the VGA BIOS, function #2.  This BIOS   *
 * routine works even in graphics mode and is used to   *
 * position the cursor for drawing text to the screen   *
 * in graphics	modes.                                   *
\********************************************************/
void SetCursorPosition(int Row, int Column) {
   union REGS Regs;
	Regs.h.bh = 0; /* page */
	Regs.h.dh = Row; /* row */
	Regs.h.dl = Column; /* column */
	Regs.h.ah = 2; /* set cursor position */
	int86(0x10, &Regs, &Regs);
}

/********************************************************\
 * Draws a character onto the screen using the VGA BIOS *
 * calls. The character is drawn in the specified color.*
\********************************************************/
void DrawChar(char Char, int Color) {
    union REGS Regs;
	 Regs.h.bh = 0; /* page */
	 Regs.h.al = Char; /* character */
	 Regs.h.bl = Color; /* attribute byte (= color) */
	 Regs.x.cx = 1; /* repeat count */
	 Regs.h.ah = 9; /* write character/attribute pair */
	 int86(0x10, &Regs, &Regs);
}

/********************************************************\
 * Draws a string to the graphics screen using the VGA  *
 * BIOS calls.  The string it drawn at the row and      *
 * column indicated using the specified color.          *
\********************************************************/
void DrawString(char *String, int Row, int Column, int Color) {
	 while (*String != '\0') {
      /* set cursor location */
      SetCursorPosition(Row, Column);
      Column++; /* advance cursor */
      /* draw the character */
      DrawChar(*String++, Color);
	 }
}

/********************************************************\
 * Centers a string on the screen. The function         *
 * calculates the correct starting column               *
 * position to center the string	on the screen and   *
 * then calls DrawString to do the actual	drawing of    *
 * the text.                                            *
\********************************************************/
void CenterString(char *String, int Row, int Color) {
    int Len;
    int Col;
	 Len = strlen(String);
	 Col = (40 - Len) / 2;
	 DrawString(String, Row, Col, Color);
}

/********************************************************\
 * Print_Char
 * this function is used to print a character on the screen.
 * It uses the internal 8x8 character set to do this. Note
 * that each character is 8 bytes where each byte represents
 * the 8 pixels that make up the row of pixels
\********************************************************/
void Print_Char(int xc,int yc,char c,int color,int transparent) {
int offset, x, y;
unsigned char far *work_char;
unsigned char bit_mask;
work_char = rom_char_set + c * ROM_CHAR_HEIGHT;
offset = (yc << 8) + (yc << 6) + xc;
for (y=0; y<ROM_CHAR_HEIGHT; y++) {
    bit_mask = 0x80;
    for (x=0; x<ROM_CHAR_WIDTH; x++) {
        if ((*work_char & bit_mask))
             video_buffer[offset+x] = (unsigned char)color;
        else
        if (!transparent)
            video_buffer[offset+x] = 0;
		  bit_mask = (bit_mask>>1);
        }
    offset      += MODE13_WIDTH;
    work_char++;
    }
}

/********************************************************\
 * Print_String
 * This function prints an entire string on the screen
 * with fixed spacing between each character by calling
 * the Print_Char() function
\********************************************************/
// Print_String( Col-x, Row-y, color, string, transparent)
void Print_String(int x,int y,int color, char *string,int transparent) {
int index, length;
length = strlen(string);
for (index=0; index<length; index++)
	  Print_Char(x+(index<<3),y,string[index],color,transparent);
}

/********************************************************\
 * Write_Pixel
 * plots the pixel in the desired color a little quicker
 * using binary shifting to accomplish the multiplications
\********************************************************/
void Write_Pixel(int x,int y,int color) {
video_buffer[((y<<8) + (y<<6)) + x] = (unsigned char )color;
}

/********************************************************\
 * Read_Pixel
 * this function read a pixel from the screen buffer
\********************************************************/
int Read_Pixel(int x,int y) {
return((int)(video_buffer[((y<<8) + (y<<6)) + x]));
}

/********************************************************\
 * See if the user has a vga card.                      *
\********************************************************/
void DetectVGA(void) {
  union REGS regs;
  /* try a BIOS call only the VGA understands */
  regs.x.bx = 0xFFFF;
  regs.x.ax = 0x101A;
  int86(0x10, &regs, &regs);
  /* if the card didn't understand, it's not a VGA */
  if(regs.x.bx == 0xFFFF) {
     cprintf("Sorry you must have a VGA card with 256 Colors.\a"); exit(0);
  }
}

/********************************************************\
 * Set_Graphics_Mode
 * use the video interrupt 10h and the C interrupt
 * function to set the video mode
\********************************************************/
void Set_Graphics_Mode(int mode) {
union REGS inregs,outregs;
inregs.h.ah = 0;
inregs.h.al = (unsigned char)mode;
int86(0x10, &inregs, &outregs);
}

/********************************************************\
 * Time_Delay
 * this function uses the internal timer to wait a
 * specified number of "clicks" the actual amount of real
 * time is the number of clicks * (time per click) usually
 * the time per click is set to 1/18th of a second or 55ms
\********************************************************/
void Time_Delay(int clicks) {
long far *clock = (long far *)0x0000046CL;
long start_time;
start_time = *clock;
while(labs(*clock - start_time) < (long)clicks){}
}

/********************************************************\
 * Line_H
 * draw a horizontal line using the memset function
 * this function does not do clipping hence x1,x2 and y
 * must all be within the bounds of the screen
\********************************************************/
void Line_H(int x1,int x2,int y,int color) {
int temp;
if (x1>x2) { temp = x1; x1 = x2; x2 = temp; }
_fmemset((char far *)(video_buffer + ((y<<8) + (y<<6)) + x1),
         (unsigned char)color,x2-x1+1);
}

/********************************************************\
 * Line_V
 * draw a vertical line, note that a memset function can
 * no longer be used since the pixel addresses are no
 * longer contiguous in memory note that the end points
 * of the line must be on the screen
\********************************************************/
void Line_V(int y1,int y2,int x,int color) {
unsigned char far *start_offset;
int index, temp;
if (y1>y2) { temp = y1; y1 = y2; y2 = temp; }
start_offset = video_buffer + ((y1<<8) + (y1<<6)) + x;
for (index=0; index<=y2-y1; index++) {
    *start_offset = (unsigned char)color;
    start_offset+=320;
    }
}

/********************************************************\
 * Write_Color_Reg
 * this function is used to write a color register with
 * the RGB value within "color"
\********************************************************/
void Write_Color_Reg(int index, RGB_color_ptr color) {
outp(COLOR_REGISTER_WR, index);
outp(COLOR_DATA,color->red);
outp(COLOR_DATA,color->green);
outp(COLOR_DATA,color->blue);
}

/********************************************************\
 * RGB_color_ptr Read_Color_Reg
 * this function reads the RGB triple out of a palette
 * register and places it into "color"
\********************************************************/
RGB_color_ptr Read_Color_Reg(int index, RGB_color_ptr color) {
outp(COLOR_REGISTER_RD, index);
color->red   = (unsigned char)inp(COLOR_DATA);
color->green = (unsigned char)inp(COLOR_DATA);
color->blue  = (unsigned char)inp(COLOR_DATA);
return(color);
}

/********************************************************\
 * Read_Palette
 * this function will read the palette registers in the
 * range start_reg to end_reg and save them into the
 * appropriate positions in colors
\********************************************************/
void Read_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette) {
int index;
RGB_color color;
for (index=start_reg; index<=end_reg; index++) {
    Read_Color_Reg(index,(RGB_color_ptr)&color);
    the_palette->colors[index].red   = color.red;
    the_palette->colors[index].green = color.green;
    the_palette->colors[index].blue  = color.blue;
    }
the_palette->start_reg = start_reg;
the_palette->end_reg   = end_reg;
}

/********************************************************\
 * Write_Palette
 * this function will write to the color registers using
 * the data in the sen palette structure
\********************************************************/
void Write_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette) {
int index;
for (index=start_reg; index<=end_reg; index++) {
    Write_Color_Reg(index,(RGB_color_ptr)&(the_palette->colors[index]));
    }
}

/********************************************************\
 * Draw_Rectangle
 * this function will draw a rectangle from (x1,y1) - (x2,y2)
 * it is assumed that each endpoint is within the screen
 * boundaries and (x1,y1) is the upper left hand corner
 * and (x2,y2) is the lower right hand corner
\********************************************************/
void Draw_Rectangle(int x1,int y1,int x2,int y2,int color) {
unsigned char far *start_offset;
int width;
start_offset = video_buffer + ((y1<<8) + (y1<<6)) + x1;
width  = 1 + x2 - x1;
while(y1++<=y2) {
     _fmemset((char far *)start_offset,(unsigned char)color,width);
     start_offset+=320;
     }
}

/********************************************************\
 * Fill_Screen
 * this function will fill the entire screen with the sent
 * color
\********************************************************/
void Fill_Screen(int color) {
_asm	{
	les di,video_buffer
	mov al,BYTE PTR color
	mov ah,al
	mov cx,320*200/2
	rep stosw
	}
}

/********************************************************\
 * Fill_Screen_Z
 * this function will fill the mode Z video buffer with
 * the sent color
\********************************************************/
void Fill_Screen_Z(int color) {
_asm   {
	mov dx,SEQUENCER
	mov al,SEQ_PLANE_ENABLE
	mov ah,0fh
	out dx,ax
	les di,video_buffer
	mov al,BYTE PTR color
	mov ah,al
	mov cx,320*400/8
	rep stosw
	}
}

/********************************************************\
 * Write_Pixel_Z
 * this function will write a pixel to screen in mode Z
\********************************************************/
void Write_Pixel_Z(int x,int y,int color) {
_asm   {
	mov dx,SEQUENCER
	mov al,SEQ_PLANE_ENABLE
	mov cl,BYTE PTR x
	and cl,03h
	mov ah,1
	shl ah,cl
	out dx,ax
	}
video_buffer[(y<<6)+(y<<4)+(x>>2)] = (unsigned char )color;
}

/********************************************************\
 * Set_Mode_Z
 * this function will set the video mode to 320x400x256
\********************************************************/
void Set_Mode_Z(void) {
int data;
_asm   {
	mov ax,0013h
	int 10h
   }
outp(CRT_CONTROLLER,CRT_MAX_SCANLINE);
data=inp(CRT_CONTROLLER+1);
outp(CRT_CONTROLLER+1,RESET_BITS(data,0x0f));
outp(CRT_CONTROLLER,CRT_ADDR_MODE);
data=inp(CRT_CONTROLLER+1);
outp(CRT_CONTROLLER+1,RESET_BITS(data,0x40));
outp(CRT_CONTROLLER,CRT_MODE_CONTROL);
data=inp(CRT_CONTROLLER+1);
outp(CRT_CONTROLLER+1,SET_BITS(data,0x40));
outp(GFX_CONTROLLER,GFX_WRITE_MODE);
data=inp(GFX_CONTROLLER+1);
outp(GFX_CONTROLLER+1,RESET_BITS(data,0x10));
outp(GFX_CONTROLLER,GFX_MISC);
data=inp(GFX_CONTROLLER+1);
outp(GFX_CONTROLLER+1,RESET_BITS(data,0x02));
outp(SEQUENCER,SEQ_MEMORY_MODE);
data =inp(SEQUENCER+1);
data = RESET_BITS(data,0x08);
data = SET_BITS(data,0x04);
outp(SEQUENCER+1,data);
outp(SEQUENCER,SEQ_PLANE_ENABLE);
outp(SEQUENCER+1,0x0f);
_asm   {
	les di,video_buffer
	xor ax,ax
	mov cx,320*400/8
	rep stosw
	}
}
