#include "Cast.h"
#include "Util.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace Cast {

  static std::string top_;

  static std::string topInclude() {
    return top_ + "/build/include";
  }

  static std::string topLib() {
    return top_ + "/build/lib";
  }

  static std::string topBin() {
    return top_ + "/build/bin";
  }

  static bool buildCwd(const Config &cfg, 
                       const std::string &dir,
                       const std::string &dest) {
    std::vector<std::string> exts = {".cpp", ".c", ".cc"};
    std::vector<std::string> sources = Util::getFiles(".", exts);
    std::stringstream cmd, archiveCmd;
    // TODO - make all strings into constants
    cmd << "g++ " << "-I. -I" << topInclude() << " " << cfg.cflags()
      << " -L" << topLib() << " " << cfg.ldflags(); 
    for(auto &source : sources) {
      cmd << " " << source;
    }
    if(cfg.target() == "a") {
      cmd << " -c";
      archiveCmd << "ar -q " << dest << dir << ".a *.o; rm -f *.o";
    } else {
      if(cfg.target() == "so") {
        cmd << " -shared";
      }        
      cmd << " -o " << dest << cfg.name();
      if(cfg.target() == "so") {
        cmd << ".so";
      }
    }
    if(!Util::run(cmd.str()) || 
       (!archiveCmd.str().empty() && !Util::run(archiveCmd.str()))) {
      return false;
    }
    // TODO need to run linker?
    return true;
  }

  static bool linkFiles(const Config &cfg, 
                        const std::string &dir,
                        const std::string &dest) {
    bool ret = true;
    do {
      bool includePath = true;
      std::vector<std::string> hExts = {".hpp", ".h"};
      std::vector<std::string> headers = Util::getFiles(".", hExts, 
                                                        includePath);
      if(!Util::symlink(headers, topInclude())) {
        ret = false;
        break;
      }
      if(cfg.target() == "exe") {
        std::string cwd = getcwd(NULL, 0);
        std::vector<std::string> exes = {cwd+"/"+dest+dir};
        if(!Util::symlink(exes, topBin())) {
          ret = false;
          break;
        }
      } else {
        std::vector<std::string> lExts = {".so", ".a"};
        std::vector<std::string> libs = Util::getFiles(".", lExts, includePath);
        if(!Util::symlink(libs, topLib())) {
          ret = false;
          break;
        }
      }
    } while(false);
    return ret; 
  }

  static int build(const std::string &dir) {
    int ret = 0;
    std::cout << "cast: Entering directory [" << dir << "]" << std::endl;
    if(Util::chdir(dir)) {
      Config cfg(dir);
      if(Util::exists("cast.cfg")) {
        cfg.read("cast.cfg");
      }
      for(auto &dir : cfg.subdirs()) {
        build(dir);
      }
      const std::string &dest = ".build/";
      Util::mkdirp(dest);
      if (!buildCwd(cfg, dir, dest) || 
          !linkFiles(cfg, dir, dest)) {
        ret = 1;
      }
    } else {
      ret = 1;
    }
    return ret;
  }

  Cast::Cast(const std::string &topDirPath) { 
    top_ = topDirPath;
    if(Util::chdir(top_)) {
      Util::mkdirp(topBin());
      Util::mkdirp(topInclude());
      Util::mkdirp(topLib());
    }
  }

  Cast::~Cast() {
  }

  int Cast::build() {
    return ::Cast::build("src");
  } 

  int Cast::clean() {

    //
    // TODO
    //

    return 1;
  }

  int Cast::check() {

    //
    // TODO
    //

    return 1;
  }

}
