/*
 * Copyright (c) 2000 Mark B. Allan. All rights reserved.
 *
 * "Chromium B.S.U." is free software; you can redistribute 
 * it and/or use it and/or modify it under the terms of the 
 * "Artistic License" 
 */
#ifndef Global_h
#define Global_h

#include <stdio.h>
#include <time.h>

#include "define.h"
#include "TexFont.h"
#include "MainToolkit.h"

class HeroAircraft;
class HeroAmmo;
class EnemyFleet;
class EnemyAmmo;
class Explosions;
class PowerUps;
class Audio;
class Ground;
class MenuGL;
class MainGL;
class StatusDisplay;
class ScreenItemAdd;

//====================================================================
class Global
{
public:
	enum GameMode { Game, Menu, LevelOver, HeroDead };
	
	~Global();
	
	static Global	*init();
	static Global	*getInstance();
	static void		destroy();
	
	void		newGame();
	void		gotoNextLevel();
	
	static void	generateRandom(bool r = true);
	
	void		createGame();
	void		deleteGame();
	
	void		deleteTextures();
	void		loadTextures();
	
	static MainToolkit *toolkit;
	
	static float 	mouseSpeed;
	static bool 	mouseActive;
	
	static TexFont	*texFont;

	static float	fps;
	static int		frame;
	static int		gameFrame;
	static float	gameSpeed;
	static float	gameSkillBase;
	static float	gameSkill;
	static int		gameLevel;
	static int		maxLevel;
	static int		gfxLevel;
	static float	viewGamma;
	static float	speedAdj;
	
	static float	scrollSpeed;
		
	bool	 game_pause;
	bool	 game_quit;
	
	static float	volSound;
	static float	volMusic;
	
	static int		heroDeath;
	static int		heroSuccess;
	
	static GameMode	gameMode;
	
	static HeroAircraft	*hero;
	static EnemyFleet	*enemyFleet;
	static HeroAmmo		*heroAmmo;
	static EnemyAmmo	*enemyAmmo;
	static ScreenItemAdd *itemAdd;
	
	static Explosions	*explosions;
	static PowerUps		*powerUps;
	static Audio		*audio;
	static Ground		*ground;
	static Ground		*groundGame;
	static Ground		*groundMenu;
	static MenuGL		*menu;
	static MainGL		*mainGL;
	static StatusDisplay *statusDisplay;
	
	static int		randI[256];
	static float	randF[256];
	static float	randS[256];
	static int		randIndex;
	
	static FILE	*eventFile;
	
	static int tipShipPast;
	static int tipSuperShield;

private:
	static Global	*instance;
	
	Global();
};

#endif // Global_h
