#pragma once
/*
 - Header file for Building Generator s/w
 - Basic building file structure:
	#x #y - int int - buildings in row
	1Str 1Ave - int int - (EW 01) and (NS 01)
	1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
	0 0 
 - Used by write and read modules
 - ECED 3401
 - 25 Oct 2023
*/

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
//#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */

#define TRUNCATE(name)	name[strlen(name)-1] = '\0'
#define NAMELEN	16	/* Filename length */

/* Internal codes */
enum ST_DIR { East, West };
enum AV_DIR { North, South };
enum BLDG_TYPE { CHG, STB, BOTH};
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };

/* Building record structure */
struct bldg_data
{
	int x;
	int y;
	enum BLDG_TYPE bt;
	enum QUAD qd;
};

/* List of valid responses and codes */
struct prompt
{
char* name;
int code;
};

typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

// Define a structure named Car to hold the x and y position of a car in the city grid
typedef struct {
    unsigned int x;  // x-position of the car
    unsigned int y;  // y-position of the car
    COORD endPos;
    enum QUAD endQuad;
} Car;

extern struct prompt bldg_t[];
extern struct prompt ew[];
extern struct prompt ns[];
extern struct prompt bldg_q[];
extern char** cityGrid;
extern COORD startOffset;
extern unsigned int ybldg;
extern Car *car;

/* Building file descriptor */
FILE* bfd;


extern void hideCursor();
void setConsoleBufferSizeAndWindow(short xBuffer, short yBuffer, short xWindow, short yWindow);
extern COORD getCursorPosition();
void setCursorPosition(int x, int y);
extern void read_file();
extern int getStartAndEndCoordinates();
extern void updateEndCoordinates();
extern void animateCar(int carNum);
extern void freeGrid(unsigned int ySize);
