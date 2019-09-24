void Mov_Ghost (void)
{
	int row, col, i;
	zero.x += PACMAN_SPEED;
	if (zero.x % BLOCK_SIZE == 0)
	{
		zero.pict = zero.pict ? 0 : 1;
		zero.x = 0;
	}
	for (i = 1; i < 5; i++)
	{
		row = GHOST[i].y / BLOCK_SIZE;
		col = 2 + GHOST[i].x / BLOCK_SIZE;
		switch (GHOST[i].mov)
		{
			case LEFT:
				if (GHOST[i].x % BLOCK_SIZE == 0)
				{
					if (14 == row)
					{
						if (col == 0)	// teleport
						{
							GHOST[i].x = (COLS-3)*BLOCK_SIZE;
						}
						else if (GHOST[i].ghost_mode > SCATER);
						else if (5 == col)
						{
							GHOST[i].speed = PACMAN_SPEED / 2;
						}
						else if (26 == col)
						{
							GHOST[i].speed = PACMAN_SPEED;
						}
					}
					if (ghost_space[row][col-1] != 'X')
					{
						GHOST[i].x -= GHOST[i].speed;
					}
					else if (ghost_space[row-1][col] != 'X')
					{
						GHOST[i].mov = UP;
					}
					else if (ghost_space[row+1][col] != 'X')
					{
						GHOST[i].mov = DOWN;
					}
				}
				else	// move only if not GHOST[i].x % BLOCK_SIZE == 0
				{
					GHOST[i].x -= GHOST[i].speed;
				}
			break;
			case RIGHT:
				if (GHOST[i].x % BLOCK_SIZE == 0)
				{
					if (14 == row)
					{
						if (col == COLS-1)	// teleport
						{
							GHOST[i].x = -2*BLOCK_SIZE;
						}
						else if (GHOST[i].ghost_mode > SCATER);
						else if (5 == col)
						{
							GHOST[i].speed = PACMAN_SPEED;
						}
						else if (26 == col)
						{
							GHOST[i].speed = PACMAN_SPEED / 2;
						}
					}
					if (ghost_space[row][col+1] != 'X')
					{
						GHOST[i].x += GHOST[i].speed;
					}
					else if (ghost_space[row-1][col] != 'X')
					{
						GHOST[i].mov = UP;
					}
					else if (ghost_space[row+1][col] != 'X')
					{
						GHOST[i].mov = DOWN;
					}
				}
				else
				{
						GHOST[i].x += GHOST[i].speed;
				}
			break;
			case UP:
				if (GHOST[i].y % BLOCK_SIZE == 0)
				{
					if (ghost_space[row-1][col] != 'X')
					{
						GHOST[i].y -= GHOST[i].speed;
					}
					else if (ghost_space[row][col-1] != 'X')
					{
						GHOST[i].mov = LEFT;
					}
					else if (ghost_space[row][col+1] != 'X')
					{
						GHOST[i].mov = RIGHT;
					}
				}
				else
				{
					GHOST[i].y -= GHOST[i].speed;
				}
			break;
			case DOWN:
				if (GHOST[i].y % BLOCK_SIZE == 0)
				{
					if (ghost_space[row+1][col] != 'X')
					{
						GHOST[i].y += GHOST[i].speed;
					}
					else if (ghost_space[row][col-1] != 'X')
					{
						GHOST[i].mov = LEFT;
					}
					else if (ghost_space[row][col+1] != 'X')
					{
						GHOST[i].mov = RIGHT;
					}
				}
				else
				{
					GHOST[i].y += GHOST[i].speed;
				}
			break;
		}
	}
}
