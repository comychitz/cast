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
  // TODO - need to determine algorithm(!)
  //

}

}
