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
  deps_[libName].insert(headers.begin(), headers.end()); 
  headers_.insert(headers.begin(), headers.end());
}

static void parseIncludedHeaders(const std::string &source,
                                 std::set<std::string> &headers) {
  //
  // TODO
  //
}

void DependencyManager::determineDepLibs(const std::string &sourceFile,
                                         std::set<std::string> &libs) const {

  // given a source file, we need to find the headers it depends on. 
  // for each header, if the header file is in our dependency map, then
  // recursively call this function on that header.
  std::set<std::string> headers;
  parseIncludedHeaders(sourceFile, headers);

  for (auto &header : headers) {
    if (headers_.find(header) != headers_.end()) {

      // TODO
      // need to add libraries this header depends on in set to return

      // need to find location of header file

      determineDepLibs(header, libs);
    }
  }
}

}
