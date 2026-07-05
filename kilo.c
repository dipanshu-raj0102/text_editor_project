// includes 

#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


// data 

struct termios orig_termios;

// terminal

void die(const char *s)
{
  perror(s);
  exit(1);
}

void disableRawMode()
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");

}

void enableRawMode()
{
 if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr"); // terminal attributes can be read into struct raw using tcgetattr
  
  atexit(disableRawMode);

  struct termios raw = orig_termios;// struct defined in termios.h

  tcgetattr(STDIN_FILENO, &raw); // terminal attributes can be read into struct raw using tcgetattr

  raw.c_lflag &= ~(BRKINT | INPCK | ISTRIP | ICRNL | IXON);
  raw.c_lflag &= ~(OPOST);
  raw.c_lflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);//ECHO feature couses each key you type to be printed in the terminal
  // here ECHO is negated and then BIT-WISE AND with flag field
  
  raw.c_cc[VMIN] == 0;
  raw.c_cc[VTIME] == 1;
  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr"); // terminal attributes can be applied using tcsetattr;
  // TCSAFLUSH arguments specifies when to apply changes.
}

// init

int main()
{
  enableRawMode();

  while(1) // read 1 byte from standart input into the variable c;
  {
    char c = '\0';

    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read"); 

    if (iscntrl(c))
    {
      printf("%d\r\n", c);
    }else {
      printf("%d ('%c')\r\n", c, c);
    }

    if (c == 'q') break;
  }

  return 0;
}
