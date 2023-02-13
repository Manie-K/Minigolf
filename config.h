#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#define BALL_FRICTION 0.005
#define MAX_POWER 150
#define MAX_HOLE_SPEED 1
#define MINIMUM_WIN_AREA_COLLISION 0.25

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 700

struct SDL_Variables {
	SDL_Window* window;
	SDL_Renderer* renderer;
};

struct Ball {
	int x, y;
	double xPosBuffer, yPosBuffer;
	double Xspeed, Yspeed;
	short int size;
	short int radius;
	bool isMoving;
	bool drawArrow;
	SDL_Rect boxModel;
	SDL_Texture* ballTexture;

	const double friction = BALL_FRICTION;
};

struct Time_Variables {
	Uint32 frameStart;
	double frameTime;
};

struct Hole {
	int x, y;
	const short int size = 30, radius = 15;
	SDL_Rect boxModel;
	SDL_Rect drawModel;
	SDL_Texture* holeTexture;
};

struct Level {
	short int levelID;
	short int levelScore;
	short int levelFreeShots;
	typedef struct {
		int x, y;
	} Point;
	Point holePos, ballPos;
	std::vector<SDL_Rect> obstacles;
};

struct Player {
	int score;
	int shotsTotal;
	int shotsPerLevel;
};