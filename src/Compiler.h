#include "Config.h"
#include <set>

namespace Cast
{

class Compiler
{
  public:
    Compiler(const std::string &topInclude, const std::string &topLib,
             const std::set<std::string> &builtLibs);

    ~Compiler();

    bool compile(const Config &cfg, const std::string &dest,
                 const std::vector<std::string> &sources);

  private:
    std::string topInclude_, topLib_;
    const std::set<std::string> &builtLibs_;
};

}
