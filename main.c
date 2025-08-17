#include "raylib.h"
#include "raymath.h"
#include "math.h"

#define MAX_TEARS 128

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef enum
{
	WALL,
	FIRE,
	POOP,
	EMPTY,
	DOOR
} TileType;

typedef union
{
	struct
	{
		int hitsLeft;
		int damage;

	} fire;
	struct
	{
		int hitsLeft;
	} poop;

} TileData;

typedef struct
{
	Rectangle rect;
	Color color;
	bool isSolid;
	TileType type;
	TileData data;
} Tile;

typedef struct
{
	Vector2 position;
	Vector2 startPosition;
	enum Direction direction;
	bool exists;
} Tear;

typedef struct
{
	int items[MAX_TEARS];
	int head;
	int tail;
} TearQueue;

void initTearQueue(TearQueue *q)
{
	q->head = -1;
	q->tail = 0;
}

void enqueueTear(TearQueue *q, int val)
{
	q->items[q->tail] = val;
	q->tail = (q->tail + 1) % MAX_TEARS;

	if (q->tail == q->head)
	{
		q->head = (q->head + 1) % MAX_TEARS;
	}
}

int dequeueTear(TearQueue *q)
{
	q->head = (q->head + 1) % MAX_TEARS;
	int val = q->items[q->head];
	return val;
}

void spawnTear(Tear *tears, Vector2 pos, enum Direction dir, TearQueue *q)
{
	int found = 0;
	int aux;
	for (int i = 0; i < MAX_TEARS; ++i)
	{
		if (!tears[i].exists)
		{
			tears[i].position = pos;
			tears[i].startPosition = pos;
			tears[i].direction = dir;
			tears[i].exists = true;
			found = 1;
			enqueueTear(q, i);
			break;
		}
	}
	if (!found)
	{
		aux = dequeueTear(q);
		tears[aux].position = pos;
		tears[aux].startPosition = pos;
		tears[aux].direction = dir;
		enqueueTear(q, aux);
	}
}
// TODO: Add different types of tiles

int main(void)
{
	// Screen size constants
	const int screenWidth = 780;
	const int screenHeight = 420;

	// Color constants
	const Color skin = {255, 215, 156, 255};
	// const Color background = {84, 53, 30, 255}; // dirt color
	const Color background = BLACK;

	// Square constants
	const float squareSize = 30;
	const float squareSpeed = 2.0f;
	const Vector2 squareDimensions = {(float)squareSize, (float)squareSize};
	Vector2 squarePosition = {(float)screenWidth / 2, (float)screenHeight / 2};
	Vector2 squareCenter;
	// Start in the middle of the screen
	squarePosition.x -= (float)squareDimensions.x / 2;
	squarePosition.y -= (float)squareDimensions.y / 2;
	Rectangle square = (Rectangle){squarePosition.x, squarePosition.y, squareDimensions.x, squareDimensions.y};

	// Tear constants
	const double tearCooldown = 0.3;
	const float tearSpeed = 5;
	const float tearOffset = 5;
	const float tearRadius = squareSize / 2;
	const float tearRange = fmin(screenWidth, screenHeight) / 3;
	double lastTearTime = 0;
	unsigned int minOuterTileX;
	unsigned int maxOuterTileX;
	unsigned int minOuterTileY;
	unsigned int maxOuterTileY;

	// Tear array
	Tear tears[MAX_TEARS];
	for (int i = 0; i < MAX_TEARS; ++i)
		tears[i].exists = false;

	// Queue for remembering the oldest spawned tear
	TearQueue tearQueue;
	initTearQueue(&tearQueue);

	// Tilemap constants
	const int horizontalTiles = 13;
	const int verticalTiles = 7;

	// TODO: Change tilesize in terms of window size and maybe add letterboxing
	const int tileSize = 60;
	// const Color colors[21] = {DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN, GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE};
	Tile tilemap[horizontalTiles][verticalTiles];

	bool collision = false;
	bool touchingSolid = false;

	// Tile constants
	const Tile WALL_TILE = {
		.rect = {0, 0, 60, 60},
		.color = DARKGRAY,
		.isSolid = true,
		.type = WALL};

	const Tile EMPTY_TILE = {
		.rect = {0, 0, 60, 60},
		.color = WHITE,
		.isSolid = false,
		.type = EMPTY};

	const Tile DOOR_TILE = {
		.rect = {0, 0, 60, 60},
		.color = BROWN,
		.isSolid = false,
		.type = DOOR};

	const Tile FIRE_TILE = {
		.rect = {0, 0, 60, 60},
		.color = RED,
		.isSolid = true,
		.type = FIRE,
		.data.fire = {.hitsLeft = 3, .damage = 1}};

	const Tile POOP_TILE = {
		.rect = {0, 0, 60, 60},
		.color = BROWN,
		.isSolid = true,
		.type = POOP,
		.data.poop = {.hitsLeft = 3}};

	// Initialize tiles
	for (unsigned int i = 0; i < horizontalTiles; ++i)
	{
		for (unsigned int j = 0; j < verticalTiles; ++j)
		{
			if ((j == 3 && (i == 0 || i == horizontalTiles - 1)) || (i == 6 && (j == 0 || j == verticalTiles - 1)))
			{
				tilemap[i][j] = DOOR_TILE;
			}
			else if (i == 0 || i == horizontalTiles - 1 || j == 0 || j == verticalTiles - 1)
			{
				tilemap[i][j] = WALL_TILE;
			}
			else
			{
				tilemap[i][j] = EMPTY_TILE;
			}
			if (i == 7 && j == 3)
				tilemap[i][j] = POOP_TILE;
			tilemap[i][j].rect = (Rectangle){i * tileSize, j * tileSize, tileSize, tileSize};
		}
	}

	double currentTime;

	// Init
	InitWindow(screenWidth, screenHeight, "isaac");
	SetTargetFPS(60);

	// Main game loop
	while (!WindowShouldClose())
	{
		// Update
		// WASD movement
		if (IsKeyDown(KEY_D))
			square.x += squareSpeed;
		if (IsKeyDown(KEY_A))
			square.x -= squareSpeed;
		if (IsKeyDown(KEY_W))
			square.y -= squareSpeed;
		if (IsKeyDown(KEY_S))
			square.y += squareSpeed;

		currentTime = GetTime();
		squareCenter = (Vector2){square.x + squareDimensions.x / 2, square.y + squareDimensions.y / 2};

		// Tear spawning
		if (currentTime - lastTearTime >= tearCooldown)
		{
			if (IsKeyDown(KEY_RIGHT))
			{
				spawnTear(tears, (Vector2){squareCenter.x + tearOffset, squareCenter.y}, RIGHT, &tearQueue);
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				spawnTear(tears, (Vector2){squareCenter.x - tearOffset, squareCenter.y}, LEFT, &tearQueue);
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_UP))
			{
				spawnTear(tears, (Vector2){squareCenter.x, squareCenter.y - tearOffset}, UP, &tearQueue);
				lastTearTime = currentTime;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				spawnTear(tears, (Vector2){squareCenter.x, squareCenter.y + tearOffset}, DOWN, &tearQueue);
				lastTearTime = currentTime;
			}
		}

		// Update tears positions and check for collisions
		for (unsigned int i = 0; i < MAX_TEARS; ++i)
		{
			if (!tears[i].exists)
				continue;
			if (Vector2Distance(tears[i].position, tears[i].startPosition) >= tearRange)
			{
				tears[i].exists = 0;
				continue;
			}

			// TODO: Reduce the number of divisions (maybe make the math faster with multiplications instead)
			// Find the tile that the tear is currently on, accounting for overlaps
			minOuterTileX = (int)((tears[i].position.x - tearRadius) / tileSize);
			maxOuterTileX = (int)((tears[i].position.x + tearRadius) / tileSize);
			minOuterTileY = (int)((tears[i].position.y - tearRadius) / tileSize);
			maxOuterTileY = (int)((tears[i].position.y + tearRadius) / tileSize);
			Tile *outerTile;
			for (unsigned int j = minOuterTileX; j <= maxOuterTileX; ++j)
			{
				for (unsigned int k = minOuterTileY; k <= maxOuterTileY; ++k)
				{
					outerTile = &tilemap[j][k];
					if (outerTile->isSolid)
					{
						bool collision = CheckCollisionCircleRec(tears[i].position, tearRadius, outerTile->rect);
						if (collision)
						{
							tears[i].exists = 0;
							if (outerTile->type == POOP)
							{
								outerTile->data.poop.hitsLeft--;
								if (outerTile->data.poop.hitsLeft == 0)
								{
									outerTile->color = WHITE;
									outerTile->isSolid = false;
								}
							}
							continue;
						}
					}
				}
			}
			switch (tears[i].direction)
			{
			case RIGHT:
				tears[i].position.x += tearSpeed;
				break;
			case LEFT:
				tears[i].position.x -= tearSpeed;
				break;
			case UP:
				tears[i].position.y -= tearSpeed;
				break;
			case DOWN:
				tears[i].position.y += tearSpeed;
				break;
			}
		}

		// Check for tiles collision
		for (unsigned int i = 0; i < horizontalTiles; ++i)
		{
			for (unsigned int j = 0; j < verticalTiles; ++j)
			{
				collision = CheckCollisionRecs(square, tilemap[i][j].rect);
				if (collision && tilemap[i][j].isSolid)
				{
					Rectangle overlap = GetCollisionRec(square, tilemap[i][j].rect);

					if (overlap.width < overlap.height)
					{
						if (square.x < tilemap[i][j].rect.x)
							square.x -= overlap.width;
						else
							square.x += overlap.width;
					}
					if (overlap.width > overlap.height)
					{
						if (square.y < tilemap[i][j].rect.y)
							square.y -= overlap.height;
						else
							square.y += overlap.height;
					}
				}
			}
		}

		// Draw
		BeginDrawing();
		ClearBackground(background);

		for (unsigned int i = 0; i < horizontalTiles; ++i)
		{
			for (unsigned int j = 0; j < verticalTiles; ++j)
			{
				DrawRectangleRec(tilemap[i][j].rect, tilemap[i][j].color);
			}
		}

		DrawText("Move the square with WASD", 10, 10, 20, RAYWHITE);
		DrawText("Shoot tears with arrow keys", 10, 40, 20, RAYWHITE);
		DrawRectangleRec(square, skin);

		for (unsigned int i = 0; i < MAX_TEARS; ++i)
		{
			if (!tears[i].exists)
				continue;
			DrawCircleV(tears[i].position, tearRadius, BLUE);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
