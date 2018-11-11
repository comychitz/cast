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

static void buildCompileCmds(const std::vector<std::string> &sources,
                             const Config &cfg, const std::string &dest,
                             const std::set<std::string> &builtLibs,
                             const std::string &topInclude,
                             const std::string &topLib,
                             std::string &cmd, std::string &archiveCmd)
{
  std::stringstream cmdss, archiveCmdss;
  cmdss << "g++ " << "-I. -I" << topInclude << " " << cfg.cflags()
    << " -L" << topLib << " " << cfg.ldflags(); 
  for(auto &source : sources) {
    cmdss << " " << source;
  }
  if(cfg.target() == "a") {
    cmdss << " -c";
    archiveCmdss << "ar -q " << dest << cfg.getTargetName() << " *.o; rm -f *.o";
  } else {
    if(cfg.target() == "so") {
      cmdss << " -shared";
    }        
    cmdss << " -o " << dest << cfg.getTargetName();
  } 
  for(auto &lib : builtLibs) {
    cmdss << " " << lib;
  }
  cmd = cmdss.str();
  archiveCmd = archiveCmdss.str();
}

Compiler::Compiler(const std::string &topInclude,
                   const std::string &topLib,
                   const std::set<std::string> &builtLibs) 
  : topInclude_(topInclude), topLib_(topLib), builtLibs_(builtLibs) {
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
  std::string cmd, archiveCmd;
  buildCompileCmds(sources, cfg, dest, builtLibs_, topInclude_, topLib_, cmd, archiveCmd); 
  bool ret = Util::run(cmd); 
  if(ret && !archiveCmd.empty()) {
    return Util::run(archiveCmd);
  }
  return ret;
}

}
