#include <posixpp/fd.h>
#include <posixpp/expected.h>
#include <fmt/core.h>

/*
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <filesystem>
#include <cstdlib>

SECTION( "error holds results and throws exceptions", "[error]" ) {
   GIVEN( "An empty temporary directory." ) {
   }
}
*/
int main()
{
   using ::fmt::print;
   auto result = posixpp::fd::open("foo", O_WRONLY|O_CREAT|O_EXCL, 0666);
//   if (result.has_error()) {
 //     print("Ooopsy!\n");
  // } else {
      auto myfd = result.result();
      print("myfd.as_fd() == {:d}\n", myfd.as_fd());
      myfd.write("Stuff\n", 6);
   //}
   return 0;
}
