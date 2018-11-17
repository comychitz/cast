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
  headers_.insert(headers.begin(), headers.end());
}

void DependencyManager::determineDepLibs(const std::string &sourceFile,
                                         std::set<std::string> &libs) const {

  // given a source file, we need to find the headers it depends on. 
  // for each header, if the header file is in our dependency map, then
  // recursively call this function on that header.

  //
  // TODO
  //
}

}
