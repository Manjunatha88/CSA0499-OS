#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 5
#define NUM_RESOURCES 1

sem_t read_count_mutex;
sem_t resource_mutex;
int read_count;

void *reader(void *arg) {
  int id = *((int *)arg);

  while (1) {
    sem_wait(&read_count_mutex);
    read_count++;
    if (read_count == 1) {
      sem_wait(&resource_mutex);
    }
    sem_post(&read_count_mutex);

    printf("Reader %d is reading\n", id);
    sleep(1);

    sem_wait(&read_count_mutex);
    read_count--;
    if (read_count == 0) {
      sem_post(&resource_mutex);
    }
    sem_post(&read_count_mutex);
  }

  return NULL;
}

void *writer(void *arg) {
  int id = *((int *)arg);

  while (1) {
    sem_wait(&resource_mutex);

    printf("Writer %d is writing\n", id);
    sleep(1);

    sem_post(&resource_mutex);
  }

  return NULL;
}

int main(void) {
  pthread_t readers[NUM_READERS];
  pthread_t writers[NUM_WRITERS];
  int reader_ids[NUM_READERS];
  int writer_ids[NUM_WRITERS];
  int i;

  sem_init(&read_count_mutex, 0, 1);
  sem_init(&resource_mutex, 0, NUM_RESOURCES);
  read_count = 0;

  for (i = 0; i < NUM_READERS; i++) {
    reader_ids[i] = i;
    pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
  }
  for (i = 0; i < NUM_WRITERS; i++) {
    writer_ids[i] = i;
    pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
  }

  for (i = 0; i < NUM_READERS; i++) {
    pthread_join(readers[i], NULL);
  }
  for (i = 0; i < NUM_WRITERS; i++) {
    pthread_join(writers[i], NULL);
  }

  sem_destroy(&read_count_mutex);
  sem_destroy(&resource_mutex);

  return 0;
}
