#include <catch2/catch.hpp>
#include "Config.h"

TEST_CASE("Config read test #1", "[config]") {
  Cast::Config config("src");
  config.read("data/cast_1.cfg");
  REQUIRE(config.target() == "exe");
  REQUIRE(config.name() == "src");
  REQUIRE(config.subdirs().size() == 3);
  if(config.subdirs().size() == 3) {
    REQUIRE(config.subdirs().at(0) == "dir");
    REQUIRE(config.subdirs().at(1) == "dir2");
    REQUIRE(config.subdirs().at(2) == "dir3");
  }
}

TEST_CASE("Config read test #2", "[config]") {
  Cast::Config config("test");
  config.read("data/cast_2.cfg");
  REQUIRE(config.target() == "a");
  REQUIRE(config.name() == "myProgram");
  REQUIRE(config.cflags() == "-std=c++11 -fPIC -Wall ");
  REQUIRE(config.ldflags() == "-ldl ");
}

