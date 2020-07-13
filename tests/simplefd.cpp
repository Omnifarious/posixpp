#include <posixpp/fd.h>
#include <posixpp/expected.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <filesystem>
#include <cstdlib>
#include <string>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

template <>
struct fmt::formatter<::std::filesystem::path>
        : fmt::formatter<::std::string_view>
{
   template <typename FormatContext>
   auto format(::std::filesystem::path const &p, FormatContext &ctx) {
      ::std::string path = p.string();
      return ::fmt::formatter<::std::string_view>::format(path, ctx);
   }
};

class tempdir {
 public:
   tempdir() {
      using ::std::string;
      namespace fs = ::std::filesystem;
      auto const tempdir = fs::temp_directory_path();
      auto dir_template = (tempdir / "posixpp_tests_XXXXXX").native();

      if (auto result = mkdtemp(dir_template.data())) {
         dirname_ = dir_template;
         ::fmt::print("dir_template == \"{}\", dirname_ == \"{}\"\n",
                      dir_template, dirname_);
      } else {
         throw ::std::system_error(errno, ::std::system_category(),
                                   "mkdtemp");
      }
   }
   ::std::filesystem::path const &get_name() { return dirname_; }

 private:
   ::std::filesystem::path dirname_;
};

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