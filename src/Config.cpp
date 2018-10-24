#include "Config.h"
#include <iostream>
#include <fstream>

namespace Cast {

  Config::Config(const std::string &dir) {
    cfg_["name"] = dir;
    cfg_["target"] = "exe";
    cfg_["cflags"] = "";
    cfg_["ldflags"] = "";
  }

  Config::~Config() { }

  void Config::read(const std::string &cfg) {
    // future todo, make more user friendly
    std::ifstream f(cfg.c_str());
    if(f.is_open()) {
      std::string line;
      while(std::getline(f, line)) {
        size_t pos;
        if((pos = line.find(":")) != std::string::npos) {
          std::string key = line.substr(0, pos);
          std::string value = line.substr(pos+1);
          if(key == "subdir") {
            subdirs_.push_back(value);
          } else {
            if(key == "ldflag" || key == "cflag") {
              key += "s";
            }
            if(key != "target" && key != "name") {
              cfg_[key] += value + " ";
            } else {
              cfg_[key] = value;
            }
          }
        }
      }
      f.close();
    }
    else {
      std::cout << "ERR> Failed to open file: " << cfg << std::endl; 
    }
  }

  const std::string &Config::target() const {
    return cfg_.at("target");
  }

  const std::vector<std::string> &Config::subdirs() const {
    return subdirs_;
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
}
