#include "Cast.h"
#include "Util.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <set>
#include <sys/stat.h>

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

  static void setupTopBuild() {
    Util::mkdirp(topBin());
    Util::mkdirp(topInclude());
    Util::mkdirp(topLib());
  }

  class DirectoryScope {
    std::string cwd_;
    public: 
      explicit DirectoryScope(const std::string &dir) : cwd_(getcwd(NULL, 0)) {
        if(!Util::chdir(dir)) {
          _exit(1);
        }
      }

      ~DirectoryScope() {
        (void)Util::chdir(cwd_);
      }
  };

  static std::string getTargetName(const Config &cfg)  {
    std::string name = cfg.name();
    if(cfg.target() == "so") {
      name += ".so";
    } else if(cfg.target() == "a") {
      name += ".a";
    }
    return name;
  }

  static void buildCompileCmds(const std::vector<std::string> &sources,
                               const Config &cfg, const std::string &dest,
                               std::string &cmd, std::string &archiveCmd)
  {
    std::stringstream cmdss, archiveCmdss;
    cmdss << "g++ " << "-I. -I" << topInclude() << " " << cfg.cflags()
          << " -L" << topLib() << " " << cfg.ldflags(); 
    for(auto &source : sources) {
      cmdss << " " << source;
    }
    if(cfg.target() == "a") {
      cmdss << " -c";
      archiveCmdss << "ar -q " << dest << getTargetName(cfg) << " *.o; rm -f *.o";
    } else {
      if(cfg.target() == "so") {
        cmdss << " -shared";
      }        
      cmdss << " -o " << dest << getTargetName(cfg);
    } 
    for(auto &lib : builtLibs_) {
      cmdss << " " << lib;
    }
    cmd = cmdss.str();
    archiveCmd = archiveCmdss.str();
  }

  static time_t getFileModTime(const std::string &path) {
    struct stat statbuf;
    if(stat(path.c_str(), &statbuf) != 0) {
      return 0;
    }
    return statbuf.st_mtimespec.tv_sec;
  }

  static bool checkFilesUpToDate(const std::string &dest, const Config &cfg,
                                 std::vector<std::string> &sources) {
    //
    // TODO - filter files that don't need to be recompiled again
    //
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

  static bool buildCwd(const Config &cfg, 
                       const std::string &dir,
                       const std::string &dest) {
    Util::mkdirp(dest);
    std::vector<std::string> exts = {".cpp", ".c", ".cc"};
    std::vector<std::string> sources = Util::getFiles(".", exts);
    checkFilesUpToDate(cfg, dest, sources);
    if(sources.empty()) {
      return true;
    }
    std::string cmd, archiveCmd;
    buildCompileCmds(sources, cfg, dest, cmd, archiveCmd); 
    if(!Util::run(cmd) || (!archiveCmd.empty() && !Util::run(archiveCmd))) {
      return false;
    }
    if(cfg.target() == "so" || cfg.target() == "a") {
      std::string libPath = std::string(getcwd(NULL, 0)) + "/" + dest +
        (cfg.target()=="so" ?  cfg.name()+".so" : cfg.name()+".a");
      builtLibs_.insert(libPath);
    }
    return dir == "test" ? true : linkFiles(cfg, dir, dest);
  } 

  static bool check(const std::string &name,
                    const std::string &dir) {
    bool ret = false;
    DirectoryScope dirScope(dir);
    const std::string &testName = name+"Test", &dest = "../.build/";
    Config testCfg(testName);
    testCfg.cflags("-std=c++14");
    if(buildCwd(testCfg, dir, dest)) {
      ret = Util::run(dest+testName);
    }
    return ret;
  } 

  static int build(const std::string &dir) {
    std::cout << "cast: Entering directory [" << dir << "]" << std::endl;
    DirectoryScope dirScope(dir);
    int ret = 0;
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
    if (!buildCwd(cfg, dir, dest)) {
      ret = 1;
    }
    if (runTests_) {
      if(Util::exists("test") && !::Cast::check(cfg.name(), "test")) {
        ret = 1;
      }
    }
    return ret;
  }

  static int clean(const std::string &dir) {
    int ret = 0;
    DirectoryScope dirScope(dir);
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
    return ret;
  }

  Cast::Cast(const std::string &topDirPath) { 
    top_ = topDirPath;
  }

  Cast::~Cast() {
  }

  int Cast::build() {
    builtLibs_.clear();
    DirectoryScope dirScope(top_); 
    setupTopBuild();
    return ::Cast::build("src");
  } 

  int Cast::clean() {
    Util::rmrf(topBuild());
    DirectoryScope dirScope(top_);
    return ::Cast::clean("src");
  }

  int Cast::check() {
    runTests_ = true;
    return build();
  }

}
