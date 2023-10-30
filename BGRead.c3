/*
 - Building file display module
 - Reads file and displays:
 - Rec1: #x #y - int int - buildings in row
 - Rec2: 1Str 1Ave - int int - (EW 01) and (NS 01)
 - Rec3-N: 1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
 - RecN+1: 0 0
 - ECED 3401
 - 25 Oct 23 - Original
*/

#include "BldgGen.h"

void read_file()
{
/*
 - Read and display
	#x #y - int int - buildings in row
	1Str 1Ave - int int - (EW 01) and (NS 01)
	1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
 - Stop when #S = 0
*/
struct bldg_data bd;
unsigned int xbldg;
unsigned int ybldg;
enum ST_DIR s1dir;
enum AV_DIR a1dir;

/* #x #y int int buildings in row */
fread(&xbldg, sizeof(int), 1, bfd);
fread(&ybldg, sizeof(int), 1, bfd);
printf("# X bldgs %d # Y bldgs %d\n", xbldg, ybldg);

/* 1Str 1Ave int int (EW 01) and (NS 01) */
fread(&s1dir, sizeof(int), 1, bfd);
fread(&a1dir, sizeof(int), 1, bfd);
printf("First Street dir %s First Avenue dir %s\n", ew[s1dir].name, ns[a1dir].name);

/* 
 - Read and display
	1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many 
 - Stop when #S=0
*/

/* Read first record */
fread(&bd, sizeof(struct bldg_data), 1, bfd);
while (bd.x > 0)
{
	printf("Bldg XY: %d %d Type: %s Quad: %s\n", bd.x, bd.y, 
		bldg_t[bd.bt].name, bldg_q[bd.qd].name);
	/* Read next record */
	fread(&bd, sizeof(struct bldg_data), 1, bfd);
}

fclose(bfd);

}

int main(int argc, char *argv[])
{
/* Open building file and display contents */

if (argc != 2)
{
	printf("Supply name of data file\n");
	(void) getchar();
	return;
}

/* Open in binary mode for reading */
//_set_fmode(O_BINARY);

if ((bfd = fopen(argv[1], "r")) < 0)
{
	printf("Can't open %s\n", argv[1]);
	getchar();
	return;
}

read_file();

printf("\nDone\n");
(void) getchar();
return 0;
}
