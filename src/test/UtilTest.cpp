#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../Util.h"

TEST_CASE( "Util::exists", "" ) {
  REQUIRE(Cast::Util::exists("/tmp/weorijwer,23ori") == false);
}
