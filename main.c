#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>
#endif

#define WIDTH 20
#define HEIGHT 40
#define MAX_TAIL (WIDTH * HEIGHT)

int maxTailX[MAX_TAIL], maxTailY[MAX_TAIL];
int key, score, gameover;
int TailLen;
int x, y, fruitx, fruity;
int speed;

#ifndef _WIN32
struct termios origTermios;

void disableRowMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &origTermios);
}

void enableRowMode() {
    tcgetattr(STDIN_FILENO, &origTermios);
    struct termios raw = origTermios;
    raw.c_lflag &= ~ (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    atexit(disableRowMode);
}

int _kbhit() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

int _getch() {
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

void Sleep(int ms) {
    usleep(ms * 1000);
}

#endif