#include "Caster.h"
#include <unistd.h>

int main(int argc, const char *argv[]) {

  std::string cwd = getcwd(NULL, 0);

  Cast::Caster caster(cwd);
  return caster.parseCmdLineArgs(argc, argv);
}
