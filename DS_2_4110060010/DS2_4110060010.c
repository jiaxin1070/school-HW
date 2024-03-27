#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int order, num;
    struct Node* next, * prev;
};

struct LinkedList
{
    struct Node* head;
};

int gcd(int a, int b);
struct Node* createNode(int order, int num);
struct LinkedList* List(int m, int* card);
void game(struct LinkedList* list);

int store[50000];
int count ;
int all ;
int len;

int main() {
    
    int T,m;
    int* card;
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
    for (int k = 0; k <T; k++)
    {
        fscanf(file, "%d", &m);

        card = (int*)malloc(sizeof(int) * m);
        for (int i = 0; i < m; i++)
        {
            fscanf(file, "%d", &card[i]);
        }

        struct LinkedList* list = List(m, card);
        len = m;
        game(list);
       // printf("%d ", all); 
        fprintf(output, "%d ", all);
       
        for (int i = 0; i < count; i++)
        {
            if (i != count - 1)
            {
                //printf("%d ", store[i]);
                fprintf(output, "%d ",store[i]);
            }
            else
            {
                //printf("%d\n", store[i]);
                fprintf(output, "%d\n", store[i]);
            }
           
        }
        fflush(output);
        //printf("\n");
       
        free(card);
       
    }
    fclose(file);
    fclose(output);
    return 0 ;
}

int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}


struct Node* createNode(int order, int num)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->order = order;
    newNode->num = num;
    newNode->next = newNode->prev = NULL;

    return newNode;
}

struct LinkedList* List(int m, int* card)
{
    struct LinkedList* list = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    struct Node* current = NULL;
    struct Node* prevNode = NULL;

    for (int i = 0; i < m; i++)
    {
        struct Node* newNode = createNode(i + 1, card[i]);
        if (i == 0)
        {
            list->head=newNode;
            current = newNode;
            prevNode = newNode;
        }
        else
        {
            current->next = newNode;
            newNode->prev = current;
            current = newNode;
        }
    }

    current->next = list->head;
    list->head->prev = current;

    return list;
}

void game(struct LinkedList* list)
{
    int same = 0;
    all = 0;
    count = 0;
    struct Node* current = list->head;

    while (same < 3)
    {
        int c = len;
        for (int i = 0; i < len; i++)
        {
            if (current->next != current)
            {
                struct Node* nextC = current->next;
                if (gcd(current->num, nextC->num) == 1)
                {
                    all++;
                    store[count++] = nextC->order;
                    struct Node* prev = nextC->prev;
                    struct Node* next = nextC->next;
                    prev->next = next;
                    next->prev = prev;
                    len--;
                    free(nextC);
                }
                current = current->next;
            }
            else
            {
                all++;
            }
        }
        if (c == len)
            same += 1;
    }
    
}
