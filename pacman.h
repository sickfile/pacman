#pragma comment (lib, ".\Lib\SDL.lib")
#pragma comment (lib, ".\Lib\SDLmain.lib")

#include <SDL\SDL.h>

#define FPS 60
#define BLOCK_SIZE 16
#define X_OFFSET -BLOCK_SIZE/2
#define Y_OFFSET 3*BLOCK_SIZE-BLOCK_SIZE/2
#define PACMAN_START_X 13*BLOCK_SIZE+BLOCK_SIZE/2
#define PACMAN_START_Y 23*BLOCK_SIZE
#define BLINKY_START_X PACMAN_START_X
#define BLINKY_START_Y 11*BLOCK_SIZE
#define PINKY_START_X PACMAN_START_X
#define PINKY_START_Y BLINKY_START_Y + 3*BLOCK_SIZE
#define INKY_START_X PACMAN_START_X - 2*BLOCK_SIZE
#define INKY_START_Y PINKY_START_Y
#define CLYDE_START_X PACMAN_START_X + 2*BLOCK_SIZE
#define CLYDE_START_Y PINKY_START_Y
#define ROWS 31
#define COLS 32
#define STOP 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define PACMAN 5
#define BLINKY 1
#define PINKY 2
#define INKY 3
#define CLYDE 4
#define ZERO 0
#define BLACK 0
#define CHARSIZE BLOCK_SIZE
#define PACMAN_SPEED 2

int kleft = 0, kright = 0, kup = 0, kdown = 0;
unsigned int start_clock, stop_watch, no_dot_clock, fruit_clock;
unsigned int panic_clock;
unsigned int progres = 0;
unsigned int run_game = 1;
unsigned int pacs2go = 244;
unsigned int score = 0;
unsigned int lives_left = 2;
unsigned int ghost_kill = 0;
unsigned int level = 1;
unsigned int scater2chase[8] = {7, 20, 7, 20, 5, 20, 5, 3000};

enum state {GAME_INIT, GAME_NEW, GAME_READY, GAME_START_LEVEL, GAME_KILL_STOP, GAME_KILL_ANIM, GAME_PACMAN_KILL, GAME_RESTART_LEVEL, GAME_RUN, GAME_SCATER, GAME_PANIC, GAME_EAT_GHOST, GAME_END_LEVEL, GAME_OVER, GAME_SHUTDOWN, GAME_EXIT};
enum state game_state = GAME_INIT;
enum mode {HOME = 0, EXIT = 1, CHASE = 2, SCATER = 4, PANIC = 8, PANIC2 = 16, EYES = 32, ENTER = 64};

struct tile
{
	int row;
	int col;
};

struct caracter
{
	int x;
	int y;
	int mov;
	unsigned int pict;		// remove this, ghost command is global in pict1 or pict2
	enum mode ghost_mode;	// every ghost can be in any mode
	struct tile target;
	unsigned int dot_net;
	unsigned int speed;
};

struct caracter zero = {BLINKY_START_X, BLINKY_START_Y, LEFT, 0, SCATER, 0, 24, 0, PACMAN_SPEED};

struct caracter GHOST[6] = {BLINKY_START_X, BLINKY_START_Y, LEFT, 0, SCATER, 0, 24, 0, PACMAN_SPEED,
							BLINKY_START_X, BLINKY_START_Y, LEFT, 0, SCATER, 0, 24, 0, PACMAN_SPEED,
							PINKY_START_X, PINKY_START_Y, DOWN, 0, EXIT, 0, 8, 0, PACMAN_SPEED/2,
							INKY_START_X, INKY_START_Y, UP, 0, HOME, 30, 24, 30, PACMAN_SPEED/2,
							CLYDE_START_X, CLYDE_START_Y, UP, 0, HOME, 30, 8, 60, PACMAN_SPEED/2,
							PACMAN_START_X, PACMAN_START_Y, LEFT, 0, CHASE};

SDL_Surface* screen = 0;
SDL_Surface* background = 0;
SDL_Surface* sprite[6] = {0, 0, 0, 0, 0, 0};
SDL_Surface* spritebck[6] = {0, 0, 0, 0, 0, 0};
SDL_Surface* ghost[6] = {0, 0, 0, 0, 0, 0};
SDL_Surface* bghost = 0; SDL_Surface* wbghost = 0; SDL_Surface* eyes = 0; SDL_Surface* eatghost = 0;
SDL_Surface* dead = 0;
SDL_Surface* charset = 0;
SDL_Surface* fruit = 0;


enum sound {WA, KA, BEGIN, EATGHOST, DEATH, EATFRUIT, PROD = 9};  //PROD must not be > 9, out of boundry
SDL_AudioSpec obtained;
SDL_AudioSpec* get_spec;
Uint8* sound_buffer[10];
unsigned int audio_len[10];
unsigned int sound_pos[10];
int sound = WA;


unsigned char maze[ROWS][COLS];
//									   0123456789012345678901234567
unsigned char rmaze[ROWS][COLS] = { "  XXXXXXXXXXXXXXXXXXXXXXXXXXXX  ",//0
									"  X------------XX------------X  ",//1 24
									"  X-XXXX-XXXXX-XX-XXXXX-XXXX-X  ",//2  6
									"  XOXXXX-XXXXX-XX-XXXXX-XXXXOX  ",//3  6
									"  X-XXXX-XXXXX-XX-XXXXX-XXXX-X  ",//4  6
									"  X--------------------------X  ",//5 26
									"  X-XXXX-XX-XXXXXXXX-XX-XXXX-X  ",//6  6
									"  X-XXXX-XX-XXXXXXXX-XX-XXXX-X  ",//7  6
									"  X------XX----XX----XX------X  ",//8 20
									"  XXXXXX-XXXXX XX XXXXX-XXXXXX  ",//9  2
									"       X-XXXXX XX XXXXX-X       ",//0  2
									"       X-XX          XX-X       ",//1  2
									"       X-XX XXXXXXXX XX-X       ",//2  2
									"XXXXXXXX-XX X      X XX-XXXXXXXX",//3  2
									"        -   X      X   -        ",//4  2
									"XXXXXXXX-XX X      X XX-XXXXXXXX",//5  2
									"       X-XX XXXXXXXX XX-X       ",//6  2
									"       X-XX          XX-X       ",//7  2
									"       X-XX XXXXXXXX XX-X       ",//8  2
									"  XXXXXX-XX XXXXXXXX XX-XXXXXX  ",//9  2
									"  X------------XX------------X  ",//0 24
									"  X-XXXX-XXXXX-XX-XXXXX-XXXX-X  ",//1  6
									"  X-XXXX-XXXXX-XX-XXXXX-XXXX-X  ",//2  6
									"  XO--XX-------  -------XX--OX  ",//3 22
									"  XXX-XX-XX-XXXXXXXX-XX-XX-XXX  ",//4  6
									"  XXX-XX-XX-XXXXXXXX-XX-XX-XXX  ",//5  6
									"  X------XX----XX----XX------X  ",//6 20
									"  X-XXXXXXXXXX-XX-XXXXXXXXXX-X  ",//7  4
									"  X-XXXXXXXXXX-XX-XXXXXXXXXX-X  ",//8  4
									"  X--------------------------X  ",//9 26
									"  XXXXXXXXXXXXXXXXXXXXXXXXXXXX  "};//0
								   //01234567890123456789012345678901
									   //01234567890123456789012345678901
unsigned char ghost_space[ROWS][COLS]= {"  XXXXXXXXXXXXXXXXXXXXXXXXXXXX  ",//0
										"  X     ?      XX      ?     X  ",//1 24
										"  X XXXX XXXXX XX XXXXX XXXX X  ",//2  6
										"  X XXXX XXXXX XX XXXXX XXXX X  ",//3  6
										"  X XXXX XXXXX XX XXXXX XXXX X  ",//4  6
										"  X?    ?  ?  ?  ?  ?  ?    ?X  ",//5 26
										"  X XXXX XX XXXXXXXX XX XXXX X  ",//6  6
										"  X XXXX XX XXXXXXXX XX XXXX X  ",//7  6
										"  X     ?XX    XX    XX?     X  ",//8 20
										"  XXXXXX XXXXX XX XXXXX XXXXXX  ",//9  2
										"       X XXXXX XX XXXXX X       ",//0  2
										"       X XX   -+ -   XX X       ",//1  2
										"       X XX XXX  XXX XX X       ",//2  2
										"XXXXXXXX XX X      X XX XXXXXXXX",//3  2
										"        ?  ?X      X?  ?        ",//4  2
										"XXXXXXXX XX X      X XX XXXXXXXX",//5  2
										"       X XX XXXXXXXX XX X       ",//6  2
										"       X XX?        ?XX X       ",//7  2
										"       X XX XXXXXXXX XX X       ",//8  2
										"  XXXXXX XX XXXXXXXX XX XXXXXX  ",//9  2
										"  X     ?  ?   XX   ?  ?     X  ",//0 24
										"  X XXXX XXXXX XX XXXXX XXXX X  ",//1  6
										"  X XXXX XXXXX XX XXXXX XXXX X  ",//2  6
										"  X   XX?  ?  -  -  ?  ?XX   X  ",//3 22
										"  XXX XX XX XXXXXXXX XX XX XXX  ",//4  6
										"  XXX XX XX XXXXXXXX XX XX XXX  ",//5  6
										"  X  ?   XX    XX    XX   ?  X  ",//6 20
										"  X XXXXXXXXXX XX XXXXXXXXXX X  ",//7  4
										"  X XXXXXXXXXX XX XXXXXXXXXX X  ",//8  4
										"  X           ?  ?           X  ",//9 26
										"  XXXXXXXXXXXXXXXXXXXXXXXXXXXX  "};//0
								   	   //01234567890123456789012345678901

int Game_Init (void);
void Paste_Sprite_Background (int);
void Cut_Sprite_Background (int);
void Draw_Caracter (int);
void Dot_Remove (void);
void Pacman_Remove (void);
void Command_Pacman (void);
void Mov_Pacman (void);
void Draw_Lives (unsigned int);
void Draw_Text (void);
void Mode_Change (void);
void Command_Ghost (void);
void Mov_Ghost (void);
void On_Ghost_Mode (void);
void Change_Ghost_Mode (void);
SDL_Surface* render_text (const char*, SDL_Color*);
void Set_Character_Start (void);
void Set_Character_Restart (void);

void Set_Character_Start (void)
{
	GHOST[PACMAN].x = PACMAN_START_X;
	GHOST[PACMAN].y = PACMAN_START_Y;
	GHOST[PACMAN].mov = LEFT;
	GHOST[PACMAN].pict = 0;
	GHOST[BLINKY].x = BLINKY_START_X;
	GHOST[BLINKY].y = BLINKY_START_Y;
	GHOST[BLINKY].mov = LEFT;
	GHOST[BLINKY].pict = 0;
	GHOST[BLINKY].ghost_mode = SCATER;
	GHOST[BLINKY].speed = PACMAN_SPEED;
	GHOST[PINKY].x = PINKY_START_X;
	GHOST[PINKY].y = PINKY_START_Y;
	GHOST[PINKY].mov = DOWN;
	GHOST[PINKY].pict = 0;
	GHOST[PINKY].ghost_mode = EXIT;
	GHOST[PINKY].dot_net = 0;
	GHOST[PINKY].speed = PACMAN_SPEED/2;
	GHOST[INKY].x = INKY_START_X;
	GHOST[INKY].y = INKY_START_Y;
	GHOST[INKY].mov = UP;
	GHOST[INKY].pict = 0;
	GHOST[INKY].ghost_mode = HOME;
	GHOST[INKY].dot_net = 30;
	GHOST[INKY].speed = PACMAN_SPEED/2;
	GHOST[CLYDE].x = CLYDE_START_X;
	GHOST[CLYDE].y = CLYDE_START_Y;
	GHOST[CLYDE].mov = UP;
	GHOST[CLYDE].pict = 0;
	GHOST[CLYDE].ghost_mode = HOME;
	GHOST[CLYDE].dot_net = 60;
	GHOST[CLYDE].speed = PACMAN_SPEED/2;
	sprite[BLINKY] = ghost[BLINKY];
	sprite[PINKY] = ghost[PINKY];
	sprite[INKY] = ghost[INKY];
	sprite[CLYDE] = ghost[CLYDE];
	zero.ghost_mode = SCATER;
}

void Set_Character_Restart (void)
{
	Set_Character_Start ();
	GHOST[PINKY].mov = DOWN;
	GHOST[PINKY].ghost_mode = HOME;
	GHOST[PINKY].dot_net = 7;
	GHOST[INKY].dot_net = 17;
	GHOST[CLYDE].dot_net = 32;
	sprite[BLINKY] = ghost[BLINKY];
	sprite[PINKY] = ghost[PINKY];
	sprite[INKY] = ghost[INKY];
	sprite[CLYDE] = ghost[CLYDE];
	zero.ghost_mode = SCATER;
}

SDL_Surface* render_text (const char* buffer, SDL_Color* boja)
{
	SDL_Surface* temp;
	unsigned int w = strlen (buffer);
	w *= CHARSIZE;
	temp = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCCOLORKEY, w, CHARSIZE, charset->format->BitsPerPixel, charset->format->Rmask, charset->format->Gmask, charset->format->Bmask, charset->format->Amask);
	SDL_SetColors (temp, boja, 9, 1); // indekso of ink color is 9
	for (w = 0; w < strlen (buffer); w++)
	{
		SDL_Rect src = {0, 0, CHARSIZE, CHARSIZE};
		SDL_Rect pos = {0, 0, CHARSIZE, CHARSIZE};
		src.y = (buffer[w] / 32) * CHARSIZE;
		src.x = (buffer[w] % 32) * CHARSIZE;
		pos.x = w*CHARSIZE;
		SDL_BlitSurface (charset, &src, temp, &pos);
	}
	
	return temp;
}

void Paste_Sprite_Background (int i)
{
	SDL_Rect pos = {0, 0, 2*BLOCK_SIZE, 2*BLOCK_SIZE};
	pos.x = X_OFFSET + GHOST[i].x; pos.y = Y_OFFSET + GHOST[i].y;
	SDL_BlitSurface (spritebck[i], 0, screen, &pos);
}

void Cut_Sprite_Background (int i)
{
	SDL_Rect pos = {0, 0, 2*BLOCK_SIZE, 2*BLOCK_SIZE};
	pos.x = X_OFFSET + GHOST[i].x; pos.y = Y_OFFSET + GHOST[i].y;
	SDL_BlitSurface (screen, &pos, spritebck[i], 0);
}

void Draw_Caracter (int i)
{
	SDL_Rect src = {2*BLOCK_SIZE, 2*BLOCK_SIZE, 2*BLOCK_SIZE, 2*BLOCK_SIZE};
	SDL_Rect pos = {0, 0, 2*BLOCK_SIZE, 2*BLOCK_SIZE};
	GHOST[BLINKY].pict = GHOST[PINKY].pict = GHOST[INKY].pict = GHOST[CLYDE].pict = zero.pict;	// delete this
	src.x = 2*BLOCK_SIZE * GHOST[i].pict; src.y = 2*BLOCK_SIZE * GHOST[i].mov;
	pos.x = X_OFFSET + GHOST[i].x; pos.y = Y_OFFSET + GHOST[i].y;
	SDL_BlitSurface (sprite[i], &src, screen, &pos);
}

void Dot_Remove (void)
{
	int row = GHOST[PACMAN].y / BLOCK_SIZE;
	int col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;
	if ((GHOST[PACMAN].y % BLOCK_SIZE == 0) && (GHOST[PACMAN].x % BLOCK_SIZE == 0))
	{
		SDL_Rect pos;
		pos.x = GHOST[PACMAN].x; pos.y = 3*BLOCK_SIZE + GHOST[PACMAN].y; pos.w = pos.h = BLOCK_SIZE;
		if (maze[row][col] == '-')
		{
			if (sound < BEGIN)
			{
				sound_pos[WA] = sound_pos[KA] = 0;
				SDL_PauseAudio (0);
			}
			maze[row][col] = ' ';
			pacs2go--;
			score += 10;
			SDL_FillRect (screen, &pos, BLACK);
			no_dot_clock = SDL_GetTicks ();
			if (GHOST[PINKY].dot_net) GHOST[PINKY].dot_net--;
			else if (GHOST[INKY].dot_net) GHOST[INKY].dot_net--;
			else if (GHOST[CLYDE].dot_net) GHOST[CLYDE].dot_net--;
		}
		else if (maze[row][col] == 'O')
		{
			int i;
			sound = PROD;
			sound_pos[PROD] = 0;
			SDL_PauseAudio (0);
			maze[row][col] = ' ';
			pacs2go--;
			score += 50;
			SDL_FillRect (screen, &pos, BLACK);
			panic_clock = SDL_GetTicks ();
			zero.ghost_mode = PANIC;
			ghost_kill = 0;
			for (i = BLINKY; i < PACMAN; i++)
			{
				if (GHOST[i].ghost_mode < EYES)
				{
					sprite[i] = bghost;
					if (GHOST[i].ghost_mode > EXIT)
					{
						GHOST[i].ghost_mode = PANIC;
						GHOST[i].speed = PACMAN_SPEED / 2;
					}
				}
			}
		}
		else if (' ' != maze[row][col])
		{
			sound = EATFRUIT;
			sound_pos[EATFRUIT] = 0;
			SDL_PauseAudio (0);
			maze[row][col] = ' ';
			score += 100;
			pos.x = X_OFFSET+13*BLOCK_SIZE; pos.y = Y_OFFSET+17*BLOCK_SIZE;
			pos.w = pos.h = 2*BLOCK_SIZE;
			SDL_FillRect (screen, &pos, BLACK);
		}
	}
	if ((174 == pacs2go) || (74 == pacs2go))
	{
		SDL_Rect src = {BLOCK_SIZE*2, 0, BLOCK_SIZE*2, BLOCK_SIZE*2};
		SDL_Rect pos = {X_OFFSET+13*BLOCK_SIZE, Y_OFFSET+17*BLOCK_SIZE, BLOCK_SIZE*2, BLOCK_SIZE*2};
		src.x *= level;
		SDL_BlitSurface (fruit, &src, screen, &pos);
		fruit_clock = SDL_GetTicks ();
	}
	if (fruit_clock && (SDL_GetTicks () - fruit_clock > 10000))
	{
		SDL_Rect pos = {X_OFFSET+13*BLOCK_SIZE, Y_OFFSET+17*BLOCK_SIZE, BLOCK_SIZE*2, BLOCK_SIZE*2};
		SDL_FillRect (screen, &pos, BLACK);
		fruit_clock = 0;
	}
}

void Pacman_Remove (void)
{
	int i;
	int row = GHOST[PACMAN].y / BLOCK_SIZE; int col = GHOST[PACMAN].x / BLOCK_SIZE;
	for (i = BLINKY; i < PACMAN; i++)
	{
		if ((col == GHOST[i].x / BLOCK_SIZE) && (row == GHOST[i].y / BLOCK_SIZE))
		{
			if (GHOST[i].ghost_mode < PANIC)
			{
				while (SDL_AUDIO_PLAYING == SDL_GetAudioStatus ());
				zero.x = zero.pict = 0;
				stop_watch = SDL_GetTicks ();
				SDL_PauseAudio (1);
				game_state = GAME_KILL_STOP;
			}
			else if (GHOST[i].ghost_mode > PANIC2); // don't modify if ENTER, he'll spin in the house
			else
			{
				Paste_Sprite_Background (PACMAN);
				Paste_Sprite_Background (i);
				printf ("g%d\tp.x %d\tp.y %d\tg.x %d\tg.y %d\t",game_state, GHOST[PACMAN].x, GHOST[PACMAN].y, GHOST[i].x, GHOST[i].y);
				GHOST[i].x = (GHOST[i].x / 4) * 4;
				GHOST[i].y = (GHOST[i].y / 4) * 4;
				printf ("g.x %d\tg.y %d\n",GHOST[i].x, GHOST[i].y);
				sprite[i] = eyes;
				GHOST[i].speed = 2*PACMAN_SPEED;
				GHOST[i].ghost_mode = EYES;
				Cut_Sprite_Background (i);
				{
					SDL_Rect sor = {BLOCK_SIZE*2, 0, BLOCK_SIZE*2, BLOCK_SIZE*2};
					SDL_Rect pos = {X_OFFSET, Y_OFFSET, BLOCK_SIZE*2, BLOCK_SIZE*2};
					sor.x *= ghost_kill++;
					pos.x += GHOST[PACMAN].x; pos.y += GHOST[PACMAN].y;
					SDL_BlitSurface (eatghost, &sor, screen, &pos);
				}
				score += ghost_kill*200;
				SDL_Flip (screen);	// delete this
//				SDL_UpdateRect (screen, GHOST[PACMAN].x, GHOST[PACMAN].y, BLOCK_SIZE*2, BLOCK_SIZE*2);
				sound = EATGHOST;
				sound_pos[EATGHOST] = 0;
				SDL_PauseAudio (0);
				game_state = GAME_EAT_GHOST;
			}
		}
	}
}

void Command_Pacman (void)
{
	int row = GHOST[PACMAN].y / BLOCK_SIZE;
	int col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;
	if (GHOST[PACMAN].y % BLOCK_SIZE == 0)
	{
		if (kleft && GHOST[PACMAN].mov != LEFT)
		{
			if (maze[row][col-1] != 'X')
			{
				GHOST[PACMAN].mov = LEFT;
				GHOST[PACMAN].pict = 0;
			}
//			else if (GHOST[PACMAN].x % BLOCK_SIZE != 0)
//			{
//				GHOST[PACMAN].mov = LEFT;
//				GHOST[PACMAN].pict = 0;
//			}
		}
		else if (kright && GHOST[PACMAN].mov != RIGHT)
		{
			if (maze[row][col+1] != 'X')
			{
				GHOST[PACMAN].mov = RIGHT;
				GHOST[PACMAN].pict = 0;
			}
		}
	}
	if (GHOST[PACMAN].x % BLOCK_SIZE == 0)
	{
		if (kup && GHOST[PACMAN].mov != UP)
		{
			if (maze[row-1][col] != 'X')
			{
				GHOST[PACMAN].mov = UP;
				GHOST[PACMAN].pict = 0;
			}
		}
		else if (kdown && GHOST[PACMAN].mov != DOWN)
		{
			if (maze[row+1][col] != 'X')
			{
				GHOST[PACMAN].mov = DOWN;
				GHOST[PACMAN].pict = 0;
			}
		}
	}
}

void Mov_Pacman (void)
{
	int row = GHOST[PACMAN].y / BLOCK_SIZE;
	int col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;
	switch (GHOST[PACMAN].mov)
	{
		case LEFT:
			if (GHOST[PACMAN].x % (BLOCK_SIZE) == 0)
			{
				if (col == 0)
				{
					GHOST[PACMAN].x = (COLS-3)*BLOCK_SIZE;
				}
				if (maze[row][col-1] != 'X')
				{
					if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
					else GHOST[PACMAN].pict++;
					GHOST[PACMAN].x -= PACMAN_SPEED;
				}
			}
			else if (GHOST[PACMAN].x % (BLOCK_SIZE/2) == 0)
			{
				if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
				else GHOST[PACMAN].pict++;
				GHOST[PACMAN].x -= PACMAN_SPEED;
			}
			else // move only if not GHOST[PACMAN].x % BLOCK_SIZE == 0
			{
				GHOST[PACMAN].x -= PACMAN_SPEED;
			}
			break;
		case RIGHT:
			if (GHOST[PACMAN].x % BLOCK_SIZE == 0)
			{
				if (col == COLS-1)
				{
					GHOST[PACMAN].x = -2*BLOCK_SIZE;
				}
				if (maze[row][col+1] != 'X')
				{
					if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
					else GHOST[PACMAN].pict++;
					GHOST[PACMAN].x += PACMAN_SPEED;
				}
			}
			else if (GHOST[PACMAN].x % (BLOCK_SIZE/2) == 0)
			{
				if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
				else GHOST[PACMAN].pict++;
				GHOST[PACMAN].x += PACMAN_SPEED;
			}
			else
			{
					GHOST[PACMAN].x += PACMAN_SPEED;
			}
			break;
		case UP:
			if (GHOST[PACMAN].y % BLOCK_SIZE == 0)
			{
				if (maze[row-1][col] != 'X')
				{
					if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
					else GHOST[PACMAN].pict++;
					GHOST[PACMAN].y -= PACMAN_SPEED;
				}
			}
			else if (GHOST[PACMAN].y % (BLOCK_SIZE/2) == 0)
			{
				if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
				else GHOST[PACMAN].pict++;
				GHOST[PACMAN].y -= PACMAN_SPEED;
			}
			else
			{
				GHOST[PACMAN].y -= PACMAN_SPEED;
			}
			break;
		case DOWN:
			if (GHOST[PACMAN].y % BLOCK_SIZE == 0)
			{
				if (maze[row+1][col] != 'X')
				{
					if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
					else GHOST[PACMAN].pict++;
					GHOST[PACMAN].y += PACMAN_SPEED;
				}
			}
			else if (GHOST[PACMAN].y % (BLOCK_SIZE/2) == 0)
			{
				if (GHOST[PACMAN].pict > 1) GHOST[PACMAN].pict = 0;
				else GHOST[PACMAN].pict++;
				GHOST[PACMAN].y += PACMAN_SPEED;
			}
			else
			{
				GHOST[PACMAN].y += PACMAN_SPEED;
			}
			break;
	}
}

void Draw_Lives (unsigned int lives)
{
	unsigned int i = 0;
	SDL_Rect src = {2*BLOCK_SIZE, 2*BLOCK_SIZE, BLOCK_SIZE*2, BLOCK_SIZE*2};
	SDL_Rect pos = {X_OFFSET+2*BLOCK_SIZE, Y_OFFSET+31*BLOCK_SIZE+BLOCK_SIZE/2, BLOCK_SIZE*2, BLOCK_SIZE*2};
	pos.w *= (lives+1);
	SDL_FillRect (screen, &pos, BLACK);
	pos.w = BLOCK_SIZE*2;
	for (; i < lives; i++)
	{
		pos.x = (X_OFFSET + 2*BLOCK_SIZE) + i*BLOCK_SIZE*2;
		SDL_BlitSurface (sprite[PACMAN], &src, screen, &pos);
	}
}

void Draw_Text (void)
{
	SDL_Surface* text;
	char buffer[80];
	SDL_Color txcolor = {255, 255, 55, 0};
	SDL_Rect txy = {0, 2*BLOCK_SIZE, BLOCK_SIZE*28, BLOCK_SIZE};
	SDL_FillRect (screen, &txy, SDL_MapRGB (screen->format, 0, 0, 0));

	sprintf (buffer, "R %02d C %02d M %d.%d.%d TICK %d", GHOST[BLINKY].target.row, GHOST[BLINKY].target.col, game_state, GHOST[BLINKY].ghost_mode, GHOST[BLINKY].mov, SDL_GetTicks () - start_clock);
//	text = TTF_RenderText_Solid (font, buffer, txcolor);
	text = render_text (buffer, &txcolor);
	SDL_BlitSurface (text, 0, screen, &txy);
	SDL_FreeSurface (text);

	txy.y = BLOCK_SIZE;
	sprintf (buffer, "      1UP SCORE %05d", score);
//	text = TTF_RenderText_Solid (font, buffer, txcolor);
	text = render_text (buffer, &txcolor);
	SDL_FillRect (screen, &txy, SDL_MapRGB (screen->format, 0, 0, 0));
	SDL_BlitSurface (text, 0, screen, &txy);

	SDL_FreeSurface (text);
}
