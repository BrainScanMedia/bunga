/********************************************************\
 * PCXCHAR.C - Bunga game source code                   *
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
#include "header\pcx.h"
#include "header\functions.h"

unsigned int sprite_width         = SPRITE_WIDTH;
unsigned int sprite_height        = SPRITE_HEIGHT;
unsigned char far *double_buffer  = NULL;
unsigned int double_buffer_height = SCREEN_HEIGHT;
unsigned int double_buffer_size   = SCREEN_WIDTH*SCREEN_HEIGHT/2;
unsigned char far *page_0_buffer  = (unsigned char far *)0xA0000000L;
unsigned char far *page_1_buffer  = (unsigned char far *)0xA0008000L;
int mode_z_page                   = PAGE_0;

/********************************************************\
 * PCX_Init
 * this function allocates the buffer that the image data
 * will be loaded into when a PCX file is decompressed
\********************************************************/
int PCX_Init(pcx_picture_ptr image) {
if (!(image->buffer = (unsigned char far *)farmalloc(SCREEN_WIDTH * SCREEN_HEIGHT + 1))) {
      farfree(image->buffer);
      if (!(image->buffer = (unsigned char far *)farmalloc(SCREEN_WIDTH * SCREEN_HEIGHT + 1))) {
      ExitProgram("\nPCX SYSTEM - Couldn't allocate PCX image buffer"); }
      }
return(1);
}

/********************************************************\
 * PCX_Load
 * this function loads a PCX file into the image structure.
 * The function has three main parts: 1. load the PCX
 * header, 2. load the image data and decompress it and 3.
 * load the palette data and update the VGA palette
 * note: the palette will only be loaded if the
 * load_palette flag is 1
\********************************************************/
int PCX_Load(char *filename, pcx_picture_ptr image,int load_palette) {
FILE *fp;
int num_bytes, index;
long count;
unsigned char data;
char far *temp_buffer;
if ((fp = fopen(filename,"rb"))==NULL) {
    Set_Graphics_Mode(TEXT_MODE); clrscr();
    printf("\nPCX SYSTEM - Couldn't find file: %s",filename);
    exit(0); }
temp_buffer = (char far *)image;
for (index=0; index<128; index++) {
     temp_buffer[index] = (char)getc(fp); }
count=0;
while(count<=SCREEN_WIDTH * SCREEN_HEIGHT) {
      data = (unsigned char)getc(fp);
      if (data>=192 && data<=255)    {
          num_bytes = data-192;
          data  = (unsigned char)getc(fp);
          while(num_bytes-->0) { image->buffer[count++] = data; }
      }
      else { image->buffer[count++] = data; }
}
fseek(fp,-768L,SEEK_END);
for (index=0; index<256; index++) {
    image->palette[index].red   = (unsigned char)(getc(fp) >> 2);
    image->palette[index].green = (unsigned char)(getc(fp) >> 2);
    image->palette[index].blue  = (unsigned char)(getc(fp) >> 2);  }
fclose(fp);
if (load_palette) {
     for (index=0; index<256; index++) {
          Write_Color_Reg(index,(RGB_color_ptr)&image->palette[index]); } }
return(1);
}

/********************************************************\
 * PCX_Delete
 * this function de-allocates the buffer region used for
 * the pcx file load
\********************************************************/
void PCX_Delete(pcx_picture_ptr image) {
farfree(image->buffer);
}

/********************************************************\
 * PCX_Show_Buffer
 * just copy he pcx buffer into the video buffer
\********************************************************/
void PCX_Show_Buffer(pcx_picture_ptr image) {
char far *data;
data = image->buffer;
_asm {
     push ds
     les di, video_buffer
     lds si, data
     mov cx,320*200/2
     cld
     rep movsw
     pop ds
     }
}

/********************************************************\
 * PCX_Copy_To_Buffer
 * this function is used to copy the data in the PCX buffer
 * to another buffer usually the double buffer
\********************************************************/
void PCX_Copy_To_Buffer(pcx_picture_ptr image,unsigned char far *buffer) {
fwordcpy((void far *)buffer,(void far *)image->buffer,double_buffer_size);
}

/********************************************************\
 * PCX_Get_Sprite
 * this function is used to load the images for a sprite
 * into the sprite frames array. It functions by using
 * the size of the sprite and the position of the
 * requested cell to compute the proper location in the
 * pcx image buffer to extract the data from.
\********************************************************/
void PCX_Get_Sprite(pcx_picture_ptr image,
                     sprite_ptr sprite,
                     int sprite_frame,
                     int cell_x, int cell_y)
{
int x_off, y_off, y, width, height;
unsigned char far *sprite_data;
width  = sprite->width;
height = sprite->height;
sprite->frames[sprite_frame] = (unsigned char far *)farmalloc(width * height + 1);
sprite_data = sprite->frames[sprite_frame];
x_off = (width+1)  * cell_x + 1;
y_off = (height+1) * cell_y + 1;
y_off = y_off * 320; // 320 bytes per line
for (y=0; y<height; y++,y_off+=320) {
    _fmemcpy((void far *)&sprite_data[y*width],
    (void far *)&(image->buffer[y_off + x_off]), width); }
sprite->num_frames++;
}

/********************************************************\
 * Sprite_Init
 * this function initializes a sprite
\********************************************************/
void Sprite_Init(sprite_ptr sprite,int x,int y,int width,int height,
                                   int c1,int c2,int c3,
                                   int t1,int t2,int t3)
{
int index;
sprite->x            = x;
sprite->y            = y;
sprite->width        = width;
sprite->height       = height;
sprite->visible      = 1;
sprite->counter_1    = c1;
sprite->counter_2    = c2;
sprite->counter_3    = c3;
sprite->threshold_1  = t1;
sprite->threshold_2  = t2;
sprite->threshold_3  = t3;
sprite->curr_frame   = 0;
sprite->state        = SPRITE_DEAD;
sprite->num_frames   = 0;
sprite->background   = (unsigned char far *)farmalloc(width * height+1);
for (index=0; index<MAX_SPRITE_FRAMES; index++)
    sprite->frames[index] = NULL;
}

/********************************************************\
 * Sprite_Delete
 * this function deletes all the memory associated with a
 * sprite
\********************************************************/
void Sprite_Delete(sprite_ptr sprite) {
int index;
farfree(sprite->background);
for (index=0; index<MAX_SPRITE_FRAMES; index++)
     farfree(sprite->frames[index]);
}

/********************************************************\
 * Sprite_Under
 * this function scans the background under a sprite so
 * that when the sprite is drawn the background isn't
 * obliterated
\********************************************************/
void Sprite_Under(sprite_ptr sprite, unsigned char far *buffer) {
unsigned char far *back_buffer; // background buffer for sprite
int y, width, height;
back_buffer = sprite->background;
width  = sprite->width;
height = sprite->height;
buffer = buffer + (sprite->y << 8) + (sprite->y << 6) + sprite->x;
for (y=0; y<height; y++) {
    _fmemcpy((void far *)back_buffer, (void far *)buffer, width);
    buffer      += SCREEN_WIDTH;
    back_buffer += width; }
}

/********************************************************\
 * Sprite_Erase
 * replace the background that was behind the sprite
 * this function replaces the background that was saved
 * from where a sprite was going to be placed
\********************************************************/
void Sprite_Erase(sprite_ptr sprite,unsigned char far *buffer) {
unsigned char far *back_buffer;
int y, width, height;
back_buffer = sprite->background;
width  = sprite->width;
height = sprite->height;
buffer = buffer + (sprite->y << 8) + (sprite->y << 6) + sprite->x;
for (y=0; y<height; y++) {
    _fmemcpy((void far *)buffer, (void far *)back_buffer, width);
    buffer      += SCREEN_WIDTH;
    back_buffer += width; }
}

/********************************************************\
 * Sprite_Draw
 * this function draws a sprite on the screen row by row
 * very quickly note the use of shifting to implement
 * multplication if the transparent flag is true then
 * pixels wil be draw one by one else a memcpy will be
 * used to draw each line
\********************************************************/
void Sprite_Draw(sprite_ptr sprite, unsigned char far *buffer,int transparent) {
unsigned char far *sprite_data;
unsigned char far *dest_buffer;
int x,y, width, height;
unsigned char pixel;
sprite_data = sprite->frames[sprite->curr_frame];
width  = sprite->width;
height = sprite->height;
dest_buffer = buffer + (sprite->y << 8) + (sprite->y << 6) + sprite->x;
if (transparent) {
	for (y=0; y<height; y++) {
       for (x=0; x<width; x++) {
           if ((pixel=sprite_data[x]))
                dest_buffer[x] = pixel;
           }
       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;
       }
   }
else
	{
   for (y=0; y<height; y++) {
       _fmemcpy((void far *)dest_buffer,(void far *)sprite_data,width);
       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;
       }
  }
}

/********************************************************\
 * Sprite_Under_Clip
 * this function scans the background under a sprite, but
 * only those portions that are visible
\********************************************************/
void Sprite_Under_Clip(sprite_ptr sprite, unsigned char far *buffer) {
unsigned char far *back_buffer;
unsigned char far *source_buffer;
int x,y,sx,sy,width,
    bitmap_width  =0,
    bitmap_height =0;
unsigned char pixel;
width         = sprite->width;
bitmap_width  = width;
bitmap_height = sprite->height;
sx            = sprite->x;
sy            = sprite->y;
if (sx >= (int)SCREEN_WIDTH || sy >= (int)double_buffer_height ||
   (sx+width) <= 0          || (sy+bitmap_height) <= 0) {
   sprite->visible = 0;
   return; }
if (sx<0) {
   bitmap_width += sx;
	sx            = 0; }
else
if (sx+width>=(int)SCREEN_WIDTH) {
   bitmap_width  = (int)SCREEN_WIDTH-sx;
   }
if (sy<0) {
   bitmap_height += sy;
   sy             = 0;
	}
else
if (sy+bitmap_height>=(int)double_buffer_height) {
   bitmap_height  = (int)double_buffer_height - sy;
   }
source_buffer = buffer + (sy << 8) + (sy << 6) + sx;
back_buffer = sprite->background;
for (y=0; y<bitmap_height; y++) {
    _fmemcpy((void far *)back_buffer,(void far *)source_buffer,bitmap_width);
    source_buffer += SCREEN_WIDTH;
    back_buffer   += width;
    }
sprite->x_clip      = sx;
sprite->y_clip      = sy;
sprite->width_clip  = bitmap_width;
sprite->height_clip = bitmap_height;
sprite->visible     = 1;
}

/********************************************************\
 * Sprite_Erase_Clip
 * replace the background that was behind the sprite
 * this function replaces the background that was saved
 * from where a sprite was going to be placed
\********************************************************/
void Sprite_Erase_Clip(sprite_ptr sprite,unsigned char far *buffer) {
unsigned char far *back_buffer;
int y,width,bitmap_height,bitmap_width;
if (!sprite->visible)
	return;
back_buffer = sprite->background;
bitmap_width  = sprite->width_clip;
bitmap_height = sprite->height_clip;
width         = sprite->width;
buffer = buffer + (sprite->y_clip << 8) + (sprite->y_clip << 6)
                + sprite->x_clip;
for (y=0; y<bitmap_height; y++) {
    _fmemcpy((void far *)buffer,
             (void far *)back_buffer,
				 bitmap_width);
    buffer      += SCREEN_WIDTH;
    back_buffer += width;
    }
}

/********************************************************\
 * Sprite_Draw_Clip
 * this function draws a sprite on the screen row by row
 * very quickly note the use of shifting to implement
 * multplication if the transparent flag is true then
 * pixels wil be draw one by one else a memcpy will be
 * used to draw each line this function also performs
 * clipping. It will test if the sprite is totally
 * visible/invisible and will only draw the portions that
 * are visible
\********************************************************/
void Sprite_Draw_Clip(sprite_ptr sprite, unsigned char far *buffer,int transparent) {
unsigned char far *sprite_data;
unsigned char far *dest_buffer;
int x,y,sx,sy,width,
    bitmap_x      =0,
	 bitmap_y      =0,
    bitmap_width  =0,
    bitmap_height =0;
unsigned char pixel;
width         = sprite->width;
bitmap_width  = width;
bitmap_height = sprite->height;
sx            = sprite->x;
sy            = sprite->y;
if (sx >= (int)SCREEN_WIDTH || sy >= (int)double_buffer_height ||
   (sx+width) <= 0          || (sy+bitmap_height) <= 0 || !sprite->visible) {
   sprite->visible = 0;
   return;
   }
if (sx<0) {
   bitmap_x      = -sx;
   sx            = 0;
   bitmap_width -= bitmap_x;
   }
else
if (sx+width>=(int)SCREEN_WIDTH)	{
   bitmap_x      = 0;
   bitmap_width  = (int)SCREEN_WIDTH-sx;
   }
if (sy<0) {
   bitmap_y       = -sy;
   sy             = 0;
	bitmap_height -= bitmap_y;
   }
else
if (sy+bitmap_height>=(int)double_buffer_height) {
   bitmap_y       = 0;
   bitmap_height  = (int)double_buffer_height - sy;
   }
dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;
sprite_data = sprite->frames[sprite->curr_frame] + (bitmap_y*width) + bitmap_x;
if (transparent) {
   for (y=0; y<bitmap_height; y++) {
       for (x=0; x<bitmap_width; x++) {
           if ((pixel=sprite_data[x]))
                dest_buffer[x] = pixel;
           }
       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;
       }
   }
else
   {
	for (y=0; y<bitmap_height; y++) {
       _fmemcpy((void far *)dest_buffer,(void far *)sprite_data,bitmap_width);
       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;
       }
   }
sprite->x_clip      = sx;
sprite->y_clip      = sy;
sprite->width_clip  = bitmap_width;
sprite->height_clip = bitmap_height;
sprite->visible     = 1;
}

/********************************************************\
 * Sprites_Collide
 * this function tests if two sprites have intersected by
 * testing their bounding boxes for collision
\********************************************************/
int Sprites_Collide(sprite_ptr sprite_1, sprite_ptr sprite_2) {
return(0);
}

/********************************************************\
 * Display_Double_Buffer
 * this functions copies the double buffer into the video
 * buffer at the starting y location
\********************************************************/
void Display_Double_Buffer(unsigned char far *buffer,int y) {
_asm
	{
	push ds
	mov cx,double_buffer_size
	les di,video_buffer
	mov ax,320
	mul y
	add di,ax
	lds si,buffer
	rep movsw
	pop ds
	}
}

/********************************************************\
 * Create_Double_Buffer
 * allocate enough memory to hold the double buffer
\********************************************************/
int Create_Double_Buffer(int num_lines) {
if ((double_buffer = (unsigned char far *)farmalloc(SCREEN_WIDTH * (num_lines + 1)))==NULL) {
   Set_Graphics_Mode(TEXT_MODE); clrscr();
   printf("\nCouldn't allocate double buffer.");
   exit(0);
   }
double_buffer_height = num_lines;
double_buffer_size = SCREEN_WIDTH * num_lines/2;
_fmemset(double_buffer, 0, SCREEN_WIDTH * num_lines);
return(1);
}

/********************************************************\
 * Fill_Double_Buffer
 * this function fills in the double buffer with the sent
 * color a WORD at a time
\********************************************************/
void Fill_Double_Buffer(int color) {
_asm	{
	mov cx,double_buffer_size
	mov al, BYTE PTR color
	mov ah,al
	les di,double_buffer
	rep stosw
	}
}

/********************************************************\
 * Delete_Double_Buffer
 * this function free's up the memory allocated by the
 * double buffer make sure to use FAR version
\********************************************************/
void Delete_Double_Buffer(void) {
if (double_buffer)
  farfree(double_buffer);
}

/********************************************************\
 * Screen_Transition
 * this function can be called to perform a myraid of
 * screen transitions to the video buffer, note I have
 * left one for you to create!
\********************************************************/
void Screen_Transition(int effect) {
int pal_reg;
long index;
RGB_color color;
switch(effect) {
      case SCREEN_DARKNESS: {
                  for (index=0; index<30; index++) {
                       for (pal_reg=1; pal_reg<256; pal_reg++) {
                            Read_Color_Reg(pal_reg,(RGB_color_ptr)&color);
                            if (color.red > 4) color.red-=3;
                            else
                            color.red = 0;
                            if (color.green > 4) color.green-=3;
                            else
                            color.green = 0;
                            if (color.blue  > 4) color.blue-=3;
                            else
                            color.blue = 0;
                            Write_Color_Reg(pal_reg,(RGB_color_ptr)&color);
                  }
                  Time_Delay(1); }
                  } break;
      case SCREEN_WHITENESS: {
                  for (index=0; index<30; index++) {
                       for (pal_reg=0; pal_reg<256; pal_reg++) {
                            Read_Color_Reg(pal_reg,(RGB_color_ptr)&color);
                            color.red+=4;
                            if (color.red > 63)
                                color.red = 63;
                                color.green+=4;
                            if (color.green > 63)
                                color.green = 63;
                                color.blue+=4;
                            if (color.blue >63)
                                color.blue = 63;
                                Write_Color_Reg(pal_reg,(RGB_color_ptr)&color);
                      }
                 Time_Delay(1); }
                 } break;
      case SCREEN_WARP: {
                  for (index=0; index<30; index++) {
                       for (pal_reg=0; pal_reg<256; pal_reg++) {
                            Read_Color_Reg(pal_reg,(RGB_color_ptr)&color);
                            color.red+=4;
                            if (color.red > 63)
                                color.red = 63;
                                color.green+=4;
                            if (color.green > 0)
                                color.green = 0;
                                color.blue+=4;
                            if (color.blue >0)
                                color.blue = 0;
                                Write_Color_Reg(pal_reg,(RGB_color_ptr)&color);
                       } Time_Delay(1); }
                      } break;
      case SCREEN_SWIPE_X: {
                  for (index=0; index<160; index+=2) {
                       Wait_For_Vertical_Retrace();
                       Line_V(0,199,319-index,0);
                       Line_V(0,199,index,0);
                       Line_V(0,199,319-(index+1),0);
                       Line_V(0,199,index+1,0); }
                  } break;
		case SCREEN_SWIPE_Y: {
                  for (index=0; index<100; index+=2) {
                       Wait_For_Vertical_Retrace();
                       Line_H(0,319,199-index,0);
                       Line_H(0,319,index,0);
                       Line_H(0,319,199-(index+1),0);
                       Line_H(0,319,index+1,0); }
                  } break;
      case SCREEN_DISOLVE: {
                  for (index=0; index<=300000; index++)
                       Write_Pixel(rand()%320,rand()%200, 0);
                  } break;
      default:break;
      }
}

/********************************************************\
 * Wait_For_Vertical_Retrace
 * this function waits for the start of a vertical
 * retrace, if a vertical retrace is in progress then it
 * waits until the next one therefore the function can
 * wait a maximum of 2/70 th's of a second before returning
 * this function can be used to synchronize video updates
 * to the vertical blank or as a high resolution time reference
\********************************************************/
void Wait_For_Vertical_Retrace(void) {
while(inp(VGA_INPUT_STATUS_1) & VGA_VRETRACE_MASK) { }
while(!(inp(VGA_INPUT_STATUS_1) & VGA_VRETRACE_MASK)) { }
}

/********************************************************\
 * fwordcpy
 * this function is similar to fmemcpy except that is
 * moves data in words it is about 25% faster than memcpy
 * which uses bytes
\********************************************************/
void fwordcpy(void far *destination, void far *source,int num_words) {
_asm	{
	push ds
	les di,destination
	lds si,source
	mov cx,num_words
	rep movsw
	pop ds
   }
}

/********************************************************\
 * Bitmap_Put
 * this function will draw a bitmap on the destination
 * buffer which could be a double buffer or the video buffer
\********************************************************/
void Bitmap_Put(bitmap_ptr image, unsigned char far *destination,int transparent) {
int x,y,width,height;
unsigned char far *bitmap_data;
unsigned char far *dest_buffer;
unsigned char pixel;
dest_buffer = destination + (image->y << 8) + (image->y << 6) + image->x;
height      = image->height;
width       = image->width;
bitmap_data = image->buffer;
if (transparent) {
   for (y=0; y<height; y++) {
       for (x=0; x<width; x++) {
           if ((pixel=bitmap_data[x]))
                dest_buffer[x] = pixel;
           }
       dest_buffer += SCREEN_WIDTH;
       bitmap_data += width;
       }
   }
else
	{
   for (y=0; y<height; y++) {
       _fmemcpy((void far *)dest_buffer,
                (void far *)bitmap_data,width);
       dest_buffer += SCREEN_WIDTH;
       bitmap_data += width;
		 }
   }
}

/********************************************************\
 * Bitmap_Get
 * this function will scan a bitmap form the source buffer
 * could be a double buffer, video buffer or any other
 * buffer with a logical row width of 320 bytes
\********************************************************/
void Bitmap_Get(bitmap_ptr image, unsigned char far *source) {
unsigned int source_off,bitmap_off;
int y,width,height;
unsigned char far *bitmap_data;
source_off   = (image->y << 8) + (image->y << 6) + image->x;
bitmap_off = 0;
height      = image->height;
width       = image->width;
bitmap_data = image->buffer;
for (y=0; y<height; y++) {
    _fmemcpy((void far *)&bitmap_data[bitmap_off],
             (void far *)&source[source_off],width);
    source_off += SCREEN_WIDTH;
    bitmap_off += width;
    }
}

/********************************************************\
 * Bitmap_Allocate
 * this function can be used to allocate the memory needed
 * for a bitmap
\********************************************************/
int Bitmap_Allocate(bitmap_ptr image, int width, int height) {
if ((image->buffer = (unsigned char far *)farmalloc(width*height+1))==NULL)
	return(0);
else
   return(1);
}

/********************************************************\
 * Bitmap_Delete
 * this function deletes the memory used by a bitmap
\********************************************************/
void Bitmap_Delete(bitmap_ptr the_bitmap) {
if (the_bitmap->buffer)
   farfree(the_bitmap->buffer);
}

/********************************************************\
 * Layer_Draw
 * this function will map down a section of the layer
 * onto the destination buffer at the desired location,
 * note the width of the destination buffer is always
 * assumed to be 320 bytes width. Also, the function
 * will always wrap around the layer
\********************************************************/
void Layer_Draw(layer_ptr source_layer, int source_x, int source_y,
                unsigned char far *dest_buffer,int dest_y,int dest_height,
                int transparent)
{
int x,y,layer_width,right_width,left_width;
unsigned char far *layer_buffer_l;
unsigned char far *dest_buffer_l;
unsigned char far *layer_buffer_r;
unsigned char far *dest_buffer_r;
unsigned char pixel;
layer_width = source_layer->width;
dest_buffer_l  = dest_buffer + (dest_y << 8) + (dest_y << 6);
layer_buffer_l = source_layer->buffer + layer_width*source_y + source_x;
if ( ( (layer_width-source_x)-(int)SCREEN_WIDTH ) >= 0) {
   left_width  = SCREEN_WIDTH;
   right_width = 0; // no wraping flag
	}
else
   {
   left_width  = layer_width - source_x;
   right_width = SCREEN_WIDTH - left_width;
	dest_buffer_r  = dest_buffer_l + left_width;
   layer_buffer_r = layer_buffer_l - source_x;
   }
if (transparent) {
	for (y=0; y<dest_height; y++) {
       for (x=0; x<left_width; x++) {
           if ((pixel=layer_buffer_l[x]))
                dest_buffer_l[x] = pixel;
           }
		 dest_buffer_l  += SCREEN_WIDTH;
		 layer_buffer_l += layer_width;
       }
   if (right_width) {
      for (y=0; y<dest_height; y++) {
          for (x=0; x<right_width; x++)  {
              if ((pixel=layer_buffer_r[x]))
                   dest_buffer_r[x] = pixel;
              }
          dest_buffer_r  += SCREEN_WIDTH;
          layer_buffer_r += layer_width;
          }
		}
   }
else
   {
   for (y=0; y<dest_height; y++) {
       _fmemcpy((void far *)dest_buffer_l,
                (void far *)layer_buffer_l,left_width);
       dest_buffer_l  += SCREEN_WIDTH;
       layer_buffer_l += layer_width;
       }
   if (right_width) {
      for (y=0; y<dest_height; y++) {
			 _fmemcpy((void far *)dest_buffer_r,
						 (void far *)layer_buffer_r,right_width);
          dest_buffer_r  += SCREEN_WIDTH;
          layer_buffer_r += layer_width;
			 }
      }
   }
}

/********************************************************\
 * Layer_Build
 * this function is used to build up the layer out of
 * smaller pieces this allows a layer to be very long,
 * tall etc. also the source data buffer must be
 * constructed such that there are 320 bytes per row
\********************************************************/
void Layer_Build(layer_ptr dest_layer,int dest_x, int dest_y,
                 unsigned char far *source_buffer,int source_x,int source_y,
                 int width,int height)
{
int y,layer_width;
unsigned char far *source_data;
unsigned char far *layer_buffer;
layer_width  = dest_layer->width;
layer_buffer = dest_layer->buffer + layer_width*dest_y + dest_x; ;
source_data = source_buffer + (source_y << 8) + (source_y << 6) + source_x;
for (y=0; y<height; y++) {
    _fmemcpy((void far *)layer_buffer,
             (void far *)source_data,width);
    source_data  += SCREEN_WIDTH;
	 layer_buffer += layer_width;
    }
}

/********************************************************\
 * Layer_Create
 * this function can be used to allocate the memory
 * needed for a layer the width must be divisible by two.
\********************************************************/
int Layer_Create(layer_ptr dest_layer, int width, int height) {
if ((dest_layer->buffer = (unsigned char far *)farmalloc(width*height+2))==NULL)
   return(0);
else
	{
   dest_layer->width  = width;
   dest_layer->height = height;
   return(1);
	}
}

/********************************************************\
 * Layer_Delete
 * this function deletes the memory used by a layer
\********************************************************/
void Layer_Delete(layer_ptr the_layer) {
if (the_layer->buffer)
	farfree(the_layer->buffer);
}

/********************************************************\
 * Print_Char
 * this function is used to print a character on the double
 * buffer. It uses the internal 8x8 character set to do
 * this. Note that each character is 8 bytes where each
 * byte represents the 8 pixels that make up the row of pixels
\********************************************************/
void Print_Char_DB(int xc,int yc,char c,int color,int transparent) {
int offset,x,y;
unsigned char far *work_char;
unsigned char bit_mask;
work_char = rom_char_set + c * ROM_CHAR_HEIGHT;
offset = (yc << 8) + (yc << 6) + xc;
for (y=0; y<ROM_CHAR_HEIGHT; y++) {
	 bit_mask = 0x80;
    for (x=0; x<ROM_CHAR_WIDTH; x++) {
		  if ((*work_char & bit_mask))
             double_buffer[offset+x] = (unsigned char)color;
        else
        if (!transparent)
            double_buffer[offset+x] = 0;
		  bit_mask = (bit_mask>>1);
        }
    offset      += MODE13_WIDTH;
	 work_char++;
    }
}

/********************************************************\
 * Print_String
 * this function prints an entire string into the double
 * buffer with fixed spacing between each character by
 * calling the Print_Char() function
\********************************************************/
void Print_String_DB(int x,int y,int color, char *string,int transparent) {
 int index,length;
length = strlen(string);
for (index=0; index<length; index++)
     Print_Char_DB(x+(index<<3),y,string[index],color,transparent);
}

/********************************************************\
 * Write_Pixel
 * plots the pixel in the desired color to the double buffer
 * to accomplish the multiplications
\********************************************************/
void Write_Pixel_DB(int x,int y,int color) {
double_buffer[((y<<8) + (y<<6)) + x] = (unsigned char )color;
}

/********************************************************\
 * Read_Pixel
 * this function read a pixel from the double buffer
\********************************************************/
int Read_Pixel_DB(int x,int y) {
return((int)(double_buffer[((y<<8) + (y<<6)) + x]));
}

/********************************************************\
 * Set_Visual_Page_Mode_Z
 * this function sets the visual page that will be
 * displayed by the VGA
\********************************************************/
void Set_Visual_Page_Mode_Z(int page) {
if (page==PAGE_0) {
   outp(CRT_CONTROLLER,CRT_ADDR_LOW);
   outp(CRT_CONTROLLER+1,0x00);
   outp(CRT_CONTROLLER,CRT_ADDR_HI);
	outp(CRT_CONTROLLER+1,0x00);
   }
else
if (page==PAGE_1) {
   outp(CRT_CONTROLLER,CRT_ADDR_LOW);
   outp(CRT_CONTROLLER+1,0x00);
	outp(CRT_CONTROLLER,CRT_ADDR_HI);
	outp(CRT_CONTROLLER+1,0x80);
   }
}

/********************************************************\
 * Set_Working_Page_Mode_Z
 * this function sets the page that all mode Z functions
 * will update when called
\********************************************************/
void Set_Working_Page_Mode_Z(int page) {
if (page==PAGE_0)
	video_buffer = page_0_buffer;
else
   video_buffer = page_1_buffer;
}

/********************************************************\
 * Show_PCX_FX
 * My own little easy show function with FX ending
\********************************************************/
pcx_picture image_pcx001;  // general PCX image used to load background and imagery
void Show_PCX_FX(char* pcxfile, int option, int time) {
PCX_Init((pcx_picture_ptr)&image_pcx001);                     // load the screen image
if (PCX_Load(pcxfile,(pcx_picture_ptr)&image_pcx001,1)) {
    PCX_Show_Buffer((pcx_picture_ptr)&image_pcx001);          // copy the image to the display buffer
    PCX_Delete((pcx_picture_ptr)&image_pcx001);               // delete the PCX buffer
    } else { if (PCX_Load(pcxfile,(pcx_picture_ptr)&image_pcx001,1)) {
                 PCX_Show_Buffer((pcx_picture_ptr)&image_pcx001);          // copy the image to the display buffer
                 PCX_Delete((pcx_picture_ptr)&image_pcx001);               // delete the PCX buffer
                 } else { ExitProgram("ERROR! Could Not Load image file"); } }
    Pause(time);
    if(option==0) { Screen_Transition(SCREEN_DARKNESS);  } // fade to black
    if(option==1) { Screen_Transition(SCREEN_WHITENESS); } // fade to white
    if(option==2) { Screen_Transition(SCREEN_SWIPE_X);   } // do a horizontal swipe
    if(option==3) { Screen_Transition(SCREEN_SWIPE_Y);   } // do a vertical swipe
    if(option==4) { Screen_Transition(SCREEN_DISOLVE);   } // a pixel dissolve
    if(option==5) { Screen_Transition(SCREEN_WARP);      } // warp the screen image
}

/********************************************************\
 * Show_PCX_NORM
 * My own little easy show function with no ending
\********************************************************/
pcx_picture image_pcx002;  // general PCX image used to load background and imagery
void Show_PCX_NORM(char* pcxfile) {
PCX_Init((pcx_picture_ptr)&image_pcx002);                     // load the screen image
if (PCX_Load(pcxfile,(pcx_picture_ptr)&image_pcx002,1)) {
    PCX_Show_Buffer((pcx_picture_ptr)&image_pcx002);          // copy the image to the display buffer
    PCX_Delete((pcx_picture_ptr)&image_pcx002);               // delete the PCX buffer
    } else { if (PCX_Load(pcxfile,(pcx_picture_ptr)&image_pcx002,1)) {
                 PCX_Show_Buffer((pcx_picture_ptr)&image_pcx002);          // copy the image to the display buffer
                 PCX_Delete((pcx_picture_ptr)&image_pcx002);               // delete the PCX buffer
                 } else { ExitProgram("ERROR! Could Not Load image file"); } }
}

/********************************************************\
 * Show_PCX_FX_ENDING
 * My own little easy FX ending
\********************************************************/
pcx_picture image_pcx002;  // general PCX image used to load background and imagery
void PCX_FX_ENDING(int option) {
    if(option==0) { Screen_Transition(SCREEN_DARKNESS);  } // fade to black
    if(option==1) { Screen_Transition(SCREEN_WHITENESS); } // fade to white
    if(option==2) { Screen_Transition(SCREEN_SWIPE_X);   } // do a horizontal swipe
    if(option==3) { Screen_Transition(SCREEN_SWIPE_Y);   } // do a vertical swipe
    if(option==4) { Screen_Transition(SCREEN_DISOLVE);   } // a pixel dissolve
    if(option==5) { Screen_Transition(SCREEN_WARP);      } // warp the screen image
}



