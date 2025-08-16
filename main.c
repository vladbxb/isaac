#include "raylib.h"
#include "raymath.h"
#include "math.h"

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Tear
{
    Vector2 position;
    Vector2 startPosition;
    enum Direction direction;
	bool exists;
};


int main(void)
{
	// Screen size constants
    const int screenWidth = 792;
    const int screenHeight = 440;

	// Color constants
    const Color skin = {255, 215, 156, 255};
    const Color background = {84, 53, 30, 255};

	// Square constants
	const float squareSize = 30;
    const Vector2 squareDimensions = {(float)squareSize, (float)squareSize};

	// Tear constants
	const int allocatedTears = 10000;
    const double tearCooldown = 0.3;
    const float tearSpeed = 5;
    const float tearOffset = 5;
    const float tearRadius = squareSize / 2;
	const float tearRange = fmin(screenWidth, screenHeight) / 3;
    double lastTearTime = 0;
    unsigned int tearIndex = 0;
    struct Tear *spawnedTears = MemAlloc(sizeof(struct Tear) * allocatedTears);

	// Tilemap constants
	const int horizontalTiles = 9;
	const int verticalTiles = 5;
	// TODO: Change tilesize in terms of window size and maybe add letterboxing
	const int tileSize = 88;
	const Color colors[21] = { DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN, GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE };
	Rectangle tilemap[horizontalTiles][verticalTiles];

	// Initialize tiles
	for (unsigned int i = 0; i < horizontalTiles; ++i)
	{
		for (unsigned int j = 0; j < verticalTiles; ++j)
		{
			tilemap[i][j] = (Rectangle){ i * tileSize, j * tileSize, tileSize, tileSize };
		}
	}

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
				spawnedTears[tearIndex].position = (Vector2){squareCenter.x + tearOffset, squareCenter.y};
				spawnedTears[tearIndex].startPosition = spawnedTears[tearIndex].position;
				spawnedTears[tearIndex].direction = RIGHT;
				spawnedTears[tearIndex].exists = 1;
				++tearIndex;
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				spawnedTears[tearIndex].position = (Vector2){squareCenter.x - tearOffset, squareCenter.y};
				spawnedTears[tearIndex].startPosition = spawnedTears[tearIndex].position;
				spawnedTears[tearIndex].direction = LEFT;
				spawnedTears[tearIndex].exists = 1;
				++tearIndex;
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_UP))
			{
				spawnedTears[tearIndex].position = (Vector2){squareCenter.x, squareCenter.y - tearOffset};
				spawnedTears[tearIndex].startPosition = spawnedTears[tearIndex].position;
				spawnedTears[tearIndex].direction = UP;
				spawnedTears[tearIndex].exists = 1;
				++tearIndex;
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				spawnedTears[tearIndex].position = (Vector2){squareCenter.x, squareCenter.y + tearOffset};
				spawnedTears[tearIndex].startPosition = spawnedTears[tearIndex].position;
				spawnedTears[tearIndex].direction = DOWN;
				spawnedTears[tearIndex].exists = 1;
				++tearIndex;
				lastTearTime = currentTime;
			}
		}

        // Update tears positions
        for (unsigned int i = 0; i < tearIndex; ++i)
        {
			if (!spawnedTears[i].exists)
				continue;
			if (Vector2Distance(spawnedTears[i].position, spawnedTears[i].startPosition) >= tearRange)
			{
				spawnedTears[i].exists = 0;
				continue;
			}
            switch (spawnedTears[i].direction)
            {
            case RIGHT:
                spawnedTears[i].position.x += tearSpeed;
                break;
            case LEFT:
                spawnedTears[i].position.x -= tearSpeed;
                break;
            case UP:
                spawnedTears[i].position.y -= tearSpeed;
                break;
            case DOWN:
                spawnedTears[i].position.y += tearSpeed;
                break;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(background);

		for (unsigned int i = 0; i < horizontalTiles; ++i)
		{
			for (unsigned int j = 0; j < verticalTiles; ++j)
			{
				DrawRectangleRec(tilemap[i][j], colors[(i + j) % 21]);
			}
		}

        DrawText("Move the square with WASD", 10, 10, 20, RAYWHITE);
        DrawText("Shoot tears with arrow keys", 10, 40, 20, RAYWHITE);
        DrawRectangleV(squarePosition, squareDimensions, skin);

        for (unsigned int i = 0; i < tearIndex; ++i)
		{
			if (!spawnedTears[i].exists)
				continue;
            DrawCircleV(spawnedTears[i].position, tearRadius, BLUE);
		}


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
