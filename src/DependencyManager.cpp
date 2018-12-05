#include "DependencyManager.h"
#include "Util.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <errno.h>
#include <string.h>

namespace Cast {

static void readDefaultCfg(std::map<std::string, std::string> &deps) {
  std::string defaultDepCfg = "/usr/local/share/cast/dep.cfg";
  DepConfig cfg;
  cfg.read(defaultDepCfg);
  const std::map<std::string, std::string> &cfgItems = cfg.getConfig();
  for(auto &cfgItem : cfgItems) {
    deps[cfgItem.first] = cfgItem.second;
  }
  //
  // TODO parse all dep config files in the build/deps dir
  //
}

DependencyManager::DependencyManager() { 
  readDefaultCfg(deps_);
}

DependencyManager::~DependencyManager() {
}

void DependencyManager::clear() {
  deps_.clear();
  readDefaultCfg(deps_);
}

void DependencyManager::addLib(const std::string &libName,
                               const std::vector<std::string> &headers) {
  for(auto &header : headers) {
    deps_[header] = libName;
  }
}

static bool parseHeaderFileFromLine(const std::string &line,
                                    std::string &header) {
  size_t start = std::string::npos, end = std::string::npos;
  size_t pos = line.find("#include") + std::string("#include").length();
  if((start = line.find("\"", pos+1)) != std::string::npos ||
      (start = line.find("<", pos+1)) != std::string::npos) {
    if((end = line.find("\"", start+1)) != std::string::npos ||
       (end = line.find(">", start+1)) != std::string::npos) {
      header = line.substr(start+1, end-(start+1));
      return true;
    }
  }
  return false;
}

static void parseIncludedHeaders(const std::string &source,
                                 std::set<std::string> &headers) {
  std::ifstream f(source);
  if(!f.good()) {
    std::cout << "Error opening file: " << source << ": " << strerror(errno) 
              << "(" << errno << ")" << std::endl;
    return;
  }
  std::string line;
  while(std::getline(f, line)) {
    std::string header;
    if(line.find("#include") != std::string::npos &&
       parseHeaderFileFromLine(line, header)) {
      headers.insert(header);
    }
  }
  f.close();
}

void DependencyManager::determineDepLibs(const std::string &sourceFile,
                                         std::set<std::string> &libs) const {
  // given a source file, we need to find the headers it depends on, then use 
  // that to find which libraries it depends on. since we expect the project
  // to be built in the proper order, we only need to parse header files that
  // exist within the same directory as the sources.
  std::set<std::string> headers;
  parseIncludedHeaders(sourceFile, headers);
  
  for(auto &header : headers) {
    std::map<std::string,std::string>::const_iterator dep;
    if((dep = deps_.find(header)) != deps_.end()) {
      libs.insert(dep->second);
      std::map<std::string,std::set<std::string> >::const_iterator depDep;
      if((depDep = depDeps_.find(dep->second)) != depDeps_.end()) {
        libs.insert(depDep->second.begin(), depDep->second.end());
      }
    } else if(Util::exists(header)) {
      determineDepLibs(header, libs); 
    }
  }
}

}
