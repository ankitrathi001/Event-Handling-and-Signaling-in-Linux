/******************************************************************************
 *
 * File Name: main5_2.c
 *
 * Author: Ankit Rathi (ASU ID: 1207543476)
 *
 * Date: 04-DEC-2014
 *
 * Description: If a task is pended (for instance, by waiting for a 
 * semaphore to become available) and a signal is sent to the task for 
 * which the task has a handler installed, then the handler will run 
 * before the semaphore is taken. When the handler returns, the task 
 * will go back to being pended (waiting for the semaphore). If there 
 * was a timeoutused for the pend, then the original value will be used 
 * again when the task returns from the signal handler and goes back to 
 * being pended. If the handler alters the execution path, via a call to
 * longjmp( ) for example, and does not return then the task does not go
 * back to being pended.
 *****************************************************************************/
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <linux/input.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define timeRun 2

void *thread_signal_generator(void *data);

int signalHandlerFlag = 0;
sem_t mutex;

/***********************************************************************
 * rdtsc() function is used to calulcate the number of clock ticks
 * and measure the time. TSC(time stamp counter) is incremented 
 * every cpu tick (1/CPU_HZ).
 * 
 * Source: http://stackoverflow.com/questions/8322782/rdtsc-too-many-cycles
 **********************************************************************/
static inline unsigned long long rdtsc()
{
        unsigned long long d;
        __asm__ __volatile__ ("rdtsc" : "=A" (d) );
        return d;
}

/***********************************************************************
* signal_handler - Signal Handler Function. This function is used to set
* 	the flag to 1 to indicate that the signal has been generated.
* 	That is the thread running have been interrupted by the Signal that
* 	has been generated.
* @signo: Signal Number
*
* Returns NULL
* 
* Description: Signal Handler Function. This function is used to set
* 	the flag to 1 to indicate that the signal has been generated.
* 	That is the thread running have been interrupted by the Signal that
* 	has been generated.
***************************************e********************************/
void signal_handler(int signo)
{
	printf("Time : %lld : Signal Handler called\n",rdtsc());
	signalHandlerFlag = 1;
	return;
}

/***********************************************************************
* main - Main Thread Function which creates two threads, one to get mice
* 	event inputs and other one to do the computation.
*
* Returns 0
* 
* Description:  Main Thread Function which creates two threads, one to 
* get mice event inputs and other one to do the computation.
***********************************************************************/
int main()
{
	pthread_t thread_sg;
	int iret_sg;
	int iRetValue = 0;
	int option = 0;
	struct sigaction usr_action;
	sigset_t block_mask;
	pid_t child_id;
	struct sched_param current_threadparams, new_threadparams;
    int current_policy, new_policy;
    time_t endTime;
	struct timespec sleepValue = {0}, sleepRemainder;
	/* Establish the signal handler.  */
	memset(&usr_action, 0, sizeof(usr_action));
	sigfillset(&block_mask);
	usr_action.sa_handler = signal_handler;
	usr_action.sa_mask = block_mask;
	//usr_action.sa_flags = 0;
	sigaction(SIGUSR1, &usr_action, NULL);
	
	while(1)
	{
		printf("Enter the option\n1. The thread is running.\n2. The thread is runnable (i.e. the running thread has a higher priority).\n3. The thread is blocked by a semaphore (i.e. sema_wait() is called) or a file IO (e.g., read).\n4. The thread is delayed (i.e., nanosleep() is called).\n5. Exit\n");
		scanf("%d",&option);
		
		if(option == 1)
		{
			iret_sg = pthread_create(&thread_sg, NULL, &thread_signal_generator, NULL);

			printf("\n****************************\n");
			printf("Task 2 Part 1 Starting");
			printf("\n****************************\n");
			while(1)
			{
				if(signalHandlerFlag == 1)
				{
					printf("Signal Interrupt Detected\n");
					break;
				}
			}
			printf("****************************\n");
			printf("Task 2 Part 1 Complete");
			printf("\n****************************\n\n");
			pthread_join(thread_sg, NULL);
		}
		else if(option == 2)
		{
			printf("\n****************************\n");
			printf("Task 2 Part 2 Starting");
			printf("\n****************************\n");
			iRetValue = pthread_getschedparam(pthread_self(), &current_policy, &current_threadparams);
			if(iRetValue != 0)
			{
				printf("Couldn't Retrieve Scheduling Parameters\n");
				return 0;
			}
			else
			{
				printf("Present Priority %d\n",current_threadparams.sched_priority);
			}

			/* Change the policy and priority to a maximum value */
			new_threadparams.sched_priority = sched_get_priority_max(SCHED_RR);
			
			// Set Real-Time Priority to the SCHED_RR policy
			iRetValue = pthread_setschedparam(pthread_self(), SCHED_RR, &new_threadparams);
			if(iRetValue != 0)
			{
				printf("Realtime Priority Set Failed\n");
				return;
			}
			else
			{
				printf("Realtime Priority Set Successful = %d\n",new_threadparams.sched_priority);
			}
			iret_sg = pthread_create(&thread_sg, NULL, &thread_signal_generator, NULL);
			while(1)
			{
				if(signalHandlerFlag == 1)
				{
					printf("Time : %lld : Signal Interrupt Detected\n",rdtsc());
					break;
				}
			}
			printf("****************************\n");
			printf("Task 2 Part 2 Complete");
			printf("\n****************************\n\n");
			pthread_join(thread_sg, NULL);
		}
		else if(option == 3)
		{
			/* Semaphore Mutex Initialization*/
			if(sem_init(&mutex, 0, 1) != 0)
			{
				printf("Semaphore Initialization Failed!!!\n");
			}
			sem_wait(&mutex);       /* down semaphore */
			
			iret_sg = pthread_create(&thread_sg, NULL, &thread_signal_generator, NULL);
			
			printf("\n****************************\n");
			printf("Task 2 Part 3 Starting");
			printf("\n****************************\n");
			endTime = time(NULL) + timeRun;
			while(time(NULL) < endTime)
			{
				/* down semaphore */
				iRetValue = sem_wait(&mutex);
				if(iRetValue < 0)
				{
					printf("Time : %lld : Semaphore Error\n",rdtsc());
					perror("Semaphore error : ");
				}
				/* up semaphore */
				sem_post(&mutex);
			}
			printf("****************************\n");
			printf("Task 2 Part 3 Complete");
			printf("\n****************************\n\n");
			/* destroy semaphore */
			sem_destroy(&mutex);
			pthread_join(thread_sg, NULL);
		}
		else if(option == 4)
		{
			printf("\n****************************\n");
			printf("Task 2 Part 4 Starting");
			printf("\n****************************\n");
			iret_sg = pthread_create(&thread_sg, NULL, &thread_signal_generator, NULL);

			sleepValue.tv_nsec = 0;
			sleepValue.tv_sec = 5;
			iRetValue = nanosleep(&sleepValue,&sleepRemainder);
			if(iRetValue < 0)
			{
				printf("Time : %lld : NanoSleep Error\n",rdtsc());
				perror("Nanosleep error : ");
				printf("Time Remaining is %d\n",(int)sleepRemainder.tv_sec);
			}
			printf("****************************\n");
			printf("Task 2 Part 4 Complete");
			printf("\n****************************\n\n");
			pthread_join(thread_sg, NULL);
		}
		else if(option == 5)
		{
			exit(0);
		}
		else
		{
			printf("Invalid Option\n");
		}
	}
	return 0;
}

/***********************************************************************
* thread_signal_generator - Thread Function to to generate signal 
* handlling interrupts to the main thread.
* @data: Thread Parameters
*
* Returns NULL
* 
* Description: Thread Function to to generate signal handlling
* interrupts to the main thread.
***************************************e********************************/
void *thread_signal_generator(void *data)
{
	int iRetValue = 0;
	//Generate a call to Signal Handler
	printf("Time : %lld : Signal Generated\n",rdtsc());
	//iRetValue = raise(SIGUSR1);
	iRetValue = kill(0,SIGUSR1);
	pthread_exit(0);
}
