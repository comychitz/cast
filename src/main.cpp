#include "Cast.h"
#include <unistd.h>
#include <iostream>

int main(int argc, const char *argv[])
{
  std::string cmd;
  if(argc > 1) {
    cmd = argv[1];
  }
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast cast(cwd);
  const std::string &srcdir = cwd + "/src";
  int ret = 1;
  if(cmd.empty()) {
    ret = cast.build(srcdir);
  } else if(cmd == "clean") {
    ret = cast.clean(srcdir);
  } else {
    std::cout << "Unknown command: " << cmd << std::endl;
  }
  return ret;
}
