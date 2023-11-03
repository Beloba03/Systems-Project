#include "BldgGen.h"

// Function to hide the console's cursor
void hideCursor() {
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO cursorInfo;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

    cursorInfo.bVisible = FALSE; // Set the cursor visibility to FALSE

    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

// Function to set the console's buffer and window sizes to allow for maps larger than the screen
void setConsoleBufferSizeAndWindow(short xBuffer, short yBuffer, short xWindow, short yWindow) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set buffer size
    COORD bufferSize = {xBuffer, yBuffer};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set window size
    SMALL_RECT windowSize = {0, 0, xWindow - 1, yWindow - 1}; // -1 because coordinates are zero-based
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

// Function to get the console's cursor position
COORD getCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.dwCursorPosition;
    } else {
        // If there's an error, return a default value or handle as appropriate
        COORD defaultCoord = {0, 0};
        return defaultCoord;
    }
}

// Function to set the console's cursor position
void setCursorPosition(int x, int y) {
    COORD coord;  // Use the COORD struct from Windows API
    coord.X = x;  // Set the X coordinate
    coord.Y = y;  // Set the Y coordinate
    // Set the cursor position using the Windows API function
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
