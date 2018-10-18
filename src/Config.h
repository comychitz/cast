#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <map>
#include <vector>
#include <string>

namespace Cast {

  class Config {
    public:
      Config();

      ~Config();

      void read(const std::string &cfg);

      const std::string &target() const;

      const std::vector<std::string> &subdirs() const;

      const std::string &cflags() const;

      const std::string &ldflags() const;

    private:
      std::map<std::string, std::string> cfg_;
      std::vector<std::string> subdirs_;
  };

}
#endif
