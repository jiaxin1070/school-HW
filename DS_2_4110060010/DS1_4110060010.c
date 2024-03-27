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

typedef struct Node
{
	int row, col, dir;
	struct Node* next;
} Node;
typedef Node* Stack;
//int top = -1;

void push(Stack* topPtr, Node item)
{
	Stack newPtr;
	newPtr = malloc(sizeof(Node));
	(*newPtr).row = item.row;
	(*newPtr).col = item.col;
	(*newPtr).dir = item.dir;
	if(!topPtr)
		(*newPtr).next = NULL;
	else
		(*newPtr).next = *topPtr;
	*topPtr = newPtr;
	//top++;
	
}

Node pop(Stack* topPtr)
{
	Stack tempPtr;
	Node popValue;
	tempPtr = *topPtr;
	popValue.row = (*topPtr)->row;
	popValue.col = (*topPtr)->col;
	popValue.dir = (*topPtr)->dir;
	*topPtr = (*topPtr)->next;
	//top--;
	free(tempPtr);
	return popValue;

}

Stack ptr[MAX_STACK_SIZE];
int** map;
int** laby;
int m;
int n;
int end_x;
int end_y;
int sx;
int sy;
int kind;
Node start;

void Map();
void go();
int compare(int** q);

int main()
{

	int T;
	FILE* file;
	FILE* file2;
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
		ptr[0] = NULL;
		kind = 0;
		
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
					
					start.row = i;
					start.col = j;
					start.dir = 0;
					start.next = NULL;
					push(ptr, start);
					map[i][j] = 1;
					sx = i;
					sy = j;
					//top = 0;
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
	fclose(output);

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
	Node *next;
	Node position;
	Node pos;
	Stack p1;
	Map();
	while (*ptr!=NULL)
	{
		pos=pop(ptr);
		nowrow = pos.row;
		nowcol = pos.col;
		dir = pos.dir;
		next = pos.next;
		push(ptr, pos);
		
		
		while (dir < 4)
		{
			p1 = *ptr;
			nextrow = nowrow + move[dir].vert;
			nextcol = nowcol + move[dir].hor;

			if (nextrow == end_x && nextcol == end_y)
			{

				(* p1).dir = ++dir;
				nowrow = (*p1).row;
				nowcol = (*p1).col;
				dir = (*p1).dir;
				int a = compare(map);
				if (a == 1)
				{
					kind++;
				}


			}
			else if (nextrow >= 0 && nextrow < m && nextcol >= 0 && nextcol < n && laby[nextrow][nextcol] == 0 && map[nextrow][nextcol] == 0)
			{
				(*p1).dir = ++dir;
				position.row = nextrow;
				position.col = nextcol;
				position.dir = 0;
				position.next = NULL;
				push(ptr,position);
				map[nextrow][nextcol] = 1;
				nowrow = nextrow;
				nowcol = nextcol;
				dir = 0;
			}
			else
				dir++;
		}
		map[nowrow][nowcol] = 0;
		pop(ptr);
		
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
	if ((count - 1) == 0)
		return 1;
	return 0;
}



