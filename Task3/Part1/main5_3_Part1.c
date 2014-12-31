/******************************************************************************
 * File Name: main5_3_Part1.c
 *
 * Author: Ankit Rathi (ASU ID: 1207543476)
 *
 * Date: 05-DEC-2014
 *
 * Description: A test program that initiates multiple threads. The 
 * number of threads can be controlled by the MACRO defined as NUMBER_OF_THREADS.
 * The program is self terminating. It runs for 5 secs and then terminate
 * irrespective of whether Signal IO events are obatined from device or not.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MICE_DEVICE "/dev/input/mice"
#define NUMBER_OF_THREADS 10
#define timeRun 10
time_t endTime;

pthread_mutex_t mutex;

void signal_handler(int signo)
{
	printf("Signal Handler called : %lu\n",(long unsigned int)pthread_self());
	return;
}

/***********************************************************************
* thread_function - Thread Function which 
* @data: Thread Parameters
*
* Returns NULL
* 
* Description: Thread Function to to generate signal handlling
* interrupts to the main thread.
***********************************************************************/
void *thread_function(void *data)
{
	struct sigaction usr_action;
	sigset_t block_mask;
	memset(&usr_action, 0, sizeof(usr_action));
	//specify a process as the owner of the file
	usr_action.sa_handler = signal_handler;
	usr_action.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &usr_action, NULL);
	
	endTime = time(NULL) + timeRun;
	while(time(NULL) < endTime)
	{
		usleep(10);
	}
	pthread_exit(0);
}	
/***********************************************************************
* main - Main function is used to get the mouse events and to create
* 	multiple threads for them to receive the generated signal.
*
* Returns 0
* 
* Description:  Main function is used to get the mouse events and to create
* 	multiple threads for them to receive the generated signal.
***********************************************************************/
int main()
{
	int fd_mice = 0, iRetValue = 0;
	int i=0;
	long flag;
	pthread_t thread[NUMBER_OF_THREADS];
	fd_mice = open(MICE_DEVICE, O_RDONLY);
	if(fd_mice < 0)
	{
		printf("Failed to Open Mice Device\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Mouse Device Opened Successfully!!!\n");
	}

	iRetValue = pthread_mutex_init(&mutex, NULL);
	if(iRetValue != 0)
	{
		printf("Mutex Initialization Failed\n");
	}
	else
	{
		printf("Mutex Initialized Successfully\n");
	}

	//specify a process as the owner of the file
	fcntl(fd_mice, F_SETOWN, getpid());
	flag = fcntl(fd_mice, F_GETFL);
	fcntl(fd_mice, F_SETFL, flag | FASYNC);

	//Create pthreads
	for(i=0; i<NUMBER_OF_THREADS; i++)
	{
		pthread_create(&thread[i], NULL, thread_function, NULL);
	}

	//Wait for each of the thread to terminate.
	for(i=0; i<NUMBER_OF_THREADS; i++)
	{
		pthread_join(thread[i], NULL);
	}

	//Destroy the mutex
	pthread_mutex_destroy(&mutex);

	return 0;
}
