#ifndef _DEPENDENCY_MANAGER_H_ 
#define _DEPENDENCY_MANAGER_H_ 

#include "Config.h"

namespace Cast {

class DependencyManager {
  public:
    DependencyManager();

    ~DependencyManager();

    void clear();

    void addLib(const std::string &libName,
                const std::vector<std::string> &headers);

    void determineDepLibs(const std::string &sourceFile,
                          std::set<std::string> &libs) const;

    void readCfgDir(const std::string &dir);

  private:
    void readCfgFile(const std::string &file,
                     const std::string &depName);

    /// map of dep name to dep config
    std::map<std::string, DepConfig> deps_;
};

}
#endif
