// Copyright 2021 by Eric Hopper
// Can be distributed under the terms of the LGPL v3.

#include <posixpp/fdflags.h>
#include <catch2/catch.hpp>
#include <type_traits>

// Only minimal testing of operations here because flagset tests are kind of
// exhaustive.
SCENARIO("openflags and fdflags interact properly")
{
   GIVEN("an some openflags values and an fdflags value") {
      using ::posixpp::fdflags;
      using ::posixpp::openflags;
      using ::std::is_same_v;

      fdflags fdf_val = fdflags::nonblock;
      openflags of_val = openflags::creat;
      openflags of_combined = fdflags::nonblock | openflags::creat;
      openflags of_val2 = openflags::creat | fdflags::sync;

      WHEN("The | operation is used, the results have the right type.") {
         STATIC_REQUIRE(is_same_v<decltype(fdf_val | of_val), openflags>);
         STATIC_REQUIRE(is_same_v<decltype(of_val | fdf_val), openflags>);
         REQUIRE((fdf_val | of_val) == of_combined);
      }
      WHEN("The & operation is used, the results have the right type.") {
         STATIC_REQUIRE(is_same_v<decltype(fdf_val & of_val), openflags>);
         STATIC_REQUIRE(is_same_v<decltype(of_val & fdf_val), openflags>);
         REQUIRE((of_combined & of_val2) == of_val);
      }
      WHEN("The ^ operation is used, the results have the right type.") {
         STATIC_REQUIRE(is_same_v<decltype(fdf_val ^ of_val), openflags>);
         STATIC_REQUIRE(is_same_v<decltype(of_val ^ fdf_val), openflags>);
         REQUIRE((of_combined ^ fdf_val) == of_val);
      }
      WHEN("The - operation is used, the results have the right type.") {
         STATIC_REQUIRE(is_same_v<decltype(fdf_val - of_val), fdflags>);
         STATIC_REQUIRE(is_same_v<decltype(of_val - fdf_val), openflags>);
         REQUIRE((of_combined - of_val2) == fdf_val);
         REQUIRE(of_val2 - of_val == fdflags::sync);
      }
   }
}
