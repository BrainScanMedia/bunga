/********************************************************\
 * TIMER.C - Bunga game source code                     *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

/* Description:
   Routines for programming and manipulating the 8253/8254 timer
   chip and generally fiddling with the PC's timing system. */

#include <assert.h>
#include <dos.h>
#include "header\timer.h"

/* system timer stuff */
#define SYS_TIMER_INT   0x08

/* constants dealing with the BIOS & DOS clock routines */
#define CLOCK_INT               0x1A
#define READ_RTC_DATE   4
#define READ_RTC_TIME   2
#define DOS_INT                 0x21
#define SET_DOS_DATE    0x2B
#define SET_DOS_TIME    0x2D

/* MACROS */
#define GET_COUNTER(Mode)       ((Mode & SELECT_MASK) >> 6)
#define GET_RWL(Mode)   ((Mode & RWL_MASK) >> 4)

/****************************************************************************
	Function: ProgramTimer
     Description:
	Programs a timer with the mode and count specified.  The
	routine understands writing only the MSB or LSB and does
	the right thing.  Don't set the mode byte to latch
	counters or do the read-back command.
****************************************************************************/
void ProgramTimer(UINT8 Mode, UINT16 Count)
{
	int Timer;
    int Bytes;

    Timer = GET_COUNTER(Mode);
    Bytes = GET_RWL(Mode);

    assert(Timer != 0x3); /* catch read-back command */

    asm pushf;
    asm cli; /* turn off interrupts just so nobody messes with us */

    /* output mode byte */
    outportb(COUNTER_MODE, Mode);

    /* write count */
    switch (Bytes) {
	case 1: /* write MSB */
			outportb(COUNTER_0 + Timer, HIGHBYTE(Count));
	    break;
	case 2: /* write LSB */
		outportb(COUNTER_0 + Timer, LOWBYTE(Count));
	    break;
	case 3: /* write LSB, MSB */
		/* output in little endian order (i.e., LSB first) */
		outportb(COUNTER_0 + Timer, LOWBYTE(Count));
	    outportb(COUNTER_0 + Timer, HIGHBYTE(Count));
	    break;
	default:
		    assert(Bytes != 0x0); /* catch latch command */
	    break;
    }

    asm popf; /* interrupts back on if they were previously on */
}

/****************************************************************************
	Function: ReadTimerCount
     Description:
	Given a timer number as input, the routine returns the current
	count.
****************************************************************************/
UINT16 ReadTimerCount(int Timer)
{
	UINT16 Count;
    UINT8 Mode;

    Mode = (Timer << 6) | LATCH_CNTR; /* mode and type are don't cares */

    outportb(COUNTER_MODE, Mode);

    Count = inportb(COUNTER_0 + Timer); /* get LSB */
    Count |= inportb(COUNTER_0 + Timer) << 8; /* get MSB */

    return Count;
}

/****************************************************************************
	Function: HookAndProgramSysTimer
     Description:
	Redirects the system timer and programs it with a new count.
	Note that simply redirecting the timer without reprogramming
	it can be done with the setvect command.  The routine
	returns the previous system timer vector.  BE SURE TO
	RESTORE THE TIMER INTERRUPT USING THIS VECTOR BEFORE THE
	PROGRAM EXITS!!
****************************************************************************/
void far interrupt (*HookAndProgramSysTimer
	(
	void far interrupt (*Address)(),
    UINT8 Mode,
	UINT16 Count
	))()
{
	void far interrupt (*RetVal)();

    Mode &= 0x3F; /* make sure we're setting counter 0 */

    assert(GET_RWL(Mode) != 0); /* catch latch command */

    RetVal = getvect(SYS_TIMER_INT);
    ProgramTimer(Mode, Count);
    setvect(SYS_TIMER_INT, Address);

    return RetVal;
}

/****************************************************************************
	Function: RestoreSysTimer
     Description:
	Restores the system timer to its original settings.  Call
	this function after reprogramming the system timer but
	before quitting the application.
****************************************************************************/
void RestoreSysTimer(void)
{
	ProgramTimer(SELECT_0 | RW_LSB_MSB | MODE_SQUARE_WAVE | TYPE_BINARY, 0);
}

/****************************************************************************
	Function: UnhookAndRestoreSysTimer
    Description:
	Undoes what HookAndProgramSysTimer does.
****************************************************************************/
void UnhookAndRestoreSysTimer(void far interrupt (*Address)())
{
    /* NOTE: set the vector first.  If this is not done, the */
    /*   user interrupt routine could still be called between the */
	/*   timer restoration and the vector restoration.  If this */
	/*   happened, the user interrupt could reinitialize the timer */
	/*   with the user-needed mode and count, clobbering the timer. */
    setvect(SYS_TIMER_INT, Address);
	RestoreSysTimer();
}

/****************************************************************************
	Function: BCD2Bin
     Description:
	Module local function used in RestoreDOSTime to convert the
	BCD number format returned from the real-time clock to
	binary.  Argument is a single byte.
****************************************************************************/
static int BCD2Bin(UINT8 Arg)
{
    return (((Arg >> 4) & 0xF) * 10) + (Arg & 0xF);
}

/****************************************************************************
	Function: RestoreDOSTime
     Description:
	Restores the DOS time-of-day counter from the real-time clock.
	This alleviates the burden on the programmer to have to call
	the system timer vector (the old INT 8 vector) 18.2 times per
	second when the vector has been hooked for some reason.  This
	is particularly useful if the system timer rate has been
	reprogrammed to a non-integer multiple of the 18.2 Hz rate.
****************************************************************************/
void RestoreDOSTime(void)
{
	union REGS Regs;
    UINT8 Century;
    UINT8 Year;
    UINT8 Month;
    UINT8 Day;
    UINT8 Hours;
    UINT8 Mins;
    UINT8 Secs;

	/* NOTE: be sure to read both date and time from the RTC before */
	/*   setting the DOS date or time.  DOS will set the RTC date */
	/*   AND time whenever either DOS set routine is called (from
	/*   its stored copies).  This will temporarily corrupt the RTC */
	/*   date or time (whichever is not being set) so we have to make */
	/*   a copy for ourselves first so we can set the second one */
	/*   from the correct information. */

    /* get RTC date */
    Regs.h.ah = READ_RTC_DATE;
    int86(CLOCK_INT, &Regs, &Regs);

    Century     = Regs.h.ch;
	Year    = Regs.h.cl;
    Month       = Regs.h.dh;
    Day         = Regs.h.dl;

    /* get RTC time */
    Regs.h.ah = READ_RTC_TIME;
    int86(CLOCK_INT, &Regs, &Regs);

    Hours       = Regs.h.ch;
    Mins        = Regs.h.cl;
    Secs        = Regs.h.dh;

    /* set DOS date */
    Regs.x.cx = BCD2Bin(Century) * 100 + BCD2Bin(Year);
    Regs.h.dh = BCD2Bin(Month);
    Regs.h.dl = BCD2Bin(Day);
    Regs.h.ah = SET_DOS_DATE;
    int86(DOS_INT, &Regs, &Regs);

    /* set DOS time */
	Regs.h.ch = BCD2Bin(Hours);
    Regs.h.cl = BCD2Bin(Mins);
    Regs.h.dh = BCD2Bin(Secs);
    Regs.h.dl = 0; /* hunredths */
    Regs.h.ah = SET_DOS_TIME;
    int86(DOS_INT, &Regs, &Regs);
}

/****************************************************************************
 Function:    ClockInterrupt
 Description: Simply increments the TickCount variable every 1/70th of
			a second when MidPak is not loaded.
****************************************************************************/
long  TickCount;
void interrupt ClockInterrupt(void)
{
	 TickCount++;
	 outportb(PIC, NONSPECIFIC_EOI);
}

/****************************************************************************
 Function:    PC_SysTimer1
 Description: Normal Loop Timer, Slow down to PC speed.
****************************************************************************/
void PC_SysTimer(int time) {
   int i=1;
   for(i = 1; i<time; i++) {
    HookAndProgramSysTimer(ClockInterrupt, SELECT_0 | RW_LSB_MSB | MODE_SQUARE_WAVE | TYPE_BINARY,	17042);
   }
}
