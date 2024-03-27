#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define MINPID 0
#define MAXPID 128
pthread_mutex_t mutex;
unsigned int bitmap[MAXPID / (4 * 8)];
int k = MINPID - 1;
int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);
void* start_routine(void* arg);

int main()
{
	int p = 0;
	allocate_map();

	while (p == 0)
	{
	L2:printf("----------------------------------------\n");
		printf("(1).create 100 process (2).exit\n");
		printf(" Please enter your choice : ");

		int c;
		scanf("%d", &c);

		if (c == 1)
		{
			pthread_mutex_init(&mutex, NULL);
			pthread_t thread[100];
			int rc[100];

			srand(time(NULL));
			for (int i = 0; i < 100; i++)
			{

				int t;
				t = (rand() % 3) + 1;
				rc[i] = pthread_create(&thread[i], NULL, start_routine, (void*)t);
				if (rc[i])
				{
					printf("error from pthread_create!\n");
					exit(-1);
				}
			}

			for (int i = 0; i < 100; i++)
			{
				void* reBuf;
				rc[i] = pthread_join(thread[i], NULL);
				if (rc[i])
				{
					printf("error from pthread_join!\n");
					exit(-1);
				}

			}

			pthread_mutex_destroy(&mutex);

		}
		else if (c == 2)
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

	pthread_mutex_lock(&mutex);
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
		pthread_mutex_unlock(&mutex);
		return 1;
	}
}
else
{
	pthread_mutex_unlock(&mutex);
	return -1;
}
}

void release_pid(int pid)
{
	pthread_mutex_lock(&mutex);
	int tmp = bitmap[(pid / 32)] & (1 << (pid % 32));
	if (tmp == 0)
		printf("\nThis Pid is not allocated\n\n\n");

	else
	{
		bitmap[(pid / 32)] = bitmap[(pid / 32)] & ~(1 << (pid % 32));
		k = MINPID - 1;
	}

	pthread_mutex_unlock(&mutex);
}

void* start_routine(void* arg)
{

	int a = allocate_pid();
	int pid = k;
	int t;
	t = (int)arg;
	printf("pid is %d, will sleep %d seconds\ntid is %lu\n", pid, t, pthread_self());
	sleep(t);

	release_pid(pid);

	pthread_exit(NULL);

}