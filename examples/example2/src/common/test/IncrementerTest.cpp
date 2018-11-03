#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("incrementer test", "[incrementer]") {
  Incrementer inc;
  REQUIRES(inc.getCount() == 0);
  inc.increment();
  REQUIRES(inc.getCount() == 1);
}
