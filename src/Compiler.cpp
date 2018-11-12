#include "Compiler.h"
#include "Util.h"
#include <iostream>
#include <sys/stat.h>
#include <sstream>

namespace Cast {

static time_t getFileModTime(const std::string &path) {
  struct stat statbuf;
  if(stat(path.c_str(), &statbuf) != 0) {
    return 0;
  }
  return statbuf.st_mtimespec.tv_sec;
}

static time_t getNewestSourceModTime(const std::vector<std::string> &sources) {
  time_t newest = 0;
  std::vector<std::string>::const_iterator source;
  for(source = sources.begin(); source != sources.end(); ++source) {
    time_t modTime = getFileModTime(*source);
    if(modTime == 0) {
      return 0;
    } else if(modTime > newest) {
      newest = modTime;
    }
  }
  return newest;
}

static bool targetUpToDate(const std::string &dest, const Config &cfg,
                           const std::vector<std::string> &sources) {
  time_t sourceModTime = getNewestSourceModTime(sources);
  time_t targetModTime = getFileModTime(dest+cfg.getTargetName());
  return sourceModTime < targetModTime;
}

static bool createStaticArchive(const std::string &dest, const Config &cfg) {
  std::stringstream cmd;
  cmd << "ar -q " << dest << cfg.getTargetName() << " *.o; rm -f *.o";
  return Util::run(cmd.str());
}

Compiler::Compiler(const std::string &topInclude,
                   const std::string &topLib,
                   const DependencyManager &depMgr)
  : topInclude_(topInclude), topLib_(topLib), depMgr_(depMgr) {
}

Compiler::~Compiler() {
}

bool Compiler::compile(const Config &cfg, const std::string &dest,
                       const std::vector<std::string> &sources) {
  if(targetUpToDate(dest, cfg, sources)) {
    std::cout << "cast: Target (" << cfg.getTargetName() << ") up to date" << std::endl;
    return true;
  }
  Util::mkdirp(dest);
  if(!compileSources_(sources, cfg, dest, depMgr_, topInclude_, topLib_)) {
    return false;
  }
  if(cfg.target() == "a") {
    return createStaticArchive(dest, cfg);
  }
  return true;
}

bool Compiler::compileSources_(const std::vector<std::string> &sources,
                               const Config &cfg, 
                               const std::string &dest) {
  std::stringstream cmd;
  cmd << "g++ " << "-I. -I" << topInclude_ << " " << cfg.cflags()
      << " -L" << topLib_ << " " << cfg.ldflags(); 
  std::set<std::string> depLibs;
  for(auto &source : sources) {
    cmd << " " << source;
    depMgr_.determineDepLibs(source, depLibs);
  }
  if(cfg.target() == "a") {
    cmd << " -c";
  } else {
    if(cfg.target() == "so") {
      cmd << " -shared";
    }        
    cmd << " -o " << dest << cfg.getTargetName();
  } 
  for(auto &lib : depLibs) {
    cmd << " " << lib;
  }
  return Util::run(cmd.str());
}

}
