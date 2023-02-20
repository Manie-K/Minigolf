#include "functions.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int initSDL(SDL_Variables& vars) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		cout << "Error while initializing SDL: " << SDL_GetError() << "\n";
		return -1;
	}

	vars.window = SDL_CreateWindow("Mini golf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!vars.window)
	{
		cout << "Error while creating WINDOW: " << SDL_GetError() << "\n";
		return -1;
	}

	vars.renderer = SDL_CreateRenderer(vars.window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!vars.renderer)
	{
		cout << "Error while creating RENDERER: " << SDL_GetError() << "\n";
		return -1;
	}
	return 0;
}
int initBall(SDL_Variables vars, Ball &ball, Level& level)
{
	const char* ballImagePath = "assets/ball.png";

	ballSet(ball, level);

	SDL_Rect temp;
	temp.x = ball.x - ball.radius;
	temp.y = ball.y - ball.radius;
	temp.w = temp.h = ball.size;
	ball.boxModel = temp;

	SDL_Surface* tempSurface = IMG_Load(ballImagePath);
	ball.ballTexture = SDL_CreateTextureFromSurface(vars.renderer, tempSurface);
	if (!tempSurface || !ball.ballTexture) {
		cout << "Error while initializing BALL: " << SDL_GetError() << "\n";
		return -1;
	}
	SDL_FreeSurface(tempSurface);

	return 0;
}
int initHole(Hole& hole, SDL_Variables vars, Level& level) 
{
	holeSetPosition(hole, level);
	const char* holeImagePath = "assets/hole.png";

	SDL_Surface* tempSurface = IMG_Load(holeImagePath);
	hole.holeTexture = SDL_CreateTextureFromSurface(vars.renderer, tempSurface);
	if (!tempSurface || !hole.holeTexture) {
		cout << "Error while initializing HOLE: " << SDL_GetError() << "\n";
		return -1;
	}
	SDL_FreeSurface(tempSurface);
	return 0;
}
void initText(SDL_Renderer* renderer, TextContainer& text) 
{
	text.font = TTF_OpenFont("assets/font.ttf", 24);

	SDL_Color color = { 255, 255, 255 };
	int w=0, h=0;

	//score
	TTF_SizeUTF8(text.font, "SCORE: ", &w, &h);
	SDL_Surface* temp = TTF_RenderText_Solid(text.font, "SCORE: ", color);
	text.staticText.score.texture = SDL_CreateTextureFromSurface(renderer, temp);
	text.staticText.score.rect = { 50,0,w,h };
	//shots
	TTF_SizeUTF8(text.font, "SHOTS: ", &w, &h);
	temp = TTF_RenderText_Solid(text.font, "SHOTS: ", color);
	text.staticText.shots.texture = SDL_CreateTextureFromSurface(renderer, temp);
	text.staticText.shots.rect = { SCREEN_WIDTH-2*w,0,w,h };
	SDL_FreeSurface(temp);
	//level
	TTF_SizeUTF8(text.font, "LEVEL: ", &w, &h);
	temp = TTF_RenderText_Solid(text.font, "LEVEL: ", color);
	text.staticText.level.texture = SDL_CreateTextureFromSurface(renderer, temp);
	text.staticText.level.rect = { SCREEN_WIDTH/2-w/2,SCREEN_HEIGHT-2*h,w,h };


	TTF_CloseFont(text.font);
	updateText(renderer, 0, "01", text);
	updateText(renderer, 1, "00", text);
	updateText(renderer, 2, "00", text);
	text.levelNum.rect = { text.staticText.level.rect.x + text.staticText.level.rect.w, text.staticText.level.rect.y, 34, 34 };
	text.shotsNum.rect = { text.staticText.shots.rect.x + text.staticText.shots.rect.w , text.staticText.shots.rect.y, 34, 34 };
	text.scoreNum.rect = { text.staticText.score.rect.x + text.staticText.score.rect.w, text.staticText.score.rect.y, 34, 34 };
	SDL_FreeSurface(temp);
}
void holeSetPosition(Hole& hole, Level level) {
	hole.x = level.holePos.x;
	hole.y = level.holePos.y;
	hole.boxModel.x = hole.drawModel.x = hole.x - hole.radius;
	hole.boxModel.y = hole.y - hole.radius;
	hole.boxModel.w = hole.boxModel.h = hole.drawModel.w = hole.size;
	hole.drawModel.h = 2 * hole.size;
	hole.drawModel.y = hole.boxModel.y - hole.size;
}
void ballSet(Ball& ball, Level level)
{
	ball.x = level.ballPos.x;
	ball.xPosBuffer = 0;

	ball.y = level.ballPos.y;
	ball.yPosBuffer = 0;

	ball.size = 24;
	ball.radius = ball.size / 2;

	ball.Xspeed = ball.Yspeed = 0;

	ball.isMoving = false;
	ball.drawArrow = false;
}

void ballPositionUpdate(Ball& ball, double frameTime) {
	int temp;

	ball.xPosBuffer += ball.Xspeed * frameTime;
	if (ball.xPosBuffer >= 0)
		temp = (int)floor(ball.xPosBuffer);
	else
		temp = (int)ceil(ball.xPosBuffer);
	ball.x += temp;
	ball.xPosBuffer -= temp;

	ball.yPosBuffer += ball.Yspeed * frameTime;
	if (ball.yPosBuffer >= 0)
		temp = (int)floor(ball.yPosBuffer);
	else
		temp = (int)ceil(ball.yPosBuffer);
	ball.y += temp;
	ball.yPosBuffer -= temp;
}
void ballSpeedUpdate(Ball& ball) {
	double cos, sin, speed;

	speed = sqrt(ball.Xspeed * ball.Xspeed + ball.Yspeed * ball.Yspeed);
	if (speed > 0) {
		cos = ball.Xspeed / speed;
		sin = ball.Yspeed / speed;
	}
	else {
		cos = sin = 0;
	}
	if (speed < -BALL_FRICTION) speed += BALL_FRICTION;
	else if (speed > BALL_FRICTION) speed -= BALL_FRICTION;
	else speed = 0;

	ball.Xspeed = speed * cos;
	ball.Yspeed = speed * sin;

	if (ball.Xspeed == 0 && ball.Yspeed == 0)
	{
		ball.isMoving = false;
	}
	else ball.isMoving = true;
}
void ballBoxModelUpdate(Ball& ball) {
	SDL_Rect tempModel;
	tempModel.w = tempModel.h = ball.size;
	tempModel.x = ball.x - ball.radius;
	tempModel.y = ball.y - ball.radius;

	ball.boxModel = tempModel;
}

void updateText(SDL_Renderer* renderer,int type, const char* message, TextContainer& text)
{
	text.font = TTF_OpenFont("assets/font.ttf", 24);
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* temp = TTF_RenderText_Solid(text.font, message, color);
	switch (type)
	{
	case 0:
		text.levelNum.texture = SDL_CreateTextureFromSurface(renderer, temp);
		break;
	case 1:
		text.scoreNum.texture = SDL_CreateTextureFromSurface(renderer, temp);
		break;
	case 2:
		text.shotsNum.texture = SDL_CreateTextureFromSurface(renderer, temp);
		break;
	default:
		break;
	}
	TTF_CloseFont(text.font);
	SDL_FreeSurface(temp);
}
void calculateCollisions(Ball& ball)
{
	if (ball.x <= ball.radius){
		ball.x = ball.size;
		ball.Xspeed *= -1;
	}
	else if (abs(SCREEN_WIDTH - ball.x) <= ball.radius) {
		ball.x = SCREEN_WIDTH-ball.size;
		ball.Xspeed *= -1;
	}
	if (ball.y <= ball.radius){
		ball.y = ball.size;
		ball.Yspeed *= -1;
	}
	else if (abs(SCREEN_HEIGHT - ball.y) <= ball.radius) {
		ball.y = SCREEN_HEIGHT - ball.size;
		ball.Yspeed *= -1;
	}
}

void shootBall(Ball& ball, Player &player, TextContainer&text,SDL_Renderer* renderer)
{	
	double power, cos, sin;
	const double powerScale = 0.01;
	int mouseX, mouseY, x, y, dX, dY;

	x = ball.x;
	y = ball.y;
	SDL_GetMouseState(&mouseX, &mouseY);

	dX = x-mouseX ;
	dY = y-mouseY;

	if (abs(dX) < ball.radius) dX = 1;
	if (abs(dY) < ball.radius) dY = 1;

	power = sqrt(dX * dX + dY * dY);

	cos = dX / power;
	sin = dY / power;

	if (power > MAX_POWER) power = MAX_POWER;

	ball.Xspeed = powerScale * power * cos;
	ball.Yspeed = powerScale * power * sin;
	ball.isMoving = true;
	ball.drawArrow = false;
	player.shotsTotal++;
	player.shotsPerLevel++;

	string shotCount = to_string(player.shotsPerLevel);
	if (player.shotsPerLevel < 10)shotCount = "0" + to_string(player.shotsPerLevel);
	updateText(renderer, 2, shotCount.c_str(), text);
}
bool checkWin(Ball& ball, Hole hole) {
	if (abs(ball.Xspeed) > MAX_HOLE_SPEED || abs(ball.Yspeed )> MAX_HOLE_SPEED) return false;
	SDL_Rect temp = { 0,0,0,0 };
	bool x = SDL_IntersectRect(&ball.boxModel, &hole.boxModel, &temp);
	double area = temp.w * temp.h;
	if (x&&area >= ball.size * ball.size * MINIMUM_WIN_AREA_COLLISION) {
		ball.x = hole.x;
		ball.y = hole.y;
		ballBoxModelUpdate(ball);
		ball.isMoving = false;
		return true;
	}
	return false;
}

void visualiseShot(Ball ball, SDL_Renderer* renderer)
{
	int mouseX, mouseY, x, y, dX, dY;

	x = ball.x;
	y = ball.y;
	SDL_GetMouseState(&mouseX, &mouseY);

	dX = mouseX-x;
	dY = mouseY-y;
	
	double cos, sin;
	double length = sqrt(dX * dX + dY * dY);

	cos = dX / length;
	sin = dY / length;

	if (length > MAX_POWER) {
		length = MAX_POWER;
		dX = int(length * cos);
		dY = int(length * sin);
	}
	double angle;
	angle = asin(sin);

	angle = angle * 180.0 / M_PI;
	angle -= 90;
	angle *= dX >= 0 ? 1 : -1;

	const int arrowWidth = 30;
	const int minimumArrowLength = 24;

	double arrowLength = length < minimumArrowLength ? 0 : length;
	SDL_Rect temp = {x-arrowWidth/2,y-arrowLength, arrowWidth, arrowLength};
	
	drawArrow(renderer, temp, angle);
}
void drawArrow(SDL_Renderer* renderer, SDL_Rect rect, double angle) {
	SDL_Surface* temp = IMG_Load("assets/arrow.png");
	SDL_Point p = { rect.w/2, rect.h };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_RenderCopyEx(renderer, texture, NULL, &rect,angle,&p, SDL_FLIP_NONE);
}
void drawBackground(SDL_Renderer* renderer) {
	const unsigned short int h = 50;
	SDL_Surface* grassSurface = IMG_Load("assets/grass.png");
	SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, grassSurface);

	SDL_Rect rect = {0,0,h,h};
	int y = 0;
	int x = 0;
	while(y < SCREEN_HEIGHT) {
		rect.y = y;
		x = 0;
		while(x < SCREEN_WIDTH) {
			rect.x = x;
			SDL_RenderCopy(renderer, temp, NULL, &rect);
			x += h;
		}
		y += h;
	}
	SDL_FreeSurface(grassSurface);
}

int gameStart(SDL_Variables& SDLvariables, Ball& ball, Hole& hole, Level& level, TextContainer &text)
{
	if (initSDL(SDLvariables) < 0) return -1;
	if (TTF_Init()<0) return -1;
	loadLevel(level);
	if (initBall(SDLvariables, ball, level) < 0) return -1;
	if (initHole(hole, SDLvariables, level) < 0) return -1;
	initText(SDLvariables.renderer, text);
	return 1;
}
//MEMORY LEAK------------------------------------
void gameRender(SDL_Variables& SDLvariables, Ball& ball, Hole& hole, Level level, TextContainer text) 
{
	SDL_SetRenderDrawColor(SDLvariables.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(SDLvariables.renderer);
	drawBackground(SDLvariables.renderer);											//draws backgorund
	drawObstacles(SDLvariables.renderer, level);									//draws obstacles
	SDL_RenderCopy(SDLvariables.renderer, hole.holeTexture, NULL, &hole.drawModel); //draws hole
	SDL_RenderCopy(SDLvariables.renderer, ball.ballTexture, NULL, &ball.boxModel);  //draws ball
	drawLevelText(level,SDLvariables.renderer, text);						//draw text
	if (ball.drawArrow)
		visualiseShot(ball, SDLvariables.renderer);									//draws arrow when shooting
	SDL_RenderPresent(SDLvariables.renderer);
}
void gameHandleEvents(Ball& ball, bool& gameRunning, Player &player, TextContainer& text, SDL_Renderer* renderer)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT && ball.isMoving == false) {
				shootBall(ball,player,text,renderer);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && ball.isMoving == false)
				ball.drawArrow = true;
			break;
		}
	}
}
void gameUpdate(Ball& ball, double frameTime, Hole& hole, Level& level, Player& player, TextContainer&text,SDL_Renderer* renderer)
{
	//ball update
	ballPositionUpdate(ball, frameTime);
	ballSpeedUpdate(ball);
	ballBoxModelUpdate(ball);

	//checks collision with screen
	calculateCollisions(ball);

	//checks collision with obstacles
	obstaclesCollision(ball, level);

	//checks win
	if (checkWin(ball, hole))
		win(renderer,level, player,text,ball,hole);
}
void win(SDL_Renderer* renderer, Level& level, Player& player, TextContainer& text, Ball& ball, Hole& hole) {
	int tempScore = level.levelScore;
	int penalty = (player.shotsPerLevel - level.levelFreeShots);
	penalty = penalty <= 0 ? 0 : penalty * 100;
	tempScore -= penalty;
	player.score += tempScore > 0 ? tempScore : 100;
	player.shotsPerLevel = 0;

	//load next level
	level.levelID++;
	string lvl;
	if (level.levelID >= 10) {
		lvl = to_string(level.levelID);
	}
	else {
		lvl = "0" + to_string(level.levelID);
	}
	string scr = to_string(player.score);

	updateText(renderer, 0, lvl.c_str(), text);
	updateText(renderer, 1, scr.c_str(), text);
	updateText(renderer, 2, "00", text);
	level.obstacles.clear();
	loadLevel(level);
	ballSet(ball, level);
	holeSetPosition(hole, level);
}
void drawObstacles(SDL_Renderer* renderer, Level level) 
{
	for (SDL_Rect obstacle : level.obstacles) {
		SDL_Surface* tempSurf = getObstacleSurface(obstacle);
		SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, tempSurf);
		SDL_RenderCopy(renderer, temp, NULL, &obstacle);
	}
}
void obstaclesCollision(Ball& ball, Level level) 
{
	for (SDL_Rect obstacle : level.obstacles)
	{
		if (circleRectangleCollision(obstacle, ball)) {
			//play sound
			break;
		}
	}
}

bool circleRectangleCollision(SDL_Rect obstacle, Ball& ball) 
{
	int halfWidth = obstacle.w / 2;
	int halfHeight = obstacle.h / 2;
	
	int x = ball.x, y = ball.y, r = ball.radius;

	int disX, disY;
	disX = abs(x - (obstacle.x + halfWidth));
	disY = abs(y - (obstacle.y + halfHeight));

	if (disX > halfWidth + r || disY > halfHeight + r) return false;

	if (pointInRectangle({x-r,y}, obstacle) || pointInRectangle({x+r,y}, obstacle)) {
		ball.Xspeed *= -1;
		if (x <= obstacle.x)
			ball.x = obstacle.x - r - 1;
		else
			ball.x = obstacle.x + obstacle.w + r + 1;
		return true;
	}
	if (pointInRectangle({ x,y-r }, obstacle) || pointInRectangle({ x,y+r }, obstacle)) {
		ball.Yspeed *= -1;

		if (y <= obstacle.y)
			ball.y = obstacle.y - r;
		else
			ball.y = obstacle.y + obstacle.h + r + 1;

		return true;
	}

	double disCorner = pow(disX - halfWidth, 2) + pow(disY-halfHeight, 2);
	if (disCorner <= pow(r, 2)) {
		ball.Xspeed *= -1;
		ball.Yspeed *= -1;
		if (x <= obstacle.x) 
		{
			if (y <= obstacle.y)
			{
				ball.x = obstacle.x - r - 1;
				ball.y = obstacle.y - r;
			}
			else
			{
				ball.x = obstacle.x - r - 1;
				ball.y = obstacle.y + obstacle.h + r + 1;
			}
		}
		else 
		{
			if (y <= obstacle.y)
			{
				ball.x = obstacle.x + obstacle.w + r + 1;
				ball.y = obstacle.y - r;
			}
			else
			{
				ball.x = obstacle.x + obstacle.w + r + 1;
				ball.y = obstacle.y + obstacle.h + r + 1;
			}
		}
		return true;
	}
	return false;
}
bool pointInRectangle(SDL_Point p, SDL_Rect rect) {
	return (p.x >= rect.x && p.x <= rect.x + rect.w && p.y >= rect.y && p.y <= rect.y + rect.h);
}
void loadLevel(Level& level) {
	string fileName = "levels/";
	if (level.levelID < 10) {
		fileName += "0";
	}
	fileName += to_string(level.levelID);	
	fileName += ".txt";

	ifstream file;
	file.open(fileName);

	while (file) {
		file >> level.levelScore >> level.levelFreeShots;
		file >> level.holePos.x >> level.holePos.y >> level.ballPos.x >> level.ballPos.y;
		int obstaclesCount;
		file >> obstaclesCount;
		for(int x = 0; x < obstaclesCount;x++) {
			SDL_Rect temp;
			file >> temp.x >> temp.y >> temp.w >> temp.h;
			level.obstacles.push_back(temp);
		}
	}

	file.close();
}

SDL_Surface* getObstacleSurface(SDL_Rect rectangle) {
	SDL_Surface* asset,* surface;

	asset = IMG_Load("assets/obstacle.png");
	int w = rectangle.w, h = rectangle.h;

	Uint32 rmask, gmask, bmask;
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;

	surface = SDL_CreateRGBSurface(0, w, h, 32, rmask,gmask,bmask, SDL_ALPHA_OPAQUE);
	
	Uint32 color = SDL_MapRGB(surface->format, 88, 47, 14);
	SDL_FillRect(surface, NULL, color);
	
	SDL_Rect destRect;
	destRect.x = destRect.y = 10;
	destRect.w = w - 20;
	destRect.h = h - 20;

	SDL_BlitScaled(asset, NULL, surface, &destRect);
	SDL_FreeSurface(asset);
	return surface;
}
void drawLevelText(Level level, SDL_Renderer* renderer, TextContainer text) {
	SDL_RenderCopy(renderer, text.staticText.level.texture, NULL, &text.staticText.level.rect);
	SDL_RenderCopy(renderer, text.staticText.score.texture, NULL, &text.staticText.score.rect);
	SDL_RenderCopy(renderer, text.staticText.shots.texture, NULL, &text.staticText.shots.rect);
	SDL_RenderCopy(renderer, text.levelNum.texture, NULL, &text.levelNum.rect);
	SDL_RenderCopy(renderer, text.scoreNum.texture, NULL, &text.scoreNum.rect);
	SDL_RenderCopy(renderer, text.shotsNum.texture, NULL, &text.shotsNum.rect);
}