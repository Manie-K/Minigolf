#include "functions.h"
#include <iostream>
#include <fstream>

using namespace std;

int initSDL(SDL_Variables& vars) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
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
int initMenu(SDL_Renderer* renderer, Menu& menu) 
{
	SDL_Surface* tempSurf = IMG_Load("assets/menu.png");
	menu.menuTexture = SDL_CreateTextureFromSurface(renderer, tempSurf);
	SDL_FreeSurface(tempSurf);

	tempSurf = IMG_Load("assets/star.png");
	menu.star.texture = SDL_CreateTextureFromSurface(renderer, tempSurf);
	SDL_FreeSurface(tempSurf);

	menu.star.rect.w = menu.star.rect.h = 100;

	if (menu.star.texture==NULL || menu.menuTexture == NULL) {
		cout << "Error while initializing MENU: " << SDL_GetError() << "\n";
		return -1;
	}
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
int initSounds(Sounds& sounds)
{
	sounds.collision = Mix_LoadWAV("assets/collision.wav");
	sounds.shot = Mix_LoadWAV("assets/shot.wav");
	sounds.win = Mix_LoadMUS("assets/win.mp3");
	if (sounds.collision == NULL || sounds.shot == NULL || sounds.win == NULL) {
		cout << "Error while initializing SOUNDS\n";
		return -1;
	}
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

int gameStart(SDL_Variables& SDLvariables, Ball& ball, Hole& hole, Level& level, TextContainer& text, Menu& menu, Sounds& sounds)
{
	if (initSDL(SDLvariables) < 0) return -1;
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) return -1;
	if(initSounds(sounds)<0) return-1;
	if (TTF_Init() < 0) return -1;
	loadLevel(level);
	if (initBall(SDLvariables, ball, level) < 0) return -1;
	if (initHole(hole, SDLvariables, level) < 0) return -1;
	if(initMenu(SDLvariables.renderer, menu)<0)return -1;
	initText(SDLvariables.renderer, text);
	return 1;
}
void gameRender(SDL_Variables& SDLvariables, Ball& ball, Hole& hole, Level level, TextContainer text)
{
	drawBackground(SDLvariables.renderer);											//draws background
	drawObstacles(SDLvariables.renderer, level);									//draws obstacles
	SDL_RenderCopy(SDLvariables.renderer, hole.holeTexture, NULL, &hole.drawModel); //draws hole
	SDL_RenderCopy(SDLvariables.renderer, ball.ballTexture, NULL, &ball.boxModel);  //draws ball
	drawLevelText(level, SDLvariables.renderer, text);								//draw text
	if (ball.drawArrow)
		visualiseShot(ball, SDLvariables.renderer);									//draws arrow when shooting
	SDL_RenderPresent(SDLvariables.renderer);
}
void gameHandleEvents(Ball& ball, bool& gameRunning, Player& player, TextContainer& text, SDL_Renderer* renderer, Sounds sounds)
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
				shootBall(ball, player, text, renderer, sounds);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && ball.isMoving == false)
				ball.drawArrow = true;
			break;
		}
	}
}
void gameUpdate(Ball& ball, double frameTime, Hole& hole, Level& level, Player& player, TextContainer& text,
	SDL_Renderer* renderer, Menu& menu, bool& gameRunning, Sounds sounds)
{
	//ball update
	ballPositionUpdate(ball, frameTime);
	ballSpeedUpdate(ball);
	ballBoxModelUpdate(ball);

	//checks collision with screen
	calculateCollisions(ball, sounds);

	//checks collision with obstacles
	obstaclesCollision(ball, level, sounds);

	//checks win
	if (checkWin(ball, hole)) {
		Mix_PlayMusic(sounds.win, 0);
		win(renderer, level, player, text, ball, hole, menu, gameRunning);
	}
}

void gameCleanUp(SDL_Variables& SDLvariables, Sounds& sounds, Ball& ball, Hole& hole, Menu& menu, TextContainer& txt)
{
	textCleanUp(txt);
	SDL_DestroyWindow(SDLvariables.window);
	SDL_DestroyRenderer(SDLvariables.renderer);
	Mix_FreeChunk(sounds.collision);
	Mix_FreeChunk(sounds.shot);
	Mix_FreeMusic(sounds.win);
	SDL_DestroyTexture(ball.ballTexture);
	SDL_DestroyTexture(hole.holeTexture);
	SDL_DestroyTexture(menu.star.texture);
	SDL_DestroyTexture(menu.menuTexture);

	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}
void textCleanUp(TextContainer& txt)
{
	SDL_DestroyTexture(txt.levelNum.texture);
	SDL_DestroyTexture(txt.scoreNum.texture);
	SDL_DestroyTexture(txt.shotsNum.texture);
	SDL_DestroyTexture(txt.staticText.level.texture);
	SDL_DestroyTexture(txt.staticText.score.texture);
	SDL_DestroyTexture(txt.staticText.shots.texture);
}
void finishGame(SDL_Renderer* renderer, Player player)
{
	TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
	const int size = 50;
	SDL_Color color = { 97, 39, 39 };
	string message;
	SDL_Texture* texture;
	SDL_Rect rect = { 0,0,size,size };

	SDL_Surface* temp = IMG_Load("assets/finish.png");
	texture = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	message = to_string(player.score);
	renderText(renderer, texture, message, rect, 460, 215, font, color);

	message = to_string(player.shotsTotal);
	renderText(renderer, texture, message, rect, 460, 300, font, color);

	SDL_RenderPresent(renderer);

	bool spacePressed = false;
	while (!spacePressed) {
		SDL_Event e;
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_ESCAPE)))
				spacePressed = true;
	}
	SDL_Delay(250);
	SDL_DestroyTexture(texture);
}

void visualiseShot(Ball ball, SDL_Renderer* renderer)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int x = ball.x;
	int y = ball.y;
	int dX = mouseX - x;
	int dY = mouseY - y;

	double cos, sin,length = sqrt(dX * dX + dY * dY);

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
	angle *= (dX >= 0) ? 1 : -1;

	const int arrowWidth = 30;
	const int minimumArrowLength = 24;

	int len = round(length);
	int arrowLength = len < minimumArrowLength ? 0 : len;
	SDL_Rect temp = { x - arrowWidth / 2,y - arrowLength, arrowWidth, arrowLength };

	drawArrow(renderer, temp, angle);
}
void drawArrow(SDL_Renderer* renderer, SDL_Rect rect, double angle) {
	SDL_Surface* temp = IMG_Load("assets/arrow.png");
	SDL_Point p = { rect.w / 2, rect.h };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &p, SDL_FLIP_NONE);
	SDL_FreeSurface(temp);
	SDL_DestroyTexture(texture);
}
void drawBackground(SDL_Renderer* renderer) {
	const unsigned short int h = 50;
	SDL_Surface* grassSurface = IMG_Load("assets/grass.png");
	SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, grassSurface);
	SDL_FreeSurface(grassSurface);
	SDL_Rect rect = { 0,0,h,h };
	int y = 0;
	int x = 0;
	while (y < SCREEN_HEIGHT) {
		rect.y = y;
		x = 0;
		while (x < SCREEN_WIDTH) {
			rect.x = x;
			SDL_RenderCopy(renderer, temp, NULL, &rect);
			x += h;
		}
		y += h;
	}
	SDL_DestroyTexture(temp);
}
void drawObstacles(SDL_Renderer* renderer, Level level)
{
	for (SDL_Rect obstacle : level.obstacles) {
		SDL_Surface* tempSurf = getObstacleSurface(obstacle);
		SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, tempSurf);
		SDL_RenderCopy(renderer, temp, NULL, &obstacle);
		SDL_FreeSurface(tempSurf);
		SDL_DestroyTexture(temp);
	}
}
SDL_Surface* getObstacleSurface(SDL_Rect rectangle) 
{
	SDL_Surface* asset, * surface;

	asset = IMG_Load("assets/obstacle.png");
	int w = rectangle.w, h = rectangle.h;

	Uint32 rmask, gmask, bmask;
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;

	surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, SDL_ALPHA_OPAQUE);

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

void shootBall(Ball& ball, Player& player, TextContainer& text, SDL_Renderer* renderer, Sounds& sounds)
{
	//play sound
	Mix_PlayChannel(-1, sounds.shot, 0);

	double power, cos, sin;
	const double powerScale = 0.01;
	int mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);
	int x = ball.x;
	int y = ball.y;
	int dX = x - mouseX;
	int dY = y - mouseY;

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
void ballPositionUpdate(Ball& ball, double frameTime) 
{
	int temp;
	double tempPos;
	//x
	tempPos= ball.Xspeed * frameTime;
	if (tempPos > ball.radius) 
		tempPos = ball.radius;
	
	ball.xPosBuffer += tempPos;
	if (ball.xPosBuffer >= 0)
		temp = (int)floor(ball.xPosBuffer);
	else
		temp = (int)ceil(ball.xPosBuffer);
	if (temp >= ball.radius)
		temp = ball.radius - 1;
	ball.x += temp;
	ball.xPosBuffer -= temp;
	
	//y
	tempPos = ball.Yspeed * frameTime;
	if (tempPos > ball.radius)
		tempPos = ball.radius;
	
	ball.yPosBuffer += tempPos;
	if (ball.yPosBuffer >= 0)
		temp = (int)floor(ball.yPosBuffer);
	else
		temp = (int)ceil(ball.yPosBuffer);
	if (temp >= ball.radius)
		temp = ball.radius - 1;
	ball.y += temp;
	ball.yPosBuffer -= temp;
}
void ballSpeedUpdate(Ball& ball) 
{
	double cos, sin, speed;

	speed = sqrt(ball.Xspeed * ball.Xspeed + ball.Yspeed * ball.Yspeed);
	if (speed > 0) {
		cos = ball.Xspeed / speed;
		sin = ball.Yspeed / speed;
	}
	else
		cos = sin = 0;
	if (speed < -BALL_FRICTION) speed += BALL_FRICTION;
	else if (speed > BALL_FRICTION) speed -= BALL_FRICTION;
	else speed = 0;

	ball.Xspeed = speed * cos;
	ball.Yspeed = speed * sin;

	if (ball.Xspeed == 0 && ball.Yspeed == 0)
		ball.isMoving = false;
	else 
		ball.isMoving = true;
}
void ballBoxModelUpdate(Ball& ball) 
{
	SDL_Rect tempModel;
	tempModel.w = tempModel.h = ball.size;
	tempModel.x = ball.x - ball.radius;
	tempModel.y = ball.y - ball.radius;

	ball.boxModel = tempModel;
}

void calculateCollisions(Ball& ball,Sounds sounds)
{
	bool coll = false;
	if (ball.x <= ball.radius){
		ball.x = ball.size;
		ball.Xspeed *= -1;
		coll = true;
	}
	else if (abs(SCREEN_WIDTH - ball.x) <= ball.radius) {
		ball.x = SCREEN_WIDTH-ball.size;
		ball.Xspeed *= -1;
		coll = true;
	}
	if (ball.y <= ball.radius){
		ball.y = ball.size;
		ball.Yspeed *= -1;
		coll = true;
	}
	else if (abs(SCREEN_HEIGHT - ball.y) <= ball.radius) {
		ball.y = SCREEN_HEIGHT - ball.size;
		ball.Yspeed *= -1;
		coll = true;
	}
	if (coll)
		Mix_PlayChannel(-1, sounds.collision, 0);
}
void obstaclesCollision(Ball& ball, Level level, Sounds sounds)
{
	for (SDL_Rect obstacle : level.obstacles)
	{
		if (circleRectangleCollision(obstacle, ball)) {
			//play sound
			Mix_PlayChannel(-1, sounds.collision, 0);
			break;
		}
	}
}
bool circleRectangleCollision(SDL_Rect obstacle, Ball& ball)
{
	int x = ball.x, y = ball.y, r = ball.radius;
	int w = obstacle.w, h = obstacle.h;
	SDL_Point topLeft = { obstacle.x,obstacle.y };
	SDL_Point topRight = { obstacle.x + w,obstacle.y };
	SDL_Point bottomRight = { obstacle.x + w,obstacle.y + h };
	SDL_Point bottomLeft = { obstacle.x,obstacle.y + h };
	SDL_Point center = { obstacle.x + w / 2,obstacle.y + h / 2 };

	int dx = abs(x - center.x);
	int dy = abs(y - center.y);

	if (dx > w / 2 + r || dy > h / 2 + r)
		return false;

	//top/bottom collision
	if (x > topLeft.x && x < topRight.x) {
		if (y<topLeft.y || y > bottomLeft.y)
		{
			ball.Yspeed = -ball.Yspeed;
			if (y < topLeft.y)
				ball.y = topLeft.y - r - 1;
			else if (y > bottomLeft.y)
				ball.y = bottomLeft.y + r + 1;
			return true;
		}
	}
	//side collision
	if (y > topLeft.y && y < bottomLeft.y) {
		if (x < topLeft.x || x > topRight.x)
		{
			ball.Xspeed = -ball.Xspeed;
			if (x < topLeft.x)
				ball.x = topLeft.x - r - 1;
			else if (x > topRight.x)
				ball.x = topRight.x + r + 1;
			return true;
		}
	}
	dx = dy = 0;
	if (ball.Xspeed != 0)
	{
		bool x = cornerCollision(ball, obstacle);
		if (x) return true;
	}
	else {
		//vertical
		if (y - bottomLeft.y <= r || topLeft.y - y <= r) {
			ball.Yspeed = -ball.Yspeed;
			if (y < topLeft.y) {
				ball.y = topLeft.y - r - 1;
			}
			else if (y > bottomLeft.y) {
				ball.y = bottomLeft.y + r + 1;
			}
			return true;
		}
	}

	return false;
}
bool cornerCollision(Ball& ball, SDL_Rect& obstacle)
{
	int x = ball.x, y = ball.y, r = ball.radius;
	int w = obstacle.w, h = obstacle.h;
	SDL_Point topLeft = { obstacle.x,obstacle.y };
	SDL_Point topRight = { obstacle.x + w,obstacle.y };
	SDL_Point bottomRight = { obstacle.x + w,obstacle.y + h };
	SDL_Point bottomLeft = { obstacle.x,obstacle.y + h };
	/*
	   11|12  1|2
		---------
	   10|     |3
		9|     |4
		---------
		8|7   6|5
	*/

	//0 - ball stationary
	if (ball.Xspeed == 0 && ball.Yspeed == 0)
		return false;
	//1
	if (ball.Xspeed >= 0 && x > topLeft.x + w / 2 && x <= topRight.x) {
		if (y <= topLeft.y && y >= topLeft.y - r) {
			ball.Yspeed = -ball.Yspeed;
			ball.y = topLeft.y - r - 1;
			return true;
		}
	}
	//2
	if (ball.Xspeed <= 0 && ball.Yspeed >= 0 && x >= topRight.x && x <= topRight.x + r) {
		if (y <= topLeft.y && y >= topLeft.y - r) {
			ball.Yspeed = -ball.Yspeed;
			ball.Xspeed = -ball.Xspeed;
			ball.x = topRight.x + r + 1;
			ball.y = topRight.y - r - 1;
			return true;
		}
	}
	//3
	if (ball.Yspeed <= 0 && y >= topRight.y && y < topRight.y + h / 2) {
		if (x >= topRight.x && x <= topRight.x + r)
		{
			ball.Xspeed = -ball.Xspeed;
			ball.x = topRight.x + r + 1;
			return true;
		}
	}
	//4
	if (ball.Yspeed >= 0 && y > topRight.y + r && y <= bottomRight.y) {
		if (x >= topRight.x && x <= topRight.x + r) {
			ball.Xspeed = -ball.Xspeed;
			ball.x = bottomRight.x + r + 1;
			return true;
		}
	}
	//5
	if (ball.Xspeed <= 0 && ball.Yspeed <= 0 && x >= bottomRight.x && x <= topRight.x + r) {
		if (y >= bottomRight.y && y <= bottomRight.y + r) {
			ball.Xspeed = -ball.Xspeed;
			ball.Yspeed = -ball.Yspeed;
			ball.x = bottomRight.x + r + 1;
			ball.y = bottomRight.y + r + 1;
			return true;
		}
	}
	//6
	if (ball.Xspeed >= 0 && x > topLeft.x + w / 2 && x <= topRight.x) {
		if (y >= bottomLeft.y && y <= bottomLeft.y + r) {
			ball.Yspeed = -ball.Yspeed;
			ball.y = bottomLeft.y + r + 1;
			return true;
		}
	}
	//7
	if (ball.Xspeed <= 0 && x >= topLeft.x - r && x <= topLeft.x) {
		if (y >= bottomLeft.y && y <= bottomLeft.y + r) {
			ball.Yspeed = -ball.Yspeed;
			ball.y = bottomLeft.y + r + 1;
			return true;
		}
	}
	//8
	if (ball.Xspeed >= 0 && ball.Yspeed <= 0 && x >= topLeft.x - r && x <= topLeft.x) {
		if (y >= bottomLeft.y && y <= bottomLeft.y + r) {
			ball.Xspeed = -ball.Xspeed;
			ball.Yspeed = -ball.Yspeed;
			ball.x = topLeft.x - r - 1;
			ball.y = bottomLeft.y + r + 1;
			return true;
		}
	}
	//9
	if (ball.Yspeed >= 0 && y <= bottomLeft.y && y > bottomLeft.y - h / 2) {
		if (x >= topLeft.x - r && x <= topLeft.x) {
			ball.Xspeed = -ball.Xspeed;
			ball.x = topLeft.x - r - 1;
			return true;
		}
	}
	//10
	if (ball.Yspeed <= 0 && y < bottomLeft.y - h / 2 && y >= topLeft.y) {
		if (x >= topLeft.x - r && x <= topLeft.x) {
			ball.Xspeed = -ball.Xspeed;
			ball.x = topLeft.x - r - 1;
			return true;
		}
	}
	//11
	if (ball.Xspeed >= 0 && ball.Yspeed >= 0 && x >= topLeft.x - r && x <= topLeft.x) {
		if (y <= topLeft.y && y >= topLeft.y - r) {
			ball.Xspeed = -ball.Xspeed;
			ball.Yspeed = -ball.Yspeed;
			ball.x = topLeft.x - r - 1;
			ball.y = topLeft.y - r - 1;
			return true;
		}
	}
	//12
	if (ball.Xspeed <= 0 && x >= topLeft.x && x < topLeft.x + w / 2) {
		if (y >= topLeft.y - r && y <= topLeft.y) {
			ball.Yspeed = -ball.Yspeed;
			ball.y = topLeft.y - r - 1;
			return true;
		}
	}
	return false;
}

bool checkWin(Ball& ball, Hole hole) 
{
	if (abs(ball.Xspeed) > MAX_HOLE_SPEED || abs(ball.Yspeed )> MAX_HOLE_SPEED) return false;
	
	int x1 = ball.x, y1=ball.y, x2=hole.x, y2=hole.y;
	int dx = x1 - x2, dy = y1 - y2;
	double distance = sqrt(dx*dx+dy*dy);

	if (abs(distance) < MINIMUM_WIN_AREA_COLLISION)
		return true;
	return false;
}
void calculatePlayerScore(int& tempScore, Player& player, Level level)
{
	tempScore = level.levelScore;
	int penalty = (player.shotsPerLevel - level.levelFreeShots) * 50;
	tempScore = max(tempScore - penalty, 100);
	player.score += tempScore;
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
	if (file.good()) {
		while (file) {
			file >> level.levelScore >> level.levelFreeShots;
			file >> level.holePos.x >> level.holePos.y >> level.ballPos.x >> level.ballPos.y;
			int obstaclesCount;
			file >> obstaclesCount;
			for (int x = 0; x < obstaclesCount; x++) {
				SDL_Rect temp;
				file >> temp.x >> temp.y >> temp.w >> temp.h;
				level.obstacles.push_back(temp);
			}
		}
	}
	else
	{
		cout << "Couldn't load next level!\n";
	}
	file.close();
}
void win(SDL_Renderer* renderer, Level& level, Player& player, TextContainer& text, Ball& ball, Hole& hole, Menu& menu, bool& gameRunning)
{
	int tempScore;
	calculatePlayerScore(tempScore, player, level);
	//show menu
	showNextLevelMenu(renderer, menu, player, tempScore);
	//load next level
	if (goToNextLevel(renderer, text, ball, hole, level, player)) {
		//finish game
		gameRunning = false;
	}
}
bool goToNextLevel(SDL_Renderer* renderer, TextContainer& text, Ball& ball, Hole& hole, Level& level, Player& player)
{
	level.levelID++;
	if (level.levelID > LEVELS) return true;
	//text updates
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

	//other setups
	player.shotsPerLevel = 0;
	level.obstacles.clear();
	loadLevel(level);
	ballSet(ball, level);
	holeSetPosition(hole, level);
	return false;
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
void showNextLevelMenu(SDL_Renderer* renderer,Menu& menu, Player& player, int tempScore) 
{
	SDL_Delay(500);
	//render background
	SDL_RenderCopy(renderer, menu.menuTexture, NULL, NULL);
	
	//calculate stars
	int numOfStars = 0;
	if (tempScore < 150)
		numOfStars = 1;
	else if (tempScore < 400)
		numOfStars = 2;
	else numOfStars = 3;
	
	//render stars
	renderStars(renderer, menu, numOfStars);
	
	//render text
	renderMenuText(renderer, player, tempScore);

	SDL_RenderPresent(renderer);
	bool spacePressed = false;
	while (!spacePressed) {
		SDL_Event e;
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE))
				spacePressed = true;
	}
	SDL_Delay(250);
}
void renderStars(SDL_Renderer* renderer, Menu& menu, const int numOfStars) 
{
	SDL_Rect rect;
	rect.y = 120;
	rect.w = menu.star.rect.h;
	rect.h = rect.w + 10;
	for (int i = 0; i < numOfStars; i++)
	{
		rect.x = 249 + i * rect.w;
		SDL_RenderCopy(renderer, menu.star.texture, NULL, &rect);
	}
}
void renderMenuText(SDL_Renderer* renderer, Player& player, int tempScore) {
	TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
	const int size = 50;
	SDL_Color color = { 255, 255, 255 };
	string message;
	SDL_Texture* texture;
	SDL_Rect rect = { 0,0,size,size };
	//Score
	message = to_string(tempScore);
	renderText(renderer, texture, message, rect, 450, 260, font, color);

	//Total Score
	message = to_string(player.score);
	renderText(renderer, texture, message, rect, 350, 438, font, color);

	//Total shots
	if (player.shotsTotal < 10)
		message = '0' + to_string(player.shotsTotal);
	else
		message = to_string(player.shotsTotal);
	renderText(renderer, texture, message, rect, 350, 555, font, color);

	//Shots
	if (player.shotsPerLevel < 10)
		message = '0' + to_string(player.shotsPerLevel);
	else
		message = to_string(player.shotsPerLevel);
	renderText(renderer, texture, message, rect, 450, 320, font, color);

	TTF_CloseFont(font);
	SDL_DestroyTexture(texture);
}
void renderText(SDL_Renderer* renderer, SDL_Texture*& texture, string message, SDL_Rect& rect,int x, int y, TTF_Font* font, SDL_Color color) 
{
	SDL_Surface* temp;
	temp = TTF_RenderText_Solid(font, message.c_str(), color);
	texture = SDL_CreateTextureFromSurface(renderer, temp);
	rect.x = x;
	rect.y = y;
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_FreeSurface(temp);
}
