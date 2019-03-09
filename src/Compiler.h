#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "Config.h"
#include "DependencyManager.h"
#include <set>

namespace Cast {

class Compiler {
  public:
    Compiler(const CompilerConfig &toolchain,
             const std::string &topInclude, 
             const std::string &topLib);

    ~Compiler();

    bool compile(const Config &cfg, 
                 const std::string &dest,
                 const std::vector<std::string> &sources,
                 const std::set<std::string> &depLibs,
                 const std::set<std::string> &depIncDirs);

  private:
    bool compileSources_(const std::vector<std::string> &sources,
                         const std::set<std::string> &depLibs,
                         const std::set<std::string> &depIncDirs,
                         const Config &cfg, 
                         const std::string &dest);

    bool createStaticArchive_(const std::string &dest, 
                              const Config &cfg);

    std::string topInclude_, topLib_;
    const CompilerConfig &toolchain_;
};

}
#endif
