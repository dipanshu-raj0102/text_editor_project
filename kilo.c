#include <unistd.h>

int main()
{
  char c;
  while(read(STDIN_FILENO, &c, 1) == 1);  // read 1 byte from standart input into the variable c;
  return 0;
}
