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

