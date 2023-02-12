#pragma once
#include "config.h"
#include <math.h>

int initSDL(SDL_Variables& vars);
int initBall(SDL_Variables vars, Ball& ball);
void updateBall(Ball& ball, double frameTime, Hole hole);
void calculateCollisions(Ball& ball);
void shootBall(Ball& ball);
int initHole(Hole& hole, SDL_Variables vars);
void ballPositionUpdate(Ball& ball, double frameTime);
void ballSpeedUpdate(Ball& ball);
void ballBoxModelUpdate(Ball& ball);
bool checkWin(Ball& ball, Hole hole);
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int length);
void visualiseShot(Ball ball, SDL_Renderer* renderer);
void drawBackground(SDL_Renderer* renderer);

int gameStart(SDL_Variables&SDLvariables, Ball& ball, Hole& hole);
void gameRender(SDL_Variables &SDLvariables, Ball &ball, Hole &hole);
void gameHandleEvents(Ball& ball, bool& gameRunning);