#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct termios orig_termios;

void disableRawMode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode()
{
  tcgetattr(STDIN_FILENO, &orig_termios); // terminal attributes can be read into struct raw using tcgetattr
  
  atexit(disableRawMode);

  struct termios raw = orig_termios;// struct defined in termios.h

  tcgetattr(STDIN_FILENO, &raw); // terminal attributes can be read into struct raw using tcgetattr

  raw.c_lflag &= ~(ECHO | ICANON);//ECHO feature couses each key you type to be printed in the terminal
  // here ECHO is negated and then BIT-WISE AND with flag field

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // terminal attributes can be applied using tcsetattr;
  // TCSAFLUSH arguments specifies when to apply changes.
}

int main()
{
  enableRawMode();

  char c;
  while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q') // read 1 byte from standart input into the variable c;
  {
    if (iscntrl(c))
    {
      printf("%d\n", c);
    }else {
      printf("%d ('%c')\n", c, c);
    }
  }

  return 0;
}
