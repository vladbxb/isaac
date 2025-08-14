#include "raylib.h"

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Tear
{
    Vector2 tearPosition;
    // Vector2 startPosition;
    enum Direction tearDirection;
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    const Color skin = {255, 215, 156, 255};
    const Color background = {84, 53, 30, 255};

	const float squareSize = 30;
    const Vector2 squareDimensions = {(float)squareSize, (float)squareSize};

	const int allocatedTears = 10000;
    const double tearCooldown = 0.3;
    const float tearSpeed = 5;
    const float tearOffset = 5;
    const float tearRadius = squareSize / 2;
	// const float tearRange = 20;
    double lastTearTime = 0;
    unsigned int tearIndex = 0;
    struct Tear *spawnedTears = MemAlloc(sizeof(struct Tear) * allocatedTears);

	double currentTime;

	// Init
    InitWindow(screenWidth, screenHeight, "isaac");
    SetTargetFPS(60);

    Vector2 squarePosition = {(float)screenWidth / 2, (float)screenHeight / 2};
	Vector2 squareCenter;
    // Start in the middle of the screen
	squarePosition.x -= (float)squareDimensions.x / 2;
    squarePosition.y -= (float)squareDimensions.y / 2;

    // Main game loop
    while (!WindowShouldClose())
    {
		// Update
        // WASD movement
        if (IsKeyDown(KEY_D))
            squarePosition.x += 2.0f;
        if (IsKeyDown(KEY_A))
            squarePosition.x -= 2.0f;
        if (IsKeyDown(KEY_W))
            squarePosition.y -= 2.0f;
        if (IsKeyDown(KEY_S))
            squarePosition.y += 2.0f;

        currentTime = GetTime();
		squareCenter = (Vector2){squarePosition.x + squareDimensions.x / 2, squarePosition.y + squareDimensions.y / 2};

        // Tear spawning
        if (currentTime - lastTearTime >= tearCooldown)
		{
			if (IsKeyDown(KEY_RIGHT))
			{
				spawnedTears[tearIndex].tearPosition = (Vector2){squareCenter.x + tearOffset, squareCenter.y};
				spawnedTears[tearIndex].tearDirection = RIGHT;
				++tearIndex;
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				spawnedTears[tearIndex].tearPosition = (Vector2){squareCenter.x - tearOffset, squareCenter.y};
				spawnedTears[tearIndex].tearDirection = LEFT;
				++tearIndex;
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_UP))
			{
				spawnedTears[tearIndex].tearPosition = (Vector2){squareCenter.x, squareCenter.y - tearOffset};
				spawnedTears[tearIndex].tearDirection = UP;
				++tearIndex;
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				spawnedTears[tearIndex].tearPosition = (Vector2){squareCenter.x, squareCenter.y + tearOffset};
				spawnedTears[tearIndex].tearDirection = DOWN;
				++tearIndex;
				lastTearTime = currentTime;
			}
		}

        // Update tears positions
        for (unsigned int i = 0; i < tearIndex; ++i)
        {
            switch (spawnedTears[i].tearDirection)
            {
            case RIGHT:
                spawnedTears[i].tearPosition.x += tearSpeed;
                break;
            case LEFT:
                spawnedTears[i].tearPosition.x -= tearSpeed;
                break;
            case UP:
                spawnedTears[i].tearPosition.y -= tearSpeed;
                break;
            case DOWN:
                spawnedTears[i].tearPosition.y += tearSpeed;
                break;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(background);

        DrawText("Move the square with WASD", 10, 10, 20, RAYWHITE);
        DrawText("Shoot tears with arrow keys", 10, 40, 20, RAYWHITE);
        DrawRectangleV(squarePosition, squareDimensions, skin);

        for (unsigned int i = 0; i < tearIndex; ++i)
            DrawCircleV(spawnedTears[i].tearPosition, tearRadius, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
