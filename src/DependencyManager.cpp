#include "DependencyManager.h"
#include "Util.h"

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
  for (auto &header : headers) {
    deps_[header] = libName;
  }
}

static void parseIncludedHeaders(const std::string &source,
                                 std::set<std::string> &headers) {
  //
  // TODO
  //
}

void DependencyManager::determineDepLibs(const std::string &sourceFile,
                                         std::set<std::string> &libs) const {

  // given a source file, we need to find the headers it depends on, then use 
  // that to find which libraries it depends on. since we expect the project
  // to be built in the proper order, we only need to parse header files that
  // exist within the same directory as the sources
  std::set<std::string> headers;
  parseIncludedHeaders(sourceFile, headers);
  
  //
  // TODO need to determine if we should parse full include path or just
  // basename
  //

  for (auto &header : headers) {
    std::map<std::string,std::string>::const_iterator dep;
    if ((dep = deps_.find(header)) != deps_.end()) {
      libs.insert(dep->second);
    } else if (Util::exists(header)) {
      determineDepLibs(header, libs); 
    }
  }
}

}
