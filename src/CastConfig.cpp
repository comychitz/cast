#include "CastConfig.h"

namespace Cast {

  static std::vector<std::string> subdirs;

  Config::Config() {
    cfg_["target"] = "exe";
    cfg_["cflags"] = "-std=c++11";
    
  }

  void Config::read(const std::string &cfg) {
    // future todo, make more user friendly
    std::ifstream f(cfg.c_str());
    if(f.isOpen()) {
      std::string line;
      while(std::readline(f, line)) {
        size_t pos;
        if((pos = line.find(":")) != std::string::npos) {
          std::key = line.substr(0, pos);
          std::value = line.substr(pos+1);
          if(key == "subdir") {
            subdirs.push_back(value);
          } else {
            if(cfg_.find(key) == cfg_.end()) {
              cfg_[key] = value;
            }
            else {
              cfg_[key] += std::string(" ") + value;
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
    return cfg_["target"];
  }

  const std::vector<std::string> &Config::subdirs() const {
    return subdirs;
  }

  const std::string &Config::cflags() const {
    return cfg_["clflags"];
  }

  const std::string &Config::ldflags() const {
    return cfg_["ldlflags"];
  }
}
