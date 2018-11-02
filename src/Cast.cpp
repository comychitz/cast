#include "Cast.h"
#include "Util.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <set>

namespace Cast {

  static std::string top_;
  static bool runTests_ = false;
  static std::set<std::string> builtLibs_;

  static std::string topBuild() {
    return top_ + "/build";
  }

  static std::string topInclude() {
    return topBuild() + "/include";
  }

  static std::string topLib() {
    return topBuild() + "/lib";
  }

  static std::string topBin() {
    return topBuild() + "/bin";
  }

  static bool buildCwd(const Config &cfg, 
                       const std::string &dir,
                       const std::string &dest) {
    Util::mkdirp(dest);
    std::vector<std::string> exts = {".cpp", ".c", ".cc"};
    std::vector<std::string> sources = Util::getFiles(".", exts);
    if(sources.empty()) {
      return true;
    }
    std::stringstream cmd, archiveCmd;
    cmd << "g++ " << "-I. -I" << topInclude() << " " << cfg.cflags()
      << " -L" << topLib() << " " << cfg.ldflags(); 
    for(auto &source : sources) {
      cmd << " " << source;
    }
    if(cfg.target() == "a") {
      cmd << " -c";
      archiveCmd << "ar -q " << dest << cfg.name() << ".a *.o; rm -f *.o";
    } else {
      if(cfg.target() == "so") {
        cmd << " -shared";
      }        
      cmd << " -o " << dest << cfg.name();
      if(cfg.target() == "so") {
        cmd << ".so";
      } 
    } 
    for(auto &lib : builtLibs_) {
      cmd << " " << lib;
    }

    if(!Util::run(cmd.str()) || 
       (!archiveCmd.str().empty() && !Util::run(archiveCmd.str()))) {
      return false;
    }

    if(cfg.target() == "so" || cfg.target() == "a") {
      std::string libPath = std::string(getcwd(NULL, 0)) + "/" + dest +
        (cfg.target()=="so" ?  cfg.name()+".so" : cfg.name()+".a");
      builtLibs_.insert(libPath);
    }
    return true;
  }

  static bool linkFiles(const Config &cfg, 
                        const std::string &dir,
                        const std::string &dest) {
    bool ret = true, includePath = true;
    std::string cwd = getcwd(NULL, 0);
    std::vector<std::string> hExts = {".hpp", ".h"};
    std::vector<std::string> headers = Util::getFiles(cwd, hExts, includePath);
    if(!Util::symlink(headers, topInclude())) {
      ret = false;
    }
    if(cfg.target() == "exe") {
      std::vector<std::string> exes = {cwd+"/"+dest+cfg.name()};
      if(!Util::symlink(exes, topBin())) {
        ret = false;
      }
    } else {
      std::vector<std::string> lExts = {".so", ".a"};
      std::vector<std::string> libs = Util::getFiles(cwd+"/"+dest, 
                                                     lExts, includePath);
      if(!Util::symlink(libs, topLib())) {
        ret = false;
      }
    }
    return ret; 
  }

  static bool check(const std::string &name,
                    const std::string &dir) {
    bool ret = false;
    std::string cwd = getcwd(NULL, 0);
    if(Util::chdir(dir)) {
      const std::string &testName = name+"Test", &dest = "../.build/";
      Config testCfg(testName);
      if(buildCwd(testCfg, dir, dest)) {
        ret = Util::run(dest+testName);
      }
    }
    (void)Util::chdir(cwd);
    return ret;
  } 

  static int build(const std::string &dir) {
    int ret = 0;
    std::cout << "cast: Entering directory [" << dir << "]" << std::endl;
    std::string cwd = getcwd(NULL, 0);
    if(Util::chdir(dir)) {
      Config cfg(dir);
      if(Util::exists("cast.cfg")) {
        cfg.read("cast.cfg");
      }
      for(auto &dir : cfg.subdirs()) {
        if(build(dir) != 0) {
          ret = 1;
        }
      }
      const std::string &dest = ".build/";
      if (!buildCwd(cfg, dir, dest) || 
          !linkFiles(cfg, dir, dest)) {
        ret = 1;
      }
      if (runTests_) {
        if(Util::exists("test") && !::Cast::check(cfg.name(), "test")) {
          ret = 1;
        }
      }
    } else {
      ret = 1;
    }
    (void)Util::chdir(cwd);
    return ret;
  }

  static int clean(const std::string &dir) {
    int ret = 0;
    std::string cwd = getcwd(NULL, 0);
    if(Util::chdir(dir)) {
      Util::rmrf(".build");
      Config cfg(dir);
      if(Util::exists("cast.cfg")) {
        cfg.read("cast.cfg");
      }
      for(auto &dir : cfg.subdirs()) {
        if(clean(dir) != 0) {
          ret = 1;
        }
      }
      (void)Util::chdir(cwd);
    }
    return ret;
  }

  Cast::Cast(const std::string &topDirPath) { 
    top_ = topDirPath;
  }

  Cast::~Cast() {
  }

  int Cast::build() {
    int ret = 1;
    if(Util::chdir(top_)) {
      Util::mkdirp(topBin());
      Util::mkdirp(topInclude());
      Util::mkdirp(topLib());
      ret = ::Cast::build("src");
      Util::chdir(top_);
    }
    return ret;
  } 

  int Cast::clean() {
    Util::rmrf(topBuild());
    int ret = 1;
    if(Util::chdir(top_)) {
      ret = ::Cast::clean("src");
    }
    return ret;
  }

  int Cast::check() {
    runTests_ = true;
    return build();
  }

}
