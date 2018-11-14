#ifndef _CASTER_H_
#define _CASTER_H_

#include "Config.h"
#include "DependencyManager.h"

namespace Cast {

  class Caster {
    public:
      explicit Caster(const std::string &topDirPath); 

      ~Caster();

      int build();

      int clean();

      int check();

    private:
      DependencyManager depMgr_;
  };
}

#endif
