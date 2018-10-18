#include <catch2/catch.hpp>
#include "../Config.h"

TEST_CASE("Config test", "[config]") {
  Cast::Config config;
  config.read("test.cfg");
  REQUIRE(config.target() == "exe");
}
