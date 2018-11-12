#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "Config.h"
#include "DependencyManager.h"
#include <set>

namespace Cast {

class Compiler {
  public:
    Compiler(const std::string &topInclude, 
             const std::string &topLib,
             const DependencyManager &depMgr);

    ~Compiler();

    bool compile(const Config &cfg, 
                 const std::string &dest,
                 const std::vector<std::string> &sources);

  private:
    bool compileSources_(const std::vector<std::string> &sources,
                         const Config &cfg, 
                         const std::string &dest);

    std::string topInclude_, topLib_;
    const DependencyManager &depMgr_;
};

}
#endif
