/* This is a small demo of the high-performance ThreadX kernel.  It includes examples of six
   threads of different priorities, message queues, semaphores, and event flags.  */

#include   "tx_api.h"

#define     DEMO_STACK_SIZE 1024

/* Define the ThreadX object control blocks...  */

TX_THREAD               thread_0;
TX_THREAD               thread_1;
TX_THREAD               thread_2;
TX_THREAD               thread_3;
TX_THREAD               thread_4;
TX_THREAD               thread_5;
TX_QUEUE                queue_0;
TX_SEMAPHORE            semaphore_0;
TX_EVENT_FLAGS_GROUP    event_flags_0;


/* Define the counters used in the demo application...  */

UINT           thread_0_counter = 0;
UINT           thread_1_counter = 0;
UINT           thread_1_messages_sent = 0;
UINT           thread_2_counter = 0;
UINT           thread_2_messages_received = 0;
UINT           thread_3_counter = 0;
UINT           thread_4_counter = 0;
UINT           thread_5_counter = 0;


/* Define thread prototypes.  */

void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);
void    thread_2_entry(ULONG thread_input);
void    thread_3_and_4_entry(ULONG thread_input);
void    thread_5_entry(ULONG thread_input);


/* Define main entry point.  */

main()
{

    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}


/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{

CHAR    *pointer;

    /* Put first available memory address into a character pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    /* Put system definition stuff in here, e.g. thread creates and other assorted
       create information.  */

    /* Create the main thread.  */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,  
			pointer, DEMO_STACK_SIZE, 
			1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    /* Create threads 1 and 2. These threads pass information through a ThreadX 
       message queue.  It is also interesting to note that these threads have a time
       slice.  */
    tx_thread_create(&thread_1, "thread 1", thread_1_entry, 1,  
			pointer, DEMO_STACK_SIZE, 
			16, 16, 4, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    tx_thread_create(&thread_2, "thread 2", thread_2_entry, 2,  
			pointer, DEMO_STACK_SIZE, 
			16, 16, 4, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    /* Create threads 3 and 4.  These threads compete for a ThreadX counting semaphore.  
       An interesting thing here is that both threads share the same instruction area.  */
    tx_thread_create(&thread_3, "thread 3", thread_3_and_4_entry, 3,  
			pointer, DEMO_STACK_SIZE, 
			8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    tx_thread_create(&thread_4, "thread 4", thread_3_and_4_entry, 4,  
			pointer, DEMO_STACK_SIZE, 
			8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    /* Create thread 5.  This thread simply pends on an event flag which will be set
       by thread_0.  */
    tx_thread_create(&thread_5, "thread 5", thread_5_entry, 5,  
			pointer, DEMO_STACK_SIZE, 
			4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    /* Create the message queue shared by threads 1 and 2.  */
    tx_queue_create(&queue_0, "queue 0", TX_1_ULONG, pointer, 100*sizeof(ULONG));
    pointer = pointer + (100*sizeof(ULONG));

    /* Create the semaphore used by threads 3 and 4.  */
    tx_semaphore_create(&semaphore_0, "semaphore 0", 1);

    /* Create the event flags group used by threads 1 and 5.  */
    tx_event_flags_create(&event_flags_0, "event flags 0");
}



/* Define the test threads.  */

void    thread_0_entry(ULONG thread_input)
{

UINT    status;

    /* This thread simply sits in while-forever-sleep loop.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_0_counter++;
		/* Print results.  */
		
        printf("**** ThreadX Win32 Demonstration **** (c) 1996-2004 Express Logic, Inc.\n\n");

        printf("           thread 0 events sent:          %d\n", thread_0_counter);
        printf("           thread 1 messages sent:        %d\n", thread_1_counter);
        printf("           thread 2 messages received:    %d\n", thread_2_counter);
        printf("           thread 3 obtained semaphore:   %d\n", thread_3_counter);
        printf("           thread 4 obtained semaphore:   %d\n", thread_4_counter);
        printf("           thread 5 events received:      %d\n", thread_5_counter);
		

        /* Sleep for 100 ticks.  */
        tx_thread_sleep(100);

        /* Set event flag 0 to wakeup thread 5.  */
        status =  tx_event_flags_set(&event_flags_0, 0x1, TX_OR);

        /* Check status.  */
        if (status != TX_SUCCESS)
            break;
    }
}


void    thread_1_entry(ULONG thread_input)
{

UINT    status;

    /* This thread simply sends messages to a queue shared by thread 2.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_1_counter++;

        /* Send message to queue 0.  */
        status =  tx_queue_send(&queue_0, &thread_1_messages_sent, TX_WAIT_FOREVER);

        /* Check completion status.  */
        if (status != TX_SUCCESS)
            break;

        /* Increment the message sent.  */
        thread_1_messages_sent++;
    }
}


void    thread_2_entry(ULONG thread_input)
{

ULONG   received_message;
UINT    status;

    /* This thread retrieves messages placed on the queue by thread 1.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_2_counter++;

        /* Retrieve a message from the queue.  */
        status = tx_queue_receive(&queue_0, &received_message, TX_WAIT_FOREVER);

        /* Check completion status and make sure the message is what we 
           expected.  */
        if ((status != TX_SUCCESS) || (received_message != thread_2_messages_received))
            break;
        
        /* Otherwise, all is okay.  Increment the received message count.  */
        thread_2_messages_received++;
    }
}


void    thread_3_and_4_entry(ULONG thread_input)
{

UINT    status;


    /* This function is executed from thread 3 and thread 4.  As the loop
       below shows, these function compete for ownership of semaphore_0.  */
    while(1)
    {

        /* Increment the thread counter.  */
        if (thread_input == 3)
            thread_3_counter++;
        else
            thread_4_counter++;

        /* Get the semaphore with suspension.  */
        status =  tx_semaphore_get(&semaphore_0, TX_WAIT_FOREVER);

        /* Check status.  */
        if (status != TX_SUCCESS)
            break;

        /* Sleep for 2 ticks to hold the semaphore.  */
        tx_thread_sleep(2);

        /* Release the semaphore.  */
        status =  tx_semaphore_put(&semaphore_0);

        /* Check status.  */
        if (status != TX_SUCCESS)
            break;
    }
}


void    thread_5_entry(ULONG thread_input)
{

UINT    status;
ULONG   actual_flags;

    /* This thread simply waits for an event in a forever loop.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_5_counter++;

        /* Wait for event flag 0.  */
        status =  tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR, 
                                                &actual_flags, TX_WAIT_FOREVER);
        /* Check status.  */
        if ((status != TX_SUCCESS) || (actual_flags != 0x1))
            break;
    }
}

