
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include <stdbool.h>

int item_to_produce, curr_buf_size;
int num_produced, num_consumed;
int total_items, max_buf_size, num_workers, num_masters;

int *buffer;

// conditional variable for item produced
pthread_cond_t item_produced = PTHREAD_COND_INITIALIZER;
// conditional variable for item consumed
pthread_cond_t item_consumed = PTHREAD_COND_INITIALIZER;

// lock for num_produced
pthread_mutex_t um_produced_lock = PTHREAD_MUTEX_INITIALIZER;
// lock for num_consumed
pthread_mutex_t um_consumed_lock = PTHREAD_MUTEX_INITIALIZER; 

// lock for current buffer size
pthread_mutex_t buffer_size_lock = PTHREAD_MUTEX_INITIALIZER;

void print_produced(int num, int master) 
{
  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) 
{
  printf("Consumed %d by worker %d\n", num, worker);
}

// produce items and place in buffer
// modify code below to synchronize correctly

void * generate_requests_loop(void * data)
{
	int thread_id = * ((int *) data);

	while (true)
	{
		// termination check
		pthread_mutex_lock(&buffer_size_lock);
		if (num_produced >= total_items)
		{
			pthread_mutex_unlock(&buffer_size_lock);
			return NULL;
		}
		// pthread_mutex_unlock(&buffer_size_lock);
			
		// if buffer is full, wait for an item to be consumed
		// pthread_mutex_lock(&buffer_size_lock);
		while (curr_buf_size == max_buf_size)
			pthread_cond_wait(&item_consumed, &buffer_size_lock);

		// pthread_mutex_unlock(&buffer_size_lock);
			
		// produce 1 unit
		// pthread_mutex_lock(&buffer_size_lock); 
		buffer[curr_buf_size] = num_produced;
		curr_buf_size++; 
		print_produced(num_produced, thread_id);
		// pthread_mutex_unlock(&buffer_size_lock);

		// print information

		// update num_produced
		// pthread_mutex_lock(&buffer_size_lock);
		num_produced++;
		pthread_cond_signal(&item_produced);
		pthread_mutex_unlock(&buffer_size_lock);
		
		// signal that item has been produced
	}
}

// write function to be run by worker threads
// ensure that the workers call the function print_consumed when they consume an item
void * consume_requests_loop(void * data)
{
	int thread_id = * ((int *) data);

	while (true)
	{
		// termination check
		pthread_mutex_lock(&buffer_size_lock);
		if (num_consumed >= total_items)
		{
			pthread_mutex_unlock(&buffer_size_lock);
			return NULL;
		}
		// pthread_mutex_unlock(&buffer_size_lock);

		// if buffer is empty, wait for an item to be produced
		// pthread_mutex_lock(&buffer_size_lock);
		while (curr_buf_size == 0)
			pthread_cond_wait(&item_produced, &buffer_size_lock);
		// pthread_mutex_unlock(&buffer_size_lock);

		// consume 1 unit
		// pthread_mutex_lock(&buffer_size_lock);
		int consumed_data = buffer[curr_buf_size - 1];
		curr_buf_size--;
		// pthread_mutex_unlock(&buffer_size_lock);

		// print information
		
		// update num_consumed
		print_consumed(consumed_data, thread_id);
		// pthread_mutex_lock(&buffer_size_lock);
		num_consumed++;
		pthread_cond_signal(&item_consumed);
		pthread_mutex_unlock(&buffer_size_lock);
		
		// signal that item has been consumed
	}
}

int main(int argc, char *argv[])
{
	int * master_thread_id;
	pthread_t * master_thread;
	int * worker_thread_id;
	pthread_t * worker_thread;

	item_to_produce = 0;
	curr_buf_size = 0;
	num_produced = 0;
	num_consumed = 0;
	
	int i;
	
	if (argc < 5) 
	{
		printf("./master-worker #total_items #max_buf_size #num_workers #masters\n");
		exit(1);
	}
	else 
	{
		num_masters = atoi(argv[4]);
		num_workers = atoi(argv[3]);
		total_items = atoi(argv[1]);
		max_buf_size = atoi(argv[2]);
	}
		

	buffer = (int *) malloc(sizeof(int) * max_buf_size);

	// create master producer threads
	master_thread_id = (int *) malloc(sizeof(int) * num_masters);
	master_thread = (pthread_t *) malloc(sizeof(pthread_t) * num_masters);
	for (i = 0; i < num_masters; i++)
		master_thread_id[i] = i;

	for (i = 0; i < num_masters; i++)
		pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
	
	// create worker consumer threads
	worker_thread_id = (int *) malloc(sizeof(int) * num_workers);
	worker_thread = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
	for (i = 0; i < num_workers; i++)
		worker_thread_id[i] = i;

	for (i = 0; i < num_workers; i++)
		pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);
	
	// wait for all threads to complete
	for (i = 0; i < num_masters; i++)
	{
		pthread_join(master_thread[i], NULL);
		printf("master %d joined\n", i);
	}
	for (i = 0; i < num_workers; i++)
	{
		pthread_join(worker_thread[i], NULL);
		printf("worker %d joined\n", i);
	}
	
	/*----Deallocating Buffers---------------------*/
	free(buffer);
	free(master_thread_id);
	free(master_thread);

	free(worker_thread_id);
	free(worker_thread);
	
	return 0;
}
