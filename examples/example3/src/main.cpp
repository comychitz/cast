#include <iostream>
#include <zlib.h>

int main(int argc, const char *argv[])
{
  std::cout << zlibVersion() << std::endl;
  return 0;
}
