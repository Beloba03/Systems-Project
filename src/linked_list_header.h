#pragma once
#include "car_header.h"

extern void enqueue(int carNum, location loc);
extern location dequeue(int carNum);
extern location peek(int carNum, int locationPos);
extern Car *car;