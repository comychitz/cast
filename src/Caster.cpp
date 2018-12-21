#include "Caster.h"
#include "Util.h"
#include "Compiler.h"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <set>

namespace Cast {

  static std::string top_;
  static bool runTests_ = false;

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

  static std::string depCfgDir() {
    return topBuild() + "/deps";
  }

  static void setupTopBuild() {
    Util::mkdirp(topBin());
    Util::mkdirp(topInclude());
    Util::mkdirp(topLib());
    Util::mkdirp(depCfgDir());
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
      auto libs = Util::getFiles(cwd+"/"+dest, lExts, includePath);
      if(!Util::symlink(libs, topLib())) {
        ret = false;
      }
    }
    return ret; 
  }
 
  bool Caster::buildCwd_(const Config &cfg, 
                         const std::string &dir,
                         const std::string &dest) {
    std::vector<std::string> exts = {".cpp", ".c", ".cc"};
    std::vector<std::string> sources = Util::getFiles(".", exts);
    if(sources.empty()) {
      return true;
    }
    Compiler compiler(toolchain_, topInclude(), topLib(), depMgr_);
    if(!compiler.compile(cfg, dest, sources)) {
      return false;
    }
    if(cfg.target() == "so" || cfg.target() == "a") {
      std::string libPath = std::string(getcwd(NULL, 0)) + "/" + 
                            dest + cfg.getTargetName();
      std::vector<std::string> exts = {".h", ".hpp"};
      std::vector<std::string> headers = Util::getFiles(".", exts);
      depMgr_.addLib(libPath, headers);
    }
    return dir == "test" ? true : linkFiles(cfg, dir, dest);
  }

  bool Caster::check_(const std::string &name,
                      const std::string &dir) {
    bool ret = false;
    DirectoryScope dirScope(dir);
    const std::string &testName = name+"Test", &dest = "../.build/";
    Config testCfg(testName);
    testCfg.cflags("-std=c++14");
    if(buildCwd_(testCfg, dir, dest)) {
      ret = Util::run(dest+testName);
    }
    return ret;
  } 

  int Caster::build_(const std::string &dir) {
    std::cout << "cast: Entering directory [" << getcwd(NULL, 0) << "/" << dir
              << "]" << std::endl;
    DirectoryScope dirScope(dir);
    int ret = 0;
    Config cfg(dir);
    if(Util::exists("cast.cfg")) {
      cfg.read("cast.cfg");
    }
    for(auto &dir : cfg.subdirs()) {
      if(build_(dir) != 0) {
        ret = 1;
      }
    }
    const std::string &dest = ".build/";
    if (!buildCwd_(cfg, dir, dest)) {
      ret = 1;
    }
    if (runTests_ && Util::exists("test")) {
      if(!check_(cfg.name(), "test")) {
        ret = 1;
      }
    }
    std::cout << "cast: Leaving directory [" << getcwd(NULL, 0) << "]" << std::endl;
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

  static void downloadDependencies() {
    const std::string &cwd = getcwd(NULL, 0);
    if(Util::exists("./conanfile.txt")) {
      DirectoryScope dirScope(depCfgDir());
      Util::run("conan install " + cwd + "/conanfile.txt"); 
    }
  }

  static bool loadToolchainCfg(CompilerConfig &toolchain) {
    const std::string &toolchainCfgDir = "/usr/local/share/cast/crosstools";
    std::vector<std::string> filter = {".cfg"};
    auto cfgs = Util::getFiles(toolchainCfgDir, filter);
    for(auto &cfg : cfgs) {
      if(toolchain.name+".cfg" == cfg) {
        toolchain.read(toolchainCfgDir+"/"+cfg);
        break;
      }
    }
    if(toolchain.gxx.empty() || 
       toolchain.ar.empty()  || 
       toolchain.strip.empty()) {
      return false;
    }
    return true;
  }

  Caster::Caster(const std::string &topDirPath) { 
    top_ = topDirPath;
  }

  Caster::~Caster() {
  }

  int Caster::build(const std::string &toolchain) {

    if(!toolchain.empty() && toolchain != "native") {
      toolchain_.name = toolchain;
      if(!loadToolchainCfg(toolchain_)) {
        return 1;
      }
    } else {
      // use native compiler
      toolchain_.gxx = "g++";
      toolchain_.ar = "ar";
      toolchain_.strip = "strip";
    }

    depMgr_.clear();
    DirectoryScope dirScope(top_); 
    setupTopBuild();
    downloadDependencies();
    depMgr_.readCfgDir(depCfgDir());
    return build_("src");
  } 

  int Caster::clean() {
    Util::rmrf(topBuild());
    DirectoryScope dirScope(top_);
    return ::Cast::clean("src");
  }

  int Caster::check() {
    runTests_ = true;
    return build("");
  }

}
