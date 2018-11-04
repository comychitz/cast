#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Incrementer.h"

TEST_CASE("incrementer test", "[incrementer]") {
  Incrementer inc;
  REQUIRE(inc.getCount() == 0);
  inc.increment();
  REQUIRE(inc.getCount() == 1);
}
