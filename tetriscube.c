/*
	Tetris Cube 4x4x4 box tiling puzzle solver for Dylan
	03/30/2008 Scott Kurowski, www.scottkurowski.com

	Copyright 2008 All Rights Reserved, with the following Exception:
	You may re-publish this code or its output file or modify the
	software for other purposes provided you (1) cite/reference
	the website address given above, and (2) you email about your
	intended usage to scott@scottkurowski.com .

	Algorithm:
		For each piece, rotate it in 3D and save unique position
		orientions data into a lookup table, where the cubes in
		each piece/orientation entry are normalized relative to a
		base cube indexed by lowest x,y,z coordinate priority order.

		1 init box, piece table, piece order, base cube (0,0,0)
		2 scan to find next base cube
			if failed because of isolated cube:
				remove piece in last orientation
				goto 4 next orient to tile in piece table
		3 next piece to tile in permutation
		4 next orient to tile in piece table
			if none left:
				remove piece in last orientation
				permute piece order at this piece index
					if nothing left to try goto 6 done
				goto 3 next piece to tile in permutation
		5 try to tile piece
			if success:
				if last piece tiled, save solution
				goto 2 find next base cube
			else failed to tile:
				goto 4 next orient to tile in piece table
		6 done!

		For each solution found, rotate it in 3D and compare to all
		previously found solutions; if unique, add it to the solutions
		found, but otherwise discard it as a rotated duplicate solution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define SOLUTIONS_FILE_NAME		"TetrisCubeSolutions.txt"

#define BOX_SIZE	4
#define EMPTY_CUBE	-2
#define WALL_CUBE	-1

#define nPIECES			13
#define nCUBES			6		// some pieces have 5 cubes, one unused
#define nMAXORIENTS		24		// 4 axis positions * 6 cube faces
#define nMAXSOLUTIONS	10000	// make this larger than expected

#define X_AXIS		0
#define Y_AXIS		1
#define Z_AXIS		2

#define BASE_CUBE_FOUND			0
#define BASE_CUBE_NOT_FOUND		1
#define BASE_CUBE_ISOLATED		-1
#define BASE_CUBE_UNTILEABLE	-2

#define swap(x,y) { int tmp = x; x = y; y = tmp; }

struct p_cube {
	int x,y,z;
};

struct piece_data {
	int piece_number;
	int orientation_number;
	int cCubes;
	struct p_cube dCubes[nCUBES];
};

struct tiling_solution {
	int solution[BOX_SIZE+2][BOX_SIZE+2][BOX_SIZE+2];
};

struct tiling_solution tiled_solutions[nMAXSOLUTIONS];	// table of unique solutions found
int tiling_box[BOX_SIZE+2][BOX_SIZE+2][BOX_SIZE+2];	// [x][y][z] working piece fitting box
struct piece_data p_d[nPIECES][nMAXORIENTS];		// data by piece # and orientation #
int permuted_piece_order[nPIECES];					// piece #s by index
int orientations[nPIECES];							// # of orientations by piece #
int tiled_orients[nPIECES];							// orientation # by index
struct p_cube tiled_base_cubes[nPIECES];			// base cube coordinates of fitted pieces
struct p_cube base_cube;							// current base cube coordinates

long long tiles_attempted = 0;	// how many times tried to fit pieces
long long tiles_succeeded = 0;	// how many times pieces were fitted
long long permutes = 1;			// initial permutation is 0,1,2,3,4,5,6,7,8,9,10,11
int solutions = 0;				// unique solutions found count
int duplicates = 0;				// rotated duplicates discarded count
FILE * outputfile = NULL;

struct piece_data initial_positions[nPIECES] =
{	// five or six cubes each; unused sixth cube coordinates are 9s
	// p, o, c,   x,y,z
    { 0, 0, 5, {{0,0,0},{0,1,0},{1,1,0},{0,2,0},{0,1,1}} },
    { 1, 0, 5, {{0,0,0},{1,0,0},{2,0,0},{0,1,0},{1,0,1}} },
    { 2, 0, 5, {{0,0,0},{1,0,0},{1,1,0},{1,0,1},{2,0,1}} },
	{ 3, 0, 5, {{1,0,0},{0,1,0},{1,1,0},{2,1,0},{1,0,1}} },
    { 4, 0, 5, {{0,0,0},{0,1,0},{1,1,0},{1,1,1},{1,2,1}} },
    { 5, 0, 5, {{0,0,0},{0,1,0},{1,1,0},{2,1,0},{1,2,0}} },
    { 6, 0, 4, {{0,0,0},{0,1,0},{1,1,0},{1,1,1},{9,9,9}} },
    { 7, 0, 5, {{0,0,0},{1,0,0},{2,0,0},{2,1,0},{2,1,1}} },
    { 8, 0, 5, {{0,0,0},{0,1,0},{1,1,0},{1,2,0},{2,2,0}} },
    { 9, 0, 5, {{0,0,0},{1,0,0},{2,0,0},{0,1,0},{0,0,1}} },
    { 10, 0, 5, {{0,0,0},{1,0,0},{2,0,0},{0,1,0},{2,0,1}} } ,
    { 11, 0, 5, {{1,0,0},{2,0,0},{0,1,0},{1,1,0},{2,0,1}} },
    { 12, 0, 5, {{1,0,0},{0,1,0},{1,1,0},{2,1,0},{1,2,0}} }
};

char piece_labels[nPIECES][30] =
{	// friendly names to print and aid piece identification
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"
};

int get_base_cube_index( struct piece_data *pd )
{
	// tiling order is x-most, then y-most, finally z
	// so we must find the cube(s) with the smallest z
	// then of those, cube(s) with the smallest y
	// then of those, the cube with the smallest x
	int cube, best_cube = -1, cCubes = pd->cCubes;
	int best_ix = 99999, best_iy = 99999, best_iz = 99999;

	for ( cube = 0; cube < cCubes; cube++ )
		if ( pd->dCubes[cube].z < best_iz ) {
			best_iz = pd->dCubes[cube].z;
			best_cube = cube;
		}
	for ( cube = 0; cube < cCubes; cube++ )
		if ( pd->dCubes[cube].y < best_iy &&
			 pd->dCubes[cube].z == best_iz )
		{
			best_iy = pd->dCubes[cube].y;
			best_cube = cube;
		}
	for ( cube = 0; cube < cCubes; cube++ )
		if ( pd->dCubes[cube].x < best_ix &&
			 pd->dCubes[cube].y == best_iy &&
			 pd->dCubes[cube].z == best_iz )
		{
			best_ix = pd->dCubes[cube].x;
			best_cube = cube;
		}
	return best_cube;
}

void check_piece_data( struct piece_data *pd );	// proto fwd

void normalize_to_base_cube( struct piece_data *pd )
{
	// substract the coordinates of the indexed base cube from all cubes
	// so the base cube is at 0,0,0 afterward, and sort cube order
	// by distance from base cube, which is always at cube index 0
	int cube, c_base_cube, c, cCubes = pd->cCubes;
	double d[nCUBES], ds;
	struct p_cube bc;
	check_piece_data( pd );	// integrity check
	c_base_cube = get_base_cube_index( pd );

	bc.x = pd->dCubes[c_base_cube].x;
	bc.y = pd->dCubes[c_base_cube].y;
	bc.z = pd->dCubes[c_base_cube].z;

	for ( cube = 0; cube < cCubes; cube++ )
	{
		pd->dCubes[cube].x -= bc.x;
		pd->dCubes[cube].y -= bc.y;
		pd->dCubes[cube].z -= bc.z;
		d[cube] = sqrt(
				pd->dCubes[cube].x * pd->dCubes[cube].x +
				pd->dCubes[cube].y * pd->dCubes[cube].y +
				pd->dCubes[cube].z * pd->dCubes[cube].z );
	}
	for ( cube = 0; cube < cCubes-1; cube++ )
		for ( c = cube+1; c < cCubes; c++ )
			if ( d[cube] > d[c] )
			{
				swap( pd->dCubes[cube].x, pd->dCubes[c].x );
				swap( pd->dCubes[cube].y, pd->dCubes[c].y );
				swap( pd->dCubes[cube].z, pd->dCubes[c].z );
				ds = d[cube]; d[cube] = d[c]; d[c] = ds;
			}
	check_piece_data( pd );	// integrity check
}


void rotate_piece_090_in_plane( struct piece_data *pd, int xyz_axis )
{
	// rotate 90 degrees by transposing & negating cube coordinates
	// along two axes whose axis index is not xyz_axis
	int cube, temp, cCubes = pd->cCubes;
	for ( cube = 0; cube < cCubes; cube++ )
		switch ( xyz_axis )
		{
			case X_AXIS:	// rotate pi/2 in y-z plane around x-axis
				temp = pd->dCubes[cube].y;
				pd->dCubes[cube].y = pd->dCubes[cube].z;
				pd->dCubes[cube].z = -temp;
			break;
			case Y_AXIS:	// rotate pi/2 in x-z plane around y-axis
				temp = pd->dCubes[cube].z;
				pd->dCubes[cube].z = pd->dCubes[cube].x;
				pd->dCubes[cube].x = -temp;
			break;
			case Z_AXIS:	// rotate pi/2 in x-y plane around z-axis
				temp = pd->dCubes[cube].x;
				pd->dCubes[cube].x = pd->dCubes[cube].y;
				pd->dCubes[cube].y = -temp;
			break;
		}
	check_piece_data( pd );
	normalize_to_base_cube( pd );
}


int add_orientation_to_lookup_table( struct piece_data *pd )
{
	// returns 1 if added, else 0 if duplicate piece orientation data in table
	int cube, cCube, matches, cCubes = pd->cCubes;
	int piece = pd->piece_number;
	int orientation = 0;
	while ( orientation < orientations[ piece ] )
	{
		matches = 0;
		for ( cube = 0; cube < cCubes; cube++ )
			for ( cCube = 0; cCube < cCubes; cCube++ )
			{
				if ( p_d[piece][orientation].dCubes[cube].x == pd->dCubes[cCube].x &&
					 p_d[piece][orientation].dCubes[cube].y == pd->dCubes[cCube].y &&
					 p_d[piece][orientation].dCubes[cube].z == pd->dCubes[cCube].z )
				 matches++;
			}
		if ( matches == cCubes ) // if found it don't add duplicate table entry
			return 0;
		orientation++;
	}
	// add new orientation to lookup table for this piece
	orientation = orientations[ piece ]++;
	memcpy( &p_d[piece][orientation], pd, sizeof(struct piece_data) );
	return 1;
}

void init_piece_table( void )
{
	int piece_index;
	for ( piece_index = 0; piece_index < nPIECES; piece_index++ )
	{
		permuted_piece_order[ piece_index ] = piece_index;
		orientations[ piece_index ] = 0;
		tiled_orients[ piece_index ] = -1;
	}
	memset( &p_d, -1, sizeof(p_d) );
}

void verify_piece_table( void )
{
	int piece, orient;
	for ( piece = 0; piece < nPIECES; piece++ )
		for ( orient = 0; orient < orientations[ piece ]; orient++ )
		{	// trap bad data in debugger
			if ( p_d[piece][orient].piece_number != piece )
			{ printf("bad piece number %d\n",piece); }
			if ( p_d[piece][orient].orientation_number != orient )
			{ printf("bad orientation_number %d\n",orient); }
			check_piece_data( &p_d[piece][orient] );
		}
}

void sort_permutations_above_piece( int piece_index )
{
	// re-order above -- just a qnd bubble sort
	// and count permuted_piece_order here as shared basis
	int p, q, next_piece = nPIECES;
	for ( p = piece_index+1; p < nPIECES; p++ )
		for ( q = p+1; q < nPIECES; q++ )
			if ( permuted_piece_order[ q ] < permuted_piece_order[ p ] )
			{
				next_piece = permuted_piece_order[ q ];
				permuted_piece_order[ q ] = permuted_piece_order[ p ];
				permuted_piece_order[ p ] = next_piece;
			}
	permutes++;
}

int permute_piece_order( int piece_index )
{
	// Find first piece # larger than piece at piece_index,
	// then swap them and sort ascending order after piece_index.
	// returns 1 for success, 0 for failed (or done)
	int p, next_index = nPIECES, next_piece = nPIECES;

	for ( p = piece_index+1; p < nPIECES; p++ )
	{
		if ( permuted_piece_order[ p ] > permuted_piece_order[ piece_index ] &&
			 permuted_piece_order[ p ] < next_piece )
		{
			next_piece = permuted_piece_order[ p ];
			next_index = p;
		}
	}
	if ( next_piece == nPIECES ) return 0;	// nothing left

	// swap and re-order permutation above piece_index
	swap( permuted_piece_order[ piece_index ], permuted_piece_order[ next_index ] );
	sort_permutations_above_piece( piece_index );

	if ( permutes % 1000000 == 0 )
	{
		printf("permutation: ");
		for ( p = 0; p < nPIECES; p++ )
			printf("%02d ", permuted_piece_order[p]);
		printf("\n");
		printf("orientation: ");
		for ( p = 0; p < nPIECES; p++ )
			printf("%02d ", tiled_orients[p]);
		printf("\n");
		printf("permutations = %I64d, tiles attempted = %I64d, tiles succeeded = %I64d, solutions = %d, duplicates = %d\n",
			permutes, tiles_attempted, tiles_succeeded, solutions, duplicates );
	}
	return 1;
}


int generate_piece_orientations_table( int piece )
{
	// each axis has 4 rotation positions for a single piece and there are
	// no imaginary reflections to deal with, so for 6 cardinal cube faces
	// and 4 rotation positions, there are at most 24 orientations and
	// if a piece has two-dimensional symmetry there are half as many.
	int x_axis_positions, y_axis_positions, z_axis_positions;
	int  orients = 0;
	for ( z_axis_positions = 0; z_axis_positions < 4; z_axis_positions++ )
	{
		for ( y_axis_positions = 0; y_axis_positions < 4; y_axis_positions++ )
		{
			for ( x_axis_positions = 0; x_axis_positions < 4; x_axis_positions++ )
			{
				initial_positions[piece].orientation_number = orients;
				if ( z_axis_positions == 0 && y_axis_positions == 0 && x_axis_positions == 0 )
				{
					// load initial piece position
					orients = add_orientation_to_lookup_table( &initial_positions[piece] );
					if ( orients == 0 ) {
						printf("failed to add initial orientation for piece %d\n", piece );
					}
				} else {
					// rotate around x-axis
					rotate_piece_090_in_plane( &initial_positions[piece], X_AXIS );
					orients += add_orientation_to_lookup_table( &initial_positions[piece] );
				}
			}
			// rotate around y-axis
			rotate_piece_090_in_plane( &initial_positions[piece], Y_AXIS );
		}
		// rotate around z-axis
		rotate_piece_090_in_plane( &initial_positions[piece], Z_AXIS );
	}
	return orients;
}


int put_piece_into_box( struct piece_data *pd, struct p_cube * box_base_cube )
{
	// returns 1 if success, else 0
	int c, cCubes = pd->cCubes;
	tiles_attempted++;
	for ( c = 0; c < cCubes; c++ )
		if ( tiling_box[ box_base_cube->x + pd->dCubes[c].x ]
					   [ box_base_cube->y + pd->dCubes[c].y ]
					   [ box_base_cube->z + pd->dCubes[c].z ] != EMPTY_CUBE ) break;
	if ( c < cCubes ) return 0;
	for ( c = 0; c < cCubes; c++ )
		tiling_box[ box_base_cube->x + pd->dCubes[c].x ]
			      [ box_base_cube->y + pd->dCubes[c].y ]
			      [ box_base_cube->z + pd->dCubes[c].z ] = pd->piece_number;
	tiles_succeeded++;
	return 1;
}


int remove_piece_from_box( struct piece_data *pd, struct p_cube * box_base_cube )
{
	// returns 1 if success, else 0
	int c, cCubes = pd->cCubes;
	for ( c = 0; c < cCubes; c++ )
	{
		if ( tiling_box[ box_base_cube->x + pd->dCubes[c].x ]
					   [ box_base_cube->y + pd->dCubes[c].y ]
					   [ box_base_cube->z + pd->dCubes[c].z ] != pd->piece_number ) break;
		else
			tiling_box[ box_base_cube->x + pd->dCubes[c].x ]
				      [ box_base_cube->y + pd->dCubes[c].y ]
				      [ box_base_cube->z + pd->dCubes[c].z ] = EMPTY_CUBE;
	}
	if ( c < cCubes ) return 0;
	return 1;
}


void dump_piece_data( struct piece_data *pd )
{
	int c, cCubes = pd->cCubes;
	printf("PIECE %2d ORIENT %2d : ", pd->piece_number, pd->orientation_number );
	for ( c = 0; c < cCubes; c++ )
		printf("(%2d,%2d,%2d) ",  pd->dCubes[c].x,  pd->dCubes[c].y,  pd->dCubes[c].z );
	printf("\n");
}

void check_piece_data( struct piece_data *pd )
{
	int cube, cCubes = pd->cCubes;
	for ( cube = 0; cube < cCubes; cube++ )
	{
		if ( abs(pd->dCubes[cube].x) > nCUBES-1 ||
			 abs(pd->dCubes[cube].y) > nCUBES-1 ||
			 abs(pd->dCubes[cube].z) > nCUBES-1 )
		{
			printf("cube %d out of bounds\n",cube);
			dump_piece_data( pd );
		}
	}
}


int find_box_next_base_cube( struct p_cube * box_base_cube )
{
	// returns -1 if isolated cube is next
	// returns 1 if no base cube found (fully tiled)
	// returns 0 if next base cube is found
	int ix, iy, iz;
	ix = box_base_cube->x;
	iy = box_base_cube->y;
	iz = box_base_cube->z;
	while ( iz < BOX_SIZE+2 )
	{
		while ( iy < BOX_SIZE+2 )
		{
			while ( ix < BOX_SIZE+2 )
			{
				if ( tiling_box[ix][iy][iz] == EMPTY_CUBE ) goto done;
				ix++;
			}
			ix = 0;
			iy++;
		}
		iy = 0;
		iz++;
	}
done:
	// if we end up in the last layer, the box must be fully tiled
	if ( iz == BOX_SIZE+2 ) return BASE_CUBE_NOT_FOUND;

	// check for isolated base cube (no open adjacent neighbors - no diagnonals)
	if ( tiling_box[ix+1][iy][iz] != EMPTY_CUBE &&
		 tiling_box[ix][iy+1][iz] != EMPTY_CUBE &&
		 tiling_box[ix][iy][iz+1] != EMPTY_CUBE )
		 return BASE_CUBE_ISOLATED;

	// good base cube
	box_base_cube->x = ix;
	box_base_cube->y = iy;
	box_base_cube->z = iz;
//	printf("Found next base cube at (x=%d,y=%d,z=%d)\n", ix, iy, iz );
	return BASE_CUBE_FOUND;
}

void init_empty_box( void )
{
	int ix, iy, iz;
	for ( ix = 0; ix < BOX_SIZE+2; ix++ )
		for ( iy = 0; iy < BOX_SIZE+2; iy++ )
			for ( iz = 0; iz < BOX_SIZE+2; iz++ )
				if ( ix == 0 || ix == BOX_SIZE+1 ||
					 iy == 0 || iy == BOX_SIZE+1 ||
					 iz == 0 || iz == BOX_SIZE+1 )
					tiling_box[ix][iy][iz] = WALL_CUBE;
				else
					tiling_box[ix][iy][iz] = EMPTY_CUBE;
}


void dump_solution( FILE * output, int fPrintSolutionNumber )
{
	// put top-most to bottom-most layer from left to right
	// x increasing to the right, y increasing bottom to top
	int ix, iy, iz;
	char s[] = "  0123456789ABC";
	if ( !output ) output = stdout;
	if ( fPrintSolutionNumber )
		fprintf(output,"Solution %d\n", solutions );
	for ( iy = BOX_SIZE; iy > 0; iy-- )
	{
		for ( iz = BOX_SIZE; iz > 0; iz-- )
		{
			for ( ix = 1; ix < BOX_SIZE+2; ix++ )
				fprintf(output," %1c", s[ tiling_box[ix][iy][iz] - EMPTY_CUBE ] );
			if ( iz > 1 ) fprintf(output," ");
		}
		fprintf(output,"\n");
	}
	fflush( output );
}

void dump_box( FILE * output )
{
	// report current box contents and permutation/orientations/counts
	int i;
	if ( !output ) output = stdout;
	dump_solution( output, 0 );
	fprintf(output,"Permutation: ");
	for ( i = 0; i < nPIECES; i++ )
		fprintf(output,"%02d ", permuted_piece_order[i]);
	fprintf(output,"\nOrientation: ");
	for ( i = 0; i < nPIECES; i++ )
		fprintf(output,"%02d ", tiled_orients[i]);
	fprintf(output,"\npermutations = %I64d, tiles attempted = %I64d, tiles succeeded = %I64d, solutions = %d, duplicates = %d\n\n",
		permutes, tiles_attempted, tiles_succeeded, solutions, duplicates );
}


void rotate_box_090_in_plane( int xyz_axis )
{
	// rotate 90 degrees by transposing & negating cube coordinates
	// along two axes whose axis index is not xyz_axis
	int ix, iy, iz;
	int rotated_box[BOX_SIZE+2][BOX_SIZE+2][BOX_SIZE+2];
	memcpy( &rotated_box, &tiling_box, sizeof(tiling_box) );	// copy box walls first
	for ( iz = 1; iz < BOX_SIZE+1; iz++ )
		for ( iy = 1; iy < BOX_SIZE+1; iy++ )
			for ( ix = 1; ix < BOX_SIZE+1; ix++ )
				switch ( xyz_axis )
				{
					case X_AXIS:	// rotate pi/2 in y-z plane around x-axis
						rotated_box[ix][iz][BOX_SIZE+1-iy] = tiling_box[ix][iy][iz];
						break;
					case Y_AXIS:	// rotate pi/2 in x-z plane around y-axis
						rotated_box[BOX_SIZE+1-iz][iy][ix] = tiling_box[ix][iy][iz];
						break;
					case Z_AXIS:	// rotate pi/2 in x-y plane around z-axis
						rotated_box[iy][BOX_SIZE+1-ix][iz] = tiling_box[ix][iy][iz];
						break;
				}
	memcpy( &tiling_box, &rotated_box, sizeof(tiling_box) );
}

int add_solution_to_solutions_table( void )
{
	// returns 1 if added, else 0 if duplicate solution in table
	int x_axis_positions, y_axis_positions, z_axis_positions, s;
	int saved_tiling_box[BOX_SIZE+2][BOX_SIZE+2][BOX_SIZE+2];
	memcpy( &saved_tiling_box, &tiling_box, sizeof(tiling_box) );
	for ( z_axis_positions = 0; z_axis_positions < 4; z_axis_positions++ )
	{
		for ( y_axis_positions = 0; y_axis_positions < 4; y_axis_positions++ )
		{
			for ( x_axis_positions = 0; x_axis_positions < 4; x_axis_positions++ )
			{
				for ( s = 0; s < solutions; s++ )
					if ( memcmp( &tiled_solutions[s].solution,
								 &tiling_box,
								 sizeof(tiling_box) ) == 0 ) break;
				if ( s < solutions ) {
					// restore box to original state to continue search
					memcpy( &tiling_box, &saved_tiling_box, sizeof(tiling_box) );
					return 0;
				}
				rotate_box_090_in_plane( X_AXIS );
			}	
			// rotate around y-axis
			rotate_box_090_in_plane( Y_AXIS );
		}
		// rotate around z-axis
		rotate_box_090_in_plane( Z_AXIS );
	}
	// restore box to original state to continue search
	memcpy( &tiling_box, &saved_tiling_box, sizeof(tiling_box) );
	// add new unique solution to table
	memcpy( &tiled_solutions[solutions++].solution, &tiling_box, sizeof(tiling_box) );
	return 1;
}

void print_piece_names( FILE * output )
{
	int piece;
	if ( ! output ) output = stdout;
	for ( piece = 0; piece < nPIECES; piece++ )
		fprintf(output,"Piece %2d has %d cubes, labeled %s\n",
			initial_positions[piece].piece_number,
			initial_positions[piece].cCubes,
			piece_labels[piece] );
	fprintf(output,"\n\n");
}

int main( void )
{
	int piece, orients, last_piece_tiled_index, orientation, nbc;
	char author[] = "Tetris Cube Solver, (c)2008 www.scottkurowski.com\n\n";

	outputfile = fopen( SOLUTIONS_FILE_NAME, "a+" );
	printf( author );
	fprintf( outputfile, author );
	print_piece_names( outputfile );
	print_piece_names( NULL );

	// take the initial piece maps given and
	// generate piece/orientation lookup table
	init_piece_table();
	for ( piece = 0; piece < nPIECES; piece++ )
	{
		normalize_to_base_cube( &initial_positions[piece] );
		orients = generate_piece_orientations_table( piece );
		printf("Piece %2d has %d unique rotational orientations\n", piece, orients );
		fprintf(outputfile,"Piece %2d has %d unique rotational orientations\n", piece, orients );
	}
	printf("\n");
	fprintf(outputfile,"\n");
	verify_piece_table();

	// setup empty box, initial base cube position
	// and permutation tiling order index
	init_empty_box();
	base_cube.x = 0;
	base_cube.y = 0;
	base_cube.z = 0;
	last_piece_tiled_index = -1;

next_base_cube:
	nbc = find_box_next_base_cube( &base_cube );
check_remove_last_piece:
	if ( nbc != BASE_CUBE_FOUND )
	{
		// remove last piece fitted in last orientation, either:
		// a) box is fully tiled
		// b) next base cube is isolated
		// c) permuations are exhausted above
		//    the last tiled piece orientations
		piece = permuted_piece_order[last_piece_tiled_index];
		orientation = tiled_orients[last_piece_tiled_index];
		memcpy(&base_cube, &tiled_base_cubes[ last_piece_tiled_index ], sizeof(struct p_cube) );
		remove_piece_from_box( &p_d[piece][orientation], &base_cube );
		if ( nbc == BASE_CUBE_UNTILEABLE ) {
			sort_permutations_above_piece( last_piece_tiled_index );
//			printf("Removed piece %d orient %d, permuted_piece_order above untilable\n", piece, orientation );
		}
		last_piece_tiled_index--;
//		if ( nbc == BASE_CUBE_ISOLATED )
//			printf("Removed piece %d orient %d, isolated base cube\n", piece, orientation );
//		if ( nbc == BASE_CUBE_NOT_FOUND )
//			printf("Removed piece %d orient %d, box is fully tiled\n", piece, orientation );
//		dump_box(NULL);
		// goto 4 next orient to tile in piece table
	}

next_orientation:
	piece = permuted_piece_order[ last_piece_tiled_index + 1 ];
	orientation = ++tiled_orients[ last_piece_tiled_index + 1 ];

	if ( orientation == orientations[ piece ] )
	{
		// no orientations left to try, so we
		// permute the piece order from here upward
		tiled_orients[ last_piece_tiled_index + 1 ] = -1;
		if ( permute_piece_order( last_piece_tiled_index + 1 ) )
			goto next_orientation;
		// if no more permutations at this position
		// see if we can go back a piece; if not we are done!
		if ( last_piece_tiled_index > -1 ) {
			nbc = BASE_CUBE_UNTILEABLE;
			goto check_remove_last_piece;
		}
		else goto finished;
	}

	if ( put_piece_into_box( &p_d[ piece ][ orientation ], &base_cube ) )
	{
		last_piece_tiled_index++;
		memcpy(&tiled_base_cubes[ last_piece_tiled_index ], &base_cube, sizeof(struct p_cube) );
//		printf("tiled piece %2d, orient = %2d:\n", piece, orientation );
//		dump_box(NULL);
		if ( last_piece_tiled_index == nPIECES-1 )
		{
			if ( add_solution_to_solutions_table() ) {
				printf("\nFound solution %d!\n",solutions);
				dump_box(NULL);	// screen
				dump_solution( outputfile, 1 );	// file
			} else {
				duplicates++;
				printf("Discarded duplicate rotated solution %d\n", duplicates);
			}
		}
		goto next_base_cube;
	}
	// piece won't fit, try next orientation
	goto next_orientation;

finished:
	printf("\nAll permutations exhausted with %d solutions found\n", solutions);
	fprintf(outputfile,"\nAll permutations exhausted, %d unique solutions found, %d duplicate rotations discarded\n", solutions, duplicates);
	fprintf(outputfile,"Total permutations = %I64d, tiles attempted = %I64d, tiles succeeded = %I64d\n",
		permutes, tiles_attempted, tiles_succeeded );
	fclose( outputfile );
	return 0;
}
