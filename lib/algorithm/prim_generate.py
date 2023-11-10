#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jan 25 19:20:53 2022

@author: wst
"""

import random
import numpy as np
from matplotlib import pyplot as plt
import matplotlib.cm as cm
import json

# num_rows = int(input("Rows: "))  # number of rows
# num_cols = int(input("Columns: "))  # number of columns
num_rows, num_cols = 15, 15

# The array M is going to hold the array information for each cell.
# The first four coordinates tell if walls exist on those sides
# and the fifth indicates if the cell has been visited in the search.
# M(LEFT, UP, RIGHT, DOWN, CHECK_IF_VISITED)
M = np.zeros((num_rows, num_cols, 5), dtype=np.uint8)

# Break the walls of enter and exit
M[0, 0, 0] = 1
M[num_rows - 1, num_cols - 1, 2] = 1

# Set starting row and column.
# The possibility is the stack of visited locations.
possibility = [(0, 0)]

# Trace a path though the cells of the maze and open walls along the path.
# We do this with a while loop, repeating the loop until there is no possibility,
# which would mean we backtracked to the initial start.

# Imagination(wst):
# Image that there is a tree which has many branches,
# and all cells in the array will be connected by special branches.
# Now there is a special branch, growing strongly and getting the exit cell.

# Attention(wst):
# The nearby cell which have not been visited is added to `possibility`,
# but can not be connected by other cell either connect to them,
# (by the visited status value `2`) unless it is choiced from `check`.
# Then it can connect to other cell(s) which have been visited.
# This function ensures that a cell can only connected by one branch.

while possibility:
    # random choose a candidate cell from the cell set histroy
    r, c = random.choice(possibility)
    M[r, c, 4] = 1  # designate this location as visited

    possibility.remove((r, c))
    check = []
    # If the randomly chosen cell has multiple edges
    # that connect it to the existing maze.
    if c > 0:  # the visit state of left cell (for example)
        if M[r, c - 1, 4] == 1:  # If this cell was visited,
            check.append("L")  # it can be choiced as direction.
        elif M[r, c - 1, 4] == 0:  # else if it has not been visited...
            possibility.append((r, c - 1))
            M[r, c - 1, 4] = 2
    if r > 0:  # the visit state of up cell
        if M[r - 1, c, 4] == 1:
            check.append("U")
        elif M[r - 1, c, 4] == 0:
            possibility.append((r - 1, c))
            M[r - 1, c, 4] = 2
    if c < num_cols - 1:  # the visit state of right cell
        if M[r, c + 1, 4] == 1:
            check.append("R")
        elif M[r, c + 1, 4] == 0:
            possibility.append((r, c + 1))
            M[r, c + 1, 4] = 2
    if r < num_rows - 1:  # the visit state of down cell
        if M[r + 1, c, 4] == 1:
            check.append("D")
        elif M[r + 1, c, 4] == 0:
            possibility.append((r + 1, c))
            M[r + 1, c, 4] = 2

    # Select one of these edges as direction at random,
    # and break the walls between these two cells.
    if len(check):
        move_direction = random.choice(check)  # Select a direction
        # Break the walls.
        if move_direction == "L":
            M[r, c, 0] = 1
            c = c - 1
            M[r, c, 2] = 1
        if move_direction == "U":
            M[r, c, 1] = 1
            r = r - 1
            M[r, c, 3] = 1
        if move_direction == "R":
            M[r, c, 2] = 1
            c = c + 1
            M[r, c, 0] = 1
        if move_direction == "D":
            M[r, c, 3] = 1
            r = r + 1
            M[r, c, 1] = 1

# The array image is going to be the output image to display
image = np.zeros((num_rows * 10, num_cols * 10), dtype=np.uint8)

import numpy as np

# ... (your existing code)

# Create a binary matrix to represent the maze

# ... (your existing code)

# Initialize the matrix
maze_matrix = np.zeros((num_rows * 2 + 1, num_cols * 2 + 1), dtype=np.uint8)

# Fill the matrix based on the maze information
for row in range(num_rows):
    for col in range(num_cols):
        maze_matrix[2 * row + 1, 2 * col + 1] = 1  # Set the cell itself

        if M[row, col, 0] == 1:  # Check left wall
            maze_matrix[2 * row + 1, 2 * col] = 1
        if M[row, col, 1] == 1:  # Check up wall
            maze_matrix[2 * row, 2 * col + 1] = 1
        if M[row, col, 2] == 1:  # Check right wall
            maze_matrix[2 * row + 1, 2 * col + 2] = 1
        if M[row, col, 3] == 1:  # Check down wall
            maze_matrix[2 * row + 2, 2 * col + 1] = 1

# Print or use the maze_matrix as needed
json_file_path = "maze_data.json"

data={
    "start":[0,0],
    "maze":maze_matrix.tolist(),
}
print(data)
# Write maze_list to the JSON file
with open(json_file_path, "w") as json_file:
    json.dump(data, json_file)

