#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <map>
#include <vector>
#include <string>

namespace Cast {

  class ConfigInterface {
    public:
      virtual ~ConfigInterface();

      virtual void read(const std::string &cfg);

    protected:
      std::map<std::string, std::string> cfg_;
      std::vector<std::string> subdirs_;
  };

  class Config : public ConfigInterface {
    public:
      Config(const std::string &dir);

      virtual ~Config();

      const std::string &target() const;

      const std::vector<std::string> &subdirs() const;

      void cflags(const std::string &val);
      
      const std::string &cflags() const;

      const std::string &ldflags() const;
    
      const std::string &name() const;

      std::string getTargetName() const;
  };

  class DepConfig : public ConfigInterface {
    public:
      DepConfig();

      virtual ~DepConfig();

      const std::map<std::string, std::string> &getConfig() const;
  };

}
#endif
