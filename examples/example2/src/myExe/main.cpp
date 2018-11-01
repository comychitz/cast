#include <Incrementer.h>
#include <iostream>

int main(int argc, const char *argv[])
{
  Incrementer inc;
  inc.increment();
  std::cout << inc.getCount() << std::endl;
  return 0;
}

