// includes 

#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//defines 

#define CTRL_KEY(k) ((k) & 0x1f)

// data 

struct termios orig_termios;

// terminal

void die(const char *s)
{
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
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

char editorReadKey()
{
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
  {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

// output

void editorRefreshScreen()
{
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

// input

void editorProcessKeyPress()
{
  char c = editorReadKey();

  switch(c)
  {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}

// init

int main()
{
  enableRawMode();

  while(1)
  {
    editorRefreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
