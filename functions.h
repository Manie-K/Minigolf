#pragma once
#include "config.h"
#include <math.h>
#include <string>

int initSDL(SDL_Variables& vars);
int initBall(SDL_Variables vars, Ball& ball, Level& level);
void initMenu(SDL_Renderer* renderer, Menu& menu);
void calculateCollisions(Ball& ball, Sounds sounds);
void shootBall(Ball& ball, Player& player, TextContainer& text, SDL_Renderer* renderer, Sounds& sounds);
int initHole(Hole& hole, SDL_Variables vars, Level& level);
void ballPositionUpdate(Ball& ball, double frameTime);
void ballSpeedUpdate(Ball& ball);
void ballBoxModelUpdate(Ball& ball);
bool checkWin(Ball& ball, Hole hole);
void visualiseShot(Ball ball, SDL_Renderer* renderer);
void drawArrow(SDL_Renderer* renderer, SDL_Rect rect, double angle);
void drawBackground(SDL_Renderer* renderer);

int gameStart(SDL_Variables&SDLvariables, Ball& ball, Hole& hole, Level& level, TextContainer& text, Menu& menu, Sounds& sounds);
void gameRender(SDL_Variables &SDLvariables, Ball &ball, Hole &hole, Level level, TextContainer text);
void gameHandleEvents(Ball& ball, bool& gameRunning, Player& player, TextContainer& text, SDL_Renderer* renderer, Sounds sounds);
void gameUpdate(Ball& ball, double frameTime, Hole& hole, Level& level, 
	Player& player, TextContainer& text, SDL_Renderer* renderer, Menu& menu, bool& gameRunning, Sounds sounds);


void drawObstacles(SDL_Renderer* renderer, Level level);
void obstaclesCollision(Ball& ball, Level level, Sounds sounds);
bool circleRectangleCollision(SDL_Rect obstacle, Ball& ball);
bool pointInRectangle(SDL_Point p, SDL_Rect rect);
void loadLevel(Level& level);
void win(SDL_Renderer* renderer, Level& level, Player& player, TextContainer& text, Ball& ball, Hole& hole, Menu& menu, bool& gameRunning);
void holeSetPosition(Hole& hole, Level level);
void ballSet(Ball& ball, Level level);

void initText(SDL_Renderer* renderer, TextContainer& text);
SDL_Surface* getObstacleSurface(SDL_Rect rectangle);
void drawLevelText(Level level, SDL_Renderer* renderer, TextContainer text);
void updateText(SDL_Renderer* renderer, int type, const char* message, TextContainer& text);// 0 - level, 1 - score, 2 - shots

void calculatePlayerScore(int& tempScore, Player& player,Level level);
bool goToNextLevel(SDL_Renderer* renderer, TextContainer& text, Ball& ball, Hole& hole, Level& level, Player& player);
void showNextLevelMenu(SDL_Renderer* renderer, Menu& menu, Player& player, int tempScore);
void renderStars(SDL_Renderer* renderer, Menu& menu, const int numOfStars);
void renderMenuText(SDL_Renderer* renderer, Player& player, int tempScore);

void gameCleanUp(SDL_Variables& SDLvariables, Sounds &sounds);
void finishGame(SDL_Renderer* renderer, Player player);
void renderText(SDL_Renderer* renderer, SDL_Texture*& texture, std::string message, SDL_Rect& rect, int x, int y, TTF_Font* font, SDL_Color color);
void initSounds(Sounds& sounds);