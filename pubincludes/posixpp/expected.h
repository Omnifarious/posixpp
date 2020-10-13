#pragma once // -*- c++ -*-

#include <system_error>
#include <utility>
#include <stdexcept>
#include <variant>
#include <concepts>

namespace posixpp {

namespace priv_ {
class expected_base {
 public:
   struct err_tag {};  // Just a type to serve as a tag to indicate error value.

   struct err_type {
      int errval;

      err_type(int e) : errval(e) {}
      operator int() const { return errval; }
   };  // Just a type to serve as a tag to indicate error value.
};
}

template <typename T>
class expected : private priv_::expected_base {
 public:
   // Just a type to serve as a tag to indicate error value.
   using priv_::expected_base::err_tag;

   explicit expected(T const &val) requires ::std::copyable<T> : val_{val}
   {}
   explicit expected(T &&val) requires ::std::movable<T>
           : val_{::std::move(val)}
   {}
   explicit expected(err_tag const &, int ec)
   requires ::std::movable<T> || ::std::copyable<T>
           : val_{err_type{ec}}
   {}

   [[nodiscard]] T &&result() requires ::std::movable<T> {
      if (auto result = ::std::get_if<T>(&val_)) {
         return ::std::move(*result);
      } else {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(::std::get<err_type>(val_), cat);
      }
   }

   [[nodiscard]] T const &result() const  requires ::std::copyable<T> {
      if (auto result = ::std::get_if<T>(&val_)) {
         return *result;
      } else {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(::std::get<err_type>(val_), cat);
      }
   }

   void throw_if_error() const {
      if (auto error = ::std::get_if<err_type>(&val_)) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(*error, cat);
      }
   }

   [[nodiscard]] bool has_error() const noexcept {
      return ::std::holds_alternative<err_type>(val_);
   }

   [[nodiscard]] int error() const {
      return ::std::get<err_type>(val_);
   }

   [[nodiscard]] ::std::error_condition error_condition() const {
      return ::std::system_category().default_error_condition(error());
   }

 private:
   ::std::variant<T, err_type> val_;
};

template <>
class expected<void> : private priv_::expected_base {
 public:
   expected() : err_(err_type{0}) {}
   explicit expected(err_tag const &, int ec) noexcept
           : err_{err_type{ec}}
   {}

   void result() const {
      throw_if_error();
   }

   void throw_if_error() const {
      if (err_ != 0) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(err_, cat);
      }
   }

   [[nodiscard]] bool has_error() const noexcept {
      return err_ != 0;
   }

   [[nodiscard]] int error() const noexcept {
      return err_;
   }

   [[nodiscard]] ::std::error_condition error_condition() const noexcept {
      return ::std::system_category().default_error_condition(error());
   }

 private:
   err_type err_;
};

}
