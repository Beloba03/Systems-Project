#pragma once
// A portion of this code was used from the brightspace example code for building reading. 
/* 
    BldgGen.h - Header file for the Building Generator software

    Description:
    - Basic building file structure:
        #x #y - int int - buildings in row
        1Str 1Ave - int int - (EW 01) and (NS 01)
        1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
        0 0 
    - Used by write and read modules

    Course: ECED 3401
    Date: 25 Oct 2023
*/

#define _CRT_SECURE_NO_WARNINGS

// Essential includes for functionality
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#define TRUNCATE(name)	name[strlen(name)-1] = '\0'  // Macro to truncate last character of a string
#define NAMELEN	16	/* Maximum length for filename */

// Enumerations for defining directions and building types
enum ST_DIR { East, West };
enum AV_DIR { North, South };
enum BLDG_TYPE { CHG, STB, BOTH};
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };

// Structure to define a building's properties
struct bldg_data {
	int x;          // X-coordinate
	int y;          // Y-coordinate
	enum BLDG_TYPE bt;  // Building type: Charge, Stabilize, or Both
	enum QUAD qd;       // Building's quadrant
};

// Structure to hold valid responses and associated codes
struct prompt {
    char* name;   // Name or response string
    int code;     // Associated enumeration value
};

// Enumeration to define possible car directions
typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

// Structure to define properties of a car in the city grid
typedef struct {
    unsigned int x;     // x-position
    unsigned int y;     // y-position
    COORD endPos;       // Ending position
    enum QUAD endQuad;  // Ending quadrant
} Car;

// External declarations
extern struct prompt bldg_t[];
extern struct prompt ew[];
extern struct prompt ns[];
extern struct prompt bldg_q[];
extern char** cityGrid;
extern COORD startOffset;
extern unsigned int ybldg;
extern unsigned int xbldg;
extern Car *car;
extern int numCars;

// File descriptor for the building file
FILE* bfd;

// Function declarations
extern void hideCursor();
void setConsoleBufferSizeAndWindow(short xBuffer, short yBuffer, short xWindow, short yWindow);
extern COORD getCursorPosition();
void setCursorPosition(int x, int y);
extern void read_file();
extern int getStartAndEndCoordinates();
extern void updateEndCoordinates();
extern void animateCar(int carNum);
extern void freeGrid(unsigned int ySize);
