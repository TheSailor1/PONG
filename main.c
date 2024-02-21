// TOKEN
// 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

#include "raylib.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define MAX_BRICKS 10

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
Brick bricks[MAX_BRICKS] = {0};

// Sound files
Sound fxBallHitWall;
Sound fxBallHitPad;

void drawBall(void);
void updateBall(void);
void drawPaddle(void);
void updatePaddle(void);

int main(void)
{
	InitWindow(500, 500, "PONG");
	InitAudioDevice();

	SetWindowMonitor(1); //TEMP!!!!!

	fxBallHitWall = LoadSound("assets/sounds/ball_hit_wall.wav"); 
	fxBallHitPad = LoadSound("assets/sounds/ball_hit_pad.wav"); 

	SetTargetFPS(120);

	ball.velocity = (Vector2) {300,300};
	ball.size = 18;
	ball.rec = (Rectangle) {
		WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, ball.size, ball.size};

	paddle.rec = (Rectangle) {WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40, 100, 30};
	paddle.speed = 500;
	paddle.friction = 1.2;


	while (!WindowShouldClose())
	{
		updateBall();
		updatePaddle();

		BeginDrawing();
		ClearBackground(RAYWHITE);
		drawBall();
		drawPaddle();
		EndDrawing();
	}

	UnloadSound(fxBallHitWall);
	UnloadSound(fxBallHitPad);
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
	if (ball.rec.y < 0)
	{
		PlaySound(fxBallHitWall);
		ball.rec.y = 0;
		ball.velocity.y = -ball.velocity.y;
	}
	if (ball.rec.y + ball.size > WINDOW_HEIGHT)
	{
		PlaySound(fxBallHitWall);
		ball.rec.y = WINDOW_HEIGHT - ball.size;
		ball.velocity.y = -ball.velocity.y;
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
