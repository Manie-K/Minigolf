#include "functions.h"
#include <iostream>

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
int initBall(SDL_Variables vars, Ball &ball)
{
	const char* ballImagePath = "assets/ball.png";

	ball.x = SCREEN_WIDTH/2;
	ball.xPosBuffer = 0;
	
	ball.y = SCREEN_HEIGHT/2;
	ball.yPosBuffer = 0;

	ball.size = 24;
	ball.radius = ball.size / 2;

	ball.Xspeed = ball.Yspeed = 0;

	ball.isMoving = false;
	ball.drawArrow = false;

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
int initHole(Hole& hole, SDL_Variables vars) {
	hole.x = 40;
	hole.y = 140;
	hole.boxModel.x = hole.drawModel.x = hole.x - hole.radius;
	hole.boxModel.y = hole.y - hole.radius;
	hole.boxModel.w = hole.boxModel.h = hole.drawModel.w= hole.size;
	hole.drawModel.h = 2 * hole.size;
	hole.drawModel.y = hole.boxModel.y - hole.size;

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

void updateBall(Ball& ball, double frameTime, Hole hole)
{
	//ball update
	ballPositionUpdate(ball, frameTime);
	ballSpeedUpdate(ball);
	ballBoxModelUpdate(ball);

	//checks collision with screen
	calculateCollisions(ball);

	//checks collision with obstacles

	//checks win
	checkWin(ball, hole);
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

void shootBall(Ball& ball){
	double power;
	double cos, sin;
	const double powerScale = 0.01;
	int mouseX, mouseY;
	int x, y;
	int dX, dY;

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

void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int length) 
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	
	double angle = atan2(y2 - y1, x2 - x1);

	// Calculate the endpoints of the arrowhead
	int arrowLength = 10;
	int arrowWidth = 5;
	if (length > 100) {
		arrowLength *= 3;
		arrowWidth *= 3;
	}
	else if (length > 50) {
		arrowLength *= 2;
		arrowWidth *= 2;
	}
	int arrowX1 = int(x2 - arrowLength * cos(angle) + arrowWidth * sin(angle));
	int arrowY1 = int(y2 - arrowLength * sin(angle) - arrowWidth * cos(angle));
	int arrowX2 = int(x2 - arrowLength * cos(angle) - arrowWidth * sin(angle));
	int arrowY2 = int(y2 - arrowLength * sin(angle) + arrowWidth * cos(angle));

	// Draw the line and arrowhead
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLine(renderer, x2, y2, arrowX1, arrowY1);
	SDL_RenderDrawLine(renderer, x2, y2, arrowX2, arrowY2);
}
void visualiseShot(Ball ball, SDL_Renderer* renderer)
{
	int mouseX, mouseY;
	int x, y;
	int dX, dY;

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

	drawLine(renderer, x, y, x-dX, y - dY, (int)length);
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

int gameStart(SDL_Variables& SDLvariables, Ball& ball, Hole& hole)
{
	if (initSDL(SDLvariables) < 0) return -1;
	if (initBall(SDLvariables, ball) < 0) return -1;
	if (initHole(hole, SDLvariables) < 0) return -1;
	return 1;
}
void gameRender(SDL_Variables& SDLvariables, Ball& ball, Hole& hole)
{
	SDL_SetRenderDrawColor(SDLvariables.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(SDLvariables.renderer);
	drawBackground(SDLvariables.renderer);											//draws backgorund
	SDL_RenderCopy(SDLvariables.renderer, hole.holeTexture, NULL, &hole.drawModel); //draws hole
	SDL_RenderCopy(SDLvariables.renderer, ball.ballTexture, NULL, &ball.boxModel);  //draws ball
	if (ball.drawArrow)
		visualiseShot(ball, SDLvariables.renderer);									//draws arrow when shooting
	SDL_RenderPresent(SDLvariables.renderer);
}
void gameHandleEvents(Ball& ball, bool& gameRunning)
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
				shootBall(ball);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && ball.isMoving == false)
				ball.drawArrow = true;
			break;
		}
	}
}