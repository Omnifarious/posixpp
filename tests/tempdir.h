#pragma once  // -*- c++ -*-

#include <string>
#include <string_view>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <exception>
#include <iostream>

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
         ::std::clog <<
            ::fmt::format("dir_template == \"{}\", dirname_ == \"{}\"\n",
                          dir_template, dirname_);
      } else {
         throw ::std::system_error(errno, ::std::system_category(),
                                   "mkdtemp");
      }
   }
   void dont_remove() { keep_ = true; }
   ~tempdir() {
      if (!keep_ && (::std::uncaught_exceptions() == 0)) {
         auto rmcmd = "rm -rf " + dirname_.string();
         ::system(rmcmd.c_str());
      }
   }
   ::std::filesystem::path const &get_name() { return dirname_; }

 private:
   ::std::filesystem::path dirname_;
   bool keep_ = false;
};
