# -*- coding: utf-8 -*-
# Author: Philipp Braun <braunphilipp@protonmail.com>

import numpy as np

def calc_srnd(dims, start, stop):
    '''
    Creates an Array of Possible surrounding Points over Recursion.
    The Array itself describes a Permutation Matrix and needs to be
    added to the original Point/Object.
    '''
    if dims == 1:
        return map(lambda x: [x], range(start, stop))
    else:
        # Recursion foreach Dimension
        p = calc_srnd(dims - 1, start, stop)
        a = []
        for i in range(start, stop):
            a += [j + [i] for j in p]
        return a

def calc_frms(prt, bnd):
    '''
    Returns possible Frame Points of n-dimensional Parts.

    prt defines a Point Array
    sze describes Box Boundaries using an integer array
    '''

    # Holds all Frame Points
    frm = []

    # Gets all possible surrounding Points
    for pnt in prt:
        # Frame Combinations
        adr = calc_srnd(len(pnt), -1, +2)
        # Iterate through all possible Combinations
        for ad in adr:
            frm.append((np.array(pnt) + np.array(ad)))

    # Existing Points overwrite Border Points
    tmp = []
    for i in range(len(frm)):
        # Check against Points of Part
        for j in range(len(prt)):
            if np.array_equal(frm[i], prt[j]):
                break;
            if j == ( len(prt) - 1 ):
                tmp.append(frm[i])
    # Replace Frame with Temporary Array
    frm = tmp

    # Remove Duplicates - tmp unique list
    tmp = [frm[0]]
    # Check each Point
    for i in range(len(frm)):
        # Check against unique Point List
        for j in range(len(tmp)):
            if np.array_equal(frm[i], tmp[j]):
                break;
            if j == ( len(tmp) - 1 ):
                tmp.append(frm[i])
    # Clear Frame for next Operation
    del frm[:]
    # Remove Boundary Entries
    for pnt in tmp:
        chx = 0
        for coord in range(len(pnt)):
            if coord < 0 or coord > (bnd[coord] - 1):
                chx = 1
        if chx == 0:
            frm.append(pnt)
    # Return Entire Frame as Array
    return frm

# Define XY Rotation Matrix - 4 possible solutions
xy = np.matrix([[0, -1], [1, 0]])

def calc_cmbs(prt, bnd):
    '''
    Calculate all Possible Array Combinations
    '''
    prt = []
    # Loop Operation for each piece
    for i in range(4):
        # Rotate each piece 3 times
        p = np.dot(p, xy)
        # Get Rotated Object into inner boundaries
        p = p + np.array(p.min(axis=0) * (-1))

        # Move rotated part
        min_pos = np.squeeze(np.asarray([p.min(axis=0)]))
        max_pos = np.squeeze(np.asarray([p.max(axis=0)]))

        # Index from 0 to 3 (only max - 1)
        for i in range(-min_pos[0], bnd[0]-max_pos[0]):
            for j in range(-min_pos[1], bnd[1]-max_pos[1]):
                # Create Addition Matrix
                ad_mat = []
                for n in range(len(p)):
                    ad_mat = [i, j]
                # Create new Matrix
                tmp = np.matrix(p) + np.matrix(ad_mat)
                # Save temporary computed parts
                prt.append(np.asarray(tmp)) # np.squeeze(np.asarray(p))

    # Check part for unique positions
    new_prt = [prt[0]]
    for i in range(0,len(prt)):
        for j in range(0, len(new_prt)):
            if np.array_equal(prt[i], new_prt[j]):
                break;
            if j == ( len(new_prt) - 1 ):
                new_prt.append(prt[i])
    # Add all unique part combinations
    return new_prt

def calc_rtns(prt):
    ####
    # Create rotation matrices
    # Move to positive values ****
    dim = len(prt[0])
    # dim - 1 possible rotations
    N = 10
    a = np.random.rand(N,N)
    b = np.zeros((N,N+1))
    b[:,:-1] = a


test_part_2d = [[1,1]]
test_part_3d = [[2,2,2]]
# print calc_frame(test_part_2d, [4,4])

calc_rtns(test_part_3d)
