#include "tear.h"

#include <stdbool.h>
#include "movement.h"

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
