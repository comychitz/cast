#include "Caster.h"
#include <unistd.h>
#include <iostream>

int main(int argc, const char *argv[])
{
  std::string cmd;
  if(argc > 1) {
    cmd = argv[1];
  }
  std::string cwd = getcwd(NULL, 0);
  Cast::Caster caster(cwd);
  int ret = 1;
  if(cmd.empty()) {
    ret = caster.build();
  } else if(cmd == "clean") {
    ret = caster.clean();
  } else if(cmd == "check") {
    ret = caster.check();
  } else {
    std::cout << "Unknown command: " << cmd << std::endl;
  }
  return ret;
}
