#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 10000

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
} element;

typedef struct
{
	element box;
	element player;

} Stack;

Stack stack[MAX_STACK_SIZE];

int top = -1;
int e = -1;
Stack pop()
{
	top = (top + 1) % MAX_STACK_SIZE;
	return stack[top];
}

void push(element item1, element item2)
{
	e = (e + 1) % MAX_STACK_SIZE;
	stack[e].box = item1;
	stack[e].player = item2;
}

int** laby;
int m;
int n;
int end_x;
int end_y;
int sx;
int sy;
int bx;
int by;
element b;
element p;
element target;

int Push();
int reach(int** grid, element box, element player, element f);
int compare(Stack s, int seen[21][21][21][21]);

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

	file = fopen("testcase3.txt", "r");
	output = fopen("output3.txt", "w"); 
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

		laby = (int**)malloc(sizeof(int*) * m);

		for (int i = 0; i < m; i++)
			laby[i] = (int*)malloc(sizeof(int) * n);

		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				fscanf(file, "%d", &laby[i][j]);
			
		int b = Push();
		printf("%d\n", b);
		fprintf(output, "%d\n",b);
		fflush(output);
		free(laby);
	}

	fclose(file);

	return 0;
}

int Push()
{
	top = -1;
	e = -1;
	int res = 0;
	int next_row;
	int next_col;
	int pnext_row;
	int pnext_col;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (laby[i][j] == 2)
			{
				sx = i;
				sy = j;
			}
			else if (laby[i][j] == 4)
			{
				end_x = i;
				end_y = j;
			}
			else if (laby[i][j] == 3)
			{
				bx = i;
				by = j;
			}
		}
	}
	b.row = bx;
	b.col = by;
	p.row = sx;
	p.col = sy;
	target.row = end_x;
	target.col = end_y;

	stack[0].box = b;
	stack[0].player = p;
	e = 0;

	int seen[21][21][21][21]={0};
	seen[bx][by][sx][sy]=1;

	int count = 0;
	while (e > top)
	{
		for (int i = (e - top); i > 0; --i)
		{
			Stack t = pop();
			element box = t.box;
			element player = t.player;
			element pos;
			Stack f;

			if (box.row == end_x && box.col == end_y)
				return res;

			int row = box.row;
			int col = box.col;
			for (int dir = 0; dir < 4; dir++)
			{
				
				next_row = row + move[dir].vert;
				next_col = col + move[dir].hor;
				pnext_row = row - move[dir].vert;
				pnext_col = col - move[dir].hor;

				if (next_row < 0 || next_row >= m || next_col < 0 || next_col >= n || laby[next_row][next_col] == 1)
					continue;
				if (pnext_row < 0 || pnext_row >= m || pnext_col < 0 || pnext_col >= n || laby[pnext_row][pnext_col] == 1)
					continue;

				f.box = box;
				f.player.row = pnext_row;
				f.player.col = pnext_col;
				Stack o;
				o.box.row = next_row;
				o.box.col = next_col;
				o.player.row = box.row;
				o.player.col = box.col;

				int z = compare(o,seen);

				if (z == 0)
					continue;

				int a = reach(laby, box, player, f.player);

				if (a == 1)
				{
					pos.row = next_row;
					pos.col = next_col;
					push(pos, box);

					seen[next_row][next_col][f.box.row][f.box.col] = 1;
				}
			}
		}
		++res;
	}
	return -1;
}

int reach(int** grid, element box, element player, element f)
{
	int start = -1;
	int end = -1;
	element q[MAX_STACK_SIZE];
	q[0] = player;
	end = 0;
	int** v;
	v = (int**)malloc(sizeof(int*) * m);
	for (int i = 0; i < m; i++)
		v[i] = (int*)malloc(sizeof(int) * n);

	v[player.row][player.col] = 1;
	grid[box.row][box.col] = 1;

	while (end > start)
	{
		start = (start + 1) % MAX_STACK_SIZE;
		element r = q[start];
		if (r.row == f.row && r.col == f.col)
		{
			grid[box.row][box.col] = 0;
			return 1;
		}
		int x = r.row;
		int y = r.col;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + move[dir].vert;
			int ny = y + move[dir].hor;

			if (nx < 0 || nx >= m || ny < 0 || ny >= n || grid[nx][ny] == 1 || v[nx][ny] == 1)
				continue;
			v[nx][ny] = 1;

			end = (end + 1) % MAX_STACK_SIZE;

			q[end].row = nx;
			q[end].col = ny;

		}

	}
	grid[box.row][box.col] = 0;
	return 0;
}


int compare(Stack s,int seen[21][21][21][21])
{
	if (seen[s.box.row][s.box.col][s.player.row][s.player.col] == 1)
		return 0;
	else return 1;

}