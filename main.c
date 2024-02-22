#include "raylib.h"
#include <string.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define MAX_BRICKS 8
#define MAX_LINES 4

typedef struct Ball {
	Rectangle rec;
	Vector2 velocity;
	bool active;
	int size;
} Ball;

typedef struct Paddle {
	Rectangle rec;
	int speed;
	float friction;
	float velocityX;
} Paddle;

typedef struct Brick {
	Rectangle rec;
	bool active;
} Brick;

Ball ball = {0};
Paddle paddle = {0};
Brick bricks[MAX_BRICKS][MAX_LINES] = {0};
int points = 0;
int lives = 4;
int totalBricks = MAX_LINES * MAX_BRICKS;
int count = 0;
int ballTimer = 30;
int gameState = 0; //0 Menu / 1 Game / 2 WIN / 3 Gameover

// Sound files
Sound fxBallHitWall;
Sound fxBallHitPad;
Sound fxBallHitBrick;
Sound fxBallLost;

void resetBall(void);
void drawBall(void);
void updateBall(void);
void drawPaddle(void);
void updatePaddle(void);
void drawBricks(void);
void updateBricks(void);
void drawGame(void);
void updateGame(void);
void drawMenu(void);
void updateMenu(void);
void drawGameOver(void);
void updateGameOver(void);
void drawWin(void);
void updateWin(void);

int main(void)
{
	InitWindow(500, 500, "PONG");
	InitAudioDevice();

//	SetWindowMonitor(1); //TEMP!!!!!

	fxBallHitWall = LoadSound("assets/sounds/ball_hit_wall.wav"); 
	fxBallHitPad = LoadSound("assets/sounds/ball_hit_pad.wav"); 
	fxBallHitBrick = LoadSound("assets/sounds/ball_hit_brick.wav"); 
	fxBallLost= LoadSound("assets/sounds/ball_lost.wav"); 

	SetTargetFPS(120);

	ball.velocity = (Vector2) {300,300};
	ball.size = 18;
	ball.rec = (Rectangle) {
		WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, ball.size, ball.size};

	paddle.rec = (Rectangle) {WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40, 100, 30};
	paddle.speed = 500;
	paddle.friction = 1.2;

	for (int j = 0; j < MAX_LINES; j++)
	{
		for (int i = 0; i < MAX_BRICKS; i++)
		{
			bricks[i][j].rec = (Rectangle) {10 + (i * 60),40 + (j * 50), 50, 40};
			bricks[i][j].active = true;
		}
	}

	while (!WindowShouldClose())
	{
		if (gameState == 0)
		{
			updateMenu();
		}
		else if (gameState == 1)
		{
			updateGame();
		}
		else if (gameState == 2)
		{
			updateWin();
		}
		else if (gameState == 3)
		{
			updateGameOver();
		}

		BeginDrawing();
		if (gameState == 0)
		{
			drawMenu();
		}
		else if (gameState == 1)
		{
			drawGame();
		}
		else if (gameState == 2)
		{
			drawWin();
		}
		else if (gameState == 3)
		{
			drawGameOver();
		}

		EndDrawing();
	}

	UnloadSound(fxBallHitWall);
	UnloadSound(fxBallHitPad);
	UnloadSound(fxBallHitBrick);
	UnloadSound(fxBallLost);
	CloseAudioDevice();

	CloseWindow();
	return 0;
}

void drawBall(void)
{
	DrawRectangleRec(ball.rec, RED);
}

void updateBall(void)
{
	if (lives < 0)
	{
		gameState = 3;
	}

	if (ballTimer == 0)
		ball.active = true;
	else
	{
		ballTimer = ballTimer - (1 * GetFrameTime());
	}

	if (ball.active)
	{
		bool collision = false;

		if (ball.rec.x < 0)
		{
			PlaySound(fxBallHitWall);
			ball.rec.x = 0;
			ball.velocity.x = -ball.velocity.x;
		}
		if (ball.rec.x + ball.size > WINDOW_WIDTH)
		{
			PlaySound(fxBallHitWall);
			ball.rec.x = WINDOW_WIDTH - ball.size;
			ball.velocity.x = -ball.velocity.x;
		}
		if (ball.rec.y < 30)
		{
			PlaySound(fxBallHitWall);
			ball.rec.y = 30;
			ball.velocity.y = -ball.velocity.y;
		}
		if (ball.rec.y + ball.size > WINDOW_HEIGHT)
		{
			PlaySound(fxBallLost);
			lives -= 1;
			resetBall();
		}

		// Collision with paddle
		collision = CheckCollisionRecs(ball.rec, paddle.rec);
		if (collision == true)
		{
			PlaySound(fxBallHitPad);
			ball.rec.y = paddle.rec.y - ball.size;
			ball.velocity.y = -ball.velocity.y;
		}

		ball.rec.x += ball.velocity.x * GetFrameTime();
		ball.rec.y += ball.velocity.y * GetFrameTime();

	}
}
void drawPaddle(void)
{
	DrawRectangleRec(paddle.rec, BLACK);
}

void updatePaddle(void)
{
	if (IsKeyDown(KEY_LEFT))
	{
		paddle.velocityX = -paddle.speed * GetFrameTime();
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		paddle.velocityX = paddle.speed * GetFrameTime();
	}

	// Check boundaries
	if (paddle.rec.x < 0)
	{
		paddle.rec.x = 0;
	}
	if (paddle.rec.x > WINDOW_WIDTH - paddle.rec.width)
	{
		paddle.rec.x = WINDOW_WIDTH - paddle.rec.width;
	}

	// Add friction to the paddle
	paddle.velocityX /= paddle.friction;

	// Update position of paddle
	paddle.rec.x += paddle.velocityX;

}

void drawBricks(void)
{
	for (int j = 0; j < MAX_LINES; j++)
	{
		for (int i = 0; i < MAX_BRICKS; i++)
		{
			if (bricks[i][j].active == true)
			{
				DrawRectangleRec(bricks[i][j].rec, BLUE);
			}
		}
	}
}

void updateBricks(void)
{
	bool brickHit = false;

	if (count == totalBricks)
	{
		gameState = 2;
	}

	for (int j = 0; j < MAX_LINES; j++)
	{
		for (int i = 0; i < MAX_BRICKS; i++)
		{
			brickHit = CheckCollisionRecs(ball.rec, bricks[i][j].rec);
			if (brickHit)
			{
				if (bricks[i][j].active == true)
				{
					PlaySound(fxBallHitBrick);
					ball.velocity.y = -ball.velocity.y;
					count++;
					bricks[i][j].active = false;
					points += 10;
				}
			}	
		}	
	}
}

void resetBall(void)
{
	ball.velocity = (Vector2) {300,300};
	ball.rec = (Rectangle) {
		WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, ball.size, ball.size};
	ballTimer = 60;
	ball.active = false;
}

void drawGame(void)
{
	ClearBackground(RAYWHITE);
	drawBall();
	drawPaddle();
	drawBricks();
	DrawRectangle(0, 0, WINDOW_WIDTH, 30, LIGHTGRAY);
	DrawText(TextFormat("LIVES: %d", lives), 10, 5, 20, BLACK);
	DrawText(TextFormat("SCORE: %d", points), WINDOW_WIDTH - 300, 5, 20, BLACK);
}

void updateGame(void)
{
	updateBall();
	updatePaddle();
	updateBricks();
}

void drawMenu(void)
{
	ClearBackground(BLACK);
	DrawText("BWONG!!", 200, 100, 50, RAYWHITE);
	DrawText("PRESS SPACE TO START", 100, 300, 20, RAYWHITE);
}

void updateMenu(void)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		gameState = 1;
	}
}

void drawWin(void)
{
	ClearBackground(GREEN);
	DrawText("YOU WON!", 100, 100, 50, DARKGREEN);
}

void updateWin(void)
{
	if (IsKeyPressed(KEY_SPACE))
		{
			gameState = 0;
		}
}

void updateGameOver(void)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		gameState = 0;
	}
}

void drawGameOver(void)
{
	ClearBackground(RED);
	DrawText("Game Over!", 100, 100, 40, RAYWHITE);
}
