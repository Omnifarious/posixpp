#pragma once  /*-*-c++-*-*/

#include <posixpp/expected.h>
#include <posixpp/fdflags.h>
#include <posixpp/modeflags.h>
#include <syscalls/linux/simple_io.h>

namespace posixpp {

//! A file descriptor and the associated functions
class fd {
 public:
   //! Avoid using this to create a file descriptor directly from an integer
   explicit constexpr fd(int fdval) noexcept : fd_(fdval) {}

   //! \brief Will call `close` on a seemingly valid file descriptor and ignore
   //! the return value.
   //!
   //! If you care about the return value of `close`, use the `close` method of
   //! this class.
   ~fd() noexcept {
      using ::syscalls::linux::close;
      if (fd_ >= 0) {
         close(fd_); // Ignore any error return.
      }
   }

   fd(fd &&other) noexcept : fd_(other.fd_) { other.fd_ = -1; }

   fd &operator =(fd &&other) noexcept {
      using ::syscalls::linux::close;
      if (fd_ >= 0) {
         close(fd_); // Ignore any error return.
      }
      fd_ = other.fd_;
      other.fd_ = -1;
      return *this;
   }

   [[nodiscard]] constexpr int as_fd() const noexcept { return fd_; }

   [[nodiscard]] expected<fd> dup() noexcept {
      using ::syscalls::linux::dup;
      return error_cascade(dup(fd_), int_to_fd);
   }

   [[nodiscard]] expected<void> dup2(fd &newfd) noexcept {
      using ::syscalls::linux::dup2;
      return error_cascade_void(dup2(fd_, newfd.as_fd()));
   }

   [[nodiscard]] expected<void> close() noexcept {
      using ::syscalls::linux::close;
      auto const tmpfd = fd_;
      fd_ = -1;
      return close(tmpfd);
   }



   ///@{
   [[nodiscard]] static expected<fd>
   open(char const *pathname, openflags flags) noexcept {
      // Hard coded the value for AT_FDCWD
      return openat(fd(-100), pathname, flags, modeflags{});
   }
   [[nodiscard]] static expected<fd>
   open(char const *pathname, fdflags flags) noexcept {
      return open(pathname, openflags{flags});
   }

   [[nodiscard]] static expected<fd>
   open(char const *pathname, openflags flags, modeflags mode) noexcept
   {
      return openat(fd(-100), pathname, flags, mode);
   }
   [[nodiscard]] static expected<fd>
   open(char const *pathname, fdflags flags, modeflags mode) noexcept
   {
      return open(pathname, openflags{flags}, mode);
   }

   [[nodiscard]] static expected<fd>
   openat(fd const &dirfd, char const *pathname, openflags flags) noexcept
   {
      using ::syscalls::linux::openat;
      using posixpp::error_cascade;
      auto const dfd = dirfd.as_fd();
      return error_cascade(openat(dfd, pathname, flags.getbits(), 0),
                           int_to_fd);
   }
   [[nodiscard]] static expected<fd>
   openat(fd const &dirfd, char const *pathname, fdflags flags) noexcept
   {
      return openat(dirfd, pathname, openflags{flags});
   }

   [[nodiscard]] static expected<fd>
   openat(fd const &dirfd, char const *pathname,
          openflags flags, modeflags mode) noexcept
   {
      using ::syscalls::linux::openat;
      using posixpp::error_cascade;
      return error_cascade(openat(dirfd.as_fd(),
                                  pathname,
                                  flags.getbits(),
                                  mode.getbits()),
                           int_to_fd);
   }
   [[nodiscard]] static expected<fd>
   openat(fd const &dirfd, char const *pathname,
          fdflags flags, modeflags mode) noexcept
   {
      return openat(dirfd, pathname, openflags{flags}, mode);
   }
   ///@}

   expected<::std::size_t>
   write(char const *buf, ::std::size_t size) const noexcept {
      using posixpp::error_cascade;
      return error_cascade(::syscalls::linux::write(fd_, buf, size),
                           [](auto r) { return static_cast<::std::size_t>(r);});
   }

   expected<::std::size_t>
   read(char *buf, ::std::size_t size) const noexcept {
      using posixpp::error_cascade;
      return error_cascade(::syscalls::linux::read(fd_, buf, size),
                           [](auto r) { return static_cast<::std::size_t>(r);});
   }

 protected:
   static fd int_to_fd(int fdes) noexcept {
      return fd{fdes};
   }

 private:
   int fd_;
};

} // namespace posixpp
