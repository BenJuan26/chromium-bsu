/*
 * Copyright (c) 2000 Mark B. Allan. All rights reserved.
 *
 * "Chromium B.S.U." is free software; you can redistribute 
 * it and/or use it and/or modify it under the terms of the 
 * "Artistic License" 
 */
#include "MainGLUT.h"

#ifdef USE_GLUT

#include <stdio.h>
#include <unistd.h>

#include <GL/glut.h>

#include "Global.h"
#include "Audio.h"
#include "MenuGL.h"
#include "HeroAircraft.h"


int		MainGLUT::fire	= 0;
int		MainGLUT::xLast	= 0;
int		MainGLUT::yLast	= 0;
int		MainGLUT::xMid	= 0;
int		MainGLUT::yMid	= 0;
bool	MainGLUT::mouseToggle = false;
int		MainGLUT::last_time = 0;
int		MainGLUT::now_time = 0;

//====================================================================
MainGLUT::MainGLUT(int argc, char **argv)
	: MainToolkit(argc, argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(game->screenW, game->screenH);
	glutCreateWindow("Chromium B.S.U.");
	
	//-- Initialize OpenGL
	game->createGame();
	
	glutDisplayFunc(MainGLUT::nullFunc);
	glutReshapeFunc(MainGLUT::reshape);	
	glutKeyboardFunc(MainGLUT::keyboardASCII);	
	glutSpecialFunc(MainGLUT::keyboardSpecial);	
	glutMouseFunc(MainGLUT::mouseEvent);
//	glutMotionFunc(MainGLUT::mouseMotion);

}

MainGLUT::~MainGLUT()
{
}

//----------------------------------------------------------
bool MainGLUT::run()
{
	fprintf(stderr, "MainGLUT::run()\n");
	glutTimerFunc(100, MainGLUT::drawGame, 0);
	glutMainLoop();
	return true;
}

//----------------------------------------------------------
bool MainGLUT::checkErrors()
{
	bool retVal = false;
	GLenum	gl_error;
	
	//-- Check for GL errors
	gl_error = glGetError( );
	if( gl_error != GL_NO_ERROR ) 
	{
		fprintf(stderr, "ERROR!!! OpenGL error: %s\n", gluErrorString(gl_error) );
		retVal = true;
	}

	return retVal;
}

//----------------------------------------------------------
void MainGLUT::grabMouse(bool status)
{
	mouseToggle = status;
	if(status)
	{
		glutSetCursor(GLUT_CURSOR_NONE);
		glutMotionFunc(MainGLUT::mouseMotion);
		glutPassiveMotionFunc(MainGLUT::mouseMotion);
		xMid = game->screenW/2;
		yMid = game->screenH/2;
		glutWarpPointer(xMid, yMid);
		xLast = xMid;
		yLast = yMid;
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMotionFunc(0);
		glutPassiveMotionFunc(0);
	}
}

//----------------------------------------------------------
void MainGLUT::setVideoMode()
{
	game->setScreenSize(game->screenSize); //  set screenW & screenH for new screenSize
	glutReshapeWindow(game->screenW, game->screenH);
	game->full_screen = false;
}

//----------------------------------------------------------
void MainGLUT::reshape(int w, int h)
{
	game->mainGL->reshapeGL(w, h);
}

//----------------------------------------------------------
void MainGLUT::nullFunc()
{
	game->mainGL->drawGL();
	glutSwapBuffers();
	
	game->frame++;
	if( !(game->gameFrame%10) )
	{
		now_time = glutGet(GLUT_ELAPSED_TIME);
		if(last_time)
		{
			game->fps = (10.0/(now_time-last_time))*1000.0;
		}
		last_time = now_time;
	}
	
}

//----------------------------------------------------------
void MainGLUT::drawGame(int)
{
	glutTimerFunc(33, MainGLUT::drawGame, 0);

	game->mainGL->drawGL();
	#ifdef CHECK_ERRORS
	game->toolkit->checkErrors();
	#endif// CHECK_ERRORS
	
	glutPostRedisplay();
}

//----------------------------------------------------------
void MainGLUT::keyboardASCII(unsigned char key, int, int)
{
	switch(key)
	{
		case 'q':
		case 'Q':
			game->saveConfigFile();
			game->deleteGame();
			exit(0);
			break;
		case 'G':
		case 'g':
			game->toolkit->grabMouse(!mouseToggle);
			break;
		case 27: // <esc>
			if(game->gameMode == game->Menu)
			{
				game->gameMode = game->Game;
				game->audio->setMusicMode(Audio::MusicGame);
				game->toolkit->grabMouse(true);
			}
			else
			{
				if(game->gameMode != game->Game)
				{
					game->newGame();
				}
				game->gameMode = game->Menu;
				game->menu->startMenu();
				game->audio->setMusicMode(Audio::MusicMenu);
				game->toolkit->grabMouse(false);
			}
			break;
		default:
			if(game->gameMode == game->Menu)
			{
				MainToolkit::Key tkkey = MainToolkit::KeySpace;
				switch(key)
				{
					// up, down, etc. have to be handled with the 'special' func. Annoying.
					case 13: // <enter>
						tkkey = MainToolkit::KeyEnter;	
						break;
					default:	break;
				}
				game->menu->keyHit(tkkey);
			}
			break;
		
	}
//	fprintf(stderr, "key = %d\n", (int)key);
}

//----------------------------------------------------------
void MainGLUT::keyboardSpecial(int special, int, int)
{
	if(game->gameMode == game->Menu)
	{
		MainToolkit::Key tkkey;
		switch(special)
		{
			case GLUT_KEY_UP:		tkkey = MainToolkit::KeyUp;		break;
			case GLUT_KEY_DOWN:		tkkey = MainToolkit::KeyDown;	break;
			case GLUT_KEY_LEFT:		tkkey = MainToolkit::KeyLeft;	break;
			case GLUT_KEY_RIGHT:	tkkey = MainToolkit::KeyRight;	break;
			default: break;
		}
		game->menu->keyHit(tkkey);		
	}
}

//----------------------------------------------------------
void MainGLUT::mouseEvent(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		switch(button)
		{
			case GLUT_LEFT_BUTTON:
				game->hero->fireGun(++fire);
				break;
			case GLUT_MIDDLE_BUTTON:
				game->hero->nextItem();
				break;
			case GLUT_RIGHT_BUTTON:
				game->hero->useItem();
				break;
		}
	}
	else if(state == GLUT_UP)
	{
		switch(button)
		{
			case GLUT_LEFT_BUTTON:
				game->hero->fireGun(--fire);
				break;
			case GLUT_MIDDLE_BUTTON:
				break;
			case GLUT_RIGHT_BUTTON:
				break;
		}
	}
		
	xLast = x;
	yLast = y;
}

//----------------------------------------------------------
void MainGLUT::mouseMotion(int x, int y)
{
	int xNow;
	int yNow;
	int xDiff;
	int yDiff;
	
	if(mouseToggle)
	{
		xNow = x;
		yNow = y;
		if(xNow == xMid && yNow == yMid)
		{
			// ignore
		}
		else
		{
			xDiff =  xNow - xLast;
			yDiff =  yNow - yLast;
			if(xDiff || yDiff)
			{
				game->hero->moveEvent(xDiff, yDiff);
				glutWarpPointer(xMid, yMid);
			}
		}
		xLast = xNow;
		yLast = yNow;
	}
}


#endif // USE_GLUT
