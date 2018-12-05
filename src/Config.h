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

      virtual void processKeyValue(const std::string &key,
                                   const std::string &value) = 0;
  };

  class Config : public ConfigInterface {
    public:
      Config(const std::string &dir);

      virtual ~Config();

      void processKeyValue(const std::string &key, const std::string &value);

      const std::string &target() const;

      const std::vector<std::string> &subdirs() const;

      void cflags(const std::string &val);
      
      const std::string &cflags() const;

      const std::string &ldflags() const;
    
      const std::string &name() const;

      std::string getTargetName() const;

    private:
      std::vector<std::string> subdirs_;
      std::map<std::string, std::string> cfg_;
  };

  class DepConfig : public ConfigInterface {
    public:
      DepConfig();

      virtual ~DepConfig();

      void processKeyValue(const std::string &key, const std::string &value);

      const std::map<std::string, std::string> &getConfig() const;

    private:
      std::vector<std::string> deps_;
      std::map<std::string, std::string> cfg_;
  };

}
#endif
