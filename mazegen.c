#include "mazegen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum {
	INVALID = -1,
	WALL    =  0,
	OPEN    =  1,
	UP      =  2,
	DOWN    =  3,
	LEFT    =  4,
	RIGHT   =  5,
} cell;

static int width;
static int height;
static cell* maze;

static cell get(int x, int y) {
	return
		x < 0 || y < 0 || x >= width || y >= height
		? INVALID
		: maze[x + y * width];
}

static void set(int x, int y, cell val) {
	if(get(x, y) == -1) return;
	maze[x + y * width] = val;
}

static void shuffle(int* arr, size_t len) {
	for(size_t i = len - 1; i >= 1; --i) {
		int j = rand() % (i + 1);
		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}
}

static int ranNei(int x, int y, int* dx, int* dy, int d) {
	int dirs[] = {UP, DOWN, LEFT, RIGHT};
	shuffle(dirs, 4);
	for(size_t i = 0; i < 4; ++i) {
		switch(dirs[i]) {
			case UP:    if(y - d >= 0)     {*dx =  0; *dy = -1; return dirs[i];}; break;
			case DOWN:  if(y + d < height) {*dx =  0; *dy = +1; return dirs[i];}; break;
			case LEFT:  if(x - d >= 0)     {*dx = -1; *dy =  0; return dirs[i];}; break;
			case RIGHT: if(x + d < width)  {*dx = +1; *dy =  0; return dirs[i];}; break;
		}
	}
	return INVALID;
}

static void dfsStep(int x, int y) {
	set(x, y, OPEN);

	int steps[] = {0, 1, 2, 3};
	shuffle(steps, 4);

	for(size_t i = 0; i < 4; ++i) {
		switch(steps[i]) {
			case 0: if(get(x, y - 2) == WALL) {set(x, y - 1, OPEN); dfsStep(x, y - 2);} break;
			case 1: if(get(x, y + 2) == WALL) {set(x, y + 1, OPEN); dfsStep(x, y + 2);} break;
			case 2: if(get(x - 2, y) == WALL) {set(x - 1, y, OPEN); dfsStep(x - 2, y);} break;
			case 3: if(get(x + 2, y) == WALL) {set(x + 1, y, OPEN); dfsStep(x + 2, y);} break;
		}
	}
}

static int lerwStep(int x, int y) {
	maze_print(maze, width, height);
	usleep(1000);
	int cell = get(x, y);
	if(cell == OPEN) return INVALID; // we have reached an open cell, write graph
	if(cell >= UP && cell <= RIGHT) return x + y * width; // we have reached ourselves, delete graph until this coordinate

	for(;;) {
		const int d = 2;
		int dx, dy;
		int dir = ranNei(x, y, &dx, &dy, d);
		set(x, y, dir);

		int res = lerwStep(x + dx * d, y + dy * d);
		if(res == INVALID) {
			set(x, y, OPEN);
			set(x + dx, y + dy, OPEN);
			return INVALID;
		} else {
			set(x, y, WALL);
			if(res != x + y * width) return res;
			// here: we have reached the start of a loop, retry the search
		}
	}
}

static void init(cell* _maze, int _width, int _height) {
	maze   = _maze,
	width  = _width;
	height = _height;
	memset(maze, WALL, width * height * sizeof(int));
}

void maze_print(int* maze, int width, int height) {
	printf("\e[H");
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int cell = maze[x + y * width];
			switch(cell) {
				case 0: printf("\e[37m█"); break;
				case 1: printf(" "); break;
				case 2: printf("\e[30;43m⯅"); break;
				case 3: printf("\e[30;43m⯆"); break;
				case 4: printf("\e[30;43m⯇"); break;
				case 5: printf("\e[30;43m⯈"); break;
			}
			printf("\e[m");
		}
		if(y < height - 1) printf("\n");
	}
}

void mazegen_dfs(cell* _maze, int _width, int _height) {
	init(_maze, _width, _height);
	dfsStep(0, 0);
}

void mazegen_lerw(cell* _maze, int _width, int _height) {
	init(_maze, _width, _height);

	int x1 = (rand() % (width >> 1)) << 1;
	int y1 = (rand() % (height >> 1)) << 1;
	set(x1, y1, OPEN);

	for(int x = 0; x < width; x += 2) {
		for(int y = 0; y < height; y += 2) {
			lerwStep(x, y);
		}
	}
}
