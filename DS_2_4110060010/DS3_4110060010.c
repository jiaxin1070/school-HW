#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int height;
	struct Node* next;
} Node;

typedef struct
{
	Node* top;
} Stack;

Node* createNode(int height)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->height = height;
	newNode->next = NULL;

	return newNode;
}

void push(Stack* stack, int height)
{
	Node* newNode = createNode(height);
	newNode->next = stack->top;
	stack->top = newNode;
}

int pop(Stack* stack)
{
	if (stack->top == NULL)
		return -1;
	Node* temp = stack->top;
	int height = temp->height;
	stack->top = temp->next;
	free(temp);
	return height;
}

int top(Stack* stack)
{
	if (stack->top == NULL)
		return -1;
	
	return stack->top->height;
}

int isEmpty(Stack* stack) {
	return stack->top == NULL;
}

void clearStack(Stack* stack) {
	while (!isEmpty(stack)) {
		pop(stack);
	}
}

int main()
{

	int T, m;
	FILE* file;
	FILE* output;
	int* u;

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
		u = (int*)malloc(sizeof(int) * m);

		Stack stack;
		stack.top = NULL;
		int total = 0;

		for (int i = 0; i < m; i++)
		{
			int h;
			fscanf(file, "%d", &u[i]);
			push(&stack, u[i]);
		}

		int ctotal = 0;
		int* r;
		int* l;
		r = (int*)malloc(sizeof(int) * m);
		l = (int*)malloc(sizeof(int) * m);
		Stack tempStack;
		Stack tempStack2;
		tempStack.top = NULL;
		tempStack2.top = NULL;

		int maxr=0;
		int maxl;
		int sum = 0;
		int p1=0;
		int p2 = 0;
		for (int i = 0; i < m; i++)
		{

			while (!isEmpty(&tempStack) && u[i] < u[top(&tempStack)])
			{
				
				p1 = top(&tempStack);
				pop(&tempStack);
				p2 = top(&tempStack);
			    
				if (p1 == 0)
					sum -= u[p1];
				else
				{
					if (p2 < 0)
						sum -= (p1+1) * u[p1];
					else
						sum -= abs(p1 - p2) * u[p1];
				}
	
			}
			
			if (tempStack.top == NULL)
			{
				if(i==0)
					sum += u[i];
				else sum += abs(i+1) * u[i];
			}
				
			else
				sum += abs(i - top(&tempStack)) * u[i];
			
			r[i] = sum;
			push(&tempStack, i);
			
		}
	  

		int sum2 = 0;
		for (int i = m-1; i >= 0; i--)
		{
			
			while (!isEmpty(&tempStack2) && u[i]< u[top(&tempStack2)])
			{
				p1 = top(&tempStack2);
				pop(&tempStack2);
				p2 = top(&tempStack2);
				
				if (p1 == m - 1)
					sum2 -= u[p1];
				else
				{
					if (p2 < 0)
						sum2 -= ((m-1)-p1+1) * u[p1];
					else
						sum2 -= abs(p1 - p2) * u[p1];
				}
			}
			
			if (tempStack2.top == NULL)
			{
				if (i == (m-1))
					sum2 += u[i];
				else sum2 += abs((m-1)-i+1)* u[i];
			}
				
			else
				sum2 += abs(i - top(&tempStack2)) * u[i];
			
			l[i] = sum2;
			push(&tempStack2, i);
			
		}
		int ma = 0;
		for (int i = 0; i < m; i++)
		{
			int t = r[i] + l[i]-u[i];
			if (ma < t)
				ma = t;
		}
		
		free(u);
		printf("%d\n", ma);
		fprintf(output, "%d\n", ma);
		
	}
	fclose(file);
	fclose(output);
	
	return 0;
}