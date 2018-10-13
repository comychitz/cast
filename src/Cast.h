
namespace Cast {

  class Cast {
    public:
      Cast(const std::string &cwd) : top_(cwd) { }

      ~Cast() { }

      int build(const std::string &dir) {
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
          if(!buildCwd() || !linkFiles()) {
            ret = 1;
          }
        } else {
          ret = 1;
        }
        return ret;
      }

      bool buildCwd() {
        std::vector<std::string> exts = {".cpp", ".c", ".cc"};
        std::vector<std::string> sources = Util::getFiles(".", exts);
        std::stringstream cmd;
        cmd << "g++ " << "-I. -I" << topInclude() << " " << cfg.cflags()
            << " -L" << topLib() << " " << cfg.ldflags(); 
        for(auto source = sources.begin(); source != sources.end(); ++source) {
          cmd << " " << *source;
        }
        const std::string dest = ".build/";
        Util::mkdirp(dest);
        if(cfg.target == "a") {
          cmd << " -c";
        } else {
          if(cfg.target == "so") {
            cmd << " -shared";
          }        
          cmd << " -o " << dest << dir;
          if(cfg.target == "so") {
            cmd << ".so";
          }
        }

        std::stringstream archiveCmd;
        if(cfg.target == "a") {
          archiveCmd << "ar -q " << dest << dir << ".a *.o; rm -f *.o";
        }
        if(!run(cmd.str())) {
          return 1;
        }
        if (!archiveCmd.str.empty() && !run(archiveCmd.str())) {
          return 1;
        }
        // TODO need to run linker?
        return 0;
      }

      bool linkFiles() const {
        // TODO
        return true; 
      }

      bool run(const std::string &cmd) const {
        std::cout << cmd << std::endl;
        return (system(cmd.c_str()) == 0);
      }

      std::string topInclude() const {
        return top_ + "/build/include";
      }

      std::string topLib() const {
        return top_ + "/build/lib";
      }

      std::string topBin() const {
        return top_ + "/build/bin";
      }

  private:
      std::string top_;
};
}
