# A pure C++ Posix interface #

With this project, I intend to implement many parts of Posix in pure
C++ without any reliance on libc.

Part of the motivation for this is that glibc and pthreads are very
bloated with features that are rarely used in most programs.

Another part of the motivation is that system calls are not exposed to
the C++ optimizer in a useful way.  The register stores and loads
required to make them work can often be combined with the calculations a
program is already doing so that the values mysteriously appear in the
registers when needed instead of having to be moved there just before
the system call.

To this end, a subgoal is to implement the system call interface for
many architectures (and potentially many operating systems) as inline
assembly inside of inline functions.

A third part of the motivation is to eliminate the horrible hack that is
errno.  Errno is the bane of clean error handling, and it requires
expensive to access thread local storage to implement.  Error returns
should not be global variables.

To this end, a simple `expected` type is used for error handling at
every level of the interface.  This also causes default error handling
(if you just assume the `expected` object always contains the expected
value) to throw an exception whenever errors are ignored.  And if you
purposely check for errors, all code relating to exceptions will be
optimized out of existence.

Ideally, parts of the C++ standard library that rely on operating system
facilities would also be implemented in this library.

For example, pthreads is a terrible interface, designed when threading
was mysterious, strange, complex, and rare. And it was designed for old
ideas about how threads should synchronize. It's far more expensive to
use than it should be for simple mutexes and condition variables.

IOstreams isn't the best part of C++. But, being able to implement it
while ignoring C's stdio might make things easier.

Lastly, it would be really nice to have namespaces that aren't chock
full macro definitions (most of which are just using the preprocessor to
implement constants). And it would be nice to have names and types for
various flags passed to various system calls that made it easier to
write correct programs.
