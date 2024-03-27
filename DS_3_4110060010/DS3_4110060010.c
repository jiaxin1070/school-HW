#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int data;
	struct Node* left;
	struct Node* right;
}Node;

Node* createNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->left = newNode->right = NULL;

	return newNode;
}

int y;
int* extrackKeys(int in[], int level[], int m, int n, int x, int f, int* hashTable, int p, int q)
{
	int* newlevel;
	int j = 0;

	if (m < 0)
	{
		newlevel = NULL;
		return newlevel;
	}
	else if (m == 0)
	{
		y = -1;
		return NULL;
	}
	newlevel = (int*)malloc(sizeof(int) * m);

	for (int i = 0; i < n; i++)
	{
		int index = hashTable[level[i]];
		if (index >= p && index < q)
		{
			newlevel[j] = level[i];
			j++;
		}
	}
	y = j;

	return newlevel;

}

Node* buildTree(int in[], int level[], int instrt, int inend, int n, int x, int num, int* hashTable)
{

	if (instrt > inend)
		return NULL;

	Node* root = createNode(level[0]);

	if (instrt == inend)
		return root;

	int inIndex = hashTable[root->data];

	int* llevel = extrackKeys(in, level, inIndex, n, inIndex, inIndex, hashTable, instrt, inIndex);
	int l = y - 1;
	y = 0;

	int* rlevel = extrackKeys(in + inIndex + 1, level, n - 1, n, x, num - inIndex - 1, hashTable, inIndex + 1, num);
	int r = y - 1;
	y = 0;
	if (llevel != NULL)
		root->left = buildTree(in, llevel, instrt, inIndex - 1, inIndex - instrt, inIndex - 1 - instrt, num, hashTable);
	else {
		if (l == -1)
			root->left->data = in[inIndex - 1];
	}

	if (rlevel != NULL)
		root->right = buildTree(in, rlevel, inIndex + 1, inend, inend - inIndex, n - (inIndex - 1 - instrt), num, hashTable);
	else {
		if (r == -1)
		{
			if ((inIndex + 1) < n)
				root->right->data = in[inIndex + 1];
		}

	}
	free(llevel);
	free(rlevel);

	return root;
}



void inOrder(Node* root)
{
	if (root != NULL)
	{

		inOrder(root->left);
		printf("%d ", root->data);
		inOrder(root->right);
	}
}

int game(Node* root, int n, int x);
int count(Node* node, int x);
int left;
int right;

int main()
{
	FILE* file;
	FILE* output;
	int T;
	int x;
	int n;

	file = fopen("testcase3.txt", "r");
	output = fopen("output3.txt", "w");
	if (file == NULL)
	{
		printf("Error!");
		exit(1);
	}

	fscanf(file, "%d", &T);
	for (int k = 0; k < T; k++)
	{
		int* inorder;
		int* level;
		int* hashTable;
		fscanf(file, "%d", &x);
		fscanf(file, "%d", &n);

		inorder = (int*)malloc(sizeof(int) * n);
		level = (int*)malloc(sizeof(int) * n);
		hashTable = (int*)malloc((6 * n + 1) * sizeof(int));

		for (int i = 0; i < n; i++)
		{
			fscanf(file, "%d", &inorder[i]);
			hashTable[inorder[i]] = i;
		}


		for (int i = 0; i < n; i++)
			fscanf(file, "%d", &level[i]);
	
		int index = hashTable[level[0]];
		Node* root = buildTree(inorder, level, 0, n - 1, n, n - index, n, hashTable);
		left = 0;
		right = 0;
		//inOrder(root);
		//printf("\n");
		int ans = game(root, n, x);
		printf("%d\n", ans);
		fprintf(output, "%d\n", ans);
		fflush(output);
		free(root);
		free(inorder);
		free(level);
		free(hashTable);

	}
	fclose(file);
	fclose(output);

	return 0;
}
int game(Node* root, int n, int x)
{

	int np;
	count(root, x);
	np = n - left - right - 1;
	if (left > (n / 2) || right > (n / 2) || np > (n / 2))
		return 1;

	return 0;
}
int count(Node* node, int x)
{
	if (node == NULL)
		return 0;

	int leftc = count(node->left, x);
	int rightc = count(node->right, x);
	if (node->data == x)
	{
		left = leftc;
		right = rightc;
		return 0;
	}

	return 1 + leftc + rightc;
}