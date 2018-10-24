#include <catch2/catch.hpp>
#include <sys/stat.h>
#include "Util.h"

TEST_CASE("Util::exists", "[util]") {
  REQUIRE(Cast::Util::exists("/tmp/weorijwer,23ori") == false);
}

TEST_CASE("Util::getFiles", "[util]") {

  REQUIRE(system("rm -rf tmpWork") == 0);
  REQUIRE(mkdir("tmpWork", 0744) == 0);

  REQUIRE(!system("touch tmpWork/a.cpp tmpWork/b.cpp tmpWork/b.h tmpWork/c.hpp"));
  std::vector<std::string> filter;
  std::vector<std::string> files = Cast::Util::getFiles("tmpWork");
  REQUIRE(files.size() == 4);
  filter.push_back(".cpp");
  files = Cast::Util::getFiles("tmpWork", filter);
  REQUIRE(files.size() == 2);
  filter.push_back(".h");
  files = Cast::Util::getFiles("tmpWork", filter);
  REQUIRE(files.size() == 3);
  filter.push_back(".hpp");
  files = Cast::Util::getFiles("tmpWork", filter);
  REQUIRE(files.size() == 4);

  REQUIRE(!system("rm -rf tmpWork"));
}
