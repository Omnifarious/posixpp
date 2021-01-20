#pragma once  /*-*-c++-*-*/

#include <cstdint>
#include <concepts>

// Copyright 2021 by Eric Hopper
// Can be distributed under the terms of the LGPL v3.

namespace pppbase {

//! A set of boolean flags, may not be modified after creation.
class flagset_base {
 protected:
   using bitvec_t = ::std::uintmax_t;

 public:
   constexpr explicit flagset_base(bitvec_t bits = 0)
        : bits_{bits}
   {}

   constexpr explicit operator bool() const { return bits_; }

   [[nodiscard]] constexpr
   flagset_base intersection(flagset_base const &b) const {
      return flagset_base{bits_ & b.bits_};
   }
   [[nodiscard]] constexpr
   flagset_base setunion(flagset_base const &b) const {
      return flagset_base{bits_ | b.bits_};
   }
   [[nodiscard]] constexpr
   flagset_base symmetric_difference(flagset_base const &b) const {
      return flagset_base{bits_ ^ b.bits_};
   }
   [[nodiscard]] constexpr
   flagset_base minus(flagset_base const &b) const {
      return flagset_base{bits_ & ~b.bits_};
   }

   [[nodiscard]] constexpr
   bool is_equal(flagset_base const &b) const {
      return bits_ == b.bits_;
   }

   [[nodiscard]] constexpr
   bitvec_t getbits() const { return bits_; }

 private:
   bitvec_t bits_;
};


template <typename T>
concept supports_flagset = requires(T const &x) {
   bool{x};
   {x.intersection(x)} -> ::std::same_as<T>;
   {x.setunion(x)} -> ::std::same_as<T>;
   {x.symmetric_difference(x)} -> ::std::same_as<T>;
   {x.minus(x)} -> ::std::same_as<T>;
   {x.is_equal(x)} -> ::std::same_as<bool>;
};


template <typename Derived>
class specific_flagset_crtp : private flagset_base {
 protected:
   using flagset_base::bitvec_t;
   using flagset_base::getbits;
   constexpr explicit specific_flagset_crtp(bitvec_t bits=0)
        : flagset_base(bits) {
   }

 public:
   using flagset_base::operator bool;

   [[nodiscard]] constexpr
   Derived intersection(Derived const &b) const {
      return Derived{flagset_base::intersection(b).getbits()};
   }
   [[nodiscard]] constexpr
   Derived setunion(Derived const &b) const {
      return Derived{flagset_base::setunion(b).getbits()};
   }
   [[nodiscard]] constexpr
   Derived symmetric_difference(Derived const &b) const {
      return Derived{flagset_base::symmetric_difference(b).getbits()};
   }
   [[nodiscard]] constexpr
   Derived minus(Derived const &b) const {
      return Derived{flagset_base::minus(b).getbits()};
   }

   [[nodiscard]] constexpr
   bool is_equal(Derived const &b) const {
      return flagset_base::is_equal(b);
   }
};

template <supports_flagset T> constexpr
bool operator ==(T const &a, T const &b)
{
   return a.is_equal(b);
}

template <supports_flagset T> constexpr
bool operator !=(T const &a, T const &b)
{
   return !a.is_equal(b);
}

template <supports_flagset T> constexpr
T operator &(T const &a, T const &b) {
   return a.intersection(b);
}

template <supports_flagset T> constexpr
T operator |(T const &a, T const &b) {
   return a.setunion(b);
}

template <supports_flagset T> constexpr
T operator ^(T const &a, T const &b) {
   return a.symmetric_difference(b);
}

template <supports_flagset T> constexpr
T operator -(T const &a, T const &b) {
   return a.minus(b);
}

} // namespace pppbase
