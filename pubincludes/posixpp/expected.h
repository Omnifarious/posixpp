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

   struct err_t {
      int errval;

      err_t(int e) : errval(e) {}
      operator int() const { return errval; }
   };  // Just a type to serve as a tag to indicate error value.
};
}

//! A value that may be an error, throws if accessed and is an error.
template <typename T>
class expected : private priv_::expected_base {
 public:
   // Just a type to serve as a tag to indicate error value.
   using priv_::expected_base::err_tag;
   using result_t = T;

   explicit expected(T const &val) requires ::std::copyable<T> : val_{val}
   {}
   explicit expected(T &&val) requires ::std::movable<T>
           : val_{::std::move(val)}
   {}
   explicit expected(err_tag const &, int ec)
   requires ::std::movable<T> || ::std::copyable<T>
           : val_{err_t{ec}}
   {}

   [[nodiscard]] T &result() requires ::std::movable<T> {
      if (auto result = ::std::get_if<T>(&val_)) {
         return *result;
      } else {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(::std::get<err_t>(val_), cat);
      }
   }

   [[nodiscard]] T const &result() const requires ::std::copyable<T> {
      if (auto result = ::std::get_if<T>(&val_)) {
         return *result;
      } else {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(::std::get<err_t>(val_), cat);
      }
   }

   void throw_if_error() const {
      if (auto error = ::std::get_if<err_t>(&val_)) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(*error, cat);
      }
   }

   [[nodiscard]] bool has_error() const noexcept {
      return ::std::holds_alternative<err_t>(val_);
   }

   [[nodiscard]] int error() const {
      return ::std::get<err_t>(val_);
   }

   [[nodiscard]] ::std::error_condition error_condition() const {
      return ::std::system_category().default_error_condition(error());
   }

 private:
   ::std::variant<T, err_t> val_;
};

//! A value that may be an error, throws if accessed and is an error.
template <>
class expected<void> : private priv_::expected_base {
 public:
   using priv_::expected_base::err_tag;
   using result_t = void;

   expected() : err_(err_t{0}) {}
   explicit expected(err_tag const &, int ec) noexcept
           : err_{err_t{ec}}
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
   err_t err_;
};

//! Call converter with result, or cascade error upward.
///
/// \param result an rvalue result of a call that may return an error.
/// \param converter a callable that must always succeed and have a simple
///        return value.
template <typename T, typename Converter>
requires ((
                  ::std::invocable<Converter, T const &> &&
                  ::std::copyable<T>
          ) || (
                  ::std::invocable<Converter, T &&> &&
                  ::std::movable<T>
          ))
auto error_cascade(expected<T> &&result, Converter converter)
-> expected<decltype(converter(::std::declval<T>()))>
{
   using ::std::move;
   using outresult_t =
           expected<decltype(converter(::std::declval<T>()))>;
   using errtag = typename outresult_t::err_tag;
   if (result.has_error()) {
      return outresult_t(errtag{}, result.error());
   } else {
      if constexpr (::std::is_move_constructible_v<T>) {
         return outresult_t{converter(move(result.result()))};
      } else {
         return outresult_t{converter(result.result())};
      }
   }
}

//! Throw away return value, but cascade error upward.
template <typename T>
expected<void> error_cascade_void(expected<T> &&result)
{
   using outresult_t = expected<void>;
   using errtag = typename outresult_t::err_tag;
   if (result.has_error()) {
      return outresult_t{errtag{}, result.error()};
   } else {
      return outresult_t{};
   }
}


} // namespace posixpp
