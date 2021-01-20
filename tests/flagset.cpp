#include <pppbase/flagset.h>
#include <catch2/catch.hpp>

class test_flagset : public ::pppbase::specific_flagset_crtp<test_flagset> {
   using base_t = ::pppbase::specific_flagset_crtp<test_flagset>;
 public:
   using bitvec_t = base_t::bitvec_t;
   constexpr explicit test_flagset(bitvec_t bits = 0) : base_t(bits) {}

   using base_t::getbits;
};


SCENARIO("flagsets support all required operations correctly")
{
   GIVEN("A three carefully chosen flagsets.")
   {
      using ::pppbase::flagset_base;
      test_flagset f0;
      test_flagset f1{1};
      test_flagset f2{2};
      test_flagset f3{3};

      WHEN("Each is tested for truth") {
         THEN("f0 is false") {
            REQUIRE_FALSE(f0);
         }
         THEN("f1 is true") {
            REQUIRE(f1);
         }
         THEN("f2 is true") {
            REQUIRE(f2);
         }
         THEN("f3 is true") {
            REQUIRE(f3);
         }
      }
      WHEN("They are each intersected with eachother") {
         THEN("f0.intersection(f0) is f0") {
            REQUIRE(f0.intersection(f0) == f0);
         }
         THEN("f0.intersection(f1) is f0 and commutative") {
            REQUIRE(f0.intersection(f1) == f1.intersection(f0));
            REQUIRE(f0.intersection(f1) == f0);
         }
         THEN("f0.intersection(f2) is f0 and commutative") {
            REQUIRE(f0.intersection(f2) == f2.intersection(f0));
            REQUIRE(f0.intersection(f2) == f0);
         }
         THEN("f0.intersection(f3) is f0 and commutative") {
            REQUIRE(f0.intersection(f3) == f3.intersection(f0));
            REQUIRE(f0.intersection(f3) == f0);
         }
         THEN("f1.intersection(f1) is f1") {
            REQUIRE(f1.intersection(f1) == f1);
         }
         THEN("f1.intersection(f2) is f0 and commutative") {
            REQUIRE(f1.intersection(f2) == f2.intersection(f1));
            REQUIRE(f1.intersection(f2) == f0);
         }
         THEN("f1.intersection(f3) is f1 and commutative") {
            REQUIRE(f1.intersection(f3) == f3.intersection(f1));
            REQUIRE(f1.intersection(f3) == f1);
         }
         THEN("f2.intersection(f2) is f2") {
            REQUIRE(f2.intersection(f2) == f2);
         }
         THEN("f2.intersection(f3) is f2 and commutative") {
            REQUIRE(f2.intersection(f3) == f3.intersection(f2));
            REQUIRE(f2.intersection(f3) == f2);
         }
         THEN("f3.intersection(f3) is f3") {
            REQUIRE(f3.intersection(f3) == f3);
         }
      }
      WHEN("They are each setuinioned with eachother") {
         THEN("f0.setunion(f0) is f0") {
            REQUIRE(f0.setunion(f0) == f0);
         }
         THEN("f0.setunion(f1) is f1 and commutative") {
            REQUIRE(f0.setunion(f1) == f1.setunion(f0));
            REQUIRE(f0.setunion(f1) == f1);
         }
         THEN("f0.setunion(f2) is f2 and commutative") {
            REQUIRE(f0.setunion(f2) == f2.setunion(f0));
            REQUIRE(f0.setunion(f2) == f2);
         }
         THEN("f0.setunion(f3) is f3 and commutative") {
            REQUIRE(f0.setunion(f3) == f3.setunion(f0));
            REQUIRE(f0.setunion(f3) == f3);
         }
         THEN("f1.setunion(f1) is f1") {
            REQUIRE(f1.setunion(f1) == f1);
         }
         THEN("f1.setunion(f2) is f3 and commutative") {
            REQUIRE(f1.setunion(f2) == f2.setunion(f1));
            REQUIRE(f1.setunion(f2) == f3);
         }
         THEN("f1.setunion(f3) is f3 and commutative") {
            REQUIRE(f1.setunion(f3) == f3.setunion(f1));
            REQUIRE(f1.setunion(f3) == f3);
         }
         THEN("f2.setunion(f2) is f2") {
            REQUIRE(f2.setunion(f2) == f2);
         }
         THEN("f2.setunion(f3) is f3 and commutative") {
            REQUIRE(f2.setunion(f3) == f3.setunion(f2));
            REQUIRE(f2.setunion(f3) == f3);
         }
         THEN("f3.setunion(f3) is f3") {
            REQUIRE(f3.setunion(f3) == f3);
         }
      }
      WHEN("They are each symmetric_differenced with eachother") {
         THEN("f0.symmetric_difference(f0) is f0") {
            REQUIRE(f0.symmetric_difference(f0) == f0);
         }
         THEN("f0.symmetric_difference(f1) is f1 and commutative") {
            REQUIRE(f0.symmetric_difference(f1) == f1.symmetric_difference(f0));
            REQUIRE(f0.symmetric_difference(f1) == f1);
         }
         THEN("f0.symmetric_difference(f2) is f2 and commutative") {
            REQUIRE(f0.symmetric_difference(f2) == f2.symmetric_difference(f0));
            REQUIRE(f0.symmetric_difference(f2) == f2);
         }
         THEN("f0.symmetric_difference(f3) is f3 and commutative") {
            REQUIRE(f0.symmetric_difference(f3) == f3.symmetric_difference(f0));
            REQUIRE(f0.symmetric_difference(f3) == f3);
         }
         THEN("f1.symmetric_difference(f1) is f0") {
            REQUIRE(f1.symmetric_difference(f1) == f0);
         }
         THEN("f1.symmetric_difference(f2) is f3 and commutative") {
            REQUIRE(f1.symmetric_difference(f2) == f2.symmetric_difference(f1));
            REQUIRE(f1.symmetric_difference(f2) == f3);
         }
         THEN("f1.symmetric_difference(f3) is f2 and commutative") {
            REQUIRE(f1.symmetric_difference(f3) == f3.symmetric_difference(f1));
            REQUIRE(f1.symmetric_difference(f3) == f2);
         }
         THEN("f2.symmetric_difference(f2) is f0") {
            REQUIRE(f2.symmetric_difference(f2) == f0);
         }
         THEN("f2.symmetric_difference(f3) is f1 and commutative") {
            REQUIRE(f2.symmetric_difference(f3) == f3.symmetric_difference(f2));
            REQUIRE(f2.symmetric_difference(f3) == f1);
         }
         THEN("f3.symmetric_difference(f3) is f0") {
            REQUIRE(f3.symmetric_difference(f3) == f0);
         }
      }
      WHEN("They are each subtracted from eachother") {
         THEN("Various subtraction operations have the correct result.") {
            REQUIRE(f0.minus(f0) == f0);
            REQUIRE(f0.minus(f1) == f0);
            REQUIRE(f0.minus(f2) == f0);
            REQUIRE(f0.minus(f3) == f0);
            REQUIRE(f1.minus(f0) == f1);
            REQUIRE(f1.minus(f1) == f0);
            REQUIRE(f1.minus(f2) == f1);
            REQUIRE(f1.minus(f3) == f0);
            REQUIRE(f2.minus(f0) == f2);
            REQUIRE(f2.minus(f1) == f2);
            REQUIRE(f2.minus(f2) == f0);
            REQUIRE(f2.minus(f3) == f0);
            REQUIRE(f3.minus(f0) == f3);
            REQUIRE(f3.minus(f1) == f2);
            REQUIRE(f3.minus(f2) == f1);
            REQUIRE(f3.minus(f3) == f0);
         }
      }
      WHEN("They are each compared for equality") {
         THEN("f0 == f0 and not f0 != f0") {
            REQUIRE(f0 == f0);
            REQUIRE_FALSE(f0 != f0);
         }
         THEN("f0 != f1 and not f0 == f1 and both are commutative") {
            REQUIRE(f0 != f1);
            REQUIRE(f1 != f0);
            REQUIRE_FALSE(f0 == f1);
            REQUIRE_FALSE(f1 == f0);
         }
         THEN("f0 != f2 and not f0 == f2 and both are commutative") {
            REQUIRE(f0 != f2);
            REQUIRE(f2 != f0);
            REQUIRE_FALSE(f0 == f2);
            REQUIRE_FALSE(f2 == f0);
         }
         THEN("f0 != f3 and not f0 == f3 and both are commutative") {
            REQUIRE(f0 != f3);
            REQUIRE(f3 != f0);
            REQUIRE_FALSE(f0 == f3);
            REQUIRE_FALSE(f3 == f0);
         }
         THEN("f1 == f1 and not f1 != f1") {
            REQUIRE(f1 == f1);
            REQUIRE_FALSE(f1 != f1);
         }
         THEN("f1 != f2 and not f1 == f2 and both are commutative") {
            REQUIRE(f1 != f2);
            REQUIRE(f2 != f1);
            REQUIRE_FALSE(f1 == f2);
            REQUIRE_FALSE(f2 == f1);
         }
         THEN("f1 != f3 and not f1 == f3 and both are commutative") {
            REQUIRE(f1 != f3);
            REQUIRE(f3 != f1);
            REQUIRE_FALSE(f1 == f3);
            REQUIRE_FALSE(f3 == f1);
         }
         THEN("f2 == f2 and not f2 != f2") {
            REQUIRE(f2 == f2);
            REQUIRE_FALSE(f2 != f2);
         }
         THEN("f2 != f3 and not f2 == f3 and both are commutative") {
            REQUIRE(f2 != f3);
            REQUIRE(f3 != f2);
            REQUIRE_FALSE(f2 == f3);
            REQUIRE_FALSE(f3 == f2);
         }
      }
   }
}
