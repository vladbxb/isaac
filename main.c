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

    Color SKIN = {255, 215, 156, 255};
    Color BACKGROUND = {84, 53, 30, 255};

    InitWindow(screenWidth, screenHeight, "isaac");

    float squareSize = 30;
    Vector2 squarePosition = {(float)screenWidth / 2, (float)screenHeight / 2};
    Vector2 squareDimensions = {(float)squareSize, (float)squareSize};

    float tearRadius = squareSize / 2;

    struct Tear *spawnedTears = MemAlloc(sizeof(struct Tear) * 1000);

    unsigned int tearIndex = 0;

    double lastTearTime = 0;

    double tearCooldown = 0.3;
    float tearSpeed = 5;
    // float range = 20;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // WASD movement
        if (IsKeyDown(KEY_D))
            squarePosition.x += 2.0f;
        if (IsKeyDown(KEY_A))
            squarePosition.x -= 2.0f;
        if (IsKeyDown(KEY_W))
            squarePosition.y -= 2.0f;
        if (IsKeyDown(KEY_S))
            squarePosition.y += 2.0f;

        float tearOffset = 5;

        Vector2 squareCenter = {squarePosition.x + squareDimensions.x / 2, squarePosition.y + squareDimensions.y / 2};

        double currentTime = GetTime();

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
        ClearBackground(BACKGROUND);

        DrawText("Move the square with WASD", 10, 10, 20, RAYWHITE);
        DrawText("Shoot tears with arrow keys", 10, 40, 20, RAYWHITE);
        DrawRectangleV(squarePosition, squareDimensions, SKIN);

        for (unsigned int i = 0; i < tearIndex; ++i)
            DrawCircleV(spawnedTears[i].tearPosition, tearRadius, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}