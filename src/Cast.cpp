#include "Cast.h"

namespace Cast {

static bool buildCwd(const Config &cfg, const std::string &dest) {
  std::vector<std::string> exts = {".cpp", ".c", ".cc"};
  std::vector<std::string> sources = Util::getFiles(".", exts);
  std::stringstream cmd, archiveCmd;
  cmd << "g++ " << "-I. -I" << topInclude() << " " << cfg.cflags()
    << " -L" << topLib() << " " << cfg.ldflags(); 
  for(auto source = sources.begin(); source != sources.end(); ++source) {
    cmd << " " << *source;
  }
  if(cfg.target == "a") {
    cmd << " -c";
    archiveCmd << "ar -q " << dest << dir << ".a *.o; rm -f *.o";
  } else {
    if(cfg.target == "so") {
      cmd << " -shared";
    }        
    cmd << " -o " << dest << dir;
    if(cfg.target == "so") {
      cmd << ".so";
    }
  }
  if(!Util::run(cmd.str()) || 
     (!archiveCmd.str.empty() && !Util::run(archiveCmd.str()))) {
    return 1;
  }
  // TODO need to run linker?
  return 0;
}

Cast::Cast(const std::string &cwd) : top_(cwd) 
{ 
}

Cast::~Cast() 
{
}

int Cast::build(const std::string &dir) {
  int ret = 0;
  std::cout << "cast: Entering directory [" << dir << "]" << std::endl;
  if(Util::chdir(dir)) {
    Config cfg;
    if(Util::exists("cast.cfg")) {
      cfg.read("cast.cfg");
    }
    for(auto dir = cfg.subdirs.begin(); dir != cfg.subdirs.end(); ++dir) {
      build(*dir);
    }
    const std::string dest = ".build/";
    Util::mkdirp(dest);
    if(!buildCwd(cfg, dest) || !linkFiles(cfg, dest)) {
      ret = 1;
    }
  } else {
    ret = 1;
  }
  return ret;
}

bool Cast::linkFiles(const Config &cfg, const std::string &dest) {
  bool ret = true;
  do {
    std::vector<std::string> hExts = {".hpp", ".h"};
    std::vector<std::string> headers = Util::getFiles(".", hExts);
    if(!Util::symlink(headers, topInclude())) {
      ret = false;
      break;
    }
    if(cfg.target() == "exe") {
      if(!Util::symlink(dest+cfg.dir, topBin())) {
        ret = false;
        break;
      }
    } else {
      std::vector<std::string> lExts = {".so", ".a"};
      std::vector<std::string> libs = Util::getFiles(".", lExts);
      if(!Util::symlink(libs, topLib())) {
        ret = false;
        break;
      }
    }
  } while(false);
  return ret; 
}

}
