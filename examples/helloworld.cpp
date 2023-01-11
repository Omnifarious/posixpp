#include <posixpp/fd.h>
#include <posixpp/basic.h>
#include <posixpp/simpleio.h>

int x = 0;

int main(int argc, char const * const *argv)
{
   const ::posixpp::fd fdin{0};
   const ::posixpp::fd fdout{1};
   char msg[] = "Hello World 0!\n";
   char buf[1024] = {'\0'};
   char *mybuf = buf;
   auto result = read(fdin, mybuf, sizeof(buf));
   unsigned long tmp = 0;
   if (result.has_error()) {
      ::posixpp::exit(2);
   } else {
      tmp = result.result();
   }
   if (tmp < 1) {
      ::posixpp::exit(2);
   }
   msg[0] = mybuf[tmp - 1];
   // sizeof(msg) - 1 to skip trailing '\0'
   result = write(fdout, msg, sizeof(msg) - 1);
   int i = 1;
   while (i < 10 && !result.has_error()) {
      msg[12] = i++ + '0';
      result = write(fdout, msg, sizeof(msg) - 1);
      if (x != 0) {
         break;
      }
   }
   ::posixpp::exit(result.has_error());
}
