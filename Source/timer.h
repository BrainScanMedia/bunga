/********************************************************\
 * TIMER.H - Bunga game source code                     *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

/* Description: Constants and declarations for use with TIMER.C. */

#ifndef _TIMER_H

typedef char            INT8;
typedef unsigned char   UINT8;
typedef int             INT16;
typedef unsigned int    UINT16;
typedef long            INT32;
typedef unsigned long   UINT32;

typedef unsigned int    WORD;
typedef unsigned long   LONG;

typedef int             BOOL;

#define TRUE (1)
#define FALSE (0)

#define DIM(x) (sizeof(x)/sizeof(x[0]))

#define LOWBYTE(x) ((x) & 0xFF)
#define HIGHBYTE(x) (((x) >> 8) & 0xFF)

/* just define these in terms of the Borland library macros */
#define MIN(a,b) min(a,b)
#define MAX(a,b) max(a,b)

/* interrupt acknowledgement stuff */
#define PIC (0x20)
#define NONSPECIFIC_EOI (0x20)

typedef struct {
	int Top;
    int Left;
    int Bottom;
    int Right;
} RECT;


/* CONSTANTS */
/* counter port addresses */
#define COUNTER_0               0x40
#define COUNTER_1               0x41
#define COUNTER_2               0x42
#define COUNTER_MODE    0x43

/* mode control bit definitions */
/* select bits (bits 7 & 6) */
#define SELECT_0                0x00
#define SELECT_1                0x40
#define SELECT_2                0x80
#define READ_BACK               0xC0
#define SELECT_MASK             0xC0

/* read/write/latch bits (bits 5 & 4) */
#define LATCH_CNTR              0x00
#define RW_MSB                  0x10
#define RW_LSB                  0x20
#define RW_LSB_MSB              0x30
#define RWL_MASK                0x30

/* counter mode bits (bits 3, 2, & 1) */
#define MODE_EVENT                      0x00
#define MODE_ONE_SHOT           0x02
#define MODE_DIV_N                      0x04
#define MODE_SQUARE_WAVE        0x06
#define MODE_SW_STROBE          0x08
#define MODE_HW_STROBE          0x0A
#define MODE_MASK                       0x0E

/* counter type bit (bit 0) */
#define TYPE_BINARY             0x00
#define TYPE_BCD                0x01
#define TYPE_MASK               0x01

void ProgramTimer(UINT8 Mode, UINT16 Count);
UINT16 ReadTimerCount(int Timer);
void far interrupt (*HookAndProgramSysTimer
	(
	void far interrupt (*Address)(),
    UINT8 Mode,
	UINT16 Count
	))();
void RestoreSysTimer(void);
void UnhookAndRestoreSysTimer(void far interrupt (*Address)());
void RestoreDOSTime(void);
void PC_SysTimer(int time);
void interrupt ClockInterrupt(void);

#define _TIMER_H

#endif
