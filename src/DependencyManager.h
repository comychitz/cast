#ifndef _DEPENDENCY_MANAGER_H_ 
#define _DEPENDENCY_MANAGER_H_ 

#include <map>
#include <set>
#include <string>

namespace Cast {

class DepenedencyManager {
  public:
    DependencyManager();

    ~DependencyManager();

    void clear();

    void addLib(const std::string &libName,
                const std::vector<std::string> &headers);

    void determineDepLibs(const std::string &sourceFile,
                          std::set<std::string> &libs) const;

  private:
    std::map<std::string, std::set<std::string> deps_;
};

}
#endif
