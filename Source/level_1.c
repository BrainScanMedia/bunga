/********************************************************\
 * LEVEL_1.C - Bunga game source code                   *
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

#include "header\level_1.h"

pcx_picture image_pcx;  // general PCX image used to load background and imagery
pcx_picture Level01_BackGround;  // general PCX image used to load background and imagery

void Move_Objects_MenuCat() {
    /*********************************************************************\
    * Do Cat Animations
   \*********************************************************************/
   /* Cat Animation On Game Panel */
   CatCount++;
   if(CatCount>=200) { CatAnim++; CatCount=0; }
   if(CatAnim==5) {
      /* Animate Cat3 */
      Sprite_Erase((sprite_ptr)&MCat3,double_buffer);
      if (++MCat3.curr_frame==5) { MCat3.curr_frame = 0; }
      Sprite_Under((sprite_ptr)&MCat3,double_buffer);
      Sprite_Draw((sprite_ptr)&MCat3,double_buffer,1);
      Display_Double_Buffer(double_buffer,0);
      CatAnim = 0;
      }
   /*********************************************************************/
}

/********************************************************\
 * Level 1 Main
\********************************************************/
void Level1() {

   Load_Level_1();                         /* Load Level 1 And Draw Board & Score */

   /*********************************************************************\
    * Load Cat3 Tile Blocks for Board Panel
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("mcat3.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&MCat3,480,365,48,27,0,0,0,0,0,0);
   for(index=0; index<5; index++)
       PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&MCat3,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/
   Sprite_Under((sprite_ptr)&MCat3,double_buffer);

   SetButtonKeysMode();

   for (;;) {

   PC_SysTimer(3);
   Move_Objects_MenuCat();

     /*********************************************************************/
     /* Pan Board Right                                                   */
     /*********************************************************************/
     if(GetKeyState(KEY_RIGHT)){
        RePaintBackTiles=1; // Repaint the back tiles
        scroll_position++;
        /*******************************/
        /**  Handle Boy Heading        */
        /*******************************/
        /**/ Boy_Heading = 0; /* Going Right */
        /**/ if (++boy.curr_frame>=3){ boy.curr_frame = 0;}
        /*******************************/
        if( LEVEL==1 ) { Level_1_Status(); }
        Boy_Heading=2; // Reset the boy to null
        }
     /*********************************************************************/
     /* Pan Board Left                                                    */
     /*********************************************************************/
     if(GetKeyState(KEY_LEFT)){
        RePaintBackTiles=1; // Repaint the back tiles
        scroll_position--;
        /*******************************/
        /**  Handle Boy Heading        */
        /*******************************/
        /**/ Boy_Heading = 1; /* Going Left */
        /**/ if (boy.curr_frame<5){  boy.curr_frame = 5;}
        /**/ if (++boy.curr_frame>=7){ boy.curr_frame = 5;}
        /*******************************/
        if( LEVEL==1 ) { Level_1_Status(); }
        Boy_Heading=2; // Reset the boy to null
        }
     /*********************************************************************/
     /* Escape                                                            */
     /*********************************************************************/
     if(GetKeyState(KEY_ESC)){  /* Menu ESC Selection */
        SetNormalKeysMode();
        Set_Graphics_Mode(TEXT_MODE);
        RestoreSysTimer();
        exit(0); }
     /*********************************************************************/
     /* Jump                                                              */
     /*********************************************************************/
     if(GetKeyState(KEY_ALT)){  /* Jump */
        JumpStatus=1;
        if( LEVEL==1 ) { Level_1_Status(); }
        }
     /*********************************************************************/
     /* Test Score = ++                                                   */
     /*********************************************************************/
     if(GetKeyState(KEY_S)){
        Score++;            /* Increase Score */
        DrawScore();        /* Process The Score Graphics */
        }             /* Return To Loop */
     /*********************************************************************/
     /* Test Lives = ++                                                   */
     /*********************************************************************/
     if(GetKeyState(KEY_L)){
        Lifes++;            /* Increase Score */
        DrawLifes();       /* Process The Score Graphics */
        }            /* Return To Loop */
     /*********************************************************************/
     /* Test Lives = ++                                                   */
     /*********************************************************************/
     if(GetKeyState(KEY_LSHIFT)){
        Lifes--;            /* Decrease Score */
        DrawLifes();       /* Process The Score Graphics */
        }            /* Return To Loop */
     }
}

/********************************************************\
 * Draw Score Routine                                   *
\********************************************************/
void DrawScore() {
   itoa(Score, ScoreText, 10);        /* Change int to string */

   for (ic=0; ic<6; ic++) {           /* Quickly find length */
   if (ScoreText[ic] == '\0') { break; } }

   j=64;

   while(ic<6) {                      /* Draw leading zeros */
         Number0.y = 156, Number0.x = j; Number0.curr_frame = 0;
         Sprite_Draw((sprite_ptr)&Number0,double_buffer,1);
         ic++; j=j+14; }

  for(ic=0; ScoreText[ic]!='\0'; ic++) { /* Draw The Numbers To Screen */
      if(ScoreText[ic]=='0') {
         Number0.y = 156, Number0.x = j; Number0.curr_frame = 0;
         Sprite_Draw((sprite_ptr)&Number0,double_buffer,1);
      }
      if(ScoreText[ic]=='1') {
         Number1.y = 156, Number1.x = j; Number1.curr_frame = 1;
         Sprite_Draw((sprite_ptr)&Number1,double_buffer,1);
      }
      if(ScoreText[ic]=='2') {
         Number2.y = 156, Number2.x = j; Number2.curr_frame = 2;
         Sprite_Draw((sprite_ptr)&Number2,double_buffer,1);
      }
      if(ScoreText[ic]=='3') {
         Number3.y = 156, Number3.x = j; Number3.curr_frame = 3;
         Sprite_Draw((sprite_ptr)&Number3,double_buffer,1);
      }
      if(ScoreText[ic]=='4') {
         Number4.y = 156, Number4.x = j; Number4.curr_frame = 4;
         Sprite_Draw((sprite_ptr)&Number4,double_buffer,1);
      }
      if(ScoreText[ic]=='5') {
         Number5.y = 156, Number5.x = j; Number5.curr_frame = 5;
         Sprite_Draw((sprite_ptr)&Number5,double_buffer,1);
      }
      if(ScoreText[ic]=='6') {
         Number6.y = 156, Number6.x = j; Number6.curr_frame = 6;
         Sprite_Draw((sprite_ptr)&Number6,double_buffer,1);
      }
      if(ScoreText[ic]=='7') {
         Number7.y = 156, Number7.x = j; Number7.curr_frame = 7;
         Sprite_Draw((sprite_ptr)&Number7,double_buffer,1);
      }
      if(ScoreText[ic]=='8') {
         Number8.y = 156, Number8.x = j; Number8.curr_frame = 8;
         Sprite_Draw((sprite_ptr)&Number8,double_buffer,1);
      }
      if(ScoreText[ic]=='9') {
         Number9.y = 156, Number9.x = j; Number9.curr_frame = 9;
         Sprite_Draw((sprite_ptr)&Number9,double_buffer,1);
      }
      j=j+14; }
      Display_Double_Buffer(double_buffer,0);
}

/********************************************************\
 * Draw Lives Routine                                   *
\********************************************************/
void DrawLifes() {

   if(Lifes>=99) { Lifes = 99; }      /* If lives hit 99, stay 99 */
   if(Lifes<=0) { Lifes = 0; }        /* If lives hit 0, Stay 0 and your dead */
   itoa(Lifes, LifesText, 10);        /* Change int to string */

   for(icc=0; icc<2; icc++) {        /* Quickly find length */
       if (LifesText[icc] == '\0') { break; }
   }

   jc=64;

   while(icc<2) {                    /* Draw leading zeros */
         Number0.y = 177, Number0.x = jc; Number0.curr_frame = 0;
         Sprite_Draw((sprite_ptr)&Number0,double_buffer,1);
         icc++; jc=jc+14; }

   for(icc=0; LifesText[icc]!='\0'; icc++) { /* Draw The Numbers To Screen */
       if(LifesText[icc]=='0') {
          Number0.y = 177, Number0.x = jc; Number0.curr_frame = 0;
          Sprite_Draw((sprite_ptr)&Number0,double_buffer,1);
          }
       if(LifesText[icc]=='1') {
          Number1.y = 177, Number1.x = jc; Number1.curr_frame = 1;
          Sprite_Draw((sprite_ptr)&Number1,double_buffer,1);
          }
       if(LifesText[icc]=='2') {
          Number2.y = 177, Number2.x = jc; Number2.curr_frame = 2;
          Sprite_Draw((sprite_ptr)&Number2,double_buffer,1);
          }
       if(LifesText[icc]=='3') {
          Number3.y = 177, Number3.x = jc; Number3.curr_frame = 3;
          Sprite_Draw((sprite_ptr)&Number3,double_buffer,1);
          }
       if(LifesText[icc]=='4') {
          Number4.y = 177, Number4.x = jc; Number4.curr_frame = 4;
          Sprite_Draw((sprite_ptr)&Number4,double_buffer,1);
          }
       if(LifesText[icc]=='5') {
          Number5.y = 177, Number5.x = jc; Number5.curr_frame = 5;
          Sprite_Draw((sprite_ptr)&Number5,double_buffer,1);
          }
       if(LifesText[icc]=='6') {
          Number6.y = 177, Number6.x = jc; Number6.curr_frame = 6;
          Sprite_Draw((sprite_ptr)&Number6,double_buffer,1);
          }
       if(LifesText[icc]=='7') {
          Number7.y = 177, Number7.x = jc; Number7.curr_frame = 7;
          Sprite_Draw((sprite_ptr)&Number7,double_buffer,1);
          }
       if(LifesText[icc]=='8') {
          Number8.y = 177, Number8.x = jc; Number8.curr_frame = 8;
          Sprite_Draw((sprite_ptr)&Number8,double_buffer,1);
          }
       if(LifesText[icc]=='9') {
          Number9.y = 177, Number9.x = jc; Number9.curr_frame = 9;
          Sprite_Draw((sprite_ptr)&Number9,double_buffer,1);
          }
       jc=jc+14; }
       Display_Double_Buffer(double_buffer,0);
}

/********************************************************\
 * Load Level 1 Graphics & Draw First Map               *
\********************************************************/
void Load_Level_1() {

   Create_Double_Buffer(200);
   Set_Graphics_Mode(GRAPHICS_MODE13);     /* Set the graphics mode to mode 13h */

   Show_PCX_NORM("loading.pcx");
   Time_Delay(25);
   PCX_FX_ENDING(4);                       /* Fade out the screen with FX */

   Load_Images();

   PCX_Copy_To_Buffer((pcx_picture_ptr)&Level01_BackGround,double_buffer);
   PCX_Delete((pcx_picture_ptr)&Level01_BackGround);

   DrawScore();                       /* Draw Score To Screen */
   DrawLifes();                       /* Draw Lives Score     */

   sprintf(MAPSIZE, "%d", strlen(BOARD01_LAYER08)); /* Start At Floor */
   SIZE = atoi(MAPSIZE);                   /* Calculate Size of Board */

   /*******************************************************************\
    * Do The Background Layers
   \*******************************************************************/
   if ((level01_layer1_x+=1) >= 319) { level01_layer1_x -= 320; }
   if ((level01_layer2_x+=3) >= 319)  { level01_layer2_x -= 320; }
   Layer_Draw((layer_ptr)&level01_layer1,level01_layer1_x,0,
               double_buffer,LEVEL01_LAYER1_Y,LEVEL01_LAYER1_HEIGHT,0);
   Layer_Draw((layer_ptr)&level01_layer2,level01_layer2_x,0,
               double_buffer,LEVEL01_LAYER2_Y,LEVEL01_LAYER2_HEIGHT,0);
   /*******************************************************************/

   /*******************************************************************/
   /* Do The Tile Layer 1                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=0;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER01[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 2                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=16;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER02[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 3                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=32;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER03[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 4                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=48;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER04[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 5                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=64;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER05[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 6                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=80;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER06[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 7                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=96;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER07[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 8                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=112;     /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER08[i], L1X, L1Y);
        L1X=L1X+16; }
   /*******************************************************************/
   /* Do The Tile Layer 9                                             */
   /*******************************************************************/
   scroll_position=0; L1X=0; L1Y=128;     /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER09[i], L1X, L1Y);
        L1X=L1X+16; }

   boy.y = 90; boy.x = 10;
   boy.curr_frame = 3;
   Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
   Sprite_Draw_Clip((sprite_ptr)&boy,double_buffer,1);
   boy.curr_frame = 0;
   Display_Double_Buffer(double_buffer,0);
}

/********************************************************\
 * Handle Level 1 Map                                   *
\********************************************************/
void Level_1_Status() {

   if(scroll_position>=SIZE-20) { scroll_position = SIZE-20; }
   if(scroll_position<=0) { scroll_position = 0; }

   /****************************************************/
   /* Handle The Boy Bitmap                            */
   /****************************************************/
   /* Boy Going Right */
   if(Boy_Heading == 0) {

      /* If Boy Reaches End Of Board */
      if(scroll_position>=SIZE-20) {
         boy.x+=6;
         if(boy.x>=scroll_position+250) { boy.x=scroll_position+250; goto Do_Nothing;}
         Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
         Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
         Layer_Draw((layer_ptr)&level01_layer1,level01_layer1_x,0,
                     double_buffer,LEVEL01_LAYER1_Y,LEVEL01_LAYER1_HEIGHT,0);
         Layer_Draw((layer_ptr)&level01_layer2,level01_layer2_x,0,
                     double_buffer,LEVEL01_LAYER2_Y,LEVEL01_LAYER2_HEIGHT,0);
         goto Skip_Background; }

      /* Calculate if boy is still at the beginning of board */
      if((boy.x+=6) > 150) { boy.x = 150; }
      else { scroll_position=0; boy.y = 90;
             Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
             Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
             goto boy; }
   }

   /* Boy Going Left */
   if(Boy_Heading == 1) {

      /* If Boy Reaches End Of Board but decides to turn back to middle of screen */
//      if(scroll_position>=SIZE-20) {
//         boy.x=210;
//         Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
//         Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
//         Layer_Draw((layer_ptr)&level01_layer1,level01_layer1_x,0,
//                    double_buffer,LEVEL01_LAYER1_Y,LEVEL01_LAYER1_HEIGHT,0);
//         Layer_Draw((layer_ptr)&level01_layer2,level01_layer2_x,0,
//                    double_buffer,LEVEL01_LAYER2_Y,LEVEL01_LAYER2_HEIGHT,0);
//         goto Skip_Background; }

      /* If The Boy Reaches The Beginning Of The Board */
      if(scroll_position<=0) { scroll_position=0;
         if(boy.x<=10) { goto Do_Nothing; }
         else { boy.x-=6;
                Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
                Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
                Layer_Draw((layer_ptr)&level01_layer1,level01_layer1_x,0,
                            double_buffer,LEVEL01_LAYER1_Y,LEVEL01_LAYER1_HEIGHT,0);
                Layer_Draw((layer_ptr)&level01_layer2,level01_layer2_x,0,
                            double_buffer,LEVEL01_LAYER2_Y,LEVEL01_LAYER2_HEIGHT,0);
                goto Skip_Background; } } }

   /***************************************************************************/
   /* If Boy Jumps */
   /***************************************************************************/
   if(JumpStatus==1) {
      int ijump=0;
      for(ijump; ijump<=48; ijump++) {
          boy.y=ijump;
          Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
          Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
          Sprite_Draw_Clip((sprite_ptr)&boy,double_buffer,1);
          }
      for(ijump; ijump>90; ijump--) {
          boy.y=ijump;
          Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
          Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
          Sprite_Draw_Clip((sprite_ptr)&boy,double_buffer,1);
          }
      //boy.y=90;
      //Sprite_Erase_Clip((sprite_ptr)&boy,double_buffer);
      //Sprite_Under_Clip((sprite_ptr)&boy,double_buffer);
      //Sprite_Draw_Clip((sprite_ptr)&boy,double_buffer,1);
      //JumpStatus=0;
      }

   /****************************************************/
   if(RePaintBackTiles==1) {
      /*******************************************************************\
       * Do The Background Layers
      \*******************************************************************/
      if(Boy_Heading==0){ /* If boy is going right */
      if ((level01_layer1_x+=1) >= 319) { level01_layer1_x -= 320; }
      if ((level01_layer2_x+=3) >= 319)  { level01_layer2_x -= 320; }
           Layer_Draw((layer_ptr)&level01_layer1,level01_layer1_x,0,
                      double_buffer,LEVEL01_LAYER1_Y,LEVEL01_LAYER1_HEIGHT,0);
           Layer_Draw((layer_ptr)&level01_layer2,level01_layer2_x,0,
                      double_buffer,LEVEL01_LAYER2_Y,LEVEL01_LAYER2_HEIGHT,0);
      /*******************************************************************/
      } else { if ((level01_layer1_x-=1) <= 0) { level01_layer1_x += 320; }
               if ((level01_layer2_x-=3) <= 0) { level01_layer2_x += 320; }
                   Layer_Draw((layer_ptr)&level01_layer1,level01_layer1_x,0,
                              double_buffer,LEVEL01_LAYER1_Y,LEVEL01_LAYER1_HEIGHT,0);
                   Layer_Draw((layer_ptr)&level01_layer2,level01_layer2_x,0,
                              double_buffer,LEVEL01_LAYER2_Y,LEVEL01_LAYER2_HEIGHT,0); }
      /*******************************************************************/
      Skip_Background:
   }

   /*******************************************************************/
   /* Handle The Board Tiles Drawing                                  */
   /*******************************************************************/
   /* Do The Tile Layer 1 */
   L1X=0; L1Y=0;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER01[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 2 */
   L1X=0; L1Y=16;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER02[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 3 */
   L1X=0; L1Y=32;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER03[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 4 */
   L1X=0; L1Y=48;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER04[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 5 */
   L1X=0; L1Y=64;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER05[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 6 */
   L1X=0; L1Y=80;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER06[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 7 */
   L1X=0; L1Y=96;       /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER07[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 8 */
   L1X=0; L1Y=112;     /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER08[i], L1X, L1Y);
        L1X=L1X+16; }
   /* Do The Tile Layer 9 */
   L1X=0; L1Y=128;     /* Draw The First Floor Map*/
   for(i = scroll_position; i<(scroll_position+map_width); i++){
        Draw_Tiles_And_Objects(BOARD01_LAYER09[i], L1X, L1Y);
        L1X=L1X+16; }

   /* Handle Drawing The Boy to Screen */
   boy: Sprite_Draw_Clip((sprite_ptr)&boy,double_buffer,1);

   Do_Nothing:
   Display_Double_Buffer(double_buffer,0);

   PC_SysTimer(2500);

}

void Load_Images(){
   int index,choice;
   /*********************************************************************\
    * Load Score Number Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("score.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Number0,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number1,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number2,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number3,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number4,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number5,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number6,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number7,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number8,64,156,14,13,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Number9,64,156,14,13,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number0,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number1,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number2,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number3,index,index,0);
   index = 4; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number4,index,index,0);
   index = 5; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number5,index,index,0);
   index = 6; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number6,index,index,0);
   index = 7; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number7,index,index,0);
   index = 8; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number8,index,index,0);
   index = 9; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Number9,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Clouds Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("clouds.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Cloud0,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Cloud1,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Cloud2,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Cloud3,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Cloud4,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&BSky,0,0,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Cloud0,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Cloud1,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Cloud2,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Cloud3,index,index,0);
   index = 4; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Cloud4,index,index,0);
   index = 5; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&BSky,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Water Floor Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("water.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Water0,0,1,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Water1,0,1,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Water0,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Water1,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Stone Floor Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("floor_00.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Floor_00_01,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_00_02,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_00_03,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_00_04,0,0,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_00_01,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_00_02,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_00_03,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_00_04,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Grass Floor Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("floor_01.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Floor_01_01,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_01_02,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_01_03,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_01_04,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_01_05,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_01_06,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_01_07,0,0,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_01,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_02,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_03,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_04,index,index,0);
   index = 4; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_05,index,index,0);
   index = 5; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_06,index,index,0);
   index = 6; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_01_07,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Grass-stone Floor Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("floor_02.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Floor_02_01,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_02_02,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_02_03,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Floor_02_04,0,0,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_02_01,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_02_02,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_02_03,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Floor_02_04,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Poll Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("polls.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Poll_B_L,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Poll_B_R,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Poll_T_L0,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Poll_T_R0,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Poll_T_L1,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Poll_T_R1,0,0,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Poll_B_L,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Poll_B_R,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Poll_T_L0,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Poll_T_R0,index,index,0);
   index = 4; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Poll_T_L1,index,index,0);
   index = 5; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Poll_T_R1,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Items
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("objects.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&Object01,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Object02,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Object03,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Object04,0,0,16,16,0,0,0,0,0,0);
   Sprite_Init((sprite_ptr)&Object05,0,0,16,16,0,0,0,0,0,0);
   index = 0; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Object01,index,index,0);
   index = 1; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Object02,index,index,0);
   index = 2; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Object03,index,index,0);
   index = 3; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Object04,index,index,0);
   index = 4; PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&Object05,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load Boy Tile Blocks
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("boy.pcx", (pcx_picture_ptr)&image_pcx,1);
   Sprite_Init((sprite_ptr)&boy,0,0,21,25,0,0,0,0,0,0);
   for (index=0; index<7; index++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&boy,index,index,0);
   PCX_Delete((pcx_picture_ptr)&image_pcx);
   /*********************************************************************/

   /*********************************************************************\
    * Load the Level01 BackGround
   \*********************************************************************/
   PCX_Init((pcx_picture_ptr)&Level01_BackGround);
   PCX_Load("level1b.pcx",(pcx_picture_ptr)&Level01_BackGround,1);
   PCX_Copy_To_Buffer((pcx_picture_ptr)&Level01_BackGround,double_buffer);
   /*********************************************************************/

   /*********************************************************************\
    * Capture BackGround and convert to Layer Tiles
   \*********************************************************************/
   Layer_Create((layer_ptr)&level01_layer1,SCREEN_WIDTH,LEVEL01_LAYER1_HEIGHT);
   Layer_Create((layer_ptr)&level01_layer2,  SCREEN_WIDTH,LEVEL01_LAYER2_HEIGHT);
   Layer_Build((layer_ptr)&level01_layer1,0,0,
               double_buffer,0,LEVEL01_LAYER1_Y,SCREEN_WIDTH,LEVEL01_LAYER1_HEIGHT);
   Layer_Build((layer_ptr)&level01_layer2,0,0,
               double_buffer,0,LEVEL01_LAYER2_Y,SCREEN_WIDTH,LEVEL01_LAYER2_HEIGHT);
   /*********************************************************************/

   /*********************************************************************\
    * scan under tiles before entering the event loop, this must
    * be done or else on the first cycle the "erase" function will draw
    * garbage
   \*********************************************************************/
   /* Score Number Tiles */
   Sprite_Under((sprite_ptr)&Number0,double_buffer);
   Sprite_Under((sprite_ptr)&Number1,double_buffer);
   Sprite_Under((sprite_ptr)&Number2,double_buffer);
   Sprite_Under((sprite_ptr)&Number3,double_buffer);
   Sprite_Under((sprite_ptr)&Number4,double_buffer);
   Sprite_Under((sprite_ptr)&Number5,double_buffer);
   Sprite_Under((sprite_ptr)&Number6,double_buffer);
   Sprite_Under((sprite_ptr)&Number7,double_buffer);
   Sprite_Under((sprite_ptr)&Number8,double_buffer);
   Sprite_Under((sprite_ptr)&Number9,double_buffer);
   /* Water Tiles */
//   Sprite_Under((sprite_ptr)&Water0,double_buffer);
//   Sprite_Under((sprite_ptr)&Water1,double_buffer);
   /*********************************************************************/

}

void Draw_Tiles_And_Objects(int Obj, int X, int Y) {

       if(Obj=='.') { /* Do Nothing */
           BSky.y = Y, BSky.x = X; BSky.curr_frame = 5;
           Sprite_Draw_Clip((sprite_ptr)&BSky,double_buffer,1);
           }
       /***************************************************************/
       if(Obj=='$') { /* Draw Small Cloud Part 1-2 */
           Cloud0.y = L1Y, Cloud0.x = L1X; Cloud0.curr_frame = 0;
           Sprite_Draw_Clip((sprite_ptr)&Cloud0,double_buffer,1);
           }
       /***************************************************************/
       if(Obj=='%') { /* Draw Small Cloud Part 2-2 */
           Cloud1.y = L1Y, Cloud1.x = L1X; Cloud1.curr_frame = 1;
           Sprite_Draw_Clip((sprite_ptr)&Cloud1,double_buffer,1);
           }
       /***************************************************************/
       if(Obj=='!') { /* Draw Big Cloud Part 1-3 */
           Cloud2.y = L1Y, Cloud2.x = L1X; Cloud2.curr_frame = 2;
           Sprite_Draw_Clip((sprite_ptr)&Cloud2,double_buffer,1);
           }
       /***************************************************************/
       if(Obj=='@') { /* Draw Big Cloud Part 2-3 */
           Cloud3.y = L1Y, Cloud3.x = L1X; Cloud3.curr_frame = 3;
           Sprite_Draw_Clip((sprite_ptr)&Cloud3,double_buffer,1);
           }
       /***************************************************************/
       if(Obj=='#') { /* Draw Big Cloud Part 3-3 */
           Cloud4.y = L1Y, Cloud4.x = L1X; Cloud4.curr_frame = 4;
           Sprite_Draw_Clip((sprite_ptr)&Cloud4,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='1') { /* Draw Floor_01 Tile 1 */
           Floor_01_01.y = Y, Floor_01_01.x = X; Floor_01_01.curr_frame = 0;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_01,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='2') { /* Draw Floor_01 Tile 2 */
           Floor_01_02.y = Y, Floor_01_02.x = X; Floor_01_02.curr_frame = 1;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_02,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='3') { /* Draw Floor_01 Tile 3 */
           Floor_01_03.y = Y, Floor_01_03.x = X; Floor_01_03.curr_frame = 2;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_03,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='4') { /* Draw Floor_01 Tile 4 */
           Floor_01_04.y = Y, Floor_01_04.x = X; Floor_01_04.curr_frame = 3;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_04,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='5') { /* Draw Floor_01 Tile 5 */
           Floor_01_05.y = Y, Floor_01_05.x = X; Floor_01_05.curr_frame = 4;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_05,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='6') { /* Draw Floor_01 Tile 6 */
           Floor_01_06.y = Y, Floor_01_06.x = X; Floor_01_06.curr_frame = 5;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_06,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='7') { /* Draw Floor_01 Tile 7 */
           Floor_01_07.y = Y, Floor_01_07.x = X; Floor_01_07.curr_frame = 6;
           Sprite_Draw_Clip((sprite_ptr)&Floor_01_07,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='A') { /* Draw Poll Bottom Left */
           Poll_B_L.y = Y, Poll_B_L.x = X; Poll_B_L.curr_frame = 0;
           Sprite_Draw_Clip((sprite_ptr)&Poll_B_L,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='B') { /* Draw Poll Bottom Right */
           Poll_B_R.y = Y, Poll_B_R.x = X; Poll_B_R.curr_frame = 1;
           Sprite_Draw_Clip((sprite_ptr)&Poll_B_R,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='C') { /* Draw Poll Top Left Tile Set 0 */
           Poll_T_L0.y = Y, Poll_T_L0.x = X; Poll_T_L0.curr_frame = 2;
           Sprite_Draw_Clip((sprite_ptr)&Poll_T_L0,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='D') { /* Draw Poll Top Right Tile Set 0 */
           Poll_T_R0.y = Y, Poll_T_R0.x = X; Poll_T_R0.curr_frame = 3;
           Sprite_Draw_Clip((sprite_ptr)&Poll_T_R0,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='E') { /* Draw Poll Top Left Tile Set 1 */
           Poll_T_L1.y = Y, Poll_T_L1.x = X; Poll_T_L1.curr_frame = 4;
           Sprite_Draw_Clip((sprite_ptr)&Poll_T_L1,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='F') { /* Draw Poll Top Right Tile Set 1 */
           Poll_T_R1.y = L1Y, Poll_T_R1.x = L1X; Poll_T_R1.curr_frame = 5;
           Sprite_Draw_Clip((sprite_ptr)&Poll_T_R1,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='G') { /* Draw Grass 1 */
           Object01.y = L1Y, Object01.x = L1X; Object01.curr_frame = 0;
           Sprite_Draw_Clip((sprite_ptr)&Object01,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='H') { /* Draw Grass 2 */
           Object02.y = L1Y, Object02.x = L1X; Object02.curr_frame = 1;
           Sprite_Draw_Clip((sprite_ptr)&Object02,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='I') { /* Draw Red Flowers */
           Object03.y = L1Y, Object03.x = L1X; Object03.curr_frame = 2;
           Sprite_Draw_Clip((sprite_ptr)&Object03,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='J') { /* Draw Plant */
           Object04.y = L1Y, Object04.x = L1X; Object04.curr_frame = 3;
           Sprite_Draw_Clip((sprite_ptr)&Object04,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='K') { /* Draw Mushroom */
           Object05.y = L1Y, Object05.x = L1X; Object05.curr_frame = 4;
           Sprite_Draw_Clip((sprite_ptr)&Object05,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='S') { /* Draw Floor_02 Tile 1 */
           Floor_02_01.y = Y, Floor_02_01.x = X; Floor_02_01.curr_frame = 0;
           Sprite_Draw_Clip((sprite_ptr)&Floor_02_01,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='T') { /* Draw Floor_02 Tile 2 */
           Floor_02_02.y = Y, Floor_02_02.x = X; Floor_02_02.curr_frame = 1;
           Sprite_Draw_Clip((sprite_ptr)&Floor_02_02,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='U') { /* Draw Floor_02 Tile 3 */
           Floor_02_03.y = Y, Floor_02_03.x = X; Floor_02_03.curr_frame = 2;
           Sprite_Draw_Clip((sprite_ptr)&Floor_02_03,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='V') { /* Draw Floor_02 Tile 4 */
           Floor_02_04.y = Y, Floor_02_04.x = X; Floor_02_04.curr_frame = 3;
           Sprite_Draw_Clip((sprite_ptr)&Floor_02_04,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='W') { /* Draw Floor_00 Tile 1 */
           Floor_00_01.y = Y, Floor_00_01.x = X; Floor_00_01.curr_frame = 0;
           Sprite_Draw_Clip((sprite_ptr)&Floor_00_01,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='X') { /* Draw Floor_00 Tile 2 */
           Floor_00_02.y = Y, Floor_00_02.x = X; Floor_00_02.curr_frame = 1;
           Sprite_Draw_Clip((sprite_ptr)&Floor_00_02,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='Y') { /* Draw Floor_00 Tile 3 */
           Floor_00_03.y = Y, Floor_00_03.x = X; Floor_00_03.curr_frame = 2;
           Sprite_Draw_Clip((sprite_ptr)&Floor_00_03,double_buffer,1);
           }
       /***************************************************************/
        if(Obj=='Z') { /* Draw Floor_00 Tile 4 */
           Floor_00_04.y = Y, Floor_00_04.x = X; Floor_00_04.curr_frame = 3;
           Sprite_Draw_Clip((sprite_ptr)&Floor_00_04,double_buffer,1);
           }
       /***************************************************************/
         if(Obj=='~') { /* Draw Water Tile Animation */
            //if(WaterFrame<=1) { WaterFrame = 2; }
            //if(WaterFrame>=3) { WaterFrame = 1; }
            //if(WaterFrame==1){
               Water0.y = Y, Water0.x = X; Water0.curr_frame = 0;
               Sprite_Draw_Clip((sprite_ptr)&Water0,double_buffer,1); }
            //if(WaterFrame==2){
            //   Water1.y = Y, Water1.x = X; Water1.curr_frame = 1;
            //   Sprite_Draw_Clip((sprite_ptr)&Water1,double_buffer,1); }
            //   WaterFrame++; }
       /***************************************************************/

}

