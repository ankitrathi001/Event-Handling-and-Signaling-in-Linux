/******************************************************************************
 * File Name: main5_3_Part2.c
 *
 * Author: Ankit Rathi (ASU ID: 1207543476)
 *
 * Date: 05-DEC-2014
 *
 * Description: A tester program that initiates multiple threads. Each
 * 	of these threads has its own function defined and its own signal
 * 	handler function.
 *****************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include "main5_3_Part2_LIB.h"


void *thread_0_function(void *data);
void *thread_1_function(void *data);
void *thread_2_function(void *data);
void *thread_3_function(void *data);
void *thread_4_function(void *data);
void signal_0_handler(int signo);
void signal_1_handler(int signo);
void signal_2_handler(int signo);
void signal_3_handler(int signo);
void signal_4_handler(int signo);

/***********************************************************************
* main - This is main function that initiates multiple threads.
*
* Returns 0
* 
* Description: This is main function that initiates multiple threads.
***********************************************************************/
int main()
{
	int iRetValue = 0;
	pthread_t thread_signal[5], thread_capture;

	//Create 6 pthreads
	pthread_create(&thread_signal[0], NULL, thread_0_function, NULL);
	pthread_create(&thread_signal[1], NULL, thread_1_function, NULL);
	pthread_create(&thread_signal[2], NULL, thread_2_function, NULL);
	pthread_create(&thread_signal[3], NULL, thread_3_function, NULL);
	pthread_create(&thread_signal[4], NULL, thread_4_function, NULL);
	pthread_create(&thread_capture, NULL, thread_capture_function, NULL);

	//Wait for each of the 6 thread to terminate.
	pthread_join(thread_signal[0], NULL);
	pthread_join(thread_signal[1], NULL);
	pthread_join(thread_signal[2], NULL);
	pthread_join(thread_signal[3], NULL);
	pthread_join(thread_signal[4], NULL);
	pthread_join(thread_capture, NULL);

	return 0;
}

/* Thread 0 Function */
void *thread_0_function(void *data)
{
	//register_my_thread(pthread_self(), signal_0_handler);
}

/* Thread 1 Function */
void *thread_1_function(void *data)
{
	register_my_thread(pthread_self(), signal_1_handler);
}

/* Thread 2 Function */
void *thread_2_function(void *data)
{
	register_my_thread(pthread_self(), signal_2_handler);
}

/* Thread 3 Function */
void *thread_3_function(void *data)
{
	register_my_thread(pthread_self(), signal_3_handler);
}

/* Thread 4 Function */
void *thread_4_function(void *data)
{
	register_my_thread(pthread_self(), signal_4_handler);
}

/* Thread 0 Signal handler */
void signal_0_handler(int signo)
{
	printf("Signal 0 Handler called\n");
}

/* Thread 1 Signal handler */
void signal_1_handler(int signo)
{
	printf("Signal 1 Handler called\n");
}

/* Thread 2 Signal handler */
void signal_2_handler(int signo)
{
	printf("Signal 2 Handler called\n");
}

/* Thread 3 Signal handler */
void signal_3_handler(int signo)
{
	printf("Signal 3 Handler called\n");
}

/* Thread 4 Signal handler */
void signal_4_handler(int signo)
{
	printf("Signal 4 Handler called\n");
}
