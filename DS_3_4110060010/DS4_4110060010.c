#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef struct Node
{
	int data;
	int cham;
	struct Node* left;
	struct Node* right;
	struct Node* parent;

}Node;

Node* createNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->cham = 0;
	newNode->left = newNode->right = NULL;
	newNode->parent = NULL;

	return newNode;
}

int search(int arr[], int strt, int end, int value)
{
	for (int i = strt; i <= end; i++)
	{
		if (arr[i] == value)
			return i;
	}
}
int preindex;
Node* buildTree(int preorder[], int inorder[], int instrt, int inend)
{

	if (instrt > inend)
		return NULL;

	Node* root = createNode(preorder[preindex++]);
	if (instrt == inend)
		return root;

	int inIndex = search(inorder, instrt, inend, root->data);
	root->left = buildTree(preorder, inorder, instrt, inIndex - 1);
	root->right = buildTree(preorder, inorder, inIndex + 1, inend);

	return root;
}
Node** p;
void preOrder(Node* root)
{
	if (root != NULL)
	{
		p[root->data] = root;
		if (root->left != NULL)
			root->left->parent = root;
		if (root->right != NULL)
			root->right->parent = root;


		preOrder(root->left);
		preOrder(root->right);
	}
}
void fill(Node* root);
void sip(Node* root);
int main()
{

	int T, n;
	FILE* file;
	FILE* output;

	file = fopen("testcase4.txt", "r");
	output = fopen("output4.txt", "w");
	if (file == NULL)
	{
		printf("error!");
		exit(1);
	}
	fscanf(file, "%d", &T);
	for (int k = 0; k < T; k++)
	{
		fscanf(file, "%d", &n);
		preindex = 0;

		int* inorder = (int*)malloc(sizeof(int) * n);
		int* preorder = (int*)malloc(sizeof(int) * n);

		for (int i = 0; i < n; i++)
			fscanf(file, "%d", &inorder[i]);
		for (int i = 0; i < n; i++)
			fscanf(file, "%d", &preorder[i]);

		int q;
		fscanf(file, "%d", &q);
		int* c = (int*)malloc(sizeof(int) * q);
		int* v = (int*)malloc(sizeof(int) * q);
		for (int i = 0; i < q; i++)
		{
			fscanf(file, "%d", &c[i]);
			fscanf(file, "%d", &v[i]);
		}
		Node* root = buildTree(preorder, inorder, 0, n - 1);
		p = (Node**)malloc(sizeof(Node*) *( n+1));
		preOrder(root);
		for (int i = 0; i < q; i++)
		{
			switch (c[i])
			{
			case 1:
				fill(p[v[i]]);
				break;

			case 2:
				sip(p[v[i]]);
				break;

			default:
				fprintf(output, "%d\n", p[v[i]]->cham);
				break;
			}
		}
		fflush(output);
		free(preorder);
		free(inorder);
		free(root);
		free(c);
		free(v);
	}
	free(p);
	fclose(file);
	fclose(output);

	return 0;
}

void fill(Node* root)
{
	root->cham = 1;
	if (root->left != NULL)
		fill(root->left);
	if (root->right != NULL)
		fill(root->right);
}

void sip(Node* root)
{
	root->cham = 0;
	if (root->parent != NULL)
		sip(root->parent);
}
