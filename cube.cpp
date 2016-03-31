//
//  main.cpp
//  PuzzleCube
//
//  Created by Philipp Braun on 3/28/16.
//  Copyright © 2016 Philipp Braun. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>

#define HEIGHT 4
#define WIDTH 4
#define LENGTH 4

#define ROW WIDTH
#define LAYER (WIDTH * LENGTH)
#define CUBE (WIDTH * LENGTH * HEIGHT)
#define LAYER_NO (CUBE / LAYER)

int SIZE[3] = {WIDTH, LENGTH, HEIGHT};

// puzzle cube parts
std::vector<std::vector<int>> parts_raw = {
    
    {1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
        0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {2,2,2,0,2,0,0,0,0,0,0,0,0,0,0,0,
        0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {3,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,
        0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {0,4,0,0,4,4,4,0,0,0,0,0,0,0,0,0,
        0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {5,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {6,0,0,0,6,6,6,0,0,6,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {7,0,0,0,7,7,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {8,8,8,0,0,0,8,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {9,0,0,0,9,9,0,0,0,9,9,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {10,10,10,0,10,0,0,0,0,0,0,0,0,0,0,0,
        10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {11,11,11,0,11,0,0,0,0,0,0,0,0,0,0,0,
        0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {0,12,12,0,12,12,0,0,0,0,0,0,0,0,0,0,
        0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
    {0,13,0,0,13,13,13,0,0,13,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int random(int min, int max) // [min, max)
{
    static bool first = true;
    if (first)
    {
        srand((unsigned)time(NULL)); //seeding first time only
        first = false;
    }
    return min + rand() % (max - min);
}

// visualize
std::ostream &visualize(std::vector<int> array, std::ostream &out)
{
    char alpha[36] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H'};
    
    for (int i=0; i<(LAYER/ROW); ++i) {
        for (int j=0; j<(CUBE/LAYER); ++j) {
            for (int k=0; k<ROW; ++k) {
                char letter = alpha[array[j*LAYER+k+i*ROW]];
                out << letter;
            }
            out << ' ';
        }
        out << std::endl;
    }
    
    return out;
}

// binary to vector
std::vector<std::vector<int>> binary_to_vector(std::vector<int> array)
{
    std::vector<std::vector<int>> point_vector;
    
    for (int i=0; i<array.size(); ++i) {
        if (array[i] != 0) {
            int z = (int)floor( (i+1) / LAYER );
            int y = (int)floor( (i-(z*LAYER)) / ROW );
            int x = int(i - (z*LAYER) - (y*ROW));
            point_vector.push_back({x,y,z});
        }
    }

    return point_vector;
}

// move vector
std::vector<std::vector<int>> move_vector(std::vector<std::vector<int>> point_vector, const int steps, const int coordinate)
{
    for (int i=0; i<point_vector.size(); ++i) {
        point_vector[i][coordinate] += steps;
    }
    
    return point_vector;
}

// rotate vector
std::vector<std::vector<int>> rotate(std::vector<std::vector<int>> point_vector, const int direction)
{
    if (direction == 0) {
        for (int i=0; i<point_vector.size(); ++i) {
            int x = -point_vector[i][1];
            int y = point_vector[i][0];
            int z = point_vector[i][2];
            point_vector[i] = {x,y,z};
        }
    }
    else {
        for (int i=0; i<point_vector.size(); ++i) {
            int x = -point_vector[i][2];
            int y = point_vector[i][1];
            int z = point_vector[i][0];
            point_vector[i] = {x,y,z};
        }
    }

    return point_vector;
}

// vector to binary
std::vector<int> vector_to_binary(std::vector<std::vector<int>> &point_vector) // reference important for parts point
{
    // for negative values move entire vector
    for (int i=0; i<point_vector.size(); ++i) {
        for (int j=0; j<3; ++j) {
            int point = point_vector[i][j];
            if (point < 0) {
                point_vector = move_vector(point_vector, -point, j);
            }
            if (point > SIZE[j]-1) {
                point_vector = move_vector(point_vector, -(point-SIZE[j]+1), j);
            }
        }
    }
    
    // vector to binary conversion
    std::vector<int> binary_vector(CUBE, 0);
    
    for (int i=0; i<point_vector.size(); ++i) {
        int pos = point_vector[i][0] + point_vector[i][1]*ROW + point_vector[i][2]*LAYER;
        binary_vector[pos] = 1;
    }
    
    return binary_vector;
}

// enable direct point and binary map access
struct _dataCombination {
    std::vector<std::vector<int>> pointArray;
    std::vector<int> binaryArray;
};

// remove duplicate
std::vector<_dataCombination> remove_duplicate(std::vector<_dataCombination> &array)
{
    std::vector<_dataCombination> unique;
    
    for (int i=0; i<array.size(); ++i) {
        bool duplicate = false;
        
        for (int j=0; j<unique.size(); ++j) {
            if (array[i].binaryArray == unique[j].binaryArray) {
                duplicate = true;
            }
        }
        
        if (duplicate == false) {
            unique.push_back(array[i]);
        }
    }
    
    return unique;
}

// combinations
std::vector<_dataCombination> combinations(std::vector<int> array)
{
    std::vector<_dataCombination> combinations;
    
    std::vector<std::vector<int>> point_vector = binary_to_vector(array);
    
    for (int i=-WIDTH; i<=WIDTH; ++i) {
        
        for (int j=-LENGTH; j<=LENGTH; ++j) {
            
            for (int k=-HEIGHT; k<=HEIGHT; ++k) {
                
                std::vector<std::vector<int>> vector_cpy = point_vector;
                
                vector_cpy = move_vector(vector_cpy, i, 0);
                vector_cpy = move_vector(vector_cpy, j, 1);
                vector_cpy = move_vector(vector_cpy, k, 2);
                
                for (int xy=0; xy<4; ++xy) {
                    
                    vector_cpy = rotate(vector_cpy, 0);
                    
                    for (int xz=0; xz<4; ++xz) {
                        
                        vector_cpy = rotate(vector_cpy, 1);
                        
                        std::vector<int> binary = vector_to_binary(vector_cpy);
                        
                        _dataCombination dataCombination;
                        
                        dataCombination.pointArray = vector_cpy;
                        dataCombination.binaryArray = binary;
                        
                        combinations.push_back(dataCombination);
                        
                    }
                }
            }
        }
    }

    combinations = remove_duplicate(combinations);

    return combinations;
}

// parts initialization
std::vector<std::vector<_dataCombination>> create_parts()
{
    std::vector<std::vector<_dataCombination>> parts;
    
    for(int i=0; i<parts_raw.size(); ++i) {
        std::vector<std::vector<int>> part_pnt = binary_to_vector(parts_raw[i]);
        
        std::vector<int> part_bin = vector_to_binary(part_pnt);
        
        std::vector<_dataCombination> cmb = combinations(part_bin);
        
        parts.push_back(cmb);
    }
    
    return parts;
}

// left shift
std::vector<int> shift(std::vector<int> binaryMap, const int shift)
{
    if (shift > 0) {
        std::rotate(binaryMap.begin(), binaryMap.begin()+shift, binaryMap.end());
        std::fill(binaryMap.end()-shift, binaryMap.end(), 0);
    }
    if (shift < 0) {
        std::rotate(binaryMap.begin(), binaryMap.end()+shift, binaryMap.end());
        std::fill(binaryMap.begin(), binaryMap.begin()-shift, 0);
    }

    return binaryMap;
}

// or operation
void or_operator(std::vector<int> *arr1, const std::vector<int> arr2) // reference saves copy
{
    for (int i=0; i<arr1->size(); ++i)
    {
        if ( (*arr1)[i] != 0 or arr2[i] != 0 )
            (*arr1)[i] = 1;
        else
            (*arr1)[i] = 0;
    }
}

// and operation
void and_operator(std::vector<int> *arr1, const std::vector<int> &arr2) // reference saves copy
{
    for (int i=0; i<arr1->size(); ++i)
    {
        if ( (*arr1)[i] != 0 and arr2[i] != 0 ) {
            (*arr1)[i] = 1;
        }
        else {
            (*arr1)[i] = 0;
        }
    }
}

// inverse operator
std::vector<int> inv_operator(std::vector<int> arr)
{
    for (int i=0; i<arr.size(); ++i)
    {
        if ( arr[i] != 0 )
            arr[i] = 0;
        else
            arr[i] = 1;
    }
    
    return arr;
}

// wave
std::vector<int> *wave(std::vector<int> &arr)
{
    // shift one left
    std::vector<int> *cpy = new std::vector<int>;
    
    // might consider diagonal edges as well later ##
    *cpy = shift( arr, +1);
    or_operator(cpy, shift( arr, -1 ));
    
    // diagonal
    or_operator(cpy, shift(shift(arr, -ROW),+1));
    or_operator(cpy, shift(shift(arr, -ROW),-1));
    or_operator(cpy, shift(shift(arr, +ROW),-1));
    or_operator(cpy, shift(shift(arr, +ROW),+1));
    
    or_operator(cpy, shift( arr, -ROW ));
    or_operator(cpy, shift( arr, ROW ));
    
    or_operator(cpy, shift( arr, -LAYER ));
    or_operator(cpy, shift( arr, LAYER ));
    
    and_operator(cpy, inv_operator(arr));
    
    return cpy;
}

// coverage
int coverage(_dataCombination &part, std::vector<int> *wave)
{
    int density = 0;
    
    for (int i=0; i<part.binaryArray.size(); ++i) {
        if (part.binaryArray[i] != 0 and (*wave)[i] != 0) {
            density += 1;
        }
    }
    
    int layer = 0;
    
    for (int i=0; i<part.pointArray.size(); ++i) {
        for (int j=0; j<part.pointArray[i].size(); ++j) {
            layer += part.pointArray[i][j];
        }
    }
    
    return density + layer;
}

// overlap
bool const overlap(const std::vector<int> &part1, const std::vector<int> &part2)
{
    for (int i=0; i<part1.size(); ++i) {
        if (part1[i] != 0 and part2[i] != 0) {
            return true;
        }
    }
    
    return false;
}

// add part
void add_part(std::vector<int> &cube, const std::vector<std::vector<int>> &part, const int &part_no)
{
    for (int i=0; i<part.size(); ++i) {
        cube[part[i][0] + ROW * part[i][1] + LAYER * part[i][2]] = part_no;
    }
}

struct _sort {
    _dataCombination part;
    int coverage;
    int part_no;
};

// coverage sort
struct _coverageSort
{
    bool operator()( const _sort &lhs, const _sort &rhs) const {
        
        return lhs.coverage > rhs.coverage;
    }
};

// best fit
_sort best_fit(std::vector<std::vector<_dataCombination>> const &parts, std::vector<int> *wave, std::vector<int> const &cube, std::vector<int> &not_placed)
{

    std::vector<_sort> sort;
    
    for (int i=0; i<not_placed.size(); ++i) {
        for (int j=0; j<parts[not_placed[i]].size(); ++j) {
            
            _dataCombination part = parts[not_placed[i]][j];
            
            if (overlap(part.binaryArray, cube) == false) {
                
                // serve lesser combinations first
                // int lesser = (int)(250 / (int)parts[not_placed[i]].size());
                
                sort.push_back({part, coverage(part, wave), not_placed[i]});
            }
        }
    }
    
    std::sort(sort.begin(), sort.end(), _coverageSort());
    int sort_length = (int)sort.size();
    
    if (sort_length > 1) {
        int rand_num = floor( sort_length / (1 + random(0, sort_length-1)) );
        
        return sort[rand_num-1];
    }
    else if (sort_length == 1) {
        return sort[0];
    }
    else {
        _sort sort_error = {parts[0][0], -1, -1};
        return sort_error;
    }
}

// loop
void loop(std::vector<int> &cube, std::vector<std::vector<_dataCombination>> &parts, std::vector<int> &not_placed, int &start, int &found)
{
    for (int i=11; i>=0; --i) {
        
        std::vector<int> *wave_pnt = wave(cube);
        _sort part = best_fit(parts, wave_pnt, cube, not_placed);
        
        wave_pnt->clear();
        delete wave_pnt;
        
        if (part.part_no != -1) {
            add_part(cube, part.part.pointArray, part.part_no);
            not_placed.erase(std::remove(not_placed.begin(), not_placed.end(), part.part_no), not_placed.end());
        }
        else {
            break;
        }
        
        if (i <= 1) {
            ++found; // new combination
            
            // write to file
            std::ofstream fs;
            fs.open("/Users/philippbraun/Desktop/PuzzleCube/PuzzleCube/cube.dat", std::ios::out | std::ios::app);
            
            if (fs.is_open()) {
                visualize(cube, fs);
                fs << "Remaining " << i << std::endl;
                fs << (time(NULL) - start) / found << " sec" << std::endl;
                fs << std::endl;
                fs.close();
            }
            else {
                std::cout << "Unable to open file";
            }
            
            visualize(cube, std::cout);
            std::cout << "Remaining " << i << std::endl;
            std::cout << (time(NULL) - start) / found << " sec" << std::endl;
            std::cout << std::endl;
            
        }
    }
}

// main
int main(int argc, const char * argv[]) {
    
    std::vector<std::vector<_dataCombination>> parts = create_parts();

    int start = (int)time(NULL);
    int found = 0;
    
    while (true) {
        for (int i=0; i<parts.size(); ++i) {

            for (int j=0; j<parts[i].size(); ++j) {
                
                std::vector<int> cube(CUBE, 0);
                std::vector<int> not_placed = {1,2,3,4,5,6,7,8,9,10,11,12,13};
                
                add_part(cube, parts[i][j].pointArray, not_placed[i]);
                not_placed.erase(std::remove(not_placed.begin(), not_placed.end(), not_placed[i]), not_placed.end());
                
                loop(cube, parts, not_placed, start, found);

            }
        }
    }
    
}
