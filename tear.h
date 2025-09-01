#ifndef TEAR_H
#define TEAR_H

#include <stdbool.h>
#include <raymath.h>
#include "movement.h"

#define MAX_TEARS 128

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

void initTearQueue(TearQueue *q);

void enqueueTear(TearQueue *q, int val);

int dequeueTear(TearQueue *q);

void spawnTear(Tear *tears, Vector2 pos, enum Direction dir, TearQueue *q);

#endif
