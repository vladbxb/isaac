#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "isaac");

    float squareSize = 30;
    Vector2 squarePosition = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 squareDimensions = { (float)squareSize, (float)squareSize };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (IsKeyDown(KEY_RIGHT)) squarePosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) squarePosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) squarePosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) squarePosition.y += 2.0f;

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawRectangleV(squarePosition, squareDimensions, MAROON);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}
