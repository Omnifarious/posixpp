#include <algorithm>
#include <posixpp/fd.h>
#include <posixpp/expected.h>
#include <fcntl.h>
#include <unistd.h>
#include "tempdir.h"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

SCENARIO("File descriptor objects can be opened, read from, and written to.",
         "posixpp::fd")
{

   GIVEN("A new file opened with creat and wronly in a new unique temporary directory.")
   {
      tempdir testdir;
      auto fooname = testdir.get_name() / "foo";
      using ::posixpp::fd;
      using of = ::posixpp::openflags;
      using fdf = ::posixpp::fdflags;
      using ::posixpp::modeflags;
      auto foo{
         fd::open(
              fooname.native().c_str(),
              of::creat | fdf::wronly,
              modeflags::irwall
         ).result()
      };

      THEN("foo is a valid file descriptor") {
         REQUIRE(foo.is_valid());
      }
      THEN(
              "The C access call shows the file "
              "existing and being readable and writable"
      ) {
         REQUIRE(::access(fooname.native().c_str(), R_OK | W_OK) == 0);
      }
      AND_GIVEN(
              "a small, constant sized message and a small similarly "
              "sized local buffer"
      ) {
         static const char msg[] = "stuff\n";
         char readmsg[sizeof(msg)];

         WHEN(
              "the small message is written to foo, foo is reopened for "
              "read, and then read into the small buffer, the data written is "
              "the same as the data read, and no more data can be read"
         ) {
            // sizeof(msg) - 1 to skip the trailing '\8'
            auto const write_result = foo.write(msg, sizeof(msg) - 1);
            REQUIRE(write_result.result() == sizeof(msg) - 1);
            auto const close_result = foo.close();
            REQUIRE_FALSE(close_result.has_error());
            REQUIRE_FALSE(foo.is_valid());

            foo = fd::open(fooname.native().c_str(), fdf::rdonly).result();
            REQUIRE(foo.is_valid());

            // Try to read more than was written to make sure we can't read
            // extra (as per the description in the WHEN clause above).
            auto read_result = foo.read(readmsg, sizeof(readmsg));
            REQUIRE(read_result.result() == sizeof(readmsg) - 1);
            REQUIRE(::std::equal(msg, msg + (sizeof(msg) - 1), readmsg));
         }
      }
   }
}
