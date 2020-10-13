#include <posixpp/fd.h>
#include <posixpp/expected.h>
#include "tempdir.h"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

SCENARIO("File descriptor objects can be opened, read from, and written to.",
         "posixpp::fd")
{

   GIVEN("A new unique temporary directory, "
         "and a filename withing that directory.")
   {
      tempdir testdir;
      auto fooname = testdir.get_name() / "foo";

      WHEN("A file 'foo' is opened with O_CREAT in this directory.") {
         auto foo{
                 ::posixpp::fd::open(
                         fooname.string().c_str(),
                         O_CREAT | O_WRONLY, 0666).result()
         };

         THEN("foo.as_fd() is >= 0") {
            REQUIRE(foo.as_fd() >= 0);
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

#if 0
int main()
{
   using ::fmt::print;
   tempdir mydir;
//   auto result = posixpp::fd::open("foo", O_WRONLY|O_CREAT|O_EXCL, 0666);
//   if (result.has_error()) {
 //     print("Ooopsy!\n");
  // } else {
//      auto myfd = result.result();
//      print("myfd.as_fd() == {:d}\n", myfd.as_fd());
//      myfd.write("Stuff\n", 6);
   //}
   return 0;
}
#endif