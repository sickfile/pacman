void On_Ghost_Mode (void)
{
	switch (GHOST[BLINKY].ghost_mode)
	{
		case HOME:
		break;
		case EXIT:
			if (GHOST[BLINKY].x == PINKY_START_X)
			{
				GHOST[BLINKY].mov = UP;
			}
		break;
		case CHASE:
			GHOST[BLINKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE;
			GHOST[BLINKY].target.col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;
		break;
		case SCATER:
			GHOST[BLINKY].target.row = 0;
			GHOST[BLINKY].target.col = 24;
		break;
		case PANIC:
		case PANIC2:
			GHOST[BLINKY].target.row = rand () % ROWS;
			GHOST[BLINKY].target.col = rand () % COLS;
		break;
		case EYES:
			GHOST[BLINKY].target.row = 11;
			GHOST[BLINKY].target.col = 15;
		break;
		case ENTER:
			if (GHOST[BLINKY].y < PINKY_START_Y)
			{
				GHOST[BLINKY].mov = DOWN;
			}
			else if (GHOST[BLINKY].x < PINKY_START_X)
			{
				GHOST[BLINKY].mov = RIGHT;
			}
			else if (GHOST[BLINKY].x > PINKY_START_X)
			{
				GHOST[BLINKY].mov = LEFT;
			}
			else
			{
				sprite[BLINKY] = ghost[BLINKY];
				GHOST[BLINKY].speed = PACMAN_SPEED/2;
				GHOST[BLINKY].ghost_mode = EXIT;
				GHOST[BLINKY].mov = UP;
			}
		break;
	}
	switch (GHOST[PINKY].ghost_mode)
	{
		case HOME:
			if (GHOST[PINKY].y == PINKY_START_Y - BLOCK_SIZE/2) GHOST[PINKY].mov = DOWN;
			else if (GHOST[PINKY].y == PINKY_START_Y + BLOCK_SIZE/2) GHOST[PINKY].mov = UP;
			break;
		case EXIT:
			if (GHOST[PINKY].x == PINKY_START_X)
			{
				GHOST[PINKY].mov = UP;
			}
			break;
		case CHASE:
			switch (GHOST[PACMAN].mov)
			{
				case LEFT:
					GHOST[PINKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE;
					GHOST[PINKY].target.col = 2 + GHOST[PACMAN].x / BLOCK_SIZE - 4;
				break;
				case RIGHT:
					GHOST[PINKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE;
					GHOST[PINKY].target.col = 2 + GHOST[PACMAN].x / BLOCK_SIZE + 4;
				break;
				case UP:
					GHOST[PINKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE - 4;
					GHOST[PINKY].target.col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;
				break;
				case DOWN:
					GHOST[PINKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE + 4;
					GHOST[PINKY].target.col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;
				break;
			}
			if (GHOST[PINKY].target.row > ROWS - 1) GHOST[PINKY].target.row = ROWS - 1;
			else if (GHOST[PINKY].target.row < 0) GHOST[PINKY].target.row = 0;
			if (GHOST[PINKY].target.col > COLS - 1) GHOST[PINKY].target.col = COLS - 1;
			else if (GHOST[PINKY].target.col < 0) GHOST[PINKY].target.col = 0;
		break;
		case SCATER:
			GHOST[PINKY].target.row = 0;
			GHOST[PINKY].target.col = 8;
		break;
		case PANIC:
		case PANIC2:
			GHOST[PINKY].target.row = rand () % ROWS;
			GHOST[PINKY].target.col = rand () % COLS;
		break;
		case EYES:
			GHOST[PINKY].target.row = 11;
			GHOST[PINKY].target.col = 15;
		break;
		case ENTER:
			if (GHOST[PINKY].y < PINKY_START_Y)
			{
				GHOST[PINKY].mov = DOWN;
			}
			else if (GHOST[PINKY].x < PINKY_START_X)
			{
				GHOST[PINKY].mov = RIGHT;
			}
			else if (GHOST[PINKY].x > PINKY_START_X)
			{
				GHOST[PINKY].mov = LEFT;
			}
			else
			{
				sprite[PINKY] = ghost[PINKY];
				GHOST[PINKY].speed = PACMAN_SPEED/2;
				GHOST[PINKY].ghost_mode = EXIT;
				GHOST[PINKY].mov = UP;
			}
		break;
	}
	switch (GHOST[INKY].ghost_mode)
	{
		case HOME:
			if (GHOST[INKY].y == INKY_START_Y - BLOCK_SIZE/2) GHOST[INKY].mov = DOWN;
			else if (GHOST[INKY].y == INKY_START_Y + BLOCK_SIZE/2) GHOST[INKY].mov = UP;
			break;
		case EXIT:
			if (GHOST[INKY].x == PINKY_START_X)
			{
				GHOST[INKY].mov = UP;
			}
			break;
		case CHASE:
			switch (GHOST[PACMAN].mov)
			{
				case LEFT:
					GHOST[INKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE;
					GHOST[INKY].target.col = GHOST[PACMAN].x / BLOCK_SIZE - 2;
				break;
				case RIGHT:
					GHOST[INKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE;
					GHOST[INKY].target.col = GHOST[PACMAN].x / BLOCK_SIZE + 2;
				break;
				case UP:
					GHOST[INKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE - 2;
					GHOST[INKY].target.col = GHOST[PACMAN].x / BLOCK_SIZE;
				break;
				case DOWN:
					GHOST[INKY].target.row = GHOST[PACMAN].y / BLOCK_SIZE + 2;
					GHOST[INKY].target.col = GHOST[PACMAN].x / BLOCK_SIZE;
				break;
			}
			GHOST[INKY].target.row = (GHOST[BLINKY].y / BLOCK_SIZE - GHOST[INKY].target.row) * 2;	// 2x distance to pacman+2 in y
			GHOST[INKY].target.col = (GHOST[BLINKY].x / BLOCK_SIZE - GHOST[INKY].target.col) * 2;	// 2x distance to pacman+2 in x
			GHOST[INKY].target.row = GHOST[BLINKY].y / BLOCK_SIZE - GHOST[INKY].target.row;	// GHOST[BLINKY] - 2x distance
			GHOST[INKY].target.col = 2 + GHOST[BLINKY].x / BLOCK_SIZE - GHOST[INKY].target.col;	// gives the target of GHOST[INKY]
			if (GHOST[INKY].target.row > ROWS - 1) GHOST[INKY].target.row = ROWS - 1;	// normalize
			else if (GHOST[INKY].target.row < 0) GHOST[INKY].target.row = 0;
			if (GHOST[INKY].target.col > COLS - 1) GHOST[INKY].target.col = COLS - 1;	// to be
			else if (GHOST[INKY].target.col < 0) GHOST[INKY].target.col = 0;	// in maze space
		break;
		case SCATER:
			GHOST[INKY].target.row = 30;
			GHOST[INKY].target.col = 24;
			break;
		case PANIC:
		case PANIC2:
			GHOST[INKY].target.row = rand () % ROWS;
			GHOST[INKY].target.col = rand () % COLS;
		break;
		case EYES:
			GHOST[INKY].target.row = 11;
			GHOST[INKY].target.col = 15;
		break;
		case ENTER:
			if (GHOST[INKY].y < PINKY_START_Y)
			{
				GHOST[INKY].mov = DOWN;
			}
			else if (GHOST[INKY].x > INKY_START_X)
			{
				GHOST[INKY].mov = LEFT;
			}
			else
			{
				sprite[INKY] = ghost[INKY];
				GHOST[INKY].speed = PACMAN_SPEED/2;
				GHOST[INKY].ghost_mode = HOME;
				GHOST[INKY].mov = UP;
			}
		break;
	}
	switch (GHOST[CLYDE].ghost_mode)
	{
		case HOME:
			if (GHOST[CLYDE].y == CLYDE_START_Y - BLOCK_SIZE/2) GHOST[CLYDE].mov = DOWN;
			else if (GHOST[CLYDE].y == CLYDE_START_Y + BLOCK_SIZE/2) GHOST[CLYDE].mov = UP;
		break;
		case EXIT:
			if (GHOST[CLYDE].x == PINKY_START_X)
			{
				GHOST[CLYDE].mov = UP;
			}
		break;
		case CHASE:
			{
				int dr = abs (GHOST[CLYDE].y - GHOST[PACMAN].y) / BLOCK_SIZE;
				int dc = abs (GHOST[CLYDE].x - GHOST[PACMAN].x) / BLOCK_SIZE;
				if (dr*dr + dc*dc > 49)	// 7 squared i.e. 7+ blocks distance
				{
					GHOST[CLYDE].target.row = GHOST[PACMAN].y / BLOCK_SIZE;
					GHOST[CLYDE].target.col = 2 + GHOST[PACMAN].x / BLOCK_SIZE;					
				}
				else	// less than 7 blocks away
				{
					GHOST[CLYDE].target.row = 30;
					GHOST[CLYDE].target.col = 8;
				}
			}
		break;
		case SCATER:
			GHOST[CLYDE].target.row = 30;
			GHOST[CLYDE].target.col = 8;
		break;
		case PANIC:
		case PANIC2:
			GHOST[CLYDE].target.row = rand () % ROWS;
			GHOST[CLYDE].target.col = rand () % COLS;
		break;
		case EYES:
			GHOST[CLYDE].target.row = 11;
			GHOST[CLYDE].target.col = 15;
		break;
		case ENTER:
			if (GHOST[CLYDE].y < PINKY_START_Y)
			{
				GHOST[CLYDE].mov = DOWN;
			}
			else if (GHOST[CLYDE].x < CLYDE_START_X)
			{
				GHOST[CLYDE].mov = RIGHT;
			}
			else
			{
				sprite[CLYDE] = ghost[CLYDE];
				GHOST[CLYDE].speed = PACMAN_SPEED/2;
				GHOST[CLYDE].ghost_mode = HOME;
				GHOST[CLYDE].mov = UP;
			}
		break;
	}
}
