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

Compiler::Compiler(const CompilerConfig &toolchain,
                   const std::string &topInclude,
                   const std::string &topLib)
  : topInclude_(topInclude), topLib_(topLib),
  toolchain_(toolchain) {
}

Compiler::~Compiler() {
}

bool Compiler::compile(const Config &cfg, const std::string &dest,
                       const std::vector<std::string> &sources,
                       const std::set<std::string> &depLibs) {
  if(targetUpToDate(dest, cfg, sources)) {
    std::cout << "cast: Target (" << cfg.getTargetName() << ") up to date" << std::endl;
    return true;
  }
  Util::mkdirp(dest);
  if(!compileSources_(sources, depLibs, cfg, dest)) {
    return false;
  }
  if(cfg.target() == "a") {
    return createStaticArchive_(dest, cfg);
  }
  return true;
}

bool Compiler::compileSources_(const std::vector<std::string> &sources,
                               const std::set<std::string> &depLibs,
                               const Config &cfg, 
                               const std::string &dest) {

  if(cfg.target() == "exe" || cfg.target() == "so") {
    std::stringstream cmd;
    cmd << toolchain_.gxx << " -I. -I" << topInclude_ << " " << cfg.cflags()
        << " -L" << topLib_ << " " << cfg.ldflags(); 
    for(auto &source : sources) {
      cmd << " " << source;
    }
    for(auto &lib : depLibs) {
      cmd << " " << lib;
    }
    if(cfg.target() == "so") {
      cmd << " -shared";
    }        
    cmd << " -o " << dest << cfg.getTargetName();
    return Util::run(cmd.str());
  } 

  // if static library...
  for(auto &source : sources) {
    std::stringstream cmd;

    // compile the source first...
    cmd << toolchain_.gxx << " -I. -I" << topInclude_ << " " << cfg.cflags() << source << " -c";
    if(!Util::run(cmd.str())) {
      return false;
    }

    std::string output(source);
    size_t pos = output.find(".c");
    output.replace(pos, output.size()-pos, ".o");

    // then link it
    cmd.str("");
    cmd << toolchain_.gxx << " " << output << " -o " << output << " -L" 
        << topLib_ << " " << cfg.ldflags(); 
    for(auto &lib : depLibs) {
      cmd << " " << lib;
    }
    if(!Util::run(cmd.str())) {
      return false;
    }
  }
  return true;
}

bool Compiler::createStaticArchive_(const std::string &dest, 
                                    const Config &cfg) {
  std::stringstream cmd;
  cmd << toolchain_.ar << " -q " << dest << cfg.getTargetName() 
      << " *.o && rm -f *.o";
  return Util::run(cmd.str());
}

}
