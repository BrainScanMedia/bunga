/********************************************************\
 * MENU.C - Bunga game source code                      *
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
 * Include GameLib Headers
\********************************************************/
#include "header\keyboard.h"
#include "header\svga.h"
#include "header\pcx.h"
#include "header\timer.h"
#include "header\bunga.h"

/********************************************************\
 * Menu Parameters                                      *
\********************************************************/
int      MenuNumber = 1;     /* Default Menu Number is the first setting */
int      MenuIntro = 0;      /* Menu Intro Default Not Enabled */
int      choice;             /* Init the character variable for case */

/********************************************************\
 * Set Image Variables                                  *
\********************************************************/
/* Menu Button & Cat Sprite Images With Transparency */ sprite Menu_S1, Menu_S2, Menu_S3, Menu_S4, Menu_S5;
/* Menu Button & Cat Sprite Images With Transparency */ sprite MCata1, MCata2, MCata3;
/* Normal 320x200 PCX Images                         */ pcx_picture  image_pcxm, Intro00, Intro01, Intro02, MenuScreen;

/********************************************************\
 * Intro Info & Screens                                 *
\********************************************************/
void Show_Intro() {
   Show_PCX_FX("intro00.pcx", 0, 5);
   Show_PCX_FX("intro01.pcx", 0, 5);
   Show_PCX_FX("credits.pcx", 0, 5);
}

/********************************************************\
 * Main Menu Routine
\********************************************************/
void Show_Menu() {

   int index, MenuNumber = 1;
   Set_Graphics_Mode(GRAPHICS_MODE13);     /* Set the graphics mode to mode 13h */

   redrawmenu:

   Delete_Double_Buffer();
   Create_Double_Buffer(200);

   /*********************************************************************\
    * Load Button 1
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("menu_s1.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&Menu_S1,86,74,144,11,0,0,0,0,0,0);
   for (index=0; index<2; index++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&Menu_S1,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load Button 2
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("menu_s2.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&Menu_S2,86,87,144,11,0,0,0,0,0,0);
   for (index=0; index<2; index++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&Menu_S2,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load Button 3
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("menu_s3.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&Menu_S3,86,100,144,11,0,0,0,0,0,0);
   for (index=0; index<2; index++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&Menu_S3,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load Button 4
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("menu_s4.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&Menu_S4,86,113,144,11,0,0,0,0,0,0);
   for (index=0; index<2; index++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&Menu_S4,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load Button 5
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("menu_s5.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&Menu_S5,86,126,144,11,0,0,0,0,0,0);
   for (index=0; index<2; index++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&Menu_S5,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load Cat1 Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("mcat1.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&MCata1,15,120,48,27,0,0,0,0,0,0);
   for(index=0; index<5; index++)
       PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&MCata1,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load Cat2 Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcxm);
   PCX_Load("mcat2.pcx", (pcx_picture_ptr)&image_pcxm,1);
   Sprite_Init((sprite_ptr)&MCata2,250,120,48,27,0,0,0,0,0,0);
   for(index=0; index<5; index++)
       PCX_Get_Sprite((pcx_picture_ptr)&image_pcxm,(sprite_ptr)&MCata2,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcxm);
   /*********************************************************************/

   /*********************************************************************\
    * Load the Menu BackGround
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&MenuScreen);
   PCX_Load("menu.pcx",(pcx_picture_ptr)&MenuScreen,1);
   PCX_Copy_To_Buffer((pcx_picture_ptr)&MenuScreen,double_buffer);
   /*********************************************************************/

   /*********************************************************************\
    * scan under ant and worm before entering the event loop, this must be
    * done or else on the first cycle the "erase" function will draw garbage
   \*********************************************************************/
   /* Buttons */
   Sprite_Under((sprite_ptr)&Menu_S1,double_buffer);
   Sprite_Under((sprite_ptr)&Menu_S2,double_buffer);
   Sprite_Under((sprite_ptr)&Menu_S3,double_buffer);
   Sprite_Under((sprite_ptr)&Menu_S4,double_buffer);
   Sprite_Under((sprite_ptr)&Menu_S5,double_buffer);
   /* Cats */
   Sprite_Under((sprite_ptr)&MCata1,double_buffer);
   Sprite_Under((sprite_ptr)&MCata2,double_buffer);
   /*********************************************************************/

   menuloop:

   PCX_Copy_To_Buffer((pcx_picture_ptr)&MenuScreen,double_buffer);

   for (; ;) {
    /*********************************************************************\
     * While waiting for a key press loop animation button
    \*********************************************************************/
    if(MenuNumber==1) {
       Sprite_Erase((sprite_ptr)&Menu_S1,double_buffer);
       if (++Menu_S1.curr_frame==2) { Menu_S1.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&Menu_S1,double_buffer);
       Sprite_Draw((sprite_ptr)&Menu_S1,double_buffer,1);
       Display_Double_Buffer(double_buffer,0); }
    /*********************************************************************/
    if(MenuNumber==2) {
       Sprite_Erase((sprite_ptr)&Menu_S2,double_buffer);
       if (++Menu_S2.curr_frame==2) { Menu_S2.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&Menu_S2,double_buffer);
       Sprite_Draw((sprite_ptr)&Menu_S2,double_buffer,1);
       Display_Double_Buffer(double_buffer,0); }
    /*********************************************************************/
    if(MenuNumber==3) {
       Sprite_Erase((sprite_ptr)&Menu_S3,double_buffer);
       if (++Menu_S3.curr_frame==2) { Menu_S3.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&Menu_S3,double_buffer);
       Sprite_Draw((sprite_ptr)&Menu_S3,double_buffer,1);
       Display_Double_Buffer(double_buffer,0); }
    /*********************************************************************/
    if(MenuNumber==4) {
       Sprite_Erase((sprite_ptr)&Menu_S4,double_buffer);
       if (++Menu_S4.curr_frame==2) { Menu_S4.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&Menu_S4,double_buffer);
       Sprite_Draw((sprite_ptr)&Menu_S4,double_buffer,1);
       Display_Double_Buffer(double_buffer,0); }
    /*********************************************************************/
    if(MenuNumber==5) {
       Sprite_Erase((sprite_ptr)&Menu_S5,double_buffer);
       if (++Menu_S5.curr_frame==2) { Menu_S5.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&Menu_S5,double_buffer);
       Sprite_Draw((sprite_ptr)&Menu_S5,double_buffer,1);
       Display_Double_Buffer(double_buffer,0); }

       /* Animate Cat */
       Sprite_Erase((sprite_ptr)&MCata1,double_buffer);
       if (++MCata1.curr_frame==5) { MCata1.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&MCata1,double_buffer);
       Sprite_Draw((sprite_ptr)&MCata1,double_buffer,1);
       Display_Double_Buffer(double_buffer,0);

       /* Animate Cat2 */
       Sprite_Erase((sprite_ptr)&MCata2,double_buffer);
       if (++MCata2.curr_frame==5) { MCata2.curr_frame = 0; }
       Sprite_Under((sprite_ptr)&MCata2,double_buffer);
       Sprite_Draw((sprite_ptr)&MCata2,double_buffer,1);
       Display_Double_Buffer(double_buffer,0);

    Time_Delay(2);

    /*********************************************************************\
     * Now check if a key was pressed
    \*********************************************************************/
    if (kbhit()) { choice = getch();
        /*****************************************************************/
        if (choice == KEY_UP) {   /* Menu Up Selection */
            MenuNumber--;         /* Decrease To Prev Menu Option */
                                  /* Redraw the background menu */
            PCX_Copy_To_Buffer((pcx_picture_ptr)&MenuScreen,double_buffer);
            if(MenuNumber<1)    { MenuNumber=5;  }
            goto menuloop; }      /* Return To Loop */
        /*****************************************************************/
        if (choice == KEY_DOWN) { /* Menu Down Selection */
            MenuNumber++;         /* Increase To Next Menu Option */
                                  /* Redraw the background menu */
            PCX_Copy_To_Buffer((pcx_picture_ptr)&MenuScreen,double_buffer);
            if(MenuNumber>5) { MenuNumber=1;  }
            goto menuloop; }      /* Return To Loop */
        /*****************************************************************/
        if (choice == 13) {    /* Menu Enter Selection */
                  if(MenuNumber==1) { PCX_FX_ENDING(4);
                                      PCX_Delete((pcx_picture_ptr)&MenuScreen);
                                      Sprite_Delete((sprite_ptr)&Menu_S1);
                                      Sprite_Delete((sprite_ptr)&Menu_S2);
                                      Sprite_Delete((sprite_ptr)&Menu_S3);
                                      Sprite_Delete((sprite_ptr)&Menu_S4);
                                      Sprite_Delete((sprite_ptr)&Menu_S5);
                                      Sprite_Delete((sprite_ptr)&MCata1);
                                      Sprite_Delete((sprite_ptr)&MCata2);
                                      Delete_Double_Buffer();
                                      break; }
                  if(MenuNumber==2) { PCX_FX_ENDING(4);
                                      Delete_Double_Buffer();
                                      Show_PCX_FX("options.pcx", 2, 10);
                                      Delete_Double_Buffer();
                                      goto redrawmenu; }
                  if(MenuNumber==3) { PCX_FX_ENDING(4);
                                      Delete_Double_Buffer();
                                      Show_PCX_FX("scorep.pcx", 2, 10);
                                      Delete_Double_Buffer();
                                      goto redrawmenu; }
                  if(MenuNumber==4) { PCX_FX_ENDING(4);
                                      Delete_Double_Buffer();
                                      Show_PCX_FX("credits.pcx", 3, 10);
                                      Delete_Double_Buffer();
                                      goto redrawmenu; }
                  if(MenuNumber==5) { /* Quit */
                                      PCX_FX_ENDING(4);
                                      Delete_Double_Buffer();
                                      PCX_Delete((pcx_picture_ptr)&MenuScreen);
                                      Sprite_Delete((sprite_ptr)&Menu_S1);
                                      Sprite_Delete((sprite_ptr)&Menu_S2);
                                      Sprite_Delete((sprite_ptr)&Menu_S3);
                                      Sprite_Delete((sprite_ptr)&Menu_S4);
                                      Sprite_Delete((sprite_ptr)&Menu_S5);
                                      Sprite_Delete((sprite_ptr)&MCata1);
                                      Sprite_Delete((sprite_ptr)&MCata2);
                                      Delete_Double_Buffer();
                                      Set_Graphics_Mode(TEXT_MODE);
                                      exit(0); } }
        /*****************************************************************/
        if (choice == KEY_ESC) {      /* Menu ESC Selection */
                                      Delete_Double_Buffer();
                                      PCX_Delete((pcx_picture_ptr)&MenuScreen);
                                      Sprite_Delete((sprite_ptr)&Menu_S1);
                                      Sprite_Delete((sprite_ptr)&Menu_S2);
                                      Sprite_Delete((sprite_ptr)&Menu_S3);
                                      Sprite_Delete((sprite_ptr)&Menu_S4);
                                      Sprite_Delete((sprite_ptr)&Menu_S5);
                                      Sprite_Delete((sprite_ptr)&MCata1);
                                      Sprite_Delete((sprite_ptr)&MCata2);
                                      Delete_Double_Buffer();
                                      Set_Graphics_Mode(TEXT_MODE);
                                      exit(0); }
        /*****************************************************************/
        else { goto menuloop; }       /* Default Loop */
    }
   }
}


