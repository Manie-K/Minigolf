#pragma once
#include "config.h"
#include <math.h>

int initSDL(SDL_Variables& vars);
int initBall(SDL_Variables vars, Ball& ball, Level& level);
void calculateCollisions(Ball& ball);
void shootBall(Ball& ball, Player& player);
int initHole(Hole& hole, SDL_Variables vars, Level& level);
void ballPositionUpdate(Ball& ball, double frameTime);
void ballSpeedUpdate(Ball& ball);
void ballBoxModelUpdate(Ball& ball);
bool checkWin(Ball& ball, Hole hole);
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int length);
void visualiseShot(Ball ball, SDL_Renderer* renderer);
void drawArrow(SDL_Renderer* renderer, SDL_Rect rect, double angle);
void drawBackground(SDL_Renderer* renderer);

int gameStart(SDL_Variables&SDLvariables, Ball& ball, Hole& hole, Level& level);
void gameRender(SDL_Variables &SDLvariables, Ball &ball, Hole &hole, Level level);
void gameHandleEvents(Ball& ball, bool& gameRunning, Player& player);
void gameUpdate(Ball& ball, double frameTime, Hole& hole, Level& level, Player& player);


void drawObstacles(SDL_Renderer* renderer, Level level);
void obstaclesCollision(Ball& ball, Level level);
bool circleRectangleCollision(SDL_Rect obstacle, Ball& ball);
bool pointInRectangle(SDL_Point p, SDL_Rect rect);
void loadLevel(Level& level);

void holeSetPosition(Hole& hole, Level level);
void ballSet(Ball& ball, Level level);

SDL_Surface* getObstacleSurface(SDL_Rect rectangle);