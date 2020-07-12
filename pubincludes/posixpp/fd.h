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
      return error_cascade(open(pathname, flags, 0));
   }
   static expected<fd> open(char const *pathname, int flags, ::mode_t mode) {
      using ::syscalls::linux::open;
      return error_cascade(open(pathname, flags, mode));
   }

   expected<::std::size_t> write(char const *buf, ::std::size_t size) {
      return error_cascade_size(::syscalls::linux::write(fd_, buf, size));
   }

   expected<::std::size_t> read(char *buf, ::std::size_t size) {
      return error_cascade_size(::syscalls::linux::read(fd_, buf, size));
   }

 private:
   int fd_;

   static expected<fd>
   error_cascade(::syscalls::linux::expected_t &&result) {
      if (result.has_error()) {
         return expected<fd>{expected<fd>::err_tag{}, result.error()};
      } else {
         auto fdval = ::std::move(result.result());
         return expected<fd>{fd{static_cast<int>(fdval)}};
      }
   }

   static expected<::std::size_t>
   error_cascade_size(::syscalls::linux::expected_t &&result) {
      using cascade_t = expected<::std::size_t>;
      if (result.has_error()) {
         return cascade_t{cascade_t::err_tag{}, result.error()};
      } else {
         return cascade_t{static_cast<::std::size_t>(result.result())};
      }
   }
};

} // namespace posixpp
