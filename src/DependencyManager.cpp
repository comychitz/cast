#include "DependencyManager.h"

namespace Cast {

DependencyManager::DependencyManager() {
}

DependencyManager::~DependencyManager() {
}

void DependencyManager::clear() {
  deps_.clear();
}

void DependencyManager::addLib(const std::string &libName,
                               const std::vector<std::string> &headers) {
  deps_[libName].insert(headers.begin(), headers.end()); 
}

void DependencyManager::determineDepLibs(const std::string &sourceFile,
                                         std::set<std::string> &libs) const {
  //
  // TODO - need to update, for now just insert all libraries blindly
  //
  std::map<std::string, std::set<std::string> >::const_iterator dep;
  for(dep = deps_.begin(); dep != deps_.end(); ++dep) {
    libs.insert(dep->first);
  }
}

}
