#include "raylib.h"

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Tear
{
    Vector2 tearPosition;
    Vector2 tearDimensions;
    enum Direction tearDirection;
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    Color SKIN =  { 255, 215, 156, 255 };
    Color BACKGROUND = { 84, 53, 30, 255 };

    InitWindow(screenWidth, screenHeight, "isaac");

    float squareSize = 30;
    Vector2 squarePosition = {(float)screenWidth / 2, (float)screenHeight / 2};
    Vector2 squareDimensions = {(float)squareSize, (float)squareSize};

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    Tear spawnedTears;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        if (IsKeyDown(KEY_D))
            squarePosition.x += 2.0f;
        if (IsKeyDown(KEY_A))
            squarePosition.x -= 2.0f;
        if (IsKeyDown(KEY_W))
            squarePosition.y -= 2.0f;
        if (IsKeyDown(KEY_S))
            squarePosition.y += 2.0f;

        if (IsKeyDown(KEY_RIGHT))
            // spawn tear to the right
        if (IsKeyDown(KEY_LEFT))
            // spawn tear to the left
        if (IsKeyDown(KEY_UP))
            // spawn tear at the top
        if (IsKeyDown(KEY_DOWN))
            // spawn tear at the bottom

        // Draw
        BeginDrawing();

        ClearBackground(BACKGROUND);

        DrawText("move the square with wasd", 10, 10, 20, RAYWHITE);

        DrawRectangleV(squarePosition, squareDimensions, SKIN);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}
