#pragma once // -*- c++ -*-

#include <system_error>
#include <utility>
#include <stdexcept>

namespace posixpp {

namespace priv_ {
class expected_base {
 public:
   struct err_tag {};  // Just a type to serve as a tag to indicate error value.
};
}

template <class T>
class expected : private priv_::expected_base {
 public:
   // Just a type to serve as a tag to indicate error value.
   using priv_::expected_base::err_tag;

   explicit expected(T const &val) : is_err_(false)
   {
      new(&val_.val) T(val);
   }
   explicit expected(T &&val) : is_err_(false)
   {
      new(&val_.val) T(::std::move(val));
   }
   explicit expected(err_tag const &, int ec) : is_err_(true)
   {
      val_.errval = ec;
   }
   ~expected()
   {
      if (is_err_) {
         val_.val.~T();
      }
   }

   [[nodiscard]] T &result() {  // Returns a reference so you can move from it.
      if (is_err_) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(val_.errval, cat);
      } else {
         return val_.val;
      }
   }

   void throw_if_error() const {
      if (is_err_) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(val_.errval, cat);
      }
   }

   [[nodiscard]] bool has_error() const { return is_err_; }

   [[nodiscard]] int error() const {
      if (!is_err_) {
         throw ::std::runtime_error("Attempt to get non-existent error.");
      } else {
         return val_.errval;
      }
   }

   [[nodiscard]] ::std::error_condition error_condition() const {
      return ::std::system_category().default_error_condition(error());
   }

 private:
   union {
      T val;
      int errval;
   } val_;
   bool const is_err_;
};

}
