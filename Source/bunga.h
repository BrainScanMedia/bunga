/********************************************************\
 * BUNGA.H - Bunga game source code                     *
 * Compiled using Borland C++ 5.02                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

#define null 0
#define BUFSIZE 512
#define BOOL char

/********************************************************\
 * Make All Function Callable                           *
\********************************************************/
void Show_Intro(void);
void Show_Menu(void);
void Load_Level_1(void);
void DrawScore(void);
void DrawLifes(void);
void Level_1_Status(void);
void Load_Images();
void Draw_Tiles_And_Objects(int Obj, int X, int Y);
void Pause(int tim);