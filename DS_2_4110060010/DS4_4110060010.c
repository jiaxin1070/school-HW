#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int stamina;
    int num;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

typedef struct {
    Node* top;
} Ptr;

Node* createNode(int stamina,int num) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->stamina = stamina;
    newNode->num = num;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void initializeQueue(Queue* queue) {
    queue->front = queue->rear = NULL;
}
int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}


void enqueue(Queue* queue, int stamina,int num) {
    Node* newNode = createNode(stamina,num);
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    }
    else {
        newNode->prev = queue->rear;
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

void add(Queue* queue, Node* item) {
    Node* newNode = item;
        newNode->prev = queue->rear;
        queue->rear->next = newNode;
        queue->rear = newNode;
}

void dequeue(Queue* queue,Node* item) {
    
    if (isQueueEmpty(queue)) {
       //return -1; 
    }
    Node* temp = item;
    int stamina = temp->stamina;
    int num = temp->num;

    if (temp->next != NULL && temp->prev != NULL)
    {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }
    else if (temp->prev == NULL && temp->next != NULL)
    {
        temp->next->prev = NULL;
        queue->front = temp->next;
    }
    else if (temp->next == NULL && temp->prev != NULL)
    {
        temp->prev->next = NULL;
        queue->rear = temp->prev;
    }
    else if (temp->next == NULL && temp->prev == NULL)
    {
        queue->front = queue->rear = NULL;
    }


    free(temp);
}

Node* foundmax(Queue queue);
Node* foundmin(Queue queue);
void fight(Queue queue);
int compare(Queue queue, int a, Node* maxp, Node* secmin);


int maxnum;
int maxs;
int total;
int x;
int main() {

    int T,m;
    FILE* file;
    FILE* output;
    file = fopen("testcase4.txt", "r");	
    output = fopen("output4.txt", "w");
   
    if (file == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    fscanf(file, "%d", &T);
    for (int k = 0; k <T; k++)
    {
        fscanf(file, "%d", &m);

        Queue queue;
        initializeQueue(&queue);

        for (int i = 0; i < m; i++) {
            int stamina;
            fscanf(file, "%d", &stamina);
            enqueue(&queue, stamina, i);
        }
        total = m;
        x = 1;
        fight(queue);
        printf("%d\n", total);
        fprintf(output, "%d\n", total);
    }
    fclose(file);
    fclose(output);
    return 0;
}

Node* foundmax(Queue queue)
{
    Queue q = queue;
    Node* pmax=NULL;
    if (q.front->next != NULL)
    {

        pmax = q.front;
        Node* p = q.front->next;
        while (p != NULL)
        {
            if (pmax->stamina < p->stamina)
            {
                pmax = p;

            }
            else if (pmax->stamina == p->stamina)
            {
                if (pmax->num < p->num)
                {
                    pmax = p;
                }
            }
            p = p->next;
        }
    }
    return pmax;
}

Node* foundmin(Queue queue)
{
    Queue q = queue;
    Node* pmin = NULL;
    if (q.front != NULL)
    {

        pmin = q.front;
        Node* p = q.front->next;
        while (p != NULL)
        {
            if (pmin->stamina > p->stamina)
            {
                pmin = p;
            }
            else if (pmin->stamina == p->stamina)
            {
                if (pmin->num > p->num)
                {
                    pmin = p;
                }
            }
            p = p->next;
        }
    }
    return pmin;
}

void fight(Queue queue)
{
    Node* minp = foundmin(queue);
    int maxs;
    while (1)
    {
        Node* maxp = foundmax(queue);
        
        maxs = maxp->stamina;
        int mins = minp->stamina;
        int maxnum = maxp->num;
        int a = (maxs - mins);
        dequeue(&queue, minp);
        Node* secmin = foundmin(queue);
        int secmins = secmin->stamina;
        int secnum = secmin->num;

        if ((maxs - mins) > secmins)
        {
            maxp->stamina = a;
            total--;
        }
        else if (a == secmins && maxnum > secnum)
        {
            maxp->stamina =a;
            total--;
        }
        else if (maxnum == secnum)
        {
            total--;
            break;
        }
        else
        {
            
            if (a < secmins)
            {
                maxp->stamina = a;

                int c = compare(queue, a, maxp, secmin);
                if (c == 1)
                    total--;
                else if (c == 0 && x == 0)
                    total--;

                break;
            }
            else if (a == secmins && maxnum < secnum)
            {
                maxp->stamina = a;
                int c = compare(queue, a, maxp, secmin);
                if (c == 1)
                    total--;
                else if (c == 0 && x == 0)
                    total--;


                break;
            }
        
            break;
        }
        minp = secmin;
    }
    
}

int compare(Queue queue, int a, Node* maxp, Node* secmin)
{
    int i=0;
    Queue q = queue;
    Node* maxc = foundmax(q);
    if (maxc->num != secmin->num)
    {
        if ((maxc->stamina - a) == secmin->stamina)
        {
            
            
            if (maxc->num > secmin->num)
            {
                i = 0;
            }

            else
            {
                i = 1;
            }
            
        }
        else if ((maxc->stamina - a) > secmin->stamina)
        {
            i = 0;
        }
           
        else
        {
            
           maxc->stamina = maxc->stamina - a;
           dequeue(&q, maxp);
           x = 0;
           int c=compare(q, maxc->stamina, maxc, secmin);
           if (c == 0)
               i = 1;
           else
           {
                i =0;
                x = 1;
           }
               
        }
    }
    else
    {
        i = 0;
    }
    return i;
}
