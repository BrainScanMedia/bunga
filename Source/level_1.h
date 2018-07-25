/********************************************************\
 * LEVEL_1.H - Bunga game source code                   *
 * Compiled using Borland C++ 5..2                      *
\********************************************************/

/********************************************************\
 * Developed & Programmed By Thomas J. Allen            *
 * OS: DOS   Req: 1MB RAM, 2MB HD, Sound Optional       *
\********************************************************/

void Move_Objects_MenuCat();
void Load_Images();

/********************************************************\
 * Game Parameters                                      *
\********************************************************/
int      index;              /* for loop counter index */
int      map_width=20;       /* Number of cells in map. */
int      screen_width=20;    /* Number of cells in map. */
int      scroll_position=0;  /* Position about to be drawn. */
int      last_position=0;    /* Position at last screen scroll. */
int      i=1;                /* Init the i for map character location */
int      L1Y, L1X;           /* Level 1 Locations */
char*    MAPSIZE;            /* Temp Char Board Size */
int      SIZE;               /* ATOI Converted Board Size */
char*    ScoreText;          /* Score 6 digits plus '\0' */
int long Score=0;            /* Default Score is 0       */
int      ic, j;              /* Needed by score function */
char     LifesText[3];       /* Lives 1 digits plus '\0' */
int      Lifes = 3;          /* Default Lives is 3       */
int      icc, jc;            /* Needed by Lives function */
int      LEVEL = 1;          /* Set the default level to 1 */
int      Cloud=10500;        /* Cloud Moment Counter Set default as move */
int      Clouds=0;           /* Clouds String Counter */
int      WaterFrame=0;       /* Needed for water Animation */
int      CatCount=0;         /* Needed for Animation Count stat on Game Panel */
int      CatAnim=0;          /* Needed for animation counter for cat on panel */
int      Boy_Heading=2;      /* Needed to tell level Direction of boy 0 = Going Right 1 = Going Left*/
int      RePaintBackTiles=0; /* Tell the engine if tiles should be moved */
int      JumpStatus=0;       /* Jumping Status*/

/********************************************************\
 * Define Board Background Layers                       *
\********************************************************/
#define LEVEL01_LAYER1_Y  0                /* the starting vertical position of layer 1 */
#define LEVEL01_LAYER2_Y  70               /* the starting vertical position of layer 2 */
#define LEVEL01_LAYER1_HEIGHT (1+180-106)  /* Hight of Layer 1 */
#define LEVEL01_LAYER2_HEIGHT (1+179-106)  /* Hight of Layer 2 */

/********************************************************\
 * Set Image Variables                                  *
\********************************************************/
/* Menu Button & Cat Sprite Images With Transparency */ sprite MCat1, MCat2, MCat3;
/* Score Sprites                                     */ sprite Number0, Number1, Number2, Number3, Number4, Number5, Number6, Number7, Number8, Number9;
/* Clouds Sprites                                    */ sprite BSky, Cloud0, Cloud1, Cloud2, Cloud3, Cloud4;
/* Water Floor Sprites                               */ sprite Water0, Water1, boy;
/* Polls                                             */ sprite Poll_B_L, Poll_B_R, Poll_T_L0, Poll_T_R0, Poll_T_L1, Poll_T_R1;
/* Objects & Items                                   */ sprite Object01, Object02, Object03, Object04, Object05;
/* Stone Floor Sprites                               */ sprite Floor_00_01, Floor_00_02, Floor_00_03, Floor_00_04;
/* Stone/Grass Sprites                               */ sprite Floor_01_01, Floor_01_02, Floor_01_03, Floor_01_04, Floor_01_05, Floor_01_06, Floor_01_07;
/* Grass/Rocks Floor Sprites                         */ sprite Floor_02_01, Floor_02_02, Floor_02_03, Floor_02_04;
/* Background Layers - for level01_layer1 and grass  */ layer level01_layer1, level01_layer2;
/* positions of scan window in each layer            */ int   level01_layer1_x=0, level01_layer2_x=0;


/********************************************************\
 * Level 1 Maps Header File                             *
\********************************************************/

/********************************************************\
 * Level 1 Map                                          *
\********************************************************/
BOOL BOARD01_LAYER01[] = ".................$%.....................................!@#...................$%.....................................!@#...................$%.....................................!@#..";
BOOL BOARD01_LAYER02[] = "...!@#.........................!@#..........$%..................!@#.........................!@#..........$%..................!@#.........................!@#..........$%...............";
BOOL BOARD01_LAYER03[] = "..............$%......................................$%...................$%......................................$%...................$%......................................$%.....";
BOOL BOARD01_LAYER04[] = ".......................................................................................................................................................................................";
BOOL BOARD01_LAYER05[] = ".......................................................................................................................................................................................";
BOOL BOARD01_LAYER06[] = ".EF........................K............................................................K............................................................K...............................CD";
BOOL BOARD01_LAYER07[] = ".ABI............GSTUTUTUTUTUTUVH...........J..................1247..............GSTUTUTUTUTUTUVH...........J.................................GSTUTUTUTUTUTUVH...........J...........IAB";
BOOL BOARD01_LAYER08[] = "1234562345623456234562345623456234562345623456234562345623447......12345623456234562345623456234562345623456234562345623456234471234562345623456234562345623456234562345623462345623447";
BOOL BOARD01_LAYER09[] = "~XYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXWX~~~~~~~~XYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXWX~~XYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZWXYZYZWXYZWXWX~";

/*========================================================================*\
  BITMAP TILES MAP LIST
\*======================================================================== /
  . = Empty Block / Blue Sky
  ! - Big Cloud Part 1 of 3
  @ - Big Cloud Part 2 of 3
  # - Big Cloud Part 3 of 3
  $ - Small Cloud Part 1 of 2
  % - Small Cloud Part 2 of 2
  1 = Grass/Stone Floor Tile 1
  2 = Grass/Stone Floor Tile 2
  3 = Grass/Stone Floor Tile 3
  4 = Grass/Stone Floor Tile 4
  5 = Grass/Stone Floor Tile 5
  6 = Grass/Stone Floor Tile 6
  7 = Grass/Stone Floor Tile 7
  A = Poll Bottom Left
  B = Poll Bottom Right
  C = Poll Top Left  Tile Set 0
  D = Poll Top Right Tile Set 0
  E = Poll Top Left  Tile Set 1
  F = Poll Top Right Tile Set 1
  G = Grass 1
  H = Grass 2
  I = Red Flowers
  J = Plant
  K = Mushroom
  S = Grass/Rocks Block Tile 1
  T = Grass/Rocks Block Tile 2
  U = Grass/Rocks Block Tile 3
  V = Grass/Rocks Block Tile 4
  W = Stone Block Tile 1
  X = Stone Block Tile 2
  Y = Stone Block Tile 3
  Z = Stone Block Tile 4

  ================================
  ANIMATED BITMAP TILES FOR LAYERS
  ================================
  ~ = Animated Water Tile
\*========================================================================*/

