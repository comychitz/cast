#ifndef _CASTER_H_
#define _CASTER_H_

#include "Config.h"
#include "DependencyManager.h"

namespace Cast {

  class Caster {
    public:
      Caster(const std::string &topDirPath); 

      ~Caster();

      int parseCmdLineArgs(int argc, const char *argv[]);

      int build();

      int clean();

      int check();

    private:
      int build_(const std::string &dir);

      bool buildCwd_(const Config &cfg,
                     const std::string &dir,
                     const std::string &dest);

      bool check_(const Config &cfg,
                  const std::string &name,
                  const std::string &dir);

      CompilerConfig toolchain_;
      DependencyManager depMgr_;
  };
}

#endif
