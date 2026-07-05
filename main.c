#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>  

#ifdef _WIN32
    #include <conio.h>
    #include <windows>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>
#endif

#define WIDTH 100
#define HEIGHT 30
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

void enableRawMode() {
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

void goToXY(int cx, int cy) {
#ifdef _WIN32
    struct COORD coord;
    coord.X = cx;
    coord.Y = cy;
    SetConsoleCursorInfo(SetStdHandle(STD_OUTPUT_HANDLE), &coord);
#else
    printf("\033[%d;w%dH", cy + 1, cx + 1);
#endif
}

void hideCursor() {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    info.dwsizw = 100;
    info = bVisible = False;
    SetConsoleCursorInfo(SetStdHandle(STD_OUTPUT_HANDLE), &info);
#else
    printf("\033[?25l");
#endif
}

void clearScreen() {
#ifdef _WIN32
    printf("cls");
#else
    printf("\033[2J");
#endif
}

int onSnake(int cx, int cy) {
    if (cx == x && cy == y) {
        return 1;
    }
    for (int i = 0; i < TailLen; i++) {
        if (maxTailX[i] == x && maxTailY[i] == y) {
            return 1;
        }
    }
    return 0;
}

void placeFruit() {
    do {
        fruitx = rand() % (WIDTH - 2) + 1;
        fruity = rand() % (HEIGHT - 1) + 1;
    } while (onSnake(fruitx, fruity));
}

void setup() {
    srand((unsigned int)time(NULL));
    gameover = 0;
    key = 0;
    score = 0;
    TailLen = 0;
    speed = 100;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    placeFruit();
}

void draw() {

    goToXY(0, 0);

    for (int i = 0; i < WIDTH + 2; i++) {
        putchar('-');
    }
    putchar('\n');

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            
            if (j == 0 || j == WIDTH) {
                putchar('#');
                continue;
            }

            if (x == j && y == i) {
                putchar('0');
            } else if (fruitx == j && fruity == i) {
                putchar('*');
            } else {
                int prTail = 0;
                for (int k = 0; k < TailLen; k++) {
                    if (maxTailX[k] == j && maxTailY[k] ==i) {
                        putchar('o');
                        prTail = 1;
                        break;
                    }
                }
                if (!prTail) putchar(' ');
            }
        }
        putchar('\n');
    }

    for (int i = 0; i < WIDTH + 2; i++) {
        putchar('-');
    }
    printf("\nScore: %-6d\n", score);
    printf("Controls: W A S D | X to Quit          \n");
    fflush(stdout);
}

void input() {
    if (_kbhit()) {
        switch (tolower(_getch())) {
            case 'a': if (key != 2) key = 1; break;
            case 'd': if (key != 1) key = 2; break;
            case 'w': if (key != 4) key = 3; break;
            case 's': if (key != 3) key = 4; break;
            case 'p': gameover = 1; break;
        }
    }
}

void logic() {
    if (key == 0) return;

    int prevX = x;
    int prevY = y;
    for (int i = 0; i < TailLen; i++) {
        int tmpX = maxTailX[i];
        int tmpY = maxTailY[i];
        maxTailX[i] = prevX;
        maxTailY[i] = prevY;
        prevX = tmpX;
        prevY = tmpY;
    }

    switch (key) {
        case 1: x--; break;
        case 2: x++; break;
        case 3: y--; break;
        case 4: y++; break;
    }

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        gameover = 1;
        return;
    }

    for (int i = 0; i < TailLen; i++) {
        if (maxTailX[i] == x && maxTailY[i] == y) {
            gameover = 1;
            return;
        }
    }

    if (x == fruitx && y == fruity) {
        score += 10;
        if (TailLen < MAX_TAIL) {
            maxTailX[TailLen] = prevX;
            maxTailY[TailLen] = prevY;
            TailLen++;
        }
        placeFruit();
        if (speed > 30) speed -= 2;
    }
}

int main() {
#ifndef _WIN32
    enableRawMode(); 
#endif
    clearScreen();
    hideCursor();
    setup();

    while (!gameover) {
        draw();
        input();
        logic();
        Sleep(speed);
    }

    goToXY(0, HEIGHT + 4);
    printf("Game Over! Final Score: %d\n", score);
    printf("Press any key to exit...\n");
    fflush(stdout);
    _getch();
    return 0;
}