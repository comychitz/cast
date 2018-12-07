#include "Config.h"
#include <iostream>
#include <fstream>

namespace Cast {

  ConfigInterface::~ConfigInterface() {
  }

  void ConfigInterface::read(const std::string &cfg) {
    std::ifstream f(cfg.c_str());
    if(!f.is_open()) {
      std::cout << "ERR> Failed to open file: " << cfg << std::endl; 
      return;
    }
    std::string line;
    while(std::getline(f, line)) {
      size_t pos;
      // future todo, make more user friendly - strip whitespace, etc.
      if((pos = line.find(":")) != std::string::npos) {
        const std::string &key = line.substr(0, pos);
        const std::string &value = line.substr(pos+1);
        processKeyValue(key, value); 
      } else {
        std::cout << "WARN> Invalid line format: " << line << std::endl;
      }
    }
    f.close();
  }

  Config::Config(const std::string &dir) {
    cfg_["name"] = dir;
    cfg_["target"] = "exe";
    cfg_["cflags"] = "";
    cfg_["ldflags"] = "";
  }

  Config::~Config() { 
  } 

  void Config::processKeyValue(const std::string &key,
                               const std::string &value) {
    if(key == "subdir") {
      subdirs_.push_back(value);
    } else {
      std::string k = key;
      if(key == "ldflag" || key == "cflag") {
        k += "s";
      }
      if(key != "target" && key != "name") {
        cfg_[k] += value + " ";
      } else {
        cfg_[k] = value;
      }
    }
  }

  const std::string &Config::target() const {
    return cfg_.at("target");
  }

  const std::vector<std::string> &Config::subdirs() const {
    return subdirs_;
  }

  void Config::cflags(const std::string &val) {
    cfg_["cflags"] = val;
  }

  const std::string &Config::cflags() const {
    return cfg_.at("cflags");
  }

  const std::string &Config::ldflags() const {
    return cfg_.at("ldflags");
  }
  
  const std::string &Config::name() const {
    return cfg_.at("name");
  }

  std::string Config::getTargetName() const {
    std::string targetName = name();
    if(target() == "so") {
      targetName += ".so";
    } else if(target() == "a") {
      targetName += ".a";
    }
    return targetName;
  }

  DepConfig::DepConfig(const std::string &n) : name(n) {
  }

  DepConfig::~DepConfig() {
  }

  void DepConfig::processKeyValue(const std::string &key,
                                  const std::string &value) {
    if(key == "dep") {
      deps.insert(value);
    } else if(key == "header") {
      headers.insert(value);
    } else if(key == "lib") {
      libs.insert(value);
    }
  }

}
