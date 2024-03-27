#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct node* treePtr;
typedef struct node
{
	int data;
	treePtr lchild, rchild;
}node;

treePtr newNode(int data)
{
	treePtr newnode = (treePtr)malloc(sizeof(struct node));
	newnode->data= data;
	newnode->lchild = newnode->rchild = NULL;

	return newnode;

}

treePtr buildTree(int inorder[], int postorder[],int instart, int inend, int* postIndex, int* hashTable)
{
	if (instart > inend)
		return NULL;

	treePtr root = newNode(postorder[*postIndex]);
	(*postIndex)--;

	if (instart == inend)
		return root;

	int inIndex = hashTable[root->data];

	root->rchild = buildTree(inorder, postorder, inIndex + 1, inend, postIndex, hashTable);
	root->lchild = buildTree(inorder, postorder, instart, inIndex - 1, postIndex, hashTable);

	return root;
}
FILE* output;
void preorder(treePtr root)
{
	if (root != NULL)
	{
		fprintf(output, "%d ", root->data);
		//printf("%d ", root->data);
		preorder(root->lchild);
		preorder(root->rchild);
	}
}
int main()
{
	int T, n;
	FILE* file;
	
	file = fopen("testcase1.txt", "r");
	output = fopen("output1.txt", "w");
	if (file == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}

	fscanf(file, "%d", &T);
	for (int k = 0; k <T;k++)
	{
		fscanf(file, "%d", &n);

		int* inorder = (int*)malloc(n * sizeof(int));
		int* postorder = (int*)malloc(n * sizeof(int));
		int* hashTable = (int*)malloc((6 * n + 1) * sizeof(int));
		for (int i = 0; i < n; i++)
		{
			fscanf(file, "%d", &inorder[i]);
			hashTable[inorder[i]] = i;
		}
		
		for (int i = 0; i < n; i++)
			fscanf(file, "%d", &postorder[i]);

		int postindex = n - 1;
		treePtr root = buildTree(inorder, postorder, 0, n - 1, &postindex, hashTable);
		
		preorder(root);
		//printf("\n");
		fprintf(output, "\n");
		fflush(output);
		free(inorder);
		free(postorder);
		free(hashTable);
	}
	fclose(file);
	fclose(output);
	return 0;
}