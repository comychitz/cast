#include <unistd.h>

namespace Cast {

  namespace Util {

    bool run(const std::string &cmd) const {
        std::cout << cmd << std::endl;
        return (system(cmd.c_str()) == 0);
      }

    bool symlink(const std::vector<std::string> &files, 
              const std::string &dest) {

      for(auto file = files.begin(); file != files.end(); ++file) {
        if(::symlink(file->c_str(), dest.c_str()) < 0) {
          std::cout << "Link error: " << *file << " -> " << dest << ": " 
            << strerror(errno) << " (" << errno << ")";
          return false;
        }
      }
      return true;
    }

  }
}
