# ApolloDianaMaze

This project includes one C++ file, maze.cpp. This file contains code that models a graph using an adjacency matrix, and traverses through the
    graph using DFS. The project reads in the input file, first saving the number of rows and columns, then reading in the arrows. All
    arrows in the input file are of the format COLOR-DIRECTION, with an O representing the goal state. The starting point is always the top left corner. The path     
    taken to the goal state is printed to output.txt with each step being represented as the number of spaces in the direction taken.

    When testing this program, manually replace the input file name in line 114. The output file name will always be output.txt.

    To compile the program, run the following command:
        g++ -std=c++11 maze.cpp -o maze

    After compiling successfully, run the following command to run the program:
        ./maze
