// Copyright 2021 by Eric Hopper
// Distributed under the terms of the LGPLv3.
#pragma once

#include <cstdint>
#include <syscalls/linux/syscall.h>

namespace syscalls::linux {

inline void exit [[noreturn]](int status) noexcept {
   syscall_expected(call_id::exit, status);
   __builtin_unreachable();
}

inline void exit_group [[noreturn]](int status) noexcept {
   syscall_expected(call_id::exit_group, status);
   __builtin_unreachable();
}

} // namespace syscalls::linux
