#include "Util.h"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

namespace Cast {

  namespace Util {

    bool chdir(const std::string &dir) {
      if(::chdir(dir.c_str()) < 0) {
        return false;
      }
      return true;
    }

    bool exists(const std::string &dir) {
      struct stat statbuf;
      if(stat(dir.c_str(), &statbuf) == 0) {
        return true;
      }
      return false;
    }

    bool run(const std::string &cmd) {
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

    bool mkdirp(const std::string &dir) { 
      return run("mkdir -p " + dir);
    }

    std::vector<std::string> getFiles(const std::string &path, 
                                      const std::vector<std::string> &filter) {
      DIR *dir;
      std::vector<std::string> files;
      if((dir = opendir(path.c_str())) != NULL) {
        struct dirent *entry;
         while((entry = readdir(dir)) != NULL) {
           struct stat statbuf;
           std::string fpath = path + "/" + entry->d_name;
           if(stat(fpath.c_str(), &statbuf) != 0) {
             continue;
           }
           if(S_ISREG(statbuf.st_mode)) {
             if(filter.empty()) {
              files.push_back(entry->d_name);
             }
             std::string file = entry->d_name;
             size_t pos = file.rfind(".");
             std::string ext = file.substr(pos);
             for(auto f = filter.begin(); f != filter.end(); ++f) {
               if(ext == *f) {
                 files.push_back(entry->d_name);
                 break;
               }
             }
           }
         }
         closedir (dir);
      } 
      else {
        std::cout << "ERR> Unable to read dir: " << path << std::endl;
      }
      return files;
    }
  }
}
