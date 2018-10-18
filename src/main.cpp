#include "Cast.h"
#include "unistd.h"

int main(int argc, const char *argv[])
{
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast cast(cwd);
  return cast.build(cwd + "/src");
}
