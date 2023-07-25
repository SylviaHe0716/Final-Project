#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <time.h>

enum eColor {
    red = 2,
    yellow,
    blue,
    green,
    orange,
    magenta,
    purple,
    brown,
    darkgreen
};

/*int grid[10][10] = {
   {1,1,1,1,1,1,1,1,1,1},
   {1,0,0,2,0,0,0,0,0,1},
   {1,0,0,3,4,5,0,0,0,1},
   {1,0,0,6,0,0,0,0,0,1},
   {1,0,0,0,0,0,6,0,4,1},
   {1,0,0,0,2,0,3,0,0,1},
   {1,5,0,7,0,0,7,8,0,1},
   {1,9,0,0,0,8,0,0,0,1},
   {1,0,0,9,0,0,0,0,0,1},
   {1,1,1,1,1,1,1,1,1,1}
};*/

typedef struct
{
    int row;
    int column;
} node;

typedef struct
{
    pthread_t;
    int N;
    int **grid;
    int colorfrom;
    int colorto;
    node *startNodes;
    node *endNodes;
} task_arg;

std::vector<int **> grid_array;
std::vector<task_arg> thread_array;

void print_grid(int N, int **array) {
    printf("==================\n");
    for (int i = 0; i < N + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            printf("%d", array[i][j]);
        }
        printf("\n");
    }
}

void intPuzzle(int N, int **array, node *startNodes, node *endNodes) {
    int color;
    for (int row = 1; row < N + 1; row++) {
        for (int column = 1; column < N + 1; column++) {
            color = array[row][column];
            if (color > 0) {
                if (startNodes[color].row != -1 && startNodes[color].column != -1) {
                    endNodes[color].row = row;
                    endNodes[color].column = column;
                }
                else {
                    startNodes[color].row = row;
                    startNodes[color].column = column;
                }
            }
        }
    }
}

/*
    int grid[5][5] = {
        {1,1,1,1,1},
        {1,2,4,4,1},
        {1,0,0,3,1},
        {1,2,0,3,1},
        {1,1,1,1,1}
    };
*/
bool checkGrid(int N, int **grid) {
    print_grid(N, grid);
    for (int row = 1; row < N + 1; row++) {
        for (int column = 1; column < N + 1; column++) {
            if (grid[row][column] > 0) {
                int color = grid[row][column];
                // A grid that contains a cell of colour surrounded by other colours is invalid... 
                if (grid[row + 1][column] > 0 && grid[row + 1][column] != color)
                    if (grid[row - 1][column] > 0 && grid[row - 1][column] != color)
                        if (grid[row][column + 1] > 0 && grid[row][column + 1] != color)
                            if (grid[row][column - 1] > 0 && grid[row][column - 1] != color) {
                                printf("test1\n");
                                return false;
                            }
                // A connection line that crosses with itself is invalid
                /*if (grid[row + 1][column] == color && grid[row - 1][column] == color) {
                    if (grid[row][column + 1] == color || grid[row][column - 1] == color) {
                        printf("test2\n");
                        return false;
                    }
                }
                else if (grid[row][column + 1] == color && grid[row][column - 1] == color) {
                    if (grid[row + 1][column] == color || grid[row - 1][column] == color) {
                        printf("test3\n");

                        return false;
                    }
                }*/
            }
        }
    }
    printf("test4\n");
    return true;
}

bool solved(int N, int **grid, node *startNodes, node *endNodes) {
    for (int color = 2; color < N + 2; color++) {
        if (startNodes[color].column != endNodes[color].column || startNodes[color].row != endNodes[color].row)
            return false;
    }
    for (int row = 1; row < N + 1; row++) {
        for (int column = 1; column < N + 1; column++) {
            if (grid[row][column] == 0)
                return false;
        }
    }
    return true;
}

int abs(int a, int b) {
    if (a > b) {
        return a - b;
    }
    else {
        return b - a;
    }
}

bool solvePuzzle(int N, int **grid, node *startNodes, node *endNodes, int colorfrom, int colorto) {

    //Applying heuristics to see if the grid can be discarded at this stage, this will save a lot of steps
    if (!checkGrid(N, grid)) {
      return false;
    }
    
    //Is the grid fully solved?
    if (solved(N, grid, startNodes, endNodes)) {
      return true;
    }

    //Let's investigate the next move!
    //for color in startNodes:
    for (int color = colorfrom; color <= colorto && color < N + 2; color++) {
        node *startNode = &startNodes[color];
        node *endNode = &endNodes[color];
        /**TODO**/
        //push the node to stack
        //node newNode;
        //newNode.row = startNode->row;
        //newNode.column = startNode->column;
        //path_stack.push_back(startNodes[color]);
        /**END**/
        printf("%d > %d, %d \n", color, startNode->row, startNode->column);
        //Check if the dots from this color are already connected
        //If not let's find out in which direction to progress...
        //empty vector
        std::vector<std::string> directions;
        if (abs(endNode->row,startNode->row) + abs(endNode->column,startNode->column) > 0) {

            if (abs(endNode->row,startNode->row) + abs(endNode->column,startNode->column) == 1) {
                directions.push_back("connect");
            }
            if (grid[startNode->row][startNode->column +1 ] == 0) {
                //directions.append("right")
                directions.push_back("right"); //Lower Priority!
            }
            if (grid[startNode->row][startNode->column - 1] == 0) {
                //directions.append("left")
                directions.push_back("left"); //Lower Priority!
            }
            if (grid[startNode->row + 1][startNode->column] == 0) {
                //directions.append("down")
                directions.push_back("down"); //Lower Priority!
            }
            if (grid[startNode->row - 1][startNode->column] == 0) {
                //directions.append("up")
                directions.push_back("up"); //Lower Priority!
            }

            if (directions.size() == 0 ) {
                printf("directions.size = 0\n");
                return false;
            }
        
            //Let's investigate possible moves in different directions...  
            //for direction in directions:
            for (int i = 0; i < directions.size(); i++) {
                if (directions[i] =="connect") {
                    int column = startNode->column;
                    int row = startNode->row;
                    startNode->column = endNode->column;
                    startNode->row = endNode->row;
                    printf("connect\n");
                    if (solvePuzzle(N, grid, startNodes, endNodes, colorfrom, colorto)) {
                        return true;
              	    } else {
                        //backtrack...
                        startNode->column = column;
                        startNode->row = row;
                    }
                } else if (directions[i] =="right") {
                    startNode->column += 1;
                    grid[startNode->row][startNode->column] = color;         
                    printf("right\n");
                    if (solvePuzzle(N, grid, startNodes, endNodes, colorfrom, colorto)) {
                        return true;
              	    } else {
                        //backtrack...
                        grid[startNode->row][startNode->column] = 0;
                        startNode->column -= 1;
            	    }          
                } else if (directions[i] == "left") {
                    startNode->column -= 1;
                    grid[startNode->row][startNode->column] = color;
                    printf("left\n");
                    if (solvePuzzle(N, grid, startNodes, endNodes, colorfrom, colorto)) {
                        return true;
                    } else {
                        //backtrack...
                        grid[startNode->row][startNode->column] = 0;
                        startNode->column += 1;
                    }
                } else if (directions[i]=="up") {
                    startNode->row -= 1;
                    grid[startNode->row][startNode->column]=color;
                    printf("up\n");
                    if (solvePuzzle(N, grid, startNodes, endNodes, colorfrom, colorto)) {
                        return true;
                    } else {
                        //backtrack...
                        grid[startNode->row][startNode->column] = 0;
                        startNode->row += 1;
                    }
                } else if (directions[i] == "down") {
                    startNode->row += 1;
                    grid[startNode->row][startNode->column] = color;
                    printf("down\n");
                    if (solvePuzzle(N, grid, startNodes, endNodes, colorfrom, colorto)) {
                        return true;
                    } else {
                        //backtrack...
                        grid[startNode->row][startNode->column] = 0;
                        startNode->row -= 1;
                   }
                }
            }
            //As none of the moves lead to a solution... 
            return false;
        }
        else {
            /**TODO**/
            //print the path from stack
            printf("color: %d\n", color);
            int **array;
            array = new int *[N + 2];
            for (int i = 0; i < N + 2; i++) {
                array[i] = new int[N + 2];
            }
            for (int i = 0; i < N + 2; i++) {
                for (int j = 0; j < N + 2; j++) {
                    array[i][j] = grid[i][j];
                }
            }
            grid_array.push_back(array);
            /**END**/
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    int N = 3;
    int grid[5][5] = {
        {1,1,1,1,1},
        {1,2,4,4,1},
        {1,0,0,3,1},
        {1,2,0,3,1},
        {1,1,1,1,1}
    };

    
    int **array;
    array = new int *[N + 2];
    for (int i = 0; i < N + 2; i++) {
        array[i] = new int[N + 2];
    }
    for (int i = 0; i < N + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            array[i][j] = grid[i][j];
        }
    }

    node *startNodes;
    node *endNodes;

    startNodes = new node[N + 2];
    endNodes = new node[N + 2];

    for (int i = 0; i < N + 2; i++) {
        startNodes[i].row = -1;
        startNodes[i].column = -1;
        endNodes[i].row = -1;
        endNodes[i].column = -1;
    }

    intPuzzle(3, array, startNodes, endNodes);

    /*
    for (int i = 0; i < N + 2; i++) {
        printf("%d, %d\n", startNodes[i].row, startNodes[i].column);
        printf("%d, %d\n", endNodes[i].row, endNodes[i].column);
    }
    */
    
    // if (solvePuzzle(3, array, red, red)) {
    // if (solvePuzzle(3, array, yellow, yellow)) {
    // if (solvePuzzle(3, array, blue, blue)) {
    if (solvePuzzle(3, array, startNodes, endNodes, red, red)) {
        printf("Problem Solved!\n");
        print_grid(N, array);
    }
    else {
        printf("This problem cannot be solved!");
        print_grid(N, array);
    }

    /*for (int i = 0; i < grid_array.size(); i++) {
        pthread_t new_thread;
        int rc = pthread_create(&new_thread, NULL, solvePuzzle, (void *))
    }*/

    for (int i = 0; i < grid_array.size(); i++) {
        printf("=======================\n");
        for (int j = 0; j < N + 2; j++) {
            for (int k = 0; k < N + 2; k++) {
                printf("%d", grid_array[i][j][k]);
            }
            printf("\n");
        }
        for (int j = 0; j < N + 2; j++) {
            delete [] grid_array[i][j];
        }
    }

    for (int i = 0; i < N + 2; i++) {
        delete [] array[i];
    }
    delete [] array;
    delete [] startNodes;
    delete [] endNodes;
    return 0;
}
