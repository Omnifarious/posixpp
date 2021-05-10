// Copyright 2021 Eric Hopper
// Distributed under the terms of the LGPLv3.

#pragma once

#include <posixpp/fd.h>
#include <syscalls/linux/simple_io.h>

namespace posixpp {

//! See write(2)
expected<::std::size_t>
inline write(fd const &file, char const *buf, ::std::size_t size) noexcept {
   using posixpp::error_cascade;
   return error_cascade(::syscalls::linux::write(file.as_fd(), buf, size),
                        [](auto r) { return static_cast<::std::size_t>(r);});
}

//! See read(2)
expected<::std::size_t>
inline read(fd const &file, char *buf, ::std::size_t size) noexcept {
   using posixpp::error_cascade;
   return error_cascade(::syscalls::linux::read(file.as_fd(), buf, size),
                        [](auto r) { return static_cast<::std::size_t>(r);});
}

///@{
[[nodiscard]] expected<fd>
inline openat(fd const &dirfd, char const *pathname,
              openflags flags, modeflags mode) noexcept
{
   // What all the other versions turn into, so it has to be first.
   using ::syscalls::linux::openat;
   using posixpp::error_cascade;
   return error_cascade(openat(dirfd.as_fd(),
                               pathname,
                               flags.getbits(),
                               mode.getbits()),
                        [](int fdint){return fd{fdint};});
}

[[nodiscard]] expected<fd>
inline open(char const *pathname, openflags flags) noexcept {
   // TODO: Fix hard coded the value for AT_FDCWD
   return openat(fd(-100), pathname, flags, modeflags{});
}
[[nodiscard]] expected<fd>
inline open(char const *pathname, fdflags flags) noexcept {
   return open(pathname, openflags{flags});
}

[[nodiscard]] expected<fd>
inline open(char const *pathname, openflags flags, modeflags mode) noexcept
{
   // TODO: Fix hard coded the value for AT_FDCWD
   return openat(fd(-100), pathname, flags, mode);
}
[[nodiscard]] expected<fd>
inline open(char const *pathname, fdflags flags, modeflags mode) noexcept
{
   return open(pathname, openflags{flags}, mode);
}

[[nodiscard]] expected<fd>
inline openat(fd const &dirfd, char const *pathname, openflags flags) noexcept
{
   using ::syscalls::linux::openat;
   using posixpp::error_cascade;
   auto const dfd = dirfd.as_fd();
   return error_cascade(openat(dfd, pathname, flags.getbits(), 0),
                        [](int fdint){return fd{fdint};});
}

[[nodiscard]] expected<fd>
inline openat(fd const &dirfd, char const *pathname, fdflags flags) noexcept
{
   return openat(dirfd, pathname, openflags{flags});
}

[[nodiscard]] expected<fd>
inline openat(fd const &dirfd, char const *pathname,
       fdflags flags, modeflags mode) noexcept
{
   return openat(dirfd, pathname, openflags{flags}, mode);
}
///@}

}
