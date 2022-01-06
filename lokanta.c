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
	int id;
	int solCatal;
	int sagCatal;
	int eatTimes;
	int tazelemeSayisi;	
	pthread_t thread;
};

struct CatalM
{
	sem_t mutex;
};

struct Table{
	int totalTableNumber;
};

struct CatalM* forks;
sem_t lock;
int yemeyen = 0;
int taz=0;

int is_finished()
{
	int cont = 0;
	sem_wait(&lock);
	cont = yemeyen;
	sem_post(&lock);	
	taz++;
	return cont==0;

}
double tpirinc=2000;

void* philosopher_thread(void *argument)
{	
	int count=0;

	int again = 1;	
	double pirinc=100;
	
	struct Philosopher* philosopher = (struct Philosopher*)argument;
	
	while(again)
	{	
	count++;
		srand(time(NULL));
		printf("Philosopher %d is Thinking\n", philosopher->id);	
		int time = rand() % 5 + 1;
		sleep(time);
				
		if (sem_trywait(&forks[philosopher->solCatal].mutex)==0 && sem_trywait(&forks[philosopher->sagCatal].mutex)==0)
		{
			while(time>0){	
					if(tpirinc>100){
					printf("Philosopher %d is Eating\n", philosopher->id);
					}

					if (!philosopher->eatTimes)
					{	
						sem_wait(&lock);
						yemeyen--;
						sem_post(&lock);
					
					}

					philosopher->eatTimes++;
				//	sleep(rand() % 5 + 1);
					
					sem_post(&forks[philosopher->sagCatal].mutex);

					time =- time;
				} 
				sem_post(&forks[philosopher->solCatal].mutex);
		}else {
			printf("Philosopher %d not eat \n", philosopher->id);

		}
		again = !is_finished();
		
		
		
		
	}
	
	
}


int main(int argc, char* argv[])
{	

	double priceTableOpen=99.90;
	double priceTableReorganize=19.90;
	double rice=20;

	double pirinc=100;
	double tpirinc=2000;
	
	double price;
	int tazeleme;
	
	int i,j;
	
	int groupNum;	
	printf("Please write a group number\n");
	scanf("%d",&groupNum);
	
	int a=groupNum*NUMBER_OF_PHILOSOPHERS;

	if(groupNum<NUMBER_OF_TABLE){
	
	struct Philosopher* philosophers = (struct Philosopher*) malloc(sizeof(struct Philosopher) * a);

	forks = (struct CatalM*)malloc(sizeof(struct CatalM) * a);
	
	struct Table* table = (struct Table*) malloc(sizeof(struct Table) * groupNum);
	table->totalTableNumber=groupNum;
	
	 //restaurant kapasitesi kadar kilitlenme yapılır
	//sem_wait(&lock);
	sem_init(&lock,0,NUMBER_OF_TABLE);
	
	yemeyen = a;
	for(i=0; i<a; i++)//filozoflar
		{
		
	        sem_init(&forks[i].mutex,0,1);

		philosophers[i].eatTimes = 0;
		philosophers[i].id = i + 1;


		philosophers[i].solCatal = i;

		if (i+1==a){
			philosophers[i].sagCatal = 0;
		}else{
			philosophers[i].sagCatal = i+1;
		}
		
		pthread_create(&philosophers[i].thread, NULL, philosopher_thread, &philosophers[i]);
	 	
		
		}

	for(i=0;i<a;i++){
	     pthread_join(philosophers[i].thread, NULL);
}


	printf("\nStatistics:\n");

	//for(j=0; j<groupNum; j++){
	price+=priceTableOpen;
	price+=(2*rice);
	int count=0;
	int n=1;
	for(i=0;i<a;i++){
		count++;
		if(tpirinc>100){
			printf("Philosopher %d eaten for %d times\n", philosophers[i].id, philosophers[i].eatTimes);
			tpirinc-=(philosophers[i].eatTimes*100);
		}
    			
    			if(yemeyen!=0 && tpirinc<100)
    			{
    				printf("tazeleme var");
    				tazeleme++;
    				price+=priceTableReorganize*taz;
				price+=(2*rice*taz);
    			}
    		  	if(count==8){
    		  	printf("table [%d] : tazeleme sayısı  %d\n",n,tazeleme);
    			printf("table [%d] : Yenilen Toplam Pirinc : %.2f\n",n ,2000-tpirinc);
    			printf("table [%d] : Price  : %.2f\n",n, price);
    			n++;
    			count=0;
    			tpirinc=2000;
    			    		  	
    		  	}

    	}
    	
  	free(table);
	free(forks);
	free(philosophers);
	}else{
	printf("10 dan fazla işlm olamazzz");
	}		
	return 0;	
}
