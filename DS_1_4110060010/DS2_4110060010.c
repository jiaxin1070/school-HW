#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 40000

typedef struct
{
	int vert;
	int hor;
}offsets;
offsets move[4];

typedef struct
{
	int row;
	int col;
	int dir;
} element;

element stack[MAX_STACK_SIZE];

int top = -1;
element pop()
{
	return stack[top--];
}

void push(element item)
{
	stack[++top] = item;
}

int** map;
int** laby;
int m;
int n;
int end_x;
int end_y;
int sx;
int sy;
int kind ;

void Map();
void go();
int compare(int** q);

int main()
{

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

	file = fopen("testcase2.txt", "r");
	output = fopen("output2.txt", "w");
	if (file == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}

	fscanf(file, "%d", &T);
	for (int k = 0; k < T; k++)
	{
		kind=0;
		fscanf(file, "%d", &m);
		fscanf(file, "%d", &n);

		laby = (int**)malloc(sizeof(int*) * m);

		for (int i = 0; i < m; i++)
			laby[i] = (int*)malloc(sizeof(int) * n);

		map = (int**)malloc(sizeof(int*) * m);

		for (int i = 0; i < m; i++)
			map[i] = (int*)malloc(sizeof(int) * n);

		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				fscanf(file, "%d", &laby[i][j]);


		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (laby[i][j] == 1)
				{
					top = 0;
					stack[0].row = i;
					stack[0].col = j;
					stack[0].dir = 0;
					map[i][j] = 1;
					stack[-1].row = i;
					stack[-1].col = j;
					stack[-1].dir = 0;
					sx = i;
					sy = j;
				}
				else if (laby[i][j] == 2)
				{
					end_x = i;
					end_y = j;
				}
			}
		}

		go();
		printf("%d\n", kind);
		fprintf(output, "%d\n", kind);
		fflush(output);
	}
	free(laby);
	free(map);

	fclose(file);

	return 0;
}



void Map()
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (laby[i][j] == -1)
				map[i][j] = -1;
			else
				map[i][j] = 0;
		}
			
	}
	map[sx][sy] = 1;
}

void go()
{
	int nextrow, nextcol, nowrow, nowcol, dir;
	element position;
	Map();
	stack[-1].row = sx;
	stack[-1].col = sy;
	stack[-1].dir = 0;
	top = 0;
	stack[0].row = sx;
	stack[0].col = sy;
	stack[0].dir = 0;
	while (top > -1)
	{
		pop();
		nowrow = stack[top].row;
		nowcol = stack[top].col;
		dir = stack[top].dir;
		while (dir < 4)
		{
			nextrow = nowrow + move[dir].vert;
			nextcol = nowcol + move[dir].hor;

			if (nextrow == end_x && nextcol == end_y)
			{
				
				stack[top].dir = ++dir;
				nowrow = stack[top].row;
				nowcol = stack[top].col;
				dir = stack[top].dir;

				int a = compare(map);
				if (a == 1)
				{
					kind++;
				}
					
				
			}
			else if (nextrow >= 0 && nextrow < m && nextcol >= 0 && nextcol<n && laby[nextrow][nextcol]==0 && map[nextrow][nextcol] == 0)
			{
				stack[top].dir = ++dir;
				position.row = nextrow;
				position.col = nextcol;
				position.dir = 0;
				push(position);
				map[nextrow][nextcol] = 1;
				nowrow = nextrow;
				nowcol = nextcol;
				dir = 0;
			}
			else
				dir++;
		}
		map[nowrow][nowcol] = 0;
	}
}

int compare(int** q)
{
	int count = 0;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (map[i][j] == 0)
				count += 1;
		}
	}
	if((count-1)==0)
		return 1;
	return 0;
}



