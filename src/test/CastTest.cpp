#include <catch2/catch.hpp>
#include <unistd.h>
#include <Cast.h>
#include <Util.h>

TEST_CASE("test building example project 1", "[catch]") {
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast caster(cwd + "/../../examples/example1");
  REQUIRE(caster.build() == 0);
  REQUIRE(Cast::Util::chdir(cwd) == true);
  REQUIRE(Cast::Util::run(cwd +"/../../examples/example1/build/bin/src") == true);
}
