#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    struct Node* left, * right;
} Node;

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

int mincover(Node* root);
int helper(Node* node);
int res;
int main() {
    int T;
    FILE* file;
    FILE* output;
    file = fopen("testcase2.txt", "r");
    output = fopen("output2.txt", "w");
    if (file == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    fscanf(file, "%d", &T);

    for (int k = 0; k <T+1; k++)
    {
        char* treeInput = (char*)malloc(sizeof(char) *20000000);

        int n = 0;
        for (int i = 0; i < 20000000; i++)
        {
            char x;
            fscanf(file, "%c", &x);

            if (x == '\n')
                break;
            else
            {
                if (x == ',')
                    continue;
                else
                {
                    treeInput[n] = x;
                    n += 1;
                }

            }
        }
        if (k > 0)
        {
            Node** nodes = (Node**)malloc(sizeof(Node*) * n);
            for (int i = 0; i < n; ++i) {
                if (treeInput[i]=='N') {
                    nodes[i] = NULL;
                }
                else {
                    nodes[i] = createNode(treeInput[i]);
                }
            }
            int b = 0;
            int f = 0;
            for (int i = 0; i < n; ++i) {
                if (nodes[i] != NULL) {
                    if (f == 0)
                    {
                        if (2 * i + 1 < n && nodes[2 * i + 1] != NULL) {
                            nodes[i]->left = nodes[2 * i + 1];
                        }
                        if (2 * i + 2 < n && nodes[2 * i + 2] != NULL) {
                            nodes[i]->right = nodes[2 * i + 2];
                        }
                    }
                    else
                    {
                        int a = 2 * i - b;
                        if (a + 1 < n && nodes[a + 1] != NULL) {
                            nodes[i]->left = nodes[a + 1];
                        }
                        if (a + 2 < n && nodes[a + 2] != NULL) {
                            nodes[i]->right = nodes[a + 2];
                        }
                    }

                }
                else
                {
                    b += 2;
                    f = 1;
                }
            }

            res = 0;
            int result = mincover(nodes[0]);
            printf("%d\n", result);
            fprintf(output, "%d\n", result);
            fflush(output);
            free(treeInput);
            free(nodes);
        }
    }
    fclose(file);
    fclose(output);
    return 0;
}
int mincover(Node* root)
{

    return (helper(root) < 1 ? 1 : 0) + res;
}
int helper(Node* node)
{
    if (!node) return 2;
    int left = helper(node->left);
    int right = helper(node->right);

    if (left == 0 || right == 0)
    {
        ++res;
        return 1;
    }

    return (left == 1 || right == 1) ? 2 : 0;
}