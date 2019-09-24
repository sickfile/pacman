void Change_Ghost_Mode (void)
{
	if (zero.ghost_mode < PANIC)
	{
		if (SDL_GetTicks () - start_clock > 1000*scater2chase[progres])
		{	int i;
			progres++;
			if (progres % 2)	// chase
			{
				game_state = GAME_RUN;
				for (i = 1; i < 5; i++)
					if (SCATER == GHOST[i].ghost_mode) GHOST[i].ghost_mode = CHASE;
			}
			else	// scater
			{
				game_state = GAME_SCATER;
				for (i = 1; i < 5; i++)
					if (CHASE == GHOST[i].ghost_mode) GHOST[i].ghost_mode = SCATER;
			}
			start_clock = SDL_GetTicks ();
		}
	}
	else if (PANIC == zero.ghost_mode)
	{
		if (SDL_GetTicks () - panic_clock > 7000)
		{	int i;
			zero.ghost_mode = PANIC2;
			panic_clock = SDL_GetTicks ();
			for (i = 1; i < 5; i++)
			{
				if (sprite[i] == bghost)	// GHOST[PINKY].ghost_mode == PANIC
				{
					sprite[i] = wbghost;
					if (GHOST[i].ghost_mode == PANIC) GHOST[i].ghost_mode = PANIC2;
				}
			}
		}
	}
	else if (PANIC2 == zero.ghost_mode)
	{
		if (SDL_GetTicks () - panic_clock > 5000)
		{	int i;
			zero.ghost_mode = (game_state == GAME_SCATER) ? SCATER : CHASE;
			start_clock += 12000;
			for (i = 1; i < 5; i++)
			{
				if (sprite[i] == wbghost)	// ghost can be HOME & PANIC
				{
					sprite[i] = ghost[i];
					if (GHOST[i].ghost_mode == PANIC2)	// but for now either HOME or PANIC
					{
						GHOST[i].speed = PACMAN_SPEED;
						GHOST[i].x = (GHOST[i].x / 2) * 2;
						GHOST[i].y = (GHOST[i].y / 2) * 2;
						GHOST[i].ghost_mode = zero.ghost_mode;
					}// else is HOME
				}
			}
		}
	}
	if (!GHOST[PINKY].dot_net && (GHOST[PINKY].ghost_mode == HOME))
	{
		GHOST[PINKY].ghost_mode = EXIT;
		GHOST[PINKY].mov = UP;
	}
	else if (!GHOST[INKY].dot_net && (GHOST[INKY].ghost_mode == HOME))
	{
		GHOST[INKY].ghost_mode = EXIT;
		GHOST[INKY].mov = RIGHT;
	}
	else if (!GHOST[CLYDE].dot_net && (GHOST[CLYDE].ghost_mode == HOME))
	{
		GHOST[CLYDE].ghost_mode = EXIT;
		GHOST[CLYDE].mov = LEFT;
	}
	if (SDL_GetTicks () - no_dot_clock > 4000)
	{
		if (GHOST[PINKY].ghost_mode == HOME)
		{
			if (GHOST[PINKY].y % BLOCK_SIZE == 0)
			{
				GHOST[PINKY].ghost_mode = EXIT;
				GHOST[PINKY].mov = UP;
				no_dot_clock = SDL_GetTicks ();
			}
		}
		else if (GHOST[INKY].ghost_mode == HOME)
		{
			if (GHOST[INKY].y % BLOCK_SIZE == 0)
			{
				GHOST[INKY].ghost_mode = EXIT;
				GHOST[INKY].mov = RIGHT;
				no_dot_clock = SDL_GetTicks ();
			}
		}
		else if (GHOST[CLYDE].ghost_mode == HOME)
		{
			if (GHOST[CLYDE].y % BLOCK_SIZE == 0)
			{
				GHOST[CLYDE].ghost_mode = EXIT;
				GHOST[CLYDE].mov = LEFT;
			}
		}
	}
}
