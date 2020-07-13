#include <posixpp/expected.h>
#include <cerrno>
#include <catch2/catch.hpp>

SCENARIO( "expected holds results and throws exceptions", "[expected]" ) {
   GIVEN( " An expected<int> result initialized with 5 " ) {
      ::posixpp::expected<int> const result{5};
      THEN(" result.result() == 5 ") {
         CHECK( result.result() == 5 );
      } THEN(" result.has_error() == false ") {
         CHECK( result.has_error() == false );
      } THEN(" result.throw_if_error() does nothing ") {
         CHECK_NOTHROW(result.throw_if_error());
      } THEN(" result.error() throws bad_variant_access ") {
         CHECK_THROWS_AS(result.error(), ::std::bad_variant_access);
      }
   }
   GIVEN("An expectedd<int> result initialized as an error with ENOENT") {
      using expected_t = ::posixpp::expected<int>;
      expected_t const result{expected_t::err_tag{}, ENOENT};
      THEN(" result.result() throws ::std:system_error ") {
         using ::Catch::Matchers::Message;
         CHECK_THROWS_MATCHES(result.result(), ::std::system_error,
                              Message("No such file or directory"));
      } THEN(" result.throw_if_error() throws ::std::system_error ") {
         using ::Catch::Matchers::Message;
         CHECK_THROWS_MATCHES(result.throw_if_error(), ::std::system_error,
                              Message("No such file or directory"));
      } THEN(" result.has_error() == true ") {
         CHECK(result.has_error() == true);
      } THEN(" result.error() == ENOENT ") {
         CHECK(result.error() == ENOENT);
      } THEN(" result.error_code() is correct ") {
         CHECK(result.error_condition() == \
               ::std::system_category().default_error_condition(ENOENT));
      }
   }
}
