#pragma once // -*- c++ -*-

#include <cstdint>
#include <syscalls/linux/syscall.h>

namespace syscalls {
namespace linux {

inline expected_t open(char const *path, int flags, int mode)
{
   return syscall_expected(call_id::open, path, flags, mode);
}

inline expected_t openat(int fd, char const *path, int flags, int mode)
{
   return syscall_expected(call_id::openat, fd, path, flags, mode);
}

inline expected_t read(int fd, char *data, ::std::int64_t size)
{
   return syscall_expected(call_id::read, fd, data, size);
}

inline expected_t write(int fd, char const *data, ::std::int64_t size)
{
   return syscall_expected(call_id::write, fd, data, size);
}

inline expected_t close(int fd)
{
   return syscall_expected(call_id::close, fd);
}

}
}
