#include <catch2/catch.hpp>
#include <unistd.h>
#include <Cast.h>
#include <Util.h>

TEST_CASE("test building example project 1", "[catch]") {
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast caster(cwd + "/../../examples/example1");
  CHECK(caster.build() == 0);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/build") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/build/bin") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/build/lib") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/build/include") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/src/.build") == true);
  CHECK(Cast::Util::run(cwd +"/../../examples/example1/build/bin/src") == true);

  CHECK(Cast::Util::chdir(cwd) == true);

  CHECK(caster.clean() == 0);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/build") == false);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example1/src/.build") == false);

  CHECK(Cast::Util::chdir(cwd) == true);
}

TEST_CASE("test building example project 2", "[catch]") {
  std::string cwd = getcwd(NULL, 0);
  Cast::Cast caster(cwd + "/../../examples/example2");
  CHECK(caster.build() == 0);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/build") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/build/bin") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/build/lib") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/build/include") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/src/common/.build") == true);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/src/myExe/.build") == true);
  CHECK(Cast::Util::run(cwd +"/../../examples/example2/build/bin/myExe") == true);

  CHECK(Cast::Util::chdir(cwd) == true);

  CHECK(caster.clean() == 0);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/build") == false);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/common/.build") == false);
  CHECK(Cast::Util::exists(cwd+"/../../examples/example2/myExe/.build") == false);

  CHECK(Cast::Util::chdir(cwd) == true);

  CHECK(caster.check() == 0);
  CHECK(Cast::Util::run(cwd +"/../../examples/example2/build/bin/myExe") == true);
  CHECK(Cast::Util::exists(cwd +"/../../examples/example2/src/.build/commonTest") == true);

  CHECK(caster.clean() == 0);
}
