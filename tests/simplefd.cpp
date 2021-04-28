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

   GIVEN("A new unique temporary directory, "
         "and a filename within that directory.")
   {
      tempdir testdir;
      auto fooname = testdir.get_name() / "foo";

      WHEN("A file 'foo' is opened with O_CREAT in this directory.") {
         using ::posixpp::fd;
         using ::posixpp::openflags;
         using ::posixpp::fdflags;
         using ::posixpp::modeflags;
         using ::std::move;
         auto foo{fd::open(
                     fooname.string().c_str(),
                     openflags::creat | fdflags::wronly, modeflags::irwall
                  ).result()
         };

         THEN("foo.is_valid()") {
            REQUIRE(foo.is_valid());
         } AND_THEN("The C access call shows the file existing "
                    "and being writeable")
         {
            REQUIRE(::access(fooname.string().c_str(), W_OK) == 0);
         } AND_THEN("foo.write(\"stuff\n\", 6) succeeds and returns 6") {
            REQUIRE(foo.write("stuff\n", 6).result() == 6);
         }
      }
   }
}
