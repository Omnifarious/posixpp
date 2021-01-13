#pragma once  /*-*-c++-*-*/

#include <cstdint>

// Copyright 2021 by Eric Hopper
// Can be distributed under the terms of the LGPL v3.

namespace pppbase {

//! A set of boolean flags, may not be modified after creation.
class flagset {
 public:
   explicit constexpr flagset(::std::uintmax_t bits = 0)
        : bits_{bits}
   {}

   constexpr explicit operator bool() const { return bits_; }

   constexpr
   flagset intersection(flagset const &b) const {
      return flagset{bits_ & b.bits_};
   }
   constexpr
   flagset setunion(flagset const &b) const {
      return flagset{bits_ | b.bits_};
   }
   constexpr
   flagset symmetric_difference(flagset const &b) const {
      return flagset{bits_ ^ b.bits_};
   }
   constexpr
   flagset minus(flagset const &b) const {
      return flagset{bits_ & ~b.bits_};
   }

   constexpr
   bool is_equal(flagset const &b) const {
      return bits_ == b.bits_;
   }

 private:
   ::std::uintmax_t bits_;
};

constexpr
bool operator ==(flagset const &a, flagset const &b)
{
   return a.is_equal(b);
}

constexpr
bool operator !=(flagset const &a, flagset const &b)
{
   return !a.is_equal(b);
}

constexpr
flagset operator &(flagset const &a, flagset const &b) {
   return a.intersection(b);
}

constexpr
flagset operator |(flagset const &a, flagset const &b) {
   return a.setunion(b);
}

constexpr
flagset operator ^(flagset const &a, flagset const &b) {
   return a.symmetric_difference(b);
}

constexpr
flagset operator -(flagset const &a, flagset const &b) {
   return a.minus(b);
}

} // namespace pppbase
