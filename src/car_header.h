#include <windows.h>
// Enumeration to define possible car directions
typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

typedef struct
{
    location loc;
    LinkedList *next;
} LinkedList;

// Structure to define a coordinate
typedef struct {
    COORD endPos;
    CarDirection endDir;
} location;

// Structure to define properties of a car in the city grid
typedef struct {
    unsigned int x;     // x-position
    unsigned int y;     // y-position
    LinkedList locQueue;  // Queue of locations to move to
    LinkedList *currentLoc; // Current location
    COORD endIntersection;
    COORD endPos;
    unsigned char endIntersectionStatus;
} Car;