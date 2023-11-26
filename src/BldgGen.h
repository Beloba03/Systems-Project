
/*
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

A the file reading portion of this code was used from the brightspace example code for building reading. 
It was written by Dr. Hughes. 

This is the header file for the AEDV program. It contains the function declarations, structure definitions, and global variables.

Building file structure:
	#x #y - int int - buildings in row
	1Str 1Ave - int int - (EW 01) and (NS 01)
	1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
	0 0 - int int - end of file
*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS

// Header files
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#define TRUNCATE(name)	name[strlen(name)-1] = '\0'  // Macro to truncate last character of a string
#define NAMELEN	16	/* Maximum length for filename */
#define SCALE_FACTOR 6
#define N_NUM 1
#define NW_NUM 2
#define W_NUM 3
#define SW_NUM 4
#define S_NUM 5
#define SE_NUM 6
#define E_NUM 7
#define NE_NUM 8
#define MAX_QUAD_LENGTH 4
#define MAX_CAR_NUM 4
#define UPDATE_INPUT_OFFSET 7
#define BUILDING_OFFSET 2
#define LAST_STREET_OFFSET 3

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
    COORD endIntersection;
    unsigned char endIntersectionStatus;
    unsigned int startX;
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

// File pointer for the building data file
FILE* bfd;

// Function declarations
extern void hideCursor();
void setConsoleBufferSizeAndWindow(short xBuffer, short yBuffer, short xWindow, short yWindow);
extern COORD getCursorPosition();
void setCursorPosition(int x, int y);
extern void read_file();
extern int getStartAndEndCoordinates();
extern void updateEndCoordinates();
//extern void animateCar(int carNum);
extern void animateCarNew(int carNum);
extern void freeGrid(unsigned int ySize);

// Global variables
int maxCars;
