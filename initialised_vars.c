#include "BldgGen.h"

struct prompt bldg_t[] = { {"Charge",CHG}, {"Stable",STB}, {"Both", BOTH} };
struct prompt ew[] = { {"East", East}, {"West", West} };
struct prompt ns[] = { {"North", North}, {"South", South} };
struct prompt bldg_q[] = {
	{"NE",NE}, {"N",N}, {"NW",NW},
	{"E",E}, {"LBL",-1}, {"W",W},
	{"SE",SE}, {"S",S}, {"SW",SW} };