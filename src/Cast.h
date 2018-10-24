#ifndef _CAST_H_
#define _CAST_H_

#include "Config.h"

namespace Cast {

  class Cast {
    public:
      explicit Cast(const std::string &cwd); 

      ~Cast();

      int build(const std::string &dir);   

      inline
      std::string topInclude() const {
        return top_ + "/build/include";
      }

      inline
      std::string topLib() const {
        return top_ + "/build/lib";
      }

      inline
      std::string topBin() const {
        return top_ + "/build/bin";
      }

    private:
      bool buildCwd(const Config &cfg, 
                    const std::string &dir,
                    const std::string &dest);

      bool linkFiles(const Config &cfg, 
                     const std::string &dir,
                     const std::string &dest);

    private:
      std::string top_;
  };
}

#endif
