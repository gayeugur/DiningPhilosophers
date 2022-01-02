
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_PHILOSOPHERS 8
#define NUMBER_OF_TABLE 10

double priceTableOpen=99.90;
double priceTableReorganize=19.90;
double priceRice=20;
double riceTableAmount=2000;
double eatingRiceQuantity=100;
double price;

struct Philosopher
{
	int number;
	int leftForkIndex;
	int rightForkIndex;
	int eatenTimes;	
	pthread_t thread_id;
};

struct Fork
{
	int index;
	sem_t mutex;
};

struct Fork* forks;
sem_t global_mutex;
int NotEatenCount = 0;


int is_finished()
{
	int counter = 0;
	sem_wait(&global_mutex);
	counter = NotEatenCount;
	sem_post(&global_mutex);	

	return counter==0;

}

void* philosopher_thread(void *argument)
{	
	struct Philosopher* philosopher = (struct Philosopher*)argument;
	int again = 1;

	while(again)
	{
		srand(time(NULL));
		printf("Philosopher %d is Thinking\n", philosopher->number);
		sleep(rand() % 5 + 1);
		printf("Philosopher %d is trying to eat...\n", philosopher->number);
		price+=priceTableOpen;
		
		
		if (sem_trywait(&forks[philosopher->leftForkIndex].mutex)==0)
		{
			price+=(priceRice*2);
			int waiting_times = rand() % 5 + 1;
	
			while(waiting_times>0)
			{
				
				if (sem_trywait(&forks[philosopher->rightForkIndex].mutex)==0)
				{		
					printf("Philosopher %d is Eating\n", philosopher->number);


					if (!philosopher->eatenTimes)
					{	
						sem_wait(&global_mutex);
						NotEatenCount--;
						sem_post(&global_mutex);
					}

					philosopher->eatenTimes++;

					sleep(rand() % 5 + 1);
					
					sem_post(&forks[philosopher->rightForkIndex].mutex);

					waiting_times =- waiting_times;
				} else {
					waiting_times--;
					sleep(rand() % 5 + 1);								
				}
			}

			if (waiting_times==0)
			{
				printf("Philosopher %d cannot take second fork...\n", philosopher->number);
				
			}
			/* put left fork on table */
			sem_post(&forks[philosopher->leftForkIndex].mutex);
			riceTableAmount-=eatingRiceQuantity;
		} else {
			printf("Philosopher %d cannot eat at this moment...\n", philosopher->number);
			
		}
		again = !is_finished();
	}
	
	printf("Price : %d",price);
}

int main(int argc, char* argv[])
{
	int groupNum;
	printf("Please write a group number\n");
	scanf("%d",&groupNum);
	printf("Group Num :\n",groupNum);
	int i;	

	struct Philosopher* philosophers = (struct Philosopher*) malloc(sizeof(struct Philosopher) * NUMBER_OF_PHILOSOPHERS);

	forks = (struct Fork*)malloc(sizeof(struct Fork) * NUMBER_OF_PHILOSOPHERS);

	sem_init(&global_mutex,0,1);


	NotEatenCount = NUMBER_OF_PHILOSOPHERS;

	for(i=0; i<NUMBER_OF_PHILOSOPHERS; i++)
	{
	        sem_init(&forks[i].mutex,0,1);

		philosophers[i].eatenTimes = 0;
		philosophers[i].number = i + 1;


		philosophers[i].leftForkIndex = i;

		if (i+1==NUMBER_OF_PHILOSOPHERS)
			philosophers[i].rightForkIndex = 0;
		else
			philosophers[i].rightForkIndex = i+1;
	}

    	for(i=0;i<NUMBER_OF_PHILOSOPHERS;i++)
        	pthread_create(&philosophers[i].thread_id, NULL, philosopher_thread, &philosophers[i]);



	while(!is_finished())
		usleep(100);


	for(i=0;i<NUMBER_OF_PHILOSOPHERS;i++)
	     pthread_join(philosophers[i].thread_id, NULL);


	printf("\nStatistics:\n");
	for(i=0;i<NUMBER_OF_PHILOSOPHERS;i++){

		printf("Philosopher %d eaten for %d times\n", philosophers[i].number, philosophers[i].eatenTimes);
    }
    	

	free(forks);
	free(philosophers);		
	return 0;	
}
