#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>


#define MAX_STACK_SIZE 100

typedef struct
{
	int vert;
	int hor;
}offsets;

offsets move[4];

int** infected;
int** map;
int m;
int n;
int walls;

int Virus(int** grid);
int path(int** grid);
int group(int** grid, int** map, int row, int col, int color);
void buildWall(int** grid, int row, int col);
void spread(int** grid, int** map, int row, int col);
void Map();


int main()
{
	int ans;

	int T;
	FILE* file;
	FILE* output;

	move[0].vert = -1;
	move[0].hor = 0;
	move[1].vert = 0;
	move[1].hor = 1;
	move[2].vert = 1;
	move[2].hor = 0;
	move[3].vert = 0;
	move[3].hor = -1;

	file = fopen("testcase1.txt", "r");
	output = fopen("output1.txt", "w");
	if (file == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}

	fscanf(file, "%d", &T);

	for (int k = 0; k < T; k++)
	{
		fscanf(file, "%d", &m);
		fscanf(file, "%d", &n);

		infected = (int**)malloc(sizeof(int*) * m);

		for (int i = 0; i < m; i++)
			infected[i] = (int*)malloc(sizeof(int) * n);

		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				fscanf(file, "%d", &infected[i][j]);

		map = (int**)malloc(sizeof(int*) * m);

		for (int i = 0; i < m; i++)
			map[i] = (int*)malloc(sizeof(int) * n);

		ans = Virus(infected);
		printf("%d\n", ans);
		fprintf(output, "%d\n", ans);
		fflush(output);

	}
	free(infected);
	free(map);
	fclose(file);
	
	return 0;
}

int Virus(int **grid)
{
	int ans = 0;
	while (true)
	{
		int walls = path(grid);
		if (walls == 0)
			break;
		ans += walls;
	}
	return ans;
}

int path(int** grid)
{
	int cnt = 0, ans = 0, color = -1, row = -1, col = -1;
	Map();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j] == 1 && map[i][j] == 0)
			{
				walls = 0;
				int area = group(grid, map, i, j, color, walls);

				if (area > cnt)
				{
					ans = walls;
					cnt = area;
					row = i;
					col = j;
				}
				color--;
			}
		}
	}
	buildWall(grid, row, col);
	Map();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j] == 1 && map[i][j] == 0)
				spread(grid, map, i, j);
		}
	}
	return ans;
}

int group(int**grid, int** map, int row, int col, int color)
{
	int ans = 0;

	if (row < 0 || row >= m || col < 0 || col >= n) return 0;

	if (grid[row][col] == 0)
	{
		walls++;
		if (map[row][col] == color)
			return 0;
		map[row][col] = color;
		return 1;
	}

	if (map[row][col] == 1 || grid[row][col] != 1)
		return 0;
	map[row][col] = 1;
	for (int i = 0; i < 4; i++)
		ans += group(grid, map, row + move[i].vert, col + move[i].hor, color, walls);
	return ans;
}

void buildWall(int** grid, int row, int col)
{
	if (row < 0 || row >= m || col < 0 || col >= n || grid[row][col] != 1)
		return;

	grid[row][col] = -1; 
	for (int i = 0; i < 4; i++)
		buildWall(grid, row + move[i].vert, col + move[i].hor);
}

void spread(int** grid, int** map, int row, int col)
{
	if (row < 0 || row >= m || col < 0 || col >= n || map[row][col] == 1) return;

	if (grid[row][col] == 0)
	{
		grid[row][col] = 1;
		map[row][col] = 1;
	}

	else if (grid[row][col] == 1)
	{
		map[row][col] = 1;
		for (int i = 0; i < 4; i++)
			spread(grid, map, row + move[i].vert, col + move[i].hor);
	}
}

void Map()
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			map[i][j] = 0;
}
