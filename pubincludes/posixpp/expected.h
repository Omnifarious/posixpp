#pragma once // -*- c++ -*-

#include <system_error>
#include <utility>
#include <stdexcept>
#include <concepts>

namespace posixpp {

namespace priv_ {
class expected_base {
 public:
   struct err_tag {};  // Just a type to serve as a tag to indicate error value.
/*
   struct err_t {
      int errval;

      err_t(int e) : errval(e) {}
      operator int() const { return errval; }
   };  // Just a type to serve as a tag to indicate error value.
*/
};

}


class no_error_here : public ::std::exception
{
 public:
   no_error_here() noexcept { }

   char const *what() const noexcept override
   {
      return reason_;
   }

 private:
    no_error_here(char const *reason) noexcept : reason_(reason) { }

    // Must point to a string with static storage duration:
    char const *reason_ = "no error in expected when error requested";
};


//! A value that may be an error, throws if accessed and is an error.
template <typename T>
class expected : private priv_::expected_base {
 public:
   // Just a type to serve as a tag to indicate error value.
   using priv_::expected_base::err_tag;
   using result_t = T;

   explicit constexpr expected(T const &val) noexcept requires ::std::copyable<T>
           : val_{val}, has_error_{false}
   {}
   explicit constexpr expected(T &&val) noexcept requires ::std::movable<T>
           : val_{::std::move(val)}, has_error_{false}
   {}
   explicit constexpr expected(err_tag const &, int ec) noexcept
   requires ::std::movable<T> || ::std::copyable<T>
           : val_{.errcode_ = ec}, has_error_{true}
   {}
   constexpr ~expected() noexcept {
      if (!has_error_) {
         val_.value_.~T();
      }
   }

   [[nodiscard]] constexpr T &&result() requires ::std::movable<T> {
      if (!has_error_) {
         return ::std::move(val_.value_);
      } else {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(val_.errcode_, cat);
      }
   }

   [[nodiscard]] constexpr T const &result() const requires ::std::copyable<T> {
      if (!has_error_) {
         return val_.value_;
      } else {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(val_.errcode_, cat);
      }
   }

   void throw_if_error() const {
      if (has_error_) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(val_.errcode_, cat);
      }
   }

   [[nodiscard]] constexpr bool has_error() const noexcept {
      return has_error_;
   }

   [[nodiscard]] constexpr int error() const {
      if (has_error_) {
         return val_.errcode_;
      } else {
         throw no_error_here{};
      }
   }

   [[nodiscard]] ::std::error_condition error_condition() const {
      return ::std::system_category().default_error_condition(error());
   }

 private:
   union anonymous {
      T value_;
      int errcode_;

      ~anonymous() {} // Destruction handled by expected<T>
   } val_;
   bool has_error_;
};

//! A value that may be an error, throws if accessed and is an error.
template <>
class expected<void> : private priv_::expected_base {
 public:
   using priv_::expected_base::err_tag;
   using result_t = void;

   expected() : errcode_{0} {}
   constexpr explicit expected(int ec) noexcept
           : errcode_{ec}
   {}

   void result() const {
      throw_if_error();
   }

   constexpr void throw_if_error() const {
      if (errcode_ != 0) {
         auto const &cat = ::std::system_category();
         throw ::std::system_error(errcode_, cat);
      }
   }

   [[nodiscard]] constexpr bool has_error() const noexcept {
      return errcode_ != 0;
   }

   [[nodiscard]] constexpr int error() const {
      if (errcode_ == 0) {
         return errcode_;
      } else {
         throw no_error_here{};
      }
   }

   [[nodiscard]] ::std::error_condition error_condition() const noexcept {
      return ::std::system_category().default_error_condition(error());
   }

 private:
   int errcode_;
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
      return outresult_t{result.error()};
   } else {
      return outresult_t{};
   }
}


} // namespace posixpp
