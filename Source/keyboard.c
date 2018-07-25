/********************************************************\
 * KEYBOARD.C - Bunga game source code                  *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

/* Description:
   Defines routines for reading the state of the keyboard as
   a set of buttons. */

#include <assert.h>
#include <dos.h>

typedef char            INT8;
typedef unsigned char   UINT8;

#define PIC                     (0x20)
#define NONSPECIFIC_EOI         (0x20)

typedef struct {
    int Top;
    int Left;
    int Bottom;
    int Right;
}   RECT;

#include "header\keyboard.h"

#define MAX_SCANCODES           (128)
#define NEW_HANDLER             (1)
#define OLD_HANDLER             (0)
#define KEYBOARD_INPUT          (0x60)
#define KEYBOARD_XT_CLEAR       (0x61)

static void far interrupt (*OldInt9Handler)();
static int      KeyboardStateFlag = OLD_HANDLER;
static UINT8	KeyStateArray[MAX_SCANCODES];
static UINT8	WasPressedArray[MAX_SCANCODES];

static void far interrupt KeyIntHandler(void)
{
    UINT8 ScanCode;
    UINT8 Temp;

    /* read scan code */
    ScanCode = inportb(KEYBOARD_INPUT);

    /* clear keyboard controller on XT machines */
    Temp = inportb(KEYBOARD_XT_CLEAR);
    outportb(KEYBOARD_XT_CLEAR, Temp | 0x80);
    outportb(KEYBOARD_XT_CLEAR, Temp & 0x7F);

    if (ScanCode & 0x80) {
    	/* key up */
        ScanCode &= 0x7F;
        KeyStateArray[ScanCode] = 0;
    }
    else {
    	/* key down */
        KeyStateArray[ScanCode] = 1;
        WasPressedArray[ScanCode] = 1;
    }

    outportb(PIC, NONSPECIFIC_EOI);
}

void SetButtonKeysMode(void)
{
    int i;

    assert(KeyboardStateFlag == OLD_HANDLER);

    /* initialize state arrays */
    for (i = 0; i < 128; i++) {
      KeyStateArray[i] = WasPressedArray[i] = 0;
    }

    /* install new handler */
    OldInt9Handler = getvect(9);
    setvect(9, KeyIntHandler);

    /* mark new handler as being installed */
    KeyboardStateFlag = NEW_HANDLER;
}

void SetNormalKeysMode(void)
{
    /* if this is not true, there could be big problems at */
    /* program termination.  */
    assert(KeyboardStateFlag == NEW_HANDLER);

    /* reinstall original handler */
    setvect(9, OldInt9Handler);

    /* indicate that the old handler is current */
    KeyboardStateFlag = OLD_HANDLER;
}

int GetKeyState(int ScanCode)
{
    int result;

    /* scan codes should only be 0-127 */
    assert(ScanCode < 128);
    assert(KeyboardStateFlag == NEW_HANDLER);

    result = KeyStateArray[ScanCode] | WasPressedArray[ScanCode];
    WasPressedArray[ScanCode] = 0;
    return result;
}

int GetTrueKeyState(int ScanCode)
{
    int result;

    /* scan codes should only be 0-127 */
    assert(ScanCode < 128);
    assert(KeyboardStateFlag == NEW_HANDLER);

    result = KeyStateArray[ScanCode];
    return result;
}
