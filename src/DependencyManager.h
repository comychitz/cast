#ifndef _DEPENDENCY_MANAGER_H_ 
#define _DEPENDENCY_MANAGER_H_ 

#include <map>
#include <set>
#include <string>
#include <vector>

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

    /// map of headers to libraries
    std::map<std::string, std::string> deps_;

    /// map of libraries to dependent libraries
    std::map<std::string, std::set<std::string> > depDeps_;
};

}
#endif
