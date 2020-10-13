#pragma once  /*-*-c++-*-*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <posixpp/expected.h>
#include <syscalls/linux/simple_io.h>

namespace posixpp {

class fd {
 public:
   // Usually, you shouldn't use this.
   explicit fd(int fdval) : fd_(fdval) {}
   ~fd() {
      using ::syscalls::linux::close;
      if (fd_ >= 0) {
         close(fd_);
      }
   }
   fd(fd &&other) : fd_(other.fd_) { other.fd_ = -1; }
   fd &operator =(fd &&other) {
      using ::syscalls::linux::close;
      if (fd_ >= 0) {
         close(fd_);
      }
      fd_ = other.fd_;
      other.fd_ = -1;
      return *this;
   }

   int as_fd() const { return fd_; }

   static expected<fd> open(char const *pathname, int flags) {
      using ::syscalls::linux::open;
      using posixpp::error_cascade;
      return error_cascade(open(pathname, flags, 0),
                           [](auto r) { return fd{static_cast<int>(r)}; });
   }
   static expected<fd> open(char const *pathname, int flags, ::mode_t mode) {
      using ::syscalls::linux::open;
      using posixpp::error_cascade;
      return error_cascade(open(pathname, flags, mode),
                           [](auto r) { return fd{static_cast<int>(r)}; });
   }

   expected<::std::size_t> write(char const *buf, ::std::size_t size) {
      using posixpp::error_cascade;
      return error_cascade(::syscalls::linux::write(fd_, buf, size),
                           [](auto r) { return static_cast<::std::size_t>(r);});
   }

   expected<::std::size_t> read(char *buf, ::std::size_t size) {
      using posixpp::error_cascade;
      return error_cascade(::syscalls::linux::read(fd_, buf, size),
                           [](auto r) { return static_cast<::std::size_t>(r);});
   }

 private:
   int fd_;
};

} // namespace posixpp
