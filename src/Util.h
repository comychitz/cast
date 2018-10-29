#ifndef _CAST_UTIL_H_
#define _CAST_UTIL_H_

#include <string>
#include <vector>

namespace Cast {

  namespace Util {

    bool chdir(const std::string &dir);

    bool exists(const std::string &f);

    bool run(const std::string &cmd);

    std::string basename(const std::string &path);

    bool symlink(const std::vector<std::string> &files, 
                 const std::string &dest);

    bool mkdirp(const std::string &dir);

    std::vector<std::string> getFiles(const std::string &path, 
                                      const std::vector<std::string> &filter = 
                                        std::vector<std::string>(),
                                      bool includePath = false);
  }
}
#endif
