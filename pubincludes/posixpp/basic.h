// Copyright 2021 Eric Hopper
// Distributed under the terms of the LGPLv3.

#pragma once

#include <syscalls/linux/basic.h>

namespace posixpp {

inline void exit [[noreturn]] (int status)
{
   ::syscalls::linux::exit_group(status);
}

inline void exit_thread [[noreturn]] (int status)
{
   ::syscalls::linux::exit(status);
}

}
