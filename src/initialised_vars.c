/*initialised_vars.c
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

This module is used to hold the variables used to map enumeration values to strings
*/

#include "BldgGen.h"

// These are the variables used to map enumeration values to strings
struct prompt bldg_t[] = { {"Charge",CHG}, {"Stable",STB}, {"Both", BOTH} };
struct prompt ew[] = { {"East", East}, {"West", West} };
struct prompt ns[] = { {"North", North}, {"South", South} };
struct prompt bldg_q[] = {
	{"NE",NE}, {"N",N}, {"NW",NW},
	{"E",E}, {"LBL",-1}, {"W",W},
	{"SE",SE}, {"S",S}, {"SW",SW} };

