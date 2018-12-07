#include "DependencyManager.h"
#include "Util.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <errno.h>
#include <string.h>

namespace Cast {

DependencyManager::DependencyManager() { 
  const std::string defaultDepCfgDir = "/usr/local/share/cast/deps";
  readCfgDir(defaultDepCfgDir);
}

DependencyManager::~DependencyManager() {
}

void DependencyManager::clear() {
  deps_.clear();
}

void DependencyManager::addLib(const std::string &libName,
                               const std::vector<std::string> &headers) {
  for(auto &header : headers) {
    DepConfig dep(Util::basename(libName));
    dep.libs.insert(libName);
    dep.headers.insert(headers.begin(), headers.end());
    deps_.emplace(libName, dep);
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
    for(auto &dep : deps_) {
      if(dep.second.headers.find(header) != dep.second.headers.end()) {
        libs.insert(dep.second.libs.begin(), dep.second.libs.end());
        for(auto &depDep : dep.second.deps) {
          auto depLibs = deps_.find(depDep);
          if(depLibs != deps_.end()) {
            libs.insert(depLibs->second.libs.begin(),
                        depLibs->second.libs.end());
          }
        }
        break;
      }
    }
    if(Util::exists(header)) {
      determineDepLibs(header, libs); 
    }
  }
}

void DependencyManager::readCfgDir(const std::string &dir) {
  std::vector<std::string> filter = {".cfg"};
  std::vector<std::string> depCfgs = Util::getFiles(dir, filter);
  for(auto &cfg : depCfgs) {
    DepConfig dep(cfg.substr(0, cfg.find(".")));
    dep.read(dir+"/"+cfg);
    deps_.emplace(dep.name, dep);
  }
}

}
