#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 1000

typedef struct
{
	int vert;
	int hor;
}offsets;
offsets move1[3];
offsets move2[3];

int** laby;;
int** map;
int** map2;
int m;
int n;
int s;

void down(int x, int y);
void up(int row, int col);


int main()
{

	int T;
	FILE* file;
	FILE* output;

	move1[0].vert = 1;
	move1[0].hor = 0;
	move1[1].vert = 1;
	move1[1].hor = 1;
	move1[2].vert = 1;
	move1[2].hor = -1;

	move2[0].vert = -1;
	move2[0].hor = 0;
	move2[1].vert = -1;
	move2[1].hor = 1;
	move2[2].vert = -1;
	move2[2].hor = -1;

	file = fopen("testcase4.txt", "r");
	output = fopen("output4.txt", "w"); 
	if (file == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}

	fscanf(file, "%d", &T);

	for (int k = 0; k < T; k++)
	{
		s = 0;
		fscanf(file, "%d", &m);
		fscanf(file, "%d", &n);

		laby = (int**)malloc(sizeof(int*) * m);
		map = (int**)malloc(sizeof(int*) * m);
		map2 = (int**)malloc(sizeof(int*) * m);

		for (int i = 0; i < m; i++)
		{
			laby[i] = (int*)malloc(sizeof(int) * n);
			map[i] = (int*)malloc(sizeof(int) * n);
			map2[i] = (int*)malloc(sizeof(int) * n);
		}

		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
			{
				fscanf(file, "%d", &laby[i][j]);
				map[i][j] = 0;
				map2[i][j] = 0;
			}

		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				if (laby[i][j] == 1)
				{
					down(i, j);
					up(i, j);
				}
		printf("%d\n", s);
		fprintf(output, "%d\n", s);
		fflush(output);
		free(laby);
		free(map);
		free(map2);
	}

	fclose(file);
	
	return 0;
}


int valid(int x, int y)
{
	if (x >= 0 && x < m && y >= 0 && y < n && laby[x][y] == 1)
		return 1;
	else return 0;
}

void down(int row, int col)
{
	int count = 0;

	if (map[row][col] == 0)
	{
		for (int k = 2; k <= m; k++)
		{
			for (int i = row; i <= (row + k - 1); i++)
				for (int j = (col - (i - row)); j <= (col + (i - row)); j++)
					if (i != row || j != col)
					{
						int a = valid(i, j);
						if (a == 1)
							down(i, j);
						else count += 1;
					}

			if (count == 0)
				s += 1;
			else
				break;
		}
		map[row][col] = 1;
	}

}

void up(int row, int col)
{
	int count = 0;
	if (map2[row][col]== 0)
	{
		for (int k = 2; k <= m; k++)
		{
			for (int i = (row - k + 1); i <= row + k - 1; i++)
				for (int j = (col - (i - row)); j >= (col + (i - row)); j--)
					if (i != row || j != col)
					{
						int a = valid(i, j);
						if (a == 1)
							up(i, j);
						else count += 1;

					}
			if (count == 0)
				s += 1;
			else break;
		}
		map2[row][col] = 1;
	}
}

