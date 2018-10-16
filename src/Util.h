#ifndef _CAST_UTIL_H_
#define  _CAST_UTIL_H_

namespace Cast {

  namespace Util {

    bool run(const std::string &cmd);

    bool symlink(const std::vector<std::string> &files, 
                 const std::string &dest);

    bool mkdirp(const std::string &dir);

    std::vector<std::string> getFiles(const std::string &path, 
                                      const std::vector<std::string> &filter = 
                                        std::vector<std::string>());
  }

}
