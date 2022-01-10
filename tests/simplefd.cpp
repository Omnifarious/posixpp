#include <posixpp/fd.h>
#include <posixpp/expected.h>
#include <posixpp/simpleio.h>
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
      using ::posixpp::open;
      auto foo{
         open(
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
            auto const write_result = write(foo, msg, sizeof(msg) - 1);
            REQUIRE(write_result.result() == sizeof(msg) - 1);
            auto const close_result = foo.close();
            REQUIRE_FALSE(close_result.has_error());
            REQUIRE_FALSE(foo.is_valid());

            foo = open(fooname.native().c_str(), fdf::rdonly).result();
            REQUIRE(foo.is_valid());

            // Try to read more than was written to make sure we can't read
            // extra (as per the description in the WHEN clause above).
            auto read_result = read(foo, readmsg, sizeof(readmsg));
            REQUIRE(read_result.result() == sizeof(readmsg) - 1);
            REQUIRE(::std::equal(msg, msg + (sizeof(msg) - 1), readmsg));
         }
      }
   }
}

SCENARIO(
     "File descriptor objects have dup and dup2 methods that work as expected."
) {
   GIVEN(
        "A new opened file foo in a new temporary directory containing "
        "some known text."
   ) {
      tempdir testdir;
      static const char known_text[] = "This is just some known text.";
      auto fooname = testdir.get_name() / "foo";
      using ::posixpp::fd;
      using of = ::posixpp::openflags;
      using fdf = ::posixpp::fdflags;
      using ::posixpp::modeflags;
      using ::posixpp::open;
      auto foo{
           open(
                fooname.native().c_str(),
                of::creat | fdf::wronly,
                modeflags::irwall
           ).result()
      };
      // Don't write out the trailing '\0';
      REQUIRE(write(foo, known_text, sizeof(known_text) - 1).result() == sizeof(known_text) - 1);
      foo.close().throw_if_error();
      foo = open(fooname.native().c_str(), fdf::rdonly).result();
      WHEN("The foo.dup() is called.") {
         fd bar{ foo.dup().result() };

         THEN("It results in a new valid file descriptor.") {
            REQUIRE(bar.is_valid());
            REQUIRE(foo.as_fd() != bar.as_fd());
         }
         AND_WHEN("You read one character from it.") {
            char buf[1];
            REQUIRE(read(bar, buf, 1).result() == 1);
            THEN("you read the first character of the known text.") {
               REQUIRE(buf[0] == known_text[0]);
            }
            AND_WHEN("you then read one character from the original fd.") {
               char buf2[1];
               REQUIRE(read(foo, buf2, 1).result() == 1);
               THEN("that character is the second character of the known text.") {
                  REQUIRE(buf2[0] == known_text[1]);
               }
            }
         }
      }
      WHEN("foo.dup2(foo) is called.") {
         REQUIRE_NOTHROW(foo.dup2(foo).throw_if_error());
         THEN("foo is still valid") {
            REQUIRE(foo.is_valid());
         }
         THEN( "a byte read from foo is the first byte of known_text") {
            char buf_foo[1];
            REQUIRE(read(foo, buf_foo, 1).result() == 1);
            REQUIRE(buf_foo[0] == known_text[0]);
         }
      }
      AND_GIVEN("Another file descriptor opened on the same file.") {
         auto foo2{open(fooname.native().c_str(), fdf::rdonly).result()};
         THEN("It is valid.") {
            REQUIRE(foo2.is_valid());
         }
         WHEN(
                 "One byte is read from each"
         ) {
            // Initialize to 0 to make sure they change when read into.
            char buf_foo[1] = {};
            char buf_foo2[1] = {};
            REQUIRE(read(foo, buf_foo, 1).result() == 1);
            REQUIRE(read(foo2, buf_foo2, 1).result() == 1);
            THEN(
                    "it's the same byte, and the  first byte of the known "
                    "text, demonstrating that they each refer to a different "
                    "underlying file descriptor."
            ) {
               REQUIRE(buf_foo2[0] == known_text[0]);
               REQUIRE(buf_foo[0] == buf_foo2[0]);
            } AND_WHEN("we dup2 the first fd to the second fd") {
               foo.dup2(foo2).throw_if_error();
               THEN("foo2 is still valid, and can be closed") {
                  REQUIRE(foo2.is_valid());
                  REQUIRE_NOTHROW(foo2.close().throw_if_error());
               } AND_WHEN ("we then read read a byte from each file.") {
                  char buff2_foo[1];
                  char buf2_foo2[1];
                  REQUIRE(read(foo, buff2_foo, 1).result() == 1);
                  REQUIRE(read(foo2, buf2_foo2, 1).result() == 1);
                  THEN(
                          "then we get the 2nd and 3rd bytes of known "
                          "text, demonstrating they now refer to the same "
                          "underlying file descriptor."
                  ) {
                     REQUIRE(buff2_foo[0] == known_text[1]);
                     REQUIRE(buf2_foo2[0] == known_text[2]);
                  }
               }
            }
         }
      }
   }
}
