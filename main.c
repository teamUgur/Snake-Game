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

