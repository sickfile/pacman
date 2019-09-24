#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"
#include "timemod.h"
#include "ghostmod.h"
#include "ghostmov.h"
#include "ghostcom.h"
#include "sound.h"


int status;

void Clear_Quit (void)
{
	if (background) SDL_FreeSurface (background);
	if (sprite[PACMAN]) SDL_FreeSurface (sprite[PACMAN]);
	if (spritebck[PACMAN]) SDL_FreeSurface (spritebck[PACMAN]);
	if (ghost[BLINKY]) SDL_FreeSurface (ghost[BLINKY]);
	if (spritebck[BLINKY]) SDL_FreeSurface (spritebck[BLINKY]);
	if (ghost[PINKY]) SDL_FreeSurface (ghost[PINKY]);
	if (spritebck[PINKY]) SDL_FreeSurface (spritebck[PINKY]);
	if (ghost[INKY]) SDL_FreeSurface (ghost[INKY]);
	if (spritebck[INKY]) SDL_FreeSurface (spritebck[INKY]);
	if (ghost[CLYDE]) SDL_FreeSurface (ghost[CLYDE]);
	if (spritebck[CLYDE]) SDL_FreeSurface (spritebck[CLYDE]);
	if (bghost) SDL_FreeSurface (bghost);
	if (wbghost) SDL_FreeSurface (wbghost);
	if (eyes) SDL_FreeSurface (eyes);
	if (eatghost) SDL_FreeSurface (eatghost);
	if (dead) SDL_FreeSurface (dead);
	if (charset) SDL_FreeSurface (charset);
	if (fruit) SDL_FreeSurface (fruit);
	if (sound_buffer[WA]) SDL_FreeWAV (sound_buffer[WA]);
	if (sound_buffer[KA]) SDL_FreeWAV (sound_buffer[KA]);
	if (sound_buffer[BEGIN]) SDL_FreeWAV (sound_buffer[BEGIN]);
	if (sound_buffer[EATGHOST]) SDL_FreeWAV (sound_buffer[EATGHOST]);
	if (sound_buffer[DEATH]) SDL_FreeWAV (sound_buffer[DEATH]);
	if (sound_buffer[PROD]) SDL_FreeWAV (sound_buffer[PROD]);

	SDL_CloseAudio ();
	SDL_Quit ();
	printf ("%d", status);	
}

void Heartbeat (void)
{
	switch (game_state)
	{
		case GAME_INIT:
			score = 0;
			lives_left = 2;		
			game_state = GAME_NEW;
		break;
		case GAME_NEW:
			memcpy (maze, rmaze, ROWS*COLS);
			SDL_BlitSurface (background, 0, screen, 0);
			{
				SDL_Rect txy = {9*BLOCK_SIZE, 14*BLOCK_SIZE, 10*BLOCK_SIZE, BLOCK_SIZE};
				SDL_Rect gxy = {11*BLOCK_SIZE, 20*BLOCK_SIZE, 6*BLOCK_SIZE, BLOCK_SIZE};
				SDL_Surface* text;
				char buffer[40];
				SDL_Color txcolor = {20, 255, 255, 0};
				SDL_Color gxcolor = {255, 255, 20, 0};
				sprintf (buffer, "PLAYER ONE");
				text = render_text (buffer, &txcolor);
				SDL_BlitSurface (text, 0, screen, &txy);
				SDL_FreeSurface (text);
				sprintf (buffer, "READY!");
				text = render_text (buffer, &gxcolor);
				SDL_BlitSurface (text, 0, screen, &gxy);
				SDL_FreeSurface (text);
			}			
			stop_watch = SDL_GetTicks ();
			sound = BEGIN;
			memset (sound_pos, 0, 10*sizeof (int));
			SDL_PauseAudio (0);
			Draw_Lives (3);
			game_state = GAME_READY;
		break;
		case GAME_READY:
			if ((SDL_GetTicks () - stop_watch) < 2000) break;
			{// dont let IF fool you, this is just a trick to declare locals in C
				SDL_Rect txy = {9*BLOCK_SIZE, 14*BLOCK_SIZE, 10*BLOCK_SIZE, BLOCK_SIZE};
				SDL_FillRect (screen, &txy, BLACK);
			}
			Set_Character_Start ();
			{	int i;
				for (i = PACMAN; i > ZERO; i--)
				{
					Cut_Sprite_Background (i);
					Draw_Caracter (i);
				}
			}
			Draw_Lives (lives_left);
			stop_watch = SDL_GetTicks ();
			game_state = GAME_START_LEVEL;
		break;
		case GAME_START_LEVEL:
			if (sound) break; // sound = 2 until end of BEGINING sound then sound = 0 i.e. WA sound
			{
				SDL_Rect gxy = {11*BLOCK_SIZE, 20*BLOCK_SIZE, 6*BLOCK_SIZE, BLOCK_SIZE};
				SDL_FillRect (screen, &gxy, BLACK);
			}
			progres = 0;
			no_dot_clock = start_clock = SDL_GetTicks ();
			fruit_clock = 0;
			Draw_Lives (lives_left);
			pacs2go = 244;
			GHOST[PINKY].mov = UP;
			game_state = GAME_SCATER;
		break;
		case GAME_RESTART_LEVEL:
			{	int i;
				for (i = BLINKY; i <= PACMAN; i++) Paste_Sprite_Background (i);
			}
			Set_Character_Restart ();
			progres = 0;
			no_dot_clock = start_clock = SDL_GetTicks ();
			fruit_clock = 0;
			{	int i;
				for (i = PACMAN; i > ZERO; i--)
				{
					Cut_Sprite_Background (i);
					Draw_Caracter (i);
				}
			}
			Draw_Lives (lives_left);
			sound = WA;
			game_state = GAME_SCATER;
		break;
		case GAME_RUN:
			{	int i;
				for (i = BLINKY; i <= PACMAN; i++) Paste_Sprite_Background (i);
			}
			Draw_Text ();
			Command_Pacman ();
			Mov_Pacman ();
			Dot_Remove ();
			Change_Ghost_Mode ();
			On_Ghost_Mode ();
			Command_Ghost ();
			Mov_Ghost ();
			{	int i;
				for (i = PACMAN; i > ZERO; i--) Cut_Sprite_Background (i);
				for (i = PACMAN; i > ZERO; i--) Draw_Caracter (i);
			}// a bug if you use the same FOR
			Pacman_Remove ();
			if (!pacs2go)
			{
				GHOST[PACMAN].pict = 0;
				game_state = GAME_END_LEVEL;
			}
		break;
		case GAME_SCATER:
			{	int i;
				for (i = BLINKY; i <= PACMAN; i++) Paste_Sprite_Background (i);
			}
			Draw_Text ();
			Command_Pacman ();
			Mov_Pacman ();
			Dot_Remove ();
			Change_Ghost_Mode ();
			On_Ghost_Mode ();
			Command_Ghost ();
			Mov_Ghost ();
			{	int i;
				for (i = PACMAN; i > ZERO; i--) Cut_Sprite_Background (i);
				for (i = PACMAN; i > ZERO; i--) Draw_Caracter (i);
			}	// GHOST[BLINKY] is drawn last, over the top
			Pacman_Remove ();
			if (!pacs2go)
			{
				GHOST[PACMAN].pict = 0;
				game_state = GAME_END_LEVEL;
			}
		break;
		case GAME_KILL_STOP:
			if ((SDL_GetTicks () - stop_watch) < 2000)
			{
				zero.x += PACMAN_SPEED;
				if ((zero.x % BLOCK_SIZE) == 0)
				{
					int i;
					zero.pict = zero.pict ? 0 : 1;
					zero.x = 0;
					for (i = PACMAN; i > ZERO; i--) Paste_Sprite_Background (i);
					for (i = PACMAN; i > ZERO; i--) Draw_Caracter (i);
				}
				break;
			}
			zero.x = GHOST[PACMAN].pict = 0;
			sound = DEATH;
			sound_pos[DEATH] = 0;
			SDL_PauseAudio (0);
			game_state = GAME_KILL_ANIM;
		break;
		case GAME_KILL_ANIM:
			zero.x += PACMAN_SPEED;
			if ((zero.x % BLOCK_SIZE) == 0)
			{
				int i;
				SDL_Rect src = {BLOCK_SIZE*2, 0, BLOCK_SIZE*2, BLOCK_SIZE*2};
				SDL_Rect pos = {X_OFFSET, Y_OFFSET, BLOCK_SIZE*2, BLOCK_SIZE*2};
				src.x *= GHOST[PACMAN].pict++;
				pos.x += GHOST[PACMAN].x; pos.y += GHOST[PACMAN].y;
				for (i = PACMAN; i > ZERO; i--) Paste_Sprite_Background (i);
				SDL_BlitSurface (dead, &src, screen, &pos);
				zero.x = 0;
			}
//			printf ("%d\t%d\t%d\n", GHOST[PACMAN].pict, sound, sound_pos[sound]);
			if ((GHOST[PACMAN].pict < 13)) break;
			SDL_PauseAudio (1);
			game_state = GAME_PACMAN_KILL;
		break;
		case GAME_PACMAN_KILL:
			if (!lives_left)
			{
				SDL_Delay (4000);
				game_state = GAME_INIT;
				break;
			}
			--lives_left;
			SDL_Delay (2000);
			game_state = GAME_RESTART_LEVEL;
		break;
		case GAME_EAT_GHOST:
			if (sound) break;	// sound = 3 EATGHOST then sound = 0 i.e. WA
			{	int i;
				for (i = PACMAN; i > ZERO; i--) Draw_Caracter (i);
			}
			game_state = (progres % 2) ? GAME_RUN : GAME_SCATER;
		break;
		case GAME_PANIC:
		break;
		case GAME_END_LEVEL:
			{
				SDL_Surface* text;
				char buffer[80];
				SDL_Color txcolor = {255, 255, 0};
				SDL_Rect txy = {0, 0, BLOCK_SIZE*24, BLOCK_SIZE};
			
				sprintf (buffer, "      BRAVO!!!");
				text = render_text (buffer, &txcolor);
				SDL_FillRect (screen, &txy, SDL_MapRGB (screen->format, 0, 0, 0));
				SDL_BlitSurface (text, 0, screen, &txy);
				SDL_FreeSurface (text);
				SDL_UpdateRects (screen, 1, &txy);
			}
			SDL_PauseAudio (1);
			game_state = GAME_NEW;
			SDL_Delay (10000);
			break;
		case GAME_OVER:
			break;
		case GAME_SHUTDOWN:
			break;
		case GAME_EXIT:
			break;
	}
}

int main (int argc, char* argv[])
{
	SDL_Event event;
	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	atexit (Clear_Quit);
	SDL_WM_SetCaption ("PACMAN", 0);
	screen = SDL_SetVideoMode ((COLS-4)*BLOCK_SIZE, (5+ROWS)*BLOCK_SIZE, 0, SDL_ANYFORMAT);
	status = Game_Init ();
	if (status) return -1;
	while (run_game)
	{
		if (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					run_game = 0;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							run_game = 0;
							break;
						case SDLK_LEFT:
							kleft = LEFT;
							break;
						case SDLK_RIGHT:
							kright = RIGHT;
							break;
						case SDLK_UP:
							kup = UP;
							break;
						case SDLK_DOWN:
							kdown = DOWN;
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case SDLK_LEFT:
							kleft = 0;
							break;
						case SDLK_RIGHT:
							kright = 0;
							break;
						case SDLK_UP:
							kup = 0;
							break;
						case SDLK_DOWN:
							kdown = 0;
							break;
					}
					break;
				default:
					break;
			}
		}
		Heartbeat ();
		SDL_Flip (screen);
		SDL_Delay (1000/FPS);
	}
	return 0;
}