#pragma once  /*-*-c++-*-*/

#include <posixpp/expected.h>
#include <posixpp/fdflags.h>
#include <posixpp/modeflags.h>
#include <syscalls/linux/simple_io.h>
#include <optional>

namespace posixpp {

//! A file descriptor and the associated functions
class fd {
 public:
   //! Avoid using this to create a file descriptor directly from an integer.
   explicit constexpr fd(int fdval) noexcept : fd_(fdval) {}

   //! Create an invalid file descriptor.
   constexpr fd() noexcept : fd_(-1) {}

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

   //! \brief Will call `close` on the destination if the file descriptor
   //! looks valid, and it will ignore the return value.
   fd &operator =(fd &&other) noexcept {
      using ::syscalls::linux::close;
      int const tmpfd = other.fd_;
      other.fd_ = -1;
      if (fd_ >= 0) {
         close(fd_); // Ignore any error return.
      }
      fd_ = tmpfd;
      return *this;
   }

   //! \brief Sets fd to invalid value and also calls close regardless of
   //! whether fd is currently an invalid value.
   [[nodiscard]] expected<void> close() noexcept {
      using ::syscalls::linux::close;
      auto const tmpfd = fd_;
      fd_ = -1;
      return close(tmpfd);
   }

   //! A true return value is maybe, a false return is definite.
   [[nodiscard]] constexpr bool is_valid() const noexcept {
      return fd_ >= 0;
   }

   //! Avoid if at all possible, just like constructor.
   [[nodiscard]] constexpr int as_fd() const noexcept { return fd_; }

   /**
    * \name Member functions for fd duplication.
    *
    * These functions duplicate file descriptors in various ways.  It's
    * tempting to be clever and have the copy constructor and assignment
    * operators do this. But I think the semantics are subtly different enough
    * to make that very confusing.
    *
    * In the Posix model, there is an underlying kernel global file handle
    * list that per-process file handles merely refer to. Most attributes
    * (like the current file position for reads or writes) are attributes of
    * this global handle. A very small number (most notably the close on exec
    * flag) are attributes of the process-level file handle.
    */
    //! @{
   /**
    * \brief See man page dup(2) - kind of like a copy constructor.
    *
    * The two file descriptors both reference the same underlying kernel file
    * handle, and so share a file pointer and other attributes.
    */
   [[nodiscard]] expected<fd> dup() const noexcept {
      using ::syscalls::linux::dup;
      return error_cascade(dup(fd_), int_to_fd);
   }

   /**
    * See man page dup2(2) or dup3(2) (if the cloexec flag has a value).
    *
    * This is sort of like an assignment operator that causes the destination
    * file descriptor to refer to the same kernel file handle that the original
    * does.
    *
    * @param fd File descriptor to replace. May or may not be already open.  If
    * already open, it will be closed, before the original file descriptor is
    * duplicated into it.
    *
    * @param cloexec Whether or not the close on exec flag should be set for
    * the destination file descriptor. Unlike most attributes associated with
    * a file handle, this flag is per process fd, not per kernel file handle.
    * This is intended to save a system call to set this flag.
    */
   [[nodiscard]] expected<void> dup2(
        fd &newfd,
        ::std::optional<bool> cloexec=::std::optional<bool>{}
   ) const noexcept
   {
      using ::syscalls::linux::dup2;
      using ::syscalls::linux::dup3;
      if (!cloexec.has_value()) {
         return error_cascade_void(dup2(fd_, newfd.fd_));
      } else {
         auto const flagval = cloexec.value() ? fdflags::cloexec : fdflags{};
         // TODO: Write a test case for this, which is hard because it
         //       requires the 'exec' system call, and that the exec'ed program
         //       run part of the test.
         return error_cascade_void(
              dup3(fd_, newfd.fd_, flagval.getbits())
         );
      }
   }
   //! @}

 protected:
   static fd int_to_fd(int fdes) noexcept {
      return fd{fdes};
   }

 private:
   int fd_;
};

} // namespace posixpp
