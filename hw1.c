#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MINPID 0
#define MAXPID 128

int bitmap[MAXPID / (4 * 8)];
int k = MINPID-1;

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);

int main()
{
	int p=0;
	allocate_map();

	printf("-----------Allocating bitmap----------------\n");
	for (int i = 0; i < (MAXPID / (4 * 8)); i++)
		printf("bitmap[%d] = %d\n", i, bitmap[i]);

	while (p==0)
	{
		L2:printf("----------------------------------------\n");
		printf("(1)creat a process (2)delete a process (3)exit\n");
		printf(" Please enter your choice : ");

		int c;
		scanf("%d", &c);

		if (c == 1)
		{
			int a=allocate_pid();

			if (a == 1)
			{
				printf("Sucessful to allocate PID. The PID of new process : %d\n", k);

				for (int i = 0; i < (MAXPID / (4 * 8)); i++)
					printf("bitmap[%d] = %d\n", i, bitmap[i]);
			}

			else
			{
				printf("Fail to allocate PID.");
				goto L2;
			}
				

		}

		else if (c == 2)
		{
			int pid;
			printf("Please enter the PID you want to delete : ");
			scanf("%d", &pid);

			release_pid(pid);
		}
		else if (c == 3)
		{
			printf("Exit this process !\n");
			p += 1;
		}
		else
			printf("Enter is not in range please enter again\n");
	}

	return 0;
}

int allocate_map(void)
{
	for (int i = 0; i < (MAXPID / (4 * 8)); i++)
		bitmap[i] = 0;

	int count = 0;
	for (int i = 0; i < (MAXPID / (4 * 8)); i++)
	{
		if (bitmap[i] == 0)
			count++;
	}
	if (count != (MAXPID / (4 * 8)))
		return -1;
	else 
		return 1;
}

int allocate_pid(void)
{
	k++;
L1:if (k < MAXPID)
{
	int tmp = bitmap[(k / 32)] & (1 << (k % 32));

	if (tmp != 0)
	{
		k++;
		goto L1;
	}

	else
	{
		bitmap[(k / 32)] = bitmap[(k / 32)] | (1 << (k % 32));
		return 1;
	}
}
else return -1;
}

void release_pid(int pid)
{
	int tmp = bitmap[(pid / 32)] & (1 << (pid % 32));
	printf("\n");

	if (tmp == 0)
		printf("\nThis Pid is not allocated\n\n\n");

	else
	{
		bitmap[(pid / 32)]=bitmap[(pid / 32)]&~(1 << (pid % 32));

		for (int i = 0; i < (MAXPID / (4 * 8)); i++)
			printf("bitmap[%d] = %d\n", i, bitmap[i]);

		k = MINPID - 1;
	}
}