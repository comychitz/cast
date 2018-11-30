#include <Incrementer.h>
#include <iostream>
#include <pthread.h>

int main(int argc, const char *argv[])
{
  Incrementer inc;
  inc.increment();
  std::cout << inc.getCount() << std::endl;

  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL);

  return 0;
}

