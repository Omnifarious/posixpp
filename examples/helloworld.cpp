#include <posixpp/fd.h>
#include <posixpp/expected.h>
#include <posixpp/basic.h>

int main(int argc, char const * const *argv)
{
   const ::posixpp::fd fdout{1};
   static constexpr char msg[] = "Hello World!\n";
   // sizeof(msg) - 1 to skip trailing '\0'
   auto result = fdout.write(msg, sizeof(msg) - 1);
   ::posixpp::exit(result.has_error());
}
