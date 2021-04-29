#pragma once // -*- c++ -*-

#include <cstdint>
#include <syscalls/linux/syscall.h>

namespace syscalls {
namespace linux {

inline expected_t open(char const *path, int flags, int mode) noexcept
{
   return syscall_expected(call_id::open, path, flags, mode);
}

inline expected_t openat(int fd, char const *path, int flags, int mode) noexcept
{
   return syscall_expected(call_id::openat, fd, path, flags, mode);
}

inline expected_t read(int fd, char *data, ::std::int64_t size) noexcept
{
   return syscall_expected(call_id::read, fd, data, size);
}

inline expected_t write(int fd, char const *data, ::std::int64_t size) noexcept
{
   return syscall_expected(call_id::write, fd, data, size);
}

inline ::posixpp::expected<void> close(int fd) noexcept
{
   return error_cascade_void(syscall_expected(call_id::close, fd));
}

inline expected_t dup(int oldfd) noexcept
{
   return syscall_expected(call_id::dup, oldfd);
}

inline expected_t dup2(int oldfd, int newfd) noexcept
{
   return syscall_expected(call_id::dup2, oldfd, newfd);
}

inline expected_t dup3(int oldfd, int newfd, int flags) noexcept
{
   return syscall_expected(call_id::dup3, oldfd, newfd, flags);
}

}
}
