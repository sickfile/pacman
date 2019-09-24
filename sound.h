void sound_callback (void* sounda, Uint8* buffer, int len);

int Game_Init (void)
{
	SDL_Surface* temp;

	temp = SDL_LoadBMP ("gfx/MAZE.bmp");
	if (!temp) return 1;
	background = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);

	temp = SDL_LoadBMP ("gfx/4pacman.bmp");
	if (!temp) return 2;
	sprite[PACMAN] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (sprite[PACMAN], SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/zERO.bmp");
	if (!temp) return 2;
	spritebck[PACMAN] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	
	temp = SDL_LoadBMP ("gfx/Blinky.bmp");
	if (!temp) return 3;
	ghost[BLINKY] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (ghost[BLINKY], SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/zERO.bmp");
	if (!temp) return 3;
	spritebck[BLINKY] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	
	temp = SDL_LoadBMP ("gfx/Pinky.bmp");
	if (!temp) return 4;
	ghost[PINKY] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (ghost[PINKY], SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/zERO.bmp");
	if (!temp) return 4;
	spritebck[PINKY] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);

	temp = SDL_LoadBMP ("gfx/Inky.bmp");
	if (!temp) return 5;
	ghost[INKY] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (ghost[INKY], SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/zERO.bmp");
	if (!temp) return 5;
	spritebck[INKY] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);

	temp = SDL_LoadBMP ("gfx/Clyde.bmp");
	if (!temp) return 6;
	ghost[CLYDE] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (ghost[CLYDE], SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/zERO.bmp");
	if (!temp) return 6;
	spritebck[CLYDE] = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);

	temp = SDL_LoadBMP ("gfx/bghost.bmp");
	if (!temp) return 23;
	bghost = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (bghost, SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/wbghost.bmp");
	if (!temp) return 24;
	wbghost = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (wbghost, SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/eyes.bmp");
	if (!temp) return 25;
	eyes = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (eyes, SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/eatghost.bmp");
	if (!temp) return 26;
	eatghost = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (eatghost, SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/dead.bmp");
	if (!temp) return 27;
	dead = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (dead, SDL_SRCCOLORKEY, BLACK);
	temp = SDL_LoadBMP ("gfx/fruit.bmp");
	if (!temp) return 28;
	fruit = SDL_DisplayFormat (temp);
	SDL_FreeSurface (temp);
	SDL_SetColorKey (fruit, SDL_SRCCOLORKEY, BLACK);

	charset = SDL_LoadBMP ("gfx/charset.bmp");
	if (!charset) return 8;
	SDL_SetColorKey (charset, SDL_SRCCOLORKEY, BLACK);
	
	memset (sound_buffer, 0, 10*sizeof (int));
	memset (audio_len, 0, 10*sizeof (int));
//	memset (sound_pos, 0, 10*sizeof (int)); goto start_game
	get_spec = SDL_LoadWAV ("snd/wa.wav", &obtained, &(sound_buffer[WA]), &audio_len[WA]);
	if (!get_spec) return 9;
	printf ("wa size %d wa samples %d\n", audio_len[WA], obtained.samples);
	get_spec = SDL_LoadWAV ("snd/ka.wav", &obtained, &(sound_buffer[KA]), &audio_len[KA]);
	if (!get_spec) return 10;
	printf ("ka size %d ka samples %d\n", audio_len[KA], obtained.samples);
	get_spec = SDL_LoadWAV ("snd/bau.wav", &obtained, &(sound_buffer[BEGIN]), &audio_len[BEGIN]);
	if (!get_spec) return 12;
	printf ("beginning size %d beginning samples %d\n", audio_len[BEGIN], obtained.samples);
	get_spec = SDL_LoadWAV ("snd/eatpill.wav", &obtained, &(sound_buffer[PROD]), &audio_len[PROD]);
	if (!get_spec) return 11;
	printf ("prod size %d prod samples %d\n", audio_len[PROD], obtained.samples);
	get_spec = SDL_LoadWAV ("snd/eatghost.wav", &obtained, &(sound_buffer[EATGHOST]), &audio_len[EATGHOST]);
	if (!get_spec) return 13;
	printf ("eatghost size %d wa samples %d\n", audio_len[EATGHOST], obtained.samples);
	get_spec = SDL_LoadWAV ("snd/death.wav", &obtained, &(sound_buffer[DEATH]), &audio_len[DEATH]);
	if (!get_spec) return 14;
	printf ("death size %d wa samples %d\n", audio_len[DEATH], obtained.samples);
	get_spec = SDL_LoadWAV ("snd/eatfruit.wav", &obtained, &(sound_buffer[EATFRUIT]), &audio_len[EATFRUIT]);
	if (!get_spec) return 15;
	printf ("eatfruit size %d wa samples %d\n", audio_len[EATFRUIT], obtained.samples);

	get_spec->callback = sound_callback;
	get_spec->userdata = (void*)&sound;
	get_spec->samples = 384;

	if (SDL_OpenAudio (get_spec, &obtained) == -1) return 22;
	SDL_PauseAudio (1);
	return 0;
}

void sound_callback (void* userdata, Uint8* buffer, int len)
{
	int i = 0;
//	printf ("%d\t%d\t%d\t%d\n", sound, sound_pos[sound], audio_len[sound], SDL_GetTicks ());
	if (sound == WA)	// zero is WA sound
	{
		for (; (i < len) && (sound_pos[WA] < audio_len[WA]); i++, sound_pos[WA]++)
		{
			buffer[i] = sound_buffer[WA][sound_pos[WA]];
	
		}
		if (sound_pos[WA] >= audio_len[WA])
		{
			SDL_PauseAudio (1);
			sound = KA;
		}
	}
	else if (sound == KA)	// 1 KA sound
	{
		for (; (i < len) && (sound_pos[KA] < audio_len[KA]); i++, sound_pos[KA]++)
		{
			buffer[i] = sound_buffer[KA][sound_pos[KA]];
	
		}
		if (sound_pos[KA] >= audio_len[KA])
		{
			SDL_PauseAudio (1);
			sound = WA;
		}
	}
	else if (sound == BEGIN)	// 2 BEGIN sound
	{
		for (; (i < len) && (sound_pos[BEGIN] < audio_len[BEGIN]); i++, sound_pos[BEGIN]++)
		{
			buffer[i] = sound_buffer[BEGIN][sound_pos[BEGIN]];
	
		}
		if (sound_pos[BEGIN] >= audio_len[BEGIN])
		{
			SDL_PauseAudio (1);
			sound = WA;
		}
	}
	else if (sound == EATGHOST)
	{
		for (; (i < len) && (sound_pos[EATGHOST] < audio_len[EATGHOST]); i++, sound_pos[EATGHOST]++)
		{
			buffer[i] = sound_buffer[EATGHOST][sound_pos[EATGHOST]];
	
		}
		if (sound_pos[EATGHOST] >= audio_len[EATGHOST])
		{
			SDL_PauseAudio (1);
			sound = WA;
		}
	}
	else if (sound == DEATH)
	{
		for (; (i < len) && (sound_pos[DEATH] < audio_len[DEATH]); i++, sound_pos[DEATH]++)
		{
			buffer[i] = sound_buffer[DEATH][sound_pos[DEATH]];
	
		}
		if (sound_pos[DEATH] >= audio_len[DEATH])
		{
			SDL_PauseAudio (1);
			sound = WA;
		}
	}
	else if (sound == EATFRUIT)
	{
		for (; (i < len) && (sound_pos[EATFRUIT] < audio_len[EATFRUIT]); i++, sound_pos[EATFRUIT]++)
		{
			buffer[i] = sound_buffer[EATFRUIT][sound_pos[EATFRUIT]];
	
		}
		if (sound_pos[EATFRUIT] >= audio_len[EATFRUIT])
		{
			SDL_PauseAudio (1);
			sound = WA;
		}
	}
	else	// PROD sound is always last
	{
		for (; (i < len) && (sound_pos[PROD] < audio_len[PROD]); i++, sound_pos[PROD]++)
		{
			buffer[i] = sound_buffer[PROD][sound_pos[PROD]];
	
		}
		if (sound_pos[PROD] >= audio_len[PROD])
		{
			SDL_PauseAudio (1);
			sound = WA;
		}
	}

}
