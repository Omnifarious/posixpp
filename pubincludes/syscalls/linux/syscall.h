#pragma once  // -*- c++ -*-

#ifndef __linux

#error "This file only supports Linux."

#endif

#ifdef __x86_64
#include <syscalls/linux/x86_64/syscall.h>

namespace syscalls {
namespace linux {

using x86_64::syscall_expected;
using x86_64::expected_t;
using x86_64::call_id;

}
}

#else

#error "Only the x86_64 architecture is currently supported."

#endif

