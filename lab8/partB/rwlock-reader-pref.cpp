#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->num_readers = 0;
  rw->lock = PTHREAD_MUTEX_INITIALIZER;
  rw->lock = PTHREAD_MUTEX_INITIALIZER;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->reader_lock);
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
  pthread_mutex_lock(&rw->lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_unlock(&rw->lock);
}
