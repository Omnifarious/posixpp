#pragma once  /*-*-c++-*-*/

// Copyright 2021 by Eric Hopper
// Can be distributed under the terms of the LGPL v3.

#include <pppbase/flagset.h>

namespace syscalls::linux::x86_64 {

class fdflags;
class openflags;


/** Flags associated with an open file descriptor. */
class fdflags :  public pppbase::specific_flagset_crtp<fdflags> {
 private:
   using base_t = pppbase::specific_flagset_crtp<fdflags>;
   friend base_t;

 public:
   friend class openflags;
   //! Default empty set
   constexpr fdflags() : base_t{0} {}

   static const fdflags accmode;
   static const fdflags tmpfile;
   static const fdflags ndelay;
   static const fdflags async;
   static const fdflags fsync;
   static const fdflags nonblock;
   static const fdflags append;
   static const fdflags rsync;
   static const fdflags dsync;
   static const fdflags rdwr;
   static const fdflags directory;
   static const fdflags noatime;
   static const fdflags cloexec;
   static const fdflags sync;
   static const fdflags direct;
   static const fdflags largefile;
   static const fdflags rdonly;
   static const fdflags wronly;
   static const fdflags nofollow;
   static const fdflags path;

   //! Avoid using this function, it's awkward for a reason.
   static constexpr
   fdflags create_from_int(bitvec_t val) { return fdflags{val}; }

   using base_t::getbits;

 protected:
   explicit constexpr fdflags(bitvec_t val) : base_t(val) {}
};


/** Flags that can only be specified when a file is opened.
 *
 * Since the flags associated with a file descriptor can also be specified on
 * open, these flags can intermingle with those, but not the other way around.
 */
class openflags : public pppbase::specific_flagset_crtp<openflags> {
 private:
   using base_t = pppbase::specific_flagset_crtp<openflags>;
   friend base_t;

 public:
   //! Default empty set.
   constexpr openflags() : base_t{0} {}
   //! All fdflags are also openflags, but not all openflags are fdflags.
   explicit constexpr openflags(fdflags const &val) : base_t(val.getbits()) {}

   static const openflags creat;  //!< O_CREAT
   static const openflags excl;   //!< O_EXCL
   static const openflags noctty; //!< O_NOCTTY
   static const openflags trunc;  //!< O_TRUNC

   //! Avoid using this function, it's awkward for a reason.
   static constexpr
   openflags create_from_int(bitvec_t val) { return openflags{val}; }

   using base_t::getbits;

 protected:
   explicit constexpr openflags(bitvec_t val) : base_t(val) {}
};


constexpr
bool operator ==(openflags const &a, fdflags const &b)
{
   return a == openflags{b};
}

constexpr
bool operator ==(fdflags const &a, openflags const &b)
{
   return b == a;
}

constexpr
bool operator !=(openflags const &a, fdflags const &b)
{
   return a != openflags{b};
}

constexpr
bool operator !=(fdflags const &a, openflags const &b)
{
   return b != a;
}

constexpr
openflags operator |(openflags const &a, fdflags const &b)
{
   return a | openflags{b};
}

constexpr
openflags operator |(fdflags const &a, openflags const &b)
{
   return b | a;
}

constexpr
openflags operator &(openflags const &a, fdflags const &b)
{
   return a & openflags{b};
}

constexpr
openflags operator &(fdflags const &a, openflags const &b)
{
   return b & a;
}

constexpr
openflags operator ^(openflags const &a, fdflags const &b)
{
   return a ^ openflags{b};
}

constexpr
openflags operator ^(fdflags const &a, openflags const &b)
{
   return b ^ a;
}

constexpr
openflags operator -(openflags const &a, fdflags const &b)
{
   return a - openflags{b};
}

constexpr
fdflags operator -(fdflags const &a, openflags const &b)
{
   // Just in case someone cheated. Normally it's impossible for any openflags
   // bits to be set in an fdflags and it would be acceptable to return the
   // first argument.
   ::pppbase::supports_flagset auto const tmp{openflags{a} - b};
   return fdflags::create_from_int(tmp.getbits());
}

constexpr const fdflags fdflags::accmode{0003};
constexpr const fdflags fdflags::tmpfile{(020000000ULL | 0200000ULL)};
constexpr const fdflags fdflags::ndelay{04000};
constexpr const fdflags fdflags::async{020000};
constexpr const fdflags fdflags::fsync{04010000};
constexpr const fdflags fdflags::nonblock{04000};
constexpr const fdflags fdflags::append{02000};
constexpr const fdflags fdflags::rsync{04010000};
constexpr const fdflags fdflags::dsync{010000};
constexpr const fdflags fdflags::rdwr{02};
constexpr const fdflags fdflags::directory{0200000};
constexpr const fdflags fdflags::noatime{01000000};
constexpr const fdflags fdflags::cloexec{02000000};
constexpr const fdflags fdflags::sync{04010000};
constexpr const fdflags fdflags::direct{040000};
constexpr const fdflags fdflags::largefile{0};
constexpr const fdflags fdflags::rdonly{00};
constexpr const fdflags fdflags::wronly{01};
constexpr const fdflags fdflags::nofollow{0400000};
constexpr const fdflags fdflags::path{010000000};

constexpr const openflags openflags::noctty{0400};
constexpr const openflags openflags::excl{0200};
constexpr const openflags openflags::creat{0100};
constexpr const openflags openflags::trunc{01000};

}
