#pragma once
#include "config.h"
#include <math.h>

int initSDL(SDL_Variables& vars);
int initBall(SDL_Variables vars, Ball& ball, Level& level);
void calculateCollisions(Ball& ball);
void shootBall(Ball& ball, Player& player, TextContainer& text, SDL_Renderer* renderer);
int initHole(Hole& hole, SDL_Variables vars, Level& level);
void ballPositionUpdate(Ball& ball, double frameTime);
void ballSpeedUpdate(Ball& ball);
void ballBoxModelUpdate(Ball& ball);
bool checkWin(Ball& ball, Hole hole);
void visualiseShot(Ball ball, SDL_Renderer* renderer);
void drawArrow(SDL_Renderer* renderer, SDL_Rect rect, double angle);
void drawBackground(SDL_Renderer* renderer);

int gameStart(SDL_Variables&SDLvariables, Ball& ball, Hole& hole, Level& level, TextContainer& text);
void gameRender(SDL_Variables &SDLvariables, Ball &ball, Hole &hole, Level level, TextContainer text);
void gameHandleEvents(Ball& ball, bool& gameRunning, Player& player, TextContainer& text, SDL_Renderer* renderer);
void gameUpdate(Ball& ball, double frameTime, Hole& hole, Level& level, Player& player, TextContainer& text, SDL_Renderer* renderer);


void drawObstacles(SDL_Renderer* renderer, Level level);
void obstaclesCollision(Ball& ball, Level level);
bool circleRectangleCollision(SDL_Rect obstacle, Ball& ball);
bool pointInRectangle(SDL_Point p, SDL_Rect rect);
void loadLevel(Level& level);
void win(SDL_Renderer* renderer, Level& level, Player& player, TextContainer& text, Ball& ball, Hole& hole);
void holeSetPosition(Hole& hole, Level level);
void ballSet(Ball& ball, Level level);

void initText(SDL_Renderer* renderer, TextContainer& text);
SDL_Surface* getObstacleSurface(SDL_Rect rectangle);
void drawLevelText(Level level, SDL_Renderer* renderer, TextContainer text);
void updateText(SDL_Renderer* renderer, int type, const char* message, TextContainer& text);// 0 - level, 1 - score, 2 - shots