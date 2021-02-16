#pragma once  /*-*-c++-*-*/

// Copyright 2021 by Eric Hopper
// Can be distributed under the terms of the LGPL v3.

#include <pppbase/flagset.h>

namespace syscalls::linux::x86_64 {

/** Permission bits (though some are about other file properties). */
class modeflags : public pppbase::specific_flagset_crtp<modeflags> {
 private:
   using base_t = pppbase::specific_flagset_crtp<modeflags>;
   friend base_t;

 public:
   static const modeflags iread;
   static const modeflags iwrite;
   static const modeflags iexec;

   static const modeflags irusr;
   static const modeflags iwusr;
   static const modeflags ixusr;
   static const modeflags irwxu;

   static const modeflags irgrp;
   static const modeflags iwgrp;
   static const modeflags ixgrp;
   static const modeflags irwxg;

   static const modeflags iroth;
   static const modeflags iwoth;
   static const modeflags ixoth;
   static const modeflags irwxo;

   static const modeflags irwall;
   static const modeflags irwxall;

   static const modeflags isuid;
   static const modeflags isgid;
   static const modeflags isvtx;

   static const modeflags ififo;
   static const modeflags ifchr;
   static const modeflags ifdir;
   static const modeflags ifblk;
   static const modeflags ifreg;
   static const modeflags iflnk;
   static const modeflags ifsock;
   static const modeflags ifmt;

   //! Avoid using this function, it's awkward for a reason.
   static constexpr
   modeflags create_from_int(bitvec_t val) { return modeflags{val}; }

   using base_t::getbits;

   constexpr modeflags() : base_t(0) {}

 protected:
   explicit constexpr modeflags(bitvec_t val) : base_t(val) {}
};

constexpr const modeflags modeflags::iread{0400};
constexpr const modeflags modeflags::iwrite{0200};
constexpr const modeflags modeflags::iexec{0100};
constexpr const modeflags modeflags::irusr{0400};
constexpr const modeflags modeflags::iwusr{0200};
constexpr const modeflags modeflags::ixusr{0100};
constexpr const modeflags modeflags::irwxu{(0400U | 0200U | 0100U)};
constexpr const modeflags modeflags::irgrp{(0400U >> 3U)};
constexpr const modeflags modeflags::iwgrp{(0200U >> 3U)};
constexpr const modeflags modeflags::ixgrp{(0100U >> 3U)};
constexpr const modeflags modeflags::irwxg{((0400U | 0200U | 0100U) >> 3U)};
constexpr const modeflags modeflags::iroth{((0400U >> 3U) >> 3U)};
constexpr const modeflags modeflags::iwoth{((0200U >> 3U) >> 3U)};
constexpr const modeflags modeflags::ixoth{((0100U >> 3U) >> 3U)};
constexpr const modeflags modeflags::irwxo{(((0400U | 0200U | 0100U) >> 3U) >> 3U)};
constexpr const modeflags modeflags::irwall{irusr|iwusr|irgrp|iwgrp|iroth|iwoth};
constexpr const modeflags modeflags::irwxall{irwxu|irwxg|irwxo};
constexpr const modeflags modeflags::isuid{04000};
constexpr const modeflags modeflags::isgid{02000};
constexpr const modeflags modeflags::isvtx{01000};
constexpr const modeflags modeflags::ififo{0010000};
constexpr const modeflags modeflags::ifchr{0020000};
constexpr const modeflags modeflags::ifdir{0040000};
constexpr const modeflags modeflags::ifblk{0060000};
constexpr const modeflags modeflags::ifreg{0100000};
constexpr const modeflags modeflags::iflnk{0120000};
constexpr const modeflags modeflags::ifsock{0140000};
constexpr const modeflags modeflags::ifmt{0170000};

} // namespace syscalls::linux::x86_64
