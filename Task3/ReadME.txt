Task 1
========================================================================
Steps to Execute
-----------------------------
1. Navigate to the folder where files have been extracted.
2. Task 1 files are present in Task1 folder.
3. Run the command in terminal "gcc main5_1.c -o main5_1.o -lpthread"

Files Present
-----------------------------
1. main5_1.c

Expalination
-----------------------------
This part of the assignment makes use of two threads. First thread is
used to generate the signal by detecting double right click from the 
mouse and second thread is used to do imprecise computation. The results
of the computation is displayed as soon as duble click event is generated.
For double click event, the thread waits for 500ms for second click
otherwise it is detected as a first click. The MOUSE DEVICE path can
be changed to the #define MICE_DEVICE "/dev/input/event5" to correct value
as per the settings of the tester machine.






Task 2
========================================================================
Steps to Execute
-----------------------------
1. Navigate to the folder where files have been extracted.
2. Task 2 files are present in Task2 folder.
3. Run the command in terminal "gcc main5_2.c -o main5_2.o -lpthread"

Files Present
-----------------------------
1. main5_2.c








Task 3 Part 1
========================================================================
Steps to Execute
-----------------------------
1. Navigate to the folder where files have been extracted.
2. Task 3 Part 1 files are present in ../Task3/Part1/ folder.
3. Run the command in terminal "cc main5_3_Part1.c -o main5_3_Part1.o -lpthread".

Files Present
-----------------------------
1. main5_3_Part1.c

Expalination
-----------------------------
This part of the assignment makes use of multiple threads. These threads
have a general common thread function to invoke. These threads are set to listen to 
SIGIO from input device "/dev/input/mice". The signal generated is 
delivered arbitrarily to any one the generated thread.
The MOUSE DEVICE path can be changed to the #define MICE_DEVICE "/dev/input/mice"
to correct value as per the settings of the tester machine.






Task 3 Part 2
========================================================================
Steps to Execute
-----------------------------
1. Navigate to the folder where files have been extracted.
2. Task 3 Part 2 files are present in ../Task3/Part2/ folder.
3. Run the command in terminal "cc main5_3_Part2.c -o main5_3_Part2.o -lpthread"

Files Present
-----------------------------
1. main5_3_Part2.c
2. main5_3_Part2_LIB.h

Expalination
-----------------------------
main5_3_Part2_LIB.h Library file is created that has a set of functions 
such as register_my_thread, signal_handler, thread_capture_function that
allows SIGIO signal being delivered to all threads that are registered 
to receive the signal. The tester program creates the threads and calls
the functions register_my_thread() to register and receive signals. If
the thread is not registered, then it will not receive the signal.
