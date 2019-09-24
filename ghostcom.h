void Command_Ghost (void)
{	int i;
	struct tile target;
	for (i = 1; i < 5; i++)
	{
		int row = GHOST[i].y / BLOCK_SIZE;
		int col = 2 + GHOST[i].x / BLOCK_SIZE;
		target.row = GHOST[i].target.row;
		target.col = GHOST[i].target.col;
		if (GHOST[i].y % BLOCK_SIZE == 0)
		{
			if (ghost_space[row][col] == '+')	// here ghost.x is not divisible by BLOCK_SIZE, but by BLOCK_SIZE/2
			{
				if (GHOST[i].mov == UP)
				{
					if (sprite[i] == ghost[i])
					{
						GHOST[i].ghost_mode = (game_state == GAME_SCATER) ? SCATER : CHASE;
//						GHOST[i].ghost_mode = CHASE;
						GHOST[i].mov = LEFT;
						GHOST[i].speed = PACMAN_SPEED;
					}
					else //exits house in PANIC
					{
						GHOST[i].ghost_mode = zero.ghost_mode;
						GHOST[i].mov = LEFT;
					}
				}
				else if (GHOST[i].ghost_mode == EYES)
				{
					GHOST[i].ghost_mode = ENTER;
//					GHOST[i].y = (row) * BLOCK_SIZE; //center on door BUG
					GHOST[i].mov = DOWN;
				}
			}
			else if (GHOST[i].x % BLOCK_SIZE == 0)
			{
				if (ghost_space[row][col] == '?')
				{
					int distance = -0xffff;
					switch (GHOST[i].mov)	// precedence: UP >= LEFT >= DOWN >= RIGHT and back RIGHT > DOWN > LEFT > UP
					{
						case LEFT:
							if (ghost_space[row][col-1] != 'X') distance = (col - 1) - target.col;
							if (ghost_space[row+1][col] != 'X')
							{
								if ((target.row - (row + 1)) > distance)
								{
									distance = target.row - (row + 1);
									GHOST[i].mov = DOWN;
								}
							}
							if (ghost_space[row-1][col] != 'X')
								GHOST[i].mov = (distance > ((row - 1) - target.row)) ? GHOST[i].mov : UP;
						break;
						case RIGHT:	// new routine, shorten the longer side from the distance triangle
							if (ghost_space[row][col+1] != 'X') distance = target.col - (col + 1);
							if (ghost_space[row+1][col] != 'X')
							{
								if ((target.row - (row + 1)) >= distance)	// if < 0, pacman is above you
								{
									distance = target.row - (row + 1);
									GHOST[i].mov = DOWN;
								}
							}
							if (ghost_space[row-1][col] != 'X')
							{
								GHOST[i].mov = (distance > ((row - 1) - target.row)) ? GHOST[i].mov : UP;
							}
						break;
						case UP:
							if (ghost_space[row-1][col] != 'X') distance = (row - 1) - target.row;
							if (ghost_space[row][col-1] != 'X')
							{
								if (distance < ((col - 1) - target.col))
								{
									distance = (col - 1) - target.col;
									GHOST[i].mov = LEFT;
								}
							}
							if (ghost_space[row][col+1] != 'X')
								GHOST[i].mov = (target.col - (col + 1)) > distance ? RIGHT : GHOST[i].mov;
						break;
						case DOWN:
							if (ghost_space[row+1][col] != 'X') distance = target.row - (row + 1);
							if (ghost_space[row][col-1] != 'X')
							{
								if (((col - 1) - target.col) >= distance)
								{
									distance = (col - 1) - target.col;
									GHOST[i].mov = LEFT;
								}
							}
							if (ghost_space[row][col+1] != 'X')
								GHOST[i].mov = (target.col - (col + 1)) > distance ? RIGHT : GHOST[i].mov;
						break;
					}
				}
				else if (ghost_space[row][col] == '-')
				{
					if ((GHOST[i].ghost_mode >= PANIC))
					{
						switch (GHOST[i].mov)
						{
							case DOWN:
								GHOST[i].mov = target.col > col ? RIGHT : LEFT;
							break;
							case LEFT:
									GHOST[i].mov = ((col - 1) - target.col) > ((row - 1) - target.row) ? LEFT : UP;
							break;
							case RIGHT:
									GHOST[i].mov = (target.col - (col + 1)) > ((row - 1) - target.row) ? RIGHT : UP;
							break;
						}
					}
					else if (GHOST[i].mov == DOWN)	// in normal mode can chose which way to go only if you come from above
					{	// scater or chase
						GHOST[i].mov = (target.col < col) ? LEFT : RIGHT;	// pacman left or right
					}
				}
			}
		}
	}
}
