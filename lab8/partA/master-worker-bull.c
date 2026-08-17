#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

pthread_mutex_t curr_buff_size_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t item_to_produce_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buff_filled_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buff_empty_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t n_items_consumed_m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buff_filled_cv = PTHREAD_COND_INITIALIZER;
pthread_cond_t buff_empty_cv = PTHREAD_COND_INITIALIZER;


int item_to_produce, curr_buf_size, n_items_consumed;
int total_items, max_buf_size, num_workers, num_masters;
int buff_filled, buff_empty;

int *buffer;

void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);



  while(1)
    {
      // check if the buffer is not filled, sleep if filled
      pthread_mutex_lock(&buff_filled_m);
      while (buff_filled) {
        pthread_cond_wait(&buff_filled_cv, &buff_filled_m);
      }
      pthread_mutex_unlock(&buff_filled_m);

      // update buffer synchronously
      pthread_mutex_lock(&curr_buff_size_m);
      pthread_mutex_lock(&item_to_produce_m);

      if(item_to_produce >= total_items) {
        pthread_mutex_unlock(&curr_buff_size_m);
        pthread_mutex_unlock(&item_to_produce_m);
	    break;
      }
 
      buffer[curr_buf_size++] = item_to_produce;
      print_produced(item_to_produce, thread_id);
      item_to_produce++;
      if (curr_buf_size == max_buf_size) buff_filled = 1;
      
      //if an empty buffer is filled, wake up all the threads waiting it to be filled
      if (curr_buf_size == 1) {
        pthread_mutex_lock(&buff_empty_m);
        buff_empty = 0;
        pthread_cond_broadcast(&buff_empty_cv);
        pthread_mutex_unlock(&buff_empty_m);
      }
      pthread_mutex_unlock(&curr_buff_size_m);
      pthread_mutex_unlock(&item_to_produce_m);
    }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item
void *consume_requests_loop(void *data) {
    int thread_id = *((int *)data);

    while (1) {

      //check if the buffer is empty, sleep if empty
      pthread_mutex_lock(&buff_empty_m);
      while (buff_empty) {
        pthread_cond_wait(&buff_empty_cv, &buff_empty_m);
      }
      pthread_mutex_unlock(&buff_empty_m);

      //update buffer synchronously
      pthread_mutex_lock(&curr_buff_size_m);
      pthread_mutex_lock(&n_items_consumed_m);

      if (n_items_consumed >= total_items) {
        pthread_mutex_lock(&curr_buff_size_m);
        pthread_mutex_lock(&n_items_consumed_m);
        break;        
      }

      print_consumed(buffer[curr_buf_size], thread_id);
      curr_buf_size--;
      n_items_consumed++;
      if (curr_buf_size == 0) buff_empty = 1;

      //if an item was consumed from filled buffer, wake up producer threads
      if (curr_buf_size = max_buf_size - 2) {
        pthread_mutex_lock(&buff_filled_m);
        buff_filled = 0;
        pthread_cond_broadcast(&buff_filled_cv);
        pthread_mutex_unlock(&buff_filled_m);
      }

      pthread_mutex_unlock(&curr_buff_size_m);
      pthread_mutex_unlock(&n_items_consumed_m);

    }
}

int main(int argc, char *argv[])
{
  int *master_thread_id;
  int *worker_thread_id;
  pthread_t *master_thread;
  pthread_t *worker_thread;
  item_to_produce = 0;
  curr_buf_size = 0;
  buff_filled = 0;
  buff_empty = 1;
  n_items_consumed = 0;
  
  int i;
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    

  buffer = (int *)malloc (sizeof(int) * max_buf_size);

  //create master producer threads
  master_thread_id = (int *)malloc(sizeof(int) * num_masters);
  master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;

  for (i = 0; i < num_masters; i++)
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
  //create worker consumer threads
  worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
  worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
  for (i = 0; i < num_workers; i++) {
    worker_thread_id[i] = i;
  }

  for (i = 0; i < num_workers; i++) {
    pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);
  }
  
  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
    {
      pthread_join(master_thread[i], NULL);
      printf("master %d joined\n", i);
    }
  
    for (i = 0;  i < num_workers; i++)
    {
      pthread_join(worker_thread[i], NULL);
      printf("worker %d joined\n", i);
    }
  
  /*----Deallocating Buffers---------------------*/
  free(buffer);
  free(master_thread_id);
  free(master_thread);
  
  return 0;
}
