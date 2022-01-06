#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_PHILOSOPHERS 8
#define NUMBER_OF_TABLE 10


struct Philosopher
{
	int number;
	int leftForkIndex;
	int rightForkIndex;
	int eatenTimes;
	int food;	
	pthread_t thread_id;
};

struct Fork
{
	int index;
	sem_t mutex;
};

struct Table{
	int tableNumber;
	int totalTableNumber;
	int tazeleme;
};

struct Fork* forks;
sem_t lock;
int NotEatenCount = 0;

int is_finished()
{
	int counter = 0;
	sem_wait(&lock);
	counter = NotEatenCount;
	sem_post(&lock);	

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
				
		
		if (sem_trywait(&forks[philosopher->leftForkIndex].mutex)==0)
		{
			int waiting_times = rand() % 5 + 1;
	
			while(waiting_times>0){	
				if (sem_trywait(&forks[philosopher->rightForkIndex].mutex)==0)
				{		
					printf("Philosopher %d is Eating\n", philosopher->number);
					philosopher->food=1;
					

					if (!philosopher->eatenTimes)
					{	
						sem_wait(&lock);
						NotEatenCount--;
						sem_post(&lock);
					
					}

					philosopher->eatenTimes++;

				//	sleep(rand() % 5 + 1);
					
					sem_post(&forks[philosopher->rightForkIndex].mutex);

					waiting_times =- waiting_times;
				} else {
					waiting_times--;
					//sleep(rand() % 5 + 1);								
				}
			}

							
			/* put left fork on table */
			sem_post(&forks[philosopher->leftForkIndex].mutex);
		} else {
			printf("Philosopher %d cannot eat at this moment...\n", philosopher->number);
			philosopher->eatenTimes=0;
			
		}
		
		again = !is_finished();
		
	}
	
	
}


int main(int argc, char* argv[])
{	
	double priceTableOpen=99.90;
	double priceTableReorganize=19.90;
	double rice=20;
	double riceTableAmount=2000;
	double eatingRiceQuantity=100;
	double pirinc=100;
	double tpirinc=2000;
	int groupNum;
	double price;
	
	int i,j;
	int tekrar=0;
	
	while(tekrar==0){	
	printf("Please write a group number\n");
	scanf("%d",&groupNum);
	int a=groupNum*NUMBER_OF_PHILOSOPHERS;

	if(groupNum<NUMBER_OF_TABLE){
	
	struct Philosopher* philosophers = (struct Philosopher*) malloc(sizeof(struct Philosopher) * a);

	forks = (struct Fork*)malloc(sizeof(struct Fork) * a);
	
	struct Table* table = (struct Table*) malloc(sizeof(struct Table) * groupNum);
	table->totalTableNumber=groupNum;
	
	 //restaurant kapasitesi kadar kilitlenme yapılır
	//sem_wait(&lock);
	sem_init(&lock,0,NUMBER_OF_TABLE);
	
	NotEatenCount = a;
	for(i=0; i<a; i++)//filozoflar
		{
		
	        sem_init(&forks[i].mutex,0,1);

		philosophers[i].eatenTimes = 0;
		philosophers[i].number = i + 1;


		philosophers[i].leftForkIndex = i;

		if (i+1==a){
			philosophers[i].rightForkIndex = 0;
		}else{
			philosophers[i].rightForkIndex = i+1;
		}
		
		pthread_create(&philosophers[i].thread_id, NULL, philosopher_thread, &philosophers[i]);

		
		}

	
	
    	//for(i=0;i<a;i++)
        //	pthread_create(&philosophers[i].thread_id, NULL, philosopher_thread, &philosophers[i]);



	//while(!is_finished())
	//	usleep(100);


	for(i=0;i<a;i++){
	     pthread_join(philosophers[i].thread_id, NULL);
}


	printf("\nStatistics:\n");
	int tazeleme=table->tazeleme;
	//for(j=0; j<groupNum; j++){
	price+=priceTableOpen;
	price+=(2*rice);
	int count=0;
	int n=1;
	for(i=0;i<a+1;i++){
		count++;
		if(tpirinc>100){
			printf("Philosopher %d eaten for %d times\n", philosophers[i].number, philosophers[i].eatenTimes);
			tpirinc-=(philosophers[i].eatenTimes*100);
		}
		
			int k; 
			int control=0;
			for(k=0; k<NUMBER_OF_PHILOSOPHERS; k++){
				if(philosophers[k].eatenTimes==0){
					control+=1;
				}
			}
			if(control>0){
    				tekrar=0;
				tazeleme+=1;
				price+=priceTableReorganize;
				price+=(2*rice);
    			}else{
    				tekrar=1;
    			}
    			
    		  	if(count==8){
    		  	printf("table [%d] : tazeleme sayısı  %d\n",n,tazeleme);
    			printf("table [%d] : Yenilen Toplam Pirinc : %.2f\n",n ,2000-tpirinc);
    			printf("table [%d] : Price  : %.2f\n",n, price);
    			n++;
    			count=0;
    			tpirinc=2000;
    		//	price=0.0;
    		  	
    		  	}
    		
   		
    	}
    	
    		
  	
	free(forks);
	free(philosophers);
	}else{
	printf("10 dan fazla işlm olamazzz");
	}
	}		
	return 0;	
}
