#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "mazegen.h"

int main(int argc, char** argv) {
	if(argc < 3) return 1;

	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	srand(now.tv_sec * now.tv_nsec);

	int width  = atoi(argv[1]);
	int height = atoi(argv[2]);
	int* maze  = malloc(width * height * sizeof(int));

	printf("\e[2J");
	mazegen_lerw(maze, width, height);
	maze_print(maze, width, height);
	free(maze);
}
