#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

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
} philosophers;

struct Fork
{
	int index;
	sem_t mutex;
};

sem_t global_mutex;
int NotEatenCount = 0;
struct Fork* forks;

int is_finished()
{
	int counter = 0;
	sem_wait(&global_mutex);
	counter = NotEatenCount;
	sem_post(&global_mutex);

	return counter==0;
}


void philosopher_thread(void *argument){
    int i;
    int waitingNumber,status;
    int groupNum=1;
    int forkChild[groupNum]; 

    	struct Philosopher* philosopher = (struct Philosopher*)argument;
    	int again=1;
    	
		if((NUMBER_OF_PHILOSOPHERS*NUMBER_OF_TABLE)> NUMBER_OF_PHILOSOPHERS*groupNum){
			price+=priceTableOpen;
			
    			for (i = 0; i < 1; i++){
    				while(again){	
        				forkChild[i] = fork();
        				if (forkChild[i] > 0){//işlemler
        				//	sleep(rand() % 5 + 1);
        					int j; 
        					for(j=0; j<8; j++){
      						 printf("Philosopher %d is Thinking\n", philosopher[j].number);	
      					//	 sleep(rand() % 5 + 1);
      						 printf("Philosopher %d is trying to eat...\n", philosopher[j].number);
      						 	if (sem_trywait(&forks[philosopher[j].leftForkIndex].mutex)==0 ){
      								if (sem_trywait(&forks[philosopher[j].rightForkIndex].mutex)==0)
								{	 
									printf("Philosopher %d is Eating\n", philosopher[j].number);
									if (!philosopher[j].eatenTimes){
										sem_wait(&global_mutex);
										NotEatenCount--;
										sem_post(&global_mutex);
									}
									
									philosopher[j].eatenTimes++;
									sem_post(&forks[philosopher[j].rightForkIndex].mutex);
									sem_post(&forks[philosopher[j].leftForkIndex].mutex);
								} 
      						 	}
      						 }     						  
      						 
        				}
        				else{
            					printf("Çıkış yapıyorum\n");
            					exit(0);
        				}
        
    				} 
    			}
    			again = !is_finished();
    }else{
    	printf("fazla yazıldı ");
    }
  
}
int main(int argc, char* argv[]){
	int groupNum,i;
	printf("Please write a group number\n");
	scanf("%d",&groupNum);
	printf("Group Num :\n",groupNum);
	
	int count=groupNum*NUMBER_OF_PHILOSOPHERS;
	
	struct Philosopher* philosophers = ((struct Philosopher*) malloc(sizeof(struct Philosopher) * (count)));
	forks = (struct Fork*)malloc(sizeof(struct Fork) * count);
	
	sem_init(&global_mutex,0,1);
	
	for(i=0; i<count; i++)
	{
	       sem_init(&forks[i].mutex,0,1);
   		philosophers[i].eatenTimes = 0;
		philosophers[i].number = i + 1;

		philosophers[i].leftForkIndex = i;

		if (i+1==count){
			philosophers[i].rightForkIndex = 0;
		}
		else{
			philosophers[i].rightForkIndex = i+1;
		}
       }
	
	for(i=0;i<1;i++){
        	pthread_create(&philosophers[i].thread_id, NULL, philosopher_thread, &philosophers[i]);
        	pthread_join(philosophers[i].thread_id, NULL);
        }
         printf("price : %d",price);
return 0;
}
