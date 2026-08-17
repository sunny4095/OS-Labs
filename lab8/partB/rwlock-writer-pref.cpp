#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->lock = PTHREAD_MUTEX_INITIALIZER;
  rw->reader_lock = PTHREAD_MUTEX_INITIALIZER;
  rw->num_readers = 0;
  rw->num_writers = 0;
  rw->r_cv = PTHREAD_COND_INITIALIZER;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->reader_lock);
  while(rw->num_writers > 0) {
    pthread_cond_wait(&rw->r_cv, &rw->reader_lock);
  }
  if (rw->num_readers == 0) {
    pthread_mutex_lock(&rw->lock);
  }
  rw->num_readers++;
  pthread_mutex_unlock(&rw->reader_lock);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->reader_lock);
  rw->num_readers--;
  if (rw->num_readers == 0) {
    pthread_mutex_unlock(&rw->lock);
  }
  pthread_mutex_unlock(&rw->reader_lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->reader_lock);
  rw->num_writers++;
  pthread_mutex_unlock(&rw->reader_lock);
  pthread_mutex_lock(&rw->lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->reader_lock);
  rw->num_writers--;
  if (rw->num_writers == 0) {
    pthread_cond_broadcast(&rw->r_cv);
  }
  pthread_mutex_unlock(&rw->reader_lock);
  pthread_mutex_unlock(&rw->lock);
}
