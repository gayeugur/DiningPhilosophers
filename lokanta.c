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

sem_t global_mutex;
int NotEatenCount = 0;


void philosopher_thread(int groupNum,void *argument){
    int i;
    int waitingNumber,status;
    int forkChild[groupNum]; 
    
    	struct Philosopher* philosopher = (struct Philosopher*)argument;
    	int again=1;
    	
		if((NUMBER_OF_PHILOSOPHERS*NUMBER_OF_TABLE)> NUMBER_OF_PHILOSOPHERS*groupNum){
			while(again){
    				for (i = 0; i < groupNum; i++){	
        				forkChild[i] = fork();
        				if (forkChild[i] > 0){//işlemler
      						 printf("Philosopher %d is Thinking\n", philosopher->number);	
        				}
        				else{
            					printf("Çıkış yapıyorum\n");
            					exit(0);
        				}
        
    				} 
    			}
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
	philosophers = (struct Philosopher*) malloc(sizeof(struct Philosopher) * (count));
	    for(i=0;i<count;i++)
        	pthread_create(&philosophers[i].thread_id, NULL, philosopher_thread, &philosophers[i]);
return 0;
}
