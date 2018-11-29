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

  private:
    /// map of headers to libraries
    std::map<std::string, std::string> deps_;
};

}
#endif
