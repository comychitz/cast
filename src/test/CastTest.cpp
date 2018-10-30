#include <catch2/catch.hpp>
#include <unistd.h>
#include <Cast.h>
#include <Util.h>

TEST_CASE("test building example project 1", "[catch]") {
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast caster(cwd + "/../../examples/example1");
  REQUIRE(caster.build() == 0);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/build") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/build/bin") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/build/lib") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/build/include") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/src/.build") == true);
  REQUIRE(Cast::Util::run(cwd +"/../../examples/example1/build/bin/src") == true);

  REQUIRE(Cast::Util::chdir(cwd) == true);

  REQUIRE(caster.clean() == 0);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/build") == false);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example1/src/.build") == false);

  REQUIRE(Cast::Util::chdir(cwd) == true);
}

TEST_CASE("test building example project 2", "[catch]") {
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast caster(cwd + "/../../examples/example2");
  REQUIRE(caster.build() == 0);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/build") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/build/bin") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/build/lib") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/build/include") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/src/common/.build") == true);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/src/myExe/.build") == true);
  REQUIRE(Cast::Util::run(cwd +"/../../examples/example2/build/bin/myExe") == true);

  REQUIRE(Cast::Util::chdir(cwd) == true);

  REQUIRE(caster.clean() == 0);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/build") == false);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/common/.build") == false);
  REQUIRE(Cast::Util::exists(cwd+"/../../examples/example2/myExe/.build") == false);

  REQUIRE(Cast::Util::chdir(cwd) == true);
}
