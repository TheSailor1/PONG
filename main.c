#include "raylib.h"

#define WIN_WIDTH 600
#define WIN_HEIGHT 450

typedef struct Ball {
	Rectangle rec;
	int speedX;
	int speedY;
	float velocityX;
	float velocityY;
	bool active;
} Ball;

typedef struct Paddle {
	Rectangle rec;
	int speedX;
	float velocityX;
	float friction;
	bool active;
} Paddle;



Ball ball = {0};
Paddle paddle = {0};

// Protos
void initGame(void);
void drawBall(void);
void drawPaddle(void);
void updateBall(void);
void updatePaddle(void);

int main(void)
{

	InitWindow( WIN_WIDTH, WIN_HEIGHT, "PONG!" );
	SetTargetFPS(60);

	initGame();

	while (!WindowShouldClose())
	{
		updateBall();
		updatePaddle();

		BeginDrawing();
		ClearBackground(BLACK);
		drawPaddle();
		drawBall();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void initGame(void)
{
	int ballSize = 24;
	ball.rec = (Rectangle) { WIN_WIDTH / 2 - ballSize / 2, 
													WIN_HEIGHT / 2, 
													ballSize, 
													ballSize };
	ball.speedX = 300;
	ball.speedY = 300;
	ball.active = false;

	int padWidth = 100;
	int padHeight = 30;
	paddle.rec = (Rectangle) { WIN_WIDTH / 2 - padWidth / 2, 
														WIN_HEIGHT - padHeight - 30, 
														padWidth, 
														padHeight };
	paddle.speedX = 600;
	paddle.friction = 1.12;
}

void drawBall(void)
{
	DrawRectangleRec(ball.rec, RED);
}

void updateBall(void)
{

	if (ball.rec.x + ball.rec.width > WIN_WIDTH 
			|| ball.rec.x < 0)
		{
			ball.speedX = -ball.speedX;
		}

	if (ball.rec.y + ball.rec.height > WIN_HEIGHT 
			|| ball.rec.y < 0)
		{
			ball.speedY = -ball.speedY;
		}

	ball.velocityX = ball.speedX * GetFrameTime();
	ball.velocityY = ball.speedY * GetFrameTime();

	ball.rec.x += ball.velocityX;
	ball.rec.y += ball.velocityY;
}


void drawPaddle(void)
{
	DrawRectangleRec( paddle.rec, WHITE );
}

void updatePaddle(void)
{
	// Check boundaries
	if (paddle.rec.x <= 0)
	{
		paddle.velocityX = 0;
		paddle.rec.x = 0;
	}
	if (paddle.rec.x + paddle.rec.width >= WIN_WIDTH)
	{
		paddle.velocityX = 0;
		paddle.rec.x = WIN_WIDTH - paddle.rec.width;
	}

	if (IsKeyDown(KEY_LEFT))
	{
		paddle.velocityX = -paddle.speedX * GetFrameTime();
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		paddle.velocityX = paddle.speedX * GetFrameTime();
	}


	// Add friction
	paddle.velocityX /= paddle.friction;

	//Update POS
	paddle.rec.x += paddle.velocityX;
	
}
