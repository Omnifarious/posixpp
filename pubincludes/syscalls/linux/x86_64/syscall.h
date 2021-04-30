#pragma once  // -*- c++ -*-

#include <cstdint>
#include <utility>
#include <posixpp/expected.h>

namespace syscalls {
namespace linux {
namespace x86_64 {

enum class call_id : ::std::uint16_t;
using val_t = ::std::int64_t;

struct syscall_param {
   syscall_param(val_t v) noexcept : value(v) { } // NOLINT
   // NOLINTNEXTLINE
   syscall_param(void *v) noexcept : value(reinterpret_cast<val_t>(v)) {
      static_assert(sizeof(void *) == sizeof(val_t));
   }
   // NOLINTNEXTLINE
   syscall_param(void const *v) noexcept : value(reinterpret_cast<val_t>(v)) {
      static_assert(sizeof(void *) == sizeof(val_t));
   }
   val_t value;
};

inline val_t do_syscall(call_id callnum) noexcept
{
   val_t retval;
   asm volatile (
      "syscall\n\t"
       :"=a"(retval)
       :"a"(static_cast<::std::uint64_t>(callnum))
       :"%rcx", "%r11", "memory"
      );
   return retval;
}

inline val_t do_syscall(call_id callnum,
                           syscall_param const &p1) noexcept
{
   val_t retval;
   asm volatile (
      "syscall\n\t"
       :"=a"(retval)
       :"a"(static_cast<::std::uint64_t>(callnum)), "D"(p1.value)
       :"%rcx", "%r11", "memory"
      );
   return retval;
}

inline val_t do_syscall(call_id callnum,
                           syscall_param const &p1,
                           syscall_param const &p2) noexcept
{
   val_t retval;
   asm volatile (
      "syscall\n\t"
       :"=a"(retval)
       :"a"(static_cast<::std::uint64_t>(callnum)), "D"(p1.value), "S"(p2.value)
       :"%rcx", "%r11", "memory"
      );
   return retval;
}

inline val_t do_syscall(call_id callnum,
                           syscall_param const &p1,
                           syscall_param const &p2,
                           syscall_param const &p3) noexcept
{
   val_t retval;
   asm volatile (
      "syscall\n\t"
       :"=a"(retval)
       :"a"(static_cast<::std::uint64_t>(callnum)), "D"(p1.value), "S"(p2.value), "d"(p3.value)
       :"%rcx", "%r11", "memory"
      );
   return retval;
}

inline val_t do_syscall(call_id callnum,
                           syscall_param const &p1,
                           syscall_param const &p2,
                           syscall_param const &p3,
                           syscall_param const &p4) noexcept
{
   val_t retval;
   register val_t rp4 asm ("r10") = p4.value;
   asm volatile (
      "syscall\n\t"
       :"=a"(retval)
      :"a"(static_cast<::std::uint64_t>(callnum)), "D"(p1.value), "S"(p2.value), "d"(p3.value), "r"(rp4)
       :"%rcx", "%r11", "memory"
      );
   return retval;
}

inline val_t do_syscall(call_id callnum,
                           syscall_param const &p1,
                           syscall_param const &p2,
                           syscall_param const &p3,
                           syscall_param const &p4,
                           syscall_param const &p5) noexcept
{
   val_t retval;
   register volatile val_t rp4 asm ("r10") = p4.value;
   register volatile val_t rp5 asm ("r8") = p5.value;
   asm volatile (
      "syscall\n\t"
      :"=a"(retval)
      :"a"(static_cast<::std::uint64_t>(callnum)), "D"(p1.value), "S"(p2.value), "d"(p3.value), "r"(rp4), "r"(rp5)
      :"%rcx", "%r11", "memory"
      );
   return retval;
}

inline val_t do_syscall(call_id callnum,
                           syscall_param const &p1,
                           syscall_param const &p2,
                           syscall_param const &p3,
                           syscall_param const &p4,
                           syscall_param const &p5,
                           syscall_param const &p6) noexcept
{
   val_t retval;
   register volatile val_t rp4 asm ("r10") = p4.value;
   register volatile val_t rp5 asm ("r8") = p5.value;
   register volatile val_t rp6 asm ("r9") = p6.value;
   asm volatile (
      "syscall\n\t"
      :"=a"(retval)
      :"a"(static_cast<::std::uint64_t>(callnum)), "D"(p1.value), "S"(p2.value), "d"(p3.value), "r"(rp4), "r"(rp5), "r"(rp6)
      :"%rcx", "%r11", "memory"
      );
   return retval;
}


using expected_t = ::posixpp::expected<val_t>;

template <typename... T>
expected_t
syscall_expected(call_id callnum, T &&... args) noexcept
{
   val_t result = do_syscall(callnum, ::std::forward<T>(args)...);

   if (result < 0) {
      return expected_t(expected_t::err_tag(), static_cast<int>(-result));
   } else {
      return expected_t(result);
   }
}


enum class call_id : ::std::uint16_t {
   read = 0,
   write,
   open,
   close,
   stat,
   fstat,
   lstat,
   poll,
   lseek,
   mmap,
   mprotect,
   munmap,
   brk,
   rt_sigaction,
   rt_sigprocmask,
   rt_sigreturn,
   ioctl,
   pread64,
   pwrite64,
   readv,
   writev,
   access,
   pip,
   select,
   sched_yield,
   mremap,
   msync,
   mincore,
   madvise,
   shmget,
   shmat,
   shmctl,
   dup,
   dup2,
   pause,
   nanosleep,
   getitimer,
   alarm,
   setitimer,
   getpid,
   sendfile,
   socket,
   connect,
   accept,
   sendto,
   recvfrom,
   sendmsg,
   recvmsg,
   shutdown,
   bind,
   listen,
   getsockname,
   getpeername,
   socketpair,
   getsockopt,
   setsockopt,
   clone,
   fork,
   vfork,
   execve,
   exit,
   wait4,
   kill,
   uname,
   semget,
   semop,
   semctl,
   shmdt,
   msgget,
   msgsnd,
   msgrcv,
   msgctl,
   fcntl,
   flock,
   fsync,
   fdatasync,
   truncate,
   ftruncate,
   getdents,
   getcwd,
   chdir,
   fchdir,
   rename,
   mkdir,
   rmdir,
   creat,
   link,
   unlink,
   symlink,
   readlink,
   chmod,
   fchmod,
   chown,
   fchown,
   lchown,
   umask,
   gettimeofday,
   getrlimit,
   getrusage,
   sysinfo,
   times,
   ptrace,
   getuid,
   syslog,
   getgid,
   setuid,
   setgid,
   geteuid,
   getegid,
   setpgid,
   getppid,
   getpgrp,
   setsid,
   setreuid,
   setregid,
   getgroups,
   setgroups,
   setresuid,
   getresuid,
   setresgid,
   getresgid,
   getpgid,
   setfsuid,
   setfsgid,
   getsid,
   capget,
   capset,
   rt_sigpending,
   rt_sigtimedwait,
   rt_sigqueueinfo,
   rt_sigsuspend,
   sigalstack,
   utime,
   mknod,
   uselib,
   personality,
   ustat,
   statfs,
   fstatfs,
   sysfs,
   getpriority,
   setpriority,
   sched_setparam,
   sched_getparam,
   sched_setscheduler,
   sched_getscheduler,
   sched_get_priority_max,
   sched_get_priority_min,
   sched_rr_get_interval,
   mlock,
   munlock,
   mlockall,
   munlockall,
   vhangup,
   modify_ldt,
   pivot_root,
   underscore_sysctl,
   prctl,
   arch_prctl,
   adjtimex,
   setrlimit,

   futex = 202,

   epoll_create = 213,
   epoll_ctl_old,
   epoll_wait_old,

   exit_group = 231,
   epoll_wait = 232,
   epoll_ctl,

   openat = 257,
   mkdirat,
   mknodat,
   fchownat,
   futimesat,
   newfstatat,
   unlinkat,
   renameat,
   linkat,
   symlinkat,
   readlinkat,
   fchmodat,
   faccessat,
   pselect6,
   ppoll,
   unshare,
   epoll_pwait = 281,
   epoll_create1 = 291,
   dup3 = 292,
   syncfs = 306,
   setns = 308
};

namespace priv_ {
inline void compiletime_tests()
{
   static_assert(static_cast<::std::uint16_t>(call_id::sendfile) == 40);
   static_assert(static_cast<::std::uint16_t>(call_id::chdir) == 80);
   static_assert(static_cast<::std::uint16_t>(call_id::getresgid) == 120);
   static_assert(static_cast<::std::uint16_t>(call_id::setrlimit) == 160);
   static_assert(static_cast<::std::uint16_t>(call_id::faccessat) == 269);
}
} // namespace priv_

} // namespace x86_64
} // namespace os_linux
} // namespace posixpp
