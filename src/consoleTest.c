#include "windows_init.c"

int main()
{
    COORD pos;
    setConsoleBufferSizeAndWindow(1000, 600, 80, 80); // Set buffer and window sizes.
    printf("hello");
    setCursorPosition(0, 10);
    printf("im here now");
    pos = getCursorPosition();
    setCursorPosition(10, 0);
    printf("im here now...");
    setCursorPosition(pos.X, pos.Y);
    printf("4");
    getchar();
}