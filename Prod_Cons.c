#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define BUFFER 3
int buffer[BUFFER];
int add_data=0;
int rem_data=0;
int num=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prod_var=PTHREAD_COND_INITIALIZER;
pthread_cond_t cons_var=PTHREAD_COND_INITIALIZER;
void *producer(void *args)
{
	for(int i=0;i<10;i++)
	{
		pthread_mutex_lock(&mutex);
		if(num>BUFFER)
		{
			printf("Overflow");
			exit(1);
		}
		while(num==BUFFER)
		{
			pthread_cond_wait(&prod_var,&mutex);
		}
		buffer[add_data]=i;
		add_data=(add_data+1)%BUFFER;
		num++;
		printf("Successful");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cons_var);
	}
	return 0;
}
void *consumer(void *args)
{
	int i;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(num<0)
		{
			printf("Underflow");
			exit(1);
		}
		while(num==BUFFER)
		{
			pthread_cond_wait(&cons_var,&mutex);
		}
		i=buffer[rem_data];
		rem_data=(rem_data+1)%BUFFER;
		num--;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&prod_var);
	}
	return 0;
}
int main()
{
	pthread_t tid_prod,tid_cons;
	int error;
	error=pthread_create(&tid_prod,NULL,producer,NULL);
	if(error!=0)
	{
		printf("Failed");
		exit(1);
	}
	error=pthread_create(&tid_cons,NULL,consumer,NULL);
	if(error!=0)
	{
		printf("Failed");
		exit(1);
	}
	pthread_join(tid_prod,NULL);
	pthread_join(tid_cons,NULL);
	return 0;
}
