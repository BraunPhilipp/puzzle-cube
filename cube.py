# bInArI
import random
import time

# cube size also describes maximum part size
HEIGHT = 4
WIDTH = 4
LENGTH = 4

SIZE = [WIDTH, LENGTH, HEIGHT]

ROW = WIDTH
LAYER = WIDTH * LENGTH
CUBE = HEIGHT * WIDTH * LENGTH
LAYER_NO = int(CUBE/LAYER)

# puzzle cube parts
parts_raw = [

[1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[2,2,2,0,2,0,0,0,0,0,0,0,0,0,0,0,
0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[3,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,
0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[0,4,0,0,4,4,4,0,0,0,0,0,0,0,0,0,
0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[5,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[6,0,0,0,6,6,6,0,0,6,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[7,0,0,0,7,7,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[8,8,8,0,0,0,8,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[9,0,0,0,9,9,0,0,0,9,9,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[10,10,10,0,10,0,0,0,0,0,0,0,0,0,0,0,
10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[11,11,11,0,11,0,0,0,0,0,0,0,0,0,0,0,
0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[0,12,12,0,12,12,0,0,0,0,0,0,0,0,0,0,
0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],

[0,13,0,0,13,13,13,0,0,13,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

];

# create brain **
def create():
    return [0] * CUBE

# positive n == left shift
def shift(seq, n):
    n = n % len(seq)
    # return seq[n:] + seq[:n]
    return seq[n:] + [0] * n
# or_operator boolean arrays
def or_operator(arr1, arr2):
    arr = [0] * CUBE
    for pos in range(CUBE):
        if ( arr1[pos] != 0 or arr2[pos] != 0 ):
            arr[pos] = 1
    return arr

# and_operator boolean arrays
def and_operator(arr1, arr2):
    arr = [0] * CUBE
    for pos in range(CUBE):
        if ( arr1[pos] != 0 and arr2[pos] != 0 ):
            arr[pos] = 1
    return arr

# inv_operator boolean arrays
def inv_operator(arr):
    cpy = [0] * len(arr)
    for i in range(CUBE):
        if (arr[i] == 0):
            cpy[i] = 1
        else:
            cpy[i] = 0
    return cpy

# get surrounding points
def wave(brain):
    # shift one right
    arr = shift( brain, -1 )
    # shift one left
    arr = or_operator(arr, shift( brain, +1 ))
    # shift one row up
    arr = or_operator(arr, shift( brain, -ROW ))
    # shift one row down
    arr = or_operator(arr, shift( brain, +ROW ))
    # shift one layer up
    arr = or_operator(arr, shift( brain, -LAYER ))
    # shift one layer down
    arr = or_operator(arr, shift( brain, +LAYER ))

    arr = and_operator(inv_operator(brain), arr)
    return arr

def binary_to_vector(arr, layer=LAYER, row=ROW): #***
    vec = []
    for i in range(len(arr)):
        if (arr[i] != 0):
            z = int((i+1) / layer)
            y = int(((i+1) - (z*layer)) / row)
            x = int(i - (z*layer) - (y*row))
            vec.append([x,y,z])
    return vec

def vector_to_binary(vec): #***
    # correct wrong positioning!!!
    for pnt in range(len(vec)):
        pnt = vec[pnt]
        for coord in range(3):
            if (pnt[coord] < 0):
                vec = move_vector(vec, -pnt[coord], coord)
            if (pnt[coord] > SIZE[coord]-1):
                vec = move_vector(vec, -(pnt[coord]-SIZE[coord]+1), coord)
    # binary conversion
    arr = [0] * CUBE
    for pnt in vec:
        pos = pnt[0] + pnt[1]*ROW + pnt[2]*LAYER
        arr[pos] = 1
    return arr

# visualize **
def visualize(arr):
    for i in range(int(LAYER/ROW)):
        for j in range(int(CUBE/LAYER)):
            for k in range(ROW):
                alpha = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
                letter = alpha[arr[j*LAYER+k+i*ROW]]
                print(letter, end='')
            print(' ', end='')
        print(' ')

########################################### Create Puzzle Combinations
def move_vector(vec, steps, coord):
    vec2 = []
    for pnt in vec:
        pnt[coord] += steps
        vec2.append(pnt)
    return vec2

# rotate pi/2
def rotate(vec, drt):
    vec2 = []
    if (drt == 0):
        for pnt in vec:
            x = -pnt[1]
            y = pnt[0]
            z = pnt[2]
            vec2.append([x,y,z])
    else:
        for pnt in vec:
            x = pnt[0]
            y = -pnt[2]
            z = pnt[1]
            vec2.append([x,y,z])
    return vec2

def equal(arr1, arr2):
    for i in range(len(arr1)):
        for j in range(len(arr2)):
            if (arr1[i] != arr2[j]):
                return False
    return True

def remove_duplicate(arr):
    unique = []
    for i in range(len(arr)):
        duplicate = 0
        if (len(unique) > 0):
            for j in range(len(unique)):
                if (arr[i][0] == unique[j][0]):
                    duplicate = 1
        if (duplicate == 0):
            unique.append(arr[i])
    return unique

def combinations(part):
    combi = []
    # correct positioning
    root = binary_to_vector(part)
    root = vector_to_binary(root)
    root = binary_to_vector(root)

    for i in range(-WIDTH,WIDTH):

        for j in range(-LENGTH,LENGTH):

            for k in range(-HEIGHT,HEIGHT):

                vec = root
                vec = move_vector(vec, i, 0)
                vec = move_vector(vec, j, 1)
                vec = move_vector(vec, k, 2)

                for xy in range(4):
                    vec = rotate(vec, 0)
                    for yz in range(4):
                        vec = rotate(vec,1)
                        binary = vector_to_binary(vec)
                        combi.append([binary, vec])
    # check for double arrays
    combi = remove_duplicate(combi)
    return combi

##############################################################

# return interception amount
def coverage(part, wave):
    density = 0
    for i in range(len(part[0])):
        if (part[0][i] != 0 and wave[i] != 0):
            density += 1
    # edges = 0
    layer = 0
    for i in range(len(part[1])):
        for j in range(len(part[1][i])):
            # if (part[1][i][j] == ROW-1 or part[1][i][j] == 0):
            #     edges += 1
            layer += part[1][i][j]
    return int(density + layer)

def overlap(part1, part2):
    for i in range(len(part1)):
        if (part1[i] != 0 and part2[i] != 0):
            return 1
    return 0

# find parts containing only points out of block and with high wave coverage
def best_fit(parts, wave, block, placed):
    sort = []
    for part_no in range(len(parts)):
        if (part_no not in placed):
            for comb_no in range(len(parts[part_no][0])):
                part = parts[part_no][0][comb_no]
                if (overlap(part[0], block) == 0):
                    ##sort.append([part, parts[part_no][1], 0])
                    sort.append([part[0], parts[part_no][1], coverage(part, wave), part_no])
                comb_no += 1
        part_no += 1

    sort = sorted(sort, key = lambda x: -(x[2]))
    sort_length = len(sort)
    if (sort_length > 1):
        # select random
        rand_num = int( (sort_length-1)/random.randint(1,sort_length-1) )
        return sort[rand_num-1]
    elif (sort_length == 1):
        return sort[0]
    else:
        return None

def add_part(brain, part, part_no):
    for i in range(len(brain)):
        if (part[i] != 0):
            brain[i] = part_no
    return brain

start = time.time()
found = 0
def loop(parts, brain, placed):
    global start, found
    insert = 12
    while True:
        part = best_fit(parts, wave(brain), brain, placed)
        insert -= 1
        ####print(parts)
        if (part != None):
            brain = add_part(brain, part[0], part[1]+1)
            placed.append(part[3]) # position in array (not part_no)
            if (insert < 4):
                visualize(brain)
                print("Remaining " + str(insert))
                found += 1
                print(str((time.time()-start)/found))
        else:
            break

# # init parts
# parts = []
# for part_no in range(len(parts_raw)):
#     part = parts_raw[part_no]
#     part = binary_to_vector(part)
#     part = vector_to_binary(part)
#     parts.append([combinations(part), part_no])
#
# #starting part
# while True:
#     for part_no in range(len(parts)):
#         for cmb_no in range(len(parts[part_no][0])):
#             # reset variables
#             brain = create()
#             placed = []
#             parts_cpy = parts[:]
#             # add part to brain
#             brain = add_part(brain, parts_cpy[part_no][0][cmb_no][0], parts_cpy[part_no][1]+1)
#             placed.append(part_no)
#             loop(parts_cpy, brain, placed)


for part_no in range(len(parts_raw)):
    part = parts_raw[part_no]
    part = binary_to_vector(part)
    print(part)
