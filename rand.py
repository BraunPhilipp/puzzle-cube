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

# create brain
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
        if ( arr1[pos] == 1 or arr2[pos] == 1 ):
            arr[pos] = 1
    return arr

# and_operator boolean arrays
def and_operator(arr1, arr2):
    arr = [0] * CUBE
    for pos in range(CUBE):
        if ( arr1[pos] == 1 and arr2[pos] == 1 ):
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

def binary_to_vector(arr, layer=LAYER, row=ROW):
    vec = []
    for i in range(len(arr)):
        if (arr[i] != 0):
            z = int((i+1) / layer)
            y = int(((i+1) - (z*layer)) / row)
            x = int(i - (z*layer) - (y*row))
            vec.append([x,y,z])
    return vec

def vector_to_binary(vec):
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

# visualize
def visualize(arr):
    for i in range(int(LAYER/ROW)):
        for j in range(int(CUBE/LAYER)):
            for k in range(ROW):
                alpha = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
                letter = alpha[arr[j*LAYER+k+i*ROW]]
                print(letter, end='')
            print(' ', end='')
        print(' ')

def equal(arr1, arr2):
    for i in range(len(arr1)):
        for j in range(len(arr2)):
            if (arr1[i] != arr2[j]):
                return False
    return True

def remove_duplicate(arr):
    unique = []

    for i in range(len(arr)):
        if (arr[i] not in unique):
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
                        binari = vector_to_binary(vec)
                        combi.append(binari)
    # check for double arrays
    combi = remove_duplicate(combi)
    return combi

# return interception amount
def coverage(part, wave):
    intercept = 0
    for i in range(len(part)):
        if (part[i] != 0 and wave[i] != 0):
            intercept += 1
    return intercept

# find parts containing only points out of block and with high wave coverage
def best_fit(parts, block):
    sort = []
    for part_no in range(len(parts)):
        for comb_no in range(len(parts[part_no][0])):
            part = parts[part_no][0][comb_no]
            if (coverage(part, block) == 0):
                sort.append([part, parts[part_no][1]])

    if (len(sort) > 1):
        return random.choice(sort)
    else:
        return None

def add_part(brain, part, part_no):
    for i in range(len(brain)):
        if (part[i] != 0):
            brain[i] = part_no
    return brain

def remove_part(parts, part_no):
    for i in range(len(parts)):
        if (parts[i][1] == part_no):
            del parts[i]
            return parts

start = time.time()
found = 0
def loop(parts, brain):
    global start, found
    while True:
        part = best_fit(parts, brain)
        if (part != None):
            brain = add_part(brain, part[0], part[1]+1)
            parts = remove_part(parts, part[1])
            if (len(parts) < 5):
                visualize(brain)
                print("Remaining " + str(len(parts)))
                found += 1
                print(str((time.time()-start)/found))
        else:
            break

# init parts
parts = []
for part_no in range(len(parts_raw)):
    part = parts_raw[part_no]
    part = binary_to_vector(part)
    part = vector_to_binary(part)
    parts.append([combinations(part), part_no])

# starting part
while True:
    for part_no in range(len(parts)):
        for cmb_no in range(len(parts[part_no][0])):
            # reset variables
            brain = create()
            parts_cpy = parts[:]
            # add part to brain
            brain = add_part(brain, parts_cpy[part_no][0][cmb_no], parts_cpy[part_no][1]+1)
            parts_cpy = remove_part(parts_cpy, part_no)

            loop(parts_cpy, brain)
