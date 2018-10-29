#ifndef _CAST_H_
#define _CAST_H_

#include "Config.h"

namespace Cast {

  class Cast {
    public:
      explicit Cast(const std::string &topDirPath); 

      ~Cast();

      int build();

      int clean();

      int check();

      const std::string &getTopDir() const;

    private:
      std::string top_;
  };
}

#endif
