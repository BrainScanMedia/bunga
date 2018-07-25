/********************************************************\
 * SVGA.H - Bunga game source code                      *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

/********************************************************\
 * THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE       *
 * PROPERTY OF MEGA TECH SOFTWARE. COPYRIGHT 1998       *
 * MEGA TECH SOFTWARE. ALL RIGHTS RESERVED.             *
\********************************************************/

#define GRAPHICS_MODE13     0x13
#define TEXT_MODE           0x03
#define COLOR_MASK        0x3C6
#define COLOR_REGISTER_RD 0x3C7
#define COLOR_REGISTER_WR 0x3C8
#define COLOR_DATA        0x3C9
#define ROM_CHAR_SET_SEG 0xF000
#define ROM_CHAR_SET_OFF 0xFA6E
#define ROM_CHAR_WIDTH      8
#define ROM_CHAR_HEIGHT     8
#define MODE13_WIDTH   (unsigned int)320
#define MODE13_HEIGHT  (unsigned int)200

/********************************************************\
 * the VGA card controller ports
\********************************************************/
#define SEQUENCER             0x3C4
#define CRT_CONTROLLER        0x3D4
#define GFX_CONTROLLER        0x3CE
#define ATTR_CONTROLLER_FF    0x3DA
#define ATTR_CONTROLLER_DATA  0x3C0

/********************************************************\
 * defines for the CRT controller registers of interest
\********************************************************/
#define CRT_MAX_SCANLINE      0x09
#define CRT_ADDR_MODE         0x14
#define CRT_MODE_CONTROL      0x17

/********************************************************\
 * defines for the GFX controller registers of interest
\********************************************************/
#define GFX_WRITE_MODE       0x05
#define GFX_MISC             0x06

/********************************************************\
 * defines for the SEQUENCER registers of interest
\********************************************************/
#define SEQ_PLANE_ENABLE     0x02
#define SEQ_MEMORY_MODE      0x04


/********************************************************\
 * MACROS
\********************************************************/
#define SET_BITS(x,bits)   (x | bits)
#define RESET_BITS(x,bits) (x & ~bits)

/********************************************************\
 * STUCTURES
\********************************************************/

// this structure holds a RGB triple in three unsigned bytes
typedef struct RGB_color_typ {
        unsigned char red;    // red   component of color 0-63
        unsigned char green;  // green component of color 0-63
        unsigned char blue;   // blue  component of color 0-63
        } RGB_color, *RGB_color_ptr;

// this structure holds an entire color palette
typedef struct RGB_palette_typ {
        int start_reg;   // index of the starting register that is save
        int end_reg;     // index of the ending registry that is saved
        RGB_color colors[256];   // the storage area for the palette
        } RGB_palette, *RGB_palette_ptr;

/********************************************************\
 * PROTOTYPES
\********************************************************/
void ExitProgram(char *msg);
void CenterString(char *String, int Row, int Color);
void DrawString(char *String, int Row, int Column, int Color);
void DrawChar(char Char, int Color);
void SetCursorPosition(int Row, int Column);
void Print_Char(int xc,int yc,char c,int color,int transparent);
void Print_String(int x,int y,int color, char *string,int transparent);
void Write_Pixel(int x,int y,int color);
int Read_Pixel(int x,int y);
void DetectVGA(void);
void Set_Graphics_Mode(int mode);
void Time_Delay(int clicks);
void Line_H(int x1,int x2,int y,int color);
void Line_V(int y1,int y2,int x,int color);
void Write_Color_Reg(int index, RGB_color_ptr color);
void Read_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette);
void Write_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette);
RGB_color_ptr Read_Color_Reg(int index, RGB_color_ptr color);
void Draw_Rectangle(int x1,int y1,int x2,int y2,int color);
void Fill_Screen(int color);
void Fill_Screen_Z(int color);
void Write_Pixel_Z(int x,int y,int color);
void Set_Mode_Z(void);

/********************************************************\
 * GLOBALS
\********************************************************/
extern unsigned char far *video_buffer;   // video ram byte ptr
extern unsigned char far *rom_char_set;   // rom characters 8x8
