#include <posixpp/expected.h>
#include <cerrno>
#include <catch2/catch.hpp>

class move_detector {
 public:
   move_detector(move_detector &&other) { other.moved_from_ = true; }
   move_detector(move_detector const &) = default;
   move_detector() = default;

   move_detector &operator =(move_detector &&other) {
      other.moved_from_ = true;
      return *this;
   }

   move_detector &operator =(move_detector const &) = default;

   bool was_moved_from() const { return moved_from_; }

 private:
   bool moved_from_ = false;
};


class cant_copy {
 public:
   cant_copy() = default;
   cant_copy(cant_copy &&) = default;
   cant_copy(cant_copy const &) = delete;
   cant_copy &operator =(cant_copy &&) = default;
   cant_copy &operator =(cant_copy const &) = delete;
};


class cant_move {
 public:
   cant_move() = default;
   cant_move(cant_move &&) = delete;
   cant_move(cant_move const &) = default;
   cant_move &operator =(cant_move &&) = delete;
   cant_move &operator =(cant_move const &) = default;
};


SCENARIO( "expected holds results and throws exceptions", "[expected]" ) {
   GIVEN( " An expected<int> result initialized with 5 " ) {
      ::posixpp::expected<int> const result{5};
      THEN(" result.result() == 5 ") {
         CHECK( result.result() == 5 );
      } THEN(" result.has_error() == false ") {
         CHECK( result.has_error() == false );
      } THEN(" result.throw_if_error() does nothing ") {
         CHECK_NOTHROW(result.throw_if_error());
      } THEN(" result.error() throws no_error_here ") {
         CHECK_THROWS_AS(result.error(), ::posixpp::no_error_here);
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
   /* This can't (and shouldn't) compile */
#if 0
   GIVEN("An expected<cant_move>.") {
      using expected_t = ::posixpp::expected<cant_move>;
      cant_move cm;
      expected_t result{cm};
   }
#endif
   GIVEN("An expected<move_detector> created from a const move_detector.") {
      using expected_t = ::posixpp::expected<move_detector>;
      move_detector const md;
      expected_t result{md};
      THEN(" original move_detector wasn't moved.") {
         CHECK_FALSE(md.was_moved_from());
      } THEN(" move_detector inside expected also wasn't moved.") {
         CHECK_FALSE(result.result().was_moved_from());
      } THEN(" and checking its status didn't move it.") {
         CHECK_FALSE(result.result().was_moved_from());
      } THEN(" but assigning it out does move it.") {
         move_detector newmd{result.result()};
         CHECK_FALSE(newmd.was_moved_from());
         CHECK(result.result().was_moved_from());
      }
   }
   GIVEN("A const expected<move_detector>.") {
      using expected_t = ::posixpp::expected<move_detector>;
      expected_t const result{move_detector{}};
      THEN( " contained move_detector wasn't moved.") {
         CHECK_FALSE(result.result().was_moved_from());
      }
      THEN( " copying the result out doesn't move it.") {
         move_detector tmp{result.result()};
         CHECK_FALSE(result.result().was_moved_from());
      }
      THEN( " returning it from a lambda also doesn't move it.") {
         auto result2 = [&result]() -> expected_t { return result; }();
         CHECK_FALSE(result.result().was_moved_from());
      }
   }
   GIVEN("An expected<cant_copy>") {
      using expected_t = ::posixpp::expected<cant_copy>;
      expected_t result{cant_copy{}};
      THEN(" you can still get the result out.") {
         // This is just a test to see if the code compiles.
         auto tmp = result.result();
      }
   }
}
