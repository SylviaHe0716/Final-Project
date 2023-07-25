#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>
#include <vector>
#include <string>
#include <time.h>
#include <math.h>
#include <stack>
#include <bitset>
#include <iostream>
#define DN 25

double START = clock();
//update DN/N/grid/color from when puzzle changed
using namespace std;
long long int s = 0;
int **result;
//vector<int> solution;
vector<long long int> Solutions[7];

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

node *startNodes;
node *endNodes;

void print_grid(int N, int **array) {
    printf("==================\n");
    for (int i = 0; i < N + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            printf("%d", array[i][j]);
        }
        printf("\n");
    }
}

void draw (int N, int **result, int color, int x) {
    //get the position
    printf("draw:");
    for (int i = 0; i < N*N ; i++) {
         if (((1<<i) & x) > 0) {
            int a = i/N+1;
            int b = i%N+1; 
            result[a][b] = color; //color the position
        }
    }
}

int andresult (int a,int b) {
    int r = a & b;
    return r;
}

int orresult (int a,int b) {
    int r = a | b;
    return r;
}

int xorresult (int a,int b) {
    int r = a ^ b;
    return r;
}

bool next(int count,int i,int x,int temp,int N, int maxcount, vector<long long int> Solutions[7]) {
    cout << "i: " << i << endl;
    if (count == maxcount) { //try all conbinations
        printf("test_1\n");
        return false;
    } else if (i == N+2 ) { //get the right amount for test
        //if (andresult(x, temp) == 0 && ((orresult(x, temp)+1) >> (N*N)) == 1) {
        if ( ( temp+1 >> (N*N) ) == 1 ) {
                printf("test_2\n");
            return true;
        } else {
            count ++;
            printf("test_3\n");
            cout << "temp_3: " << bitset<DN>(temp) << " = " << temp << endl;
            cout << "x_3: " << bitset<DN>(x) << " = " << x << endl;
            return false;
        }
    } else {
        /*if (andresult(x, temp) > 0) { //overlap
            printf("test_4\n");
            cout << "temp_4: " << bitset<9>(temp) << " = " << temp << endl;
            cout << "x_4: " << bitset<9>(x) << " = " << x << endl;
            return false;
        }*/
        //cout << "i: " << i << endl;
        vector<long long int> :: iterator iter = Solutions[i].begin();
        for (int j = 0; iter != Solutions[i].end(); ++iter, ++j) {
            x = *iter ;
            cout << "current x: " << bitset<DN>(x) << " = " << x << endl;
            cout << "current temp: " << bitset<DN>(temp) << " = " << temp << endl;
            //x = Solutions[i][j];
            if (andresult(x, temp) == 0) { //not overlap
                temp |= x;
                cout << "temp |=: " << bitset<DN>(temp) << " = " << temp << endl;
                //test: if temp update, check the next color
                if (next(count, i+1, x, temp, N, maxcount, Solutions)) {
                    draw (N, result, i, x);
                    return true;
                } else {
                    printf("test_7\n");
                    cout << "temp_7: " << bitset<DN>(temp) << " = " << temp << endl;
                    cout << "x_7: " << bitset<DN>(x) << " = " << x << endl;
                    temp = (xorresult(x, temp)); //back to previous temp
                }
                //test: end
            } else {
                printf("test_5\n"); //overlap
                cout << "temp_5: " << bitset<DN>(temp) << " = " << temp << endl;
                cout << "x_5: " << bitset<DN>(x) << " = " << x << endl;
                //return false;
            }
            /*if (next(count, i+1, x, temp, N, maxcount, Solutions)) {
                draw (N, result, i, x);
                return true;
            } else {
                printf("test_7\n");
                cout << "temp_7: " << bitset<DN>(temp) << " = " << temp << endl;
                cout << "x_7: " << bitset<DN>(x) << " = " << x << endl;
                temp = (xorresult(x, temp)); //back to previous temp
            }*/
        }
        cout << "x_6: " << bitset<DN>(x) << " = " << x << endl;
        printf("test_6\n");
        return false;
    }
}

void intPuzzle(int N, int **array) {
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
        {1,0,3,0,1},
        {1,0,2,3,1},
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

void printVector(int N, vector<long long int> Solutions[7])
{
    for (int i = 2; i < N+2 ; ++i) {
        vector<long long int> :: iterator iter = Solutions[i].begin();
        cout << "color: " << i << endl;
        for (int j = 0; iter != Solutions[i].end(); ++iter, ++j) {
            cout << *iter << " = " << bitset<DN>(*iter) << endl;
        }
    }
}

bool solved(int N, int **grid) {
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

int Location(int N, int row, int column) {
    int p = (row-1)*N + column-1;
    return p;
}


bool solvePuzzle(int N, int **grid, int colorfrom, int colorto) {

    //Applying heuristics to see if the grid can be discarded at this stage, this will save a lot of steps
    if (!checkGrid(N, grid)) {
      return false;
    }
    
    //Is the grid fully solved?
    if (solved(N, grid)) {
      return true;
    }

    //Let's investigate the next move!
    //for color in startNodes:
    for (int color = colorfrom; color <= colorto && color < N + 2; color++) {
        node *startNode = &startNodes[color];
        node *endNode = &endNodes[color];
        //Solution_array[i].insert(Solution_array[i].end(), solution.begin(), solution.end());
        //Solution_array[i].push_back(solution);
        //printf("solution");
        //printVector(solution);

        //printf("Solutions\n");
        //printVector(N, Solutions);

        /**TODO**/
        //push the node to vector

        /**END**/
        printf("%d > %d, %d \n", color, startNode->row, startNode->column);
        //printf("s: %d\n", s);
        int p = Location(N, startNode->row, startNode->column);
	    printf("current p: %d\n", p);
        //s += pow (10,p-1);
        s |= (1 << p);
	    cout << "current s:" << bitset<DN>(s) << endl;
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
                    if (solvePuzzle(N, grid, colorfrom, colorto)) {
                        return true;
              	    } else {
                        /*printf("try insert\n");
                        //test
                        Solution_array[i].insert(Solution_array[i].end(), solution.begin(), solution.end());
                        //Solution_array[i].push_back(solution);
                        printf("solution");
                        printVector(solution);
                        printf("VectorofVector");
                        printVectorofVector(Solution_array);
                        //test*/
                        //backtrack...
                        p = Location(N, startNode->row, startNode->column);
                        //s -= pow(10, p-1);
                        s &= ~(1 << p);
                        startNode->column = column;
                        startNode->row = row;
                    }
                } else if (directions[i] =="right") {
                    startNode->column += 1;
                    grid[startNode->row][startNode->column] = color;         
                    printf("right\n");
                    if (solvePuzzle(N, grid, colorfrom, colorto)) {
                        return true;
              	    } else {
                        //backtrack...
                        p = Location(N, startNode->row, startNode->column);
                        //s -= pow(10, p);
                        s &= ~(1 << p);
                        grid[startNode->row][startNode->column] = 0;
                        startNode->column -= 1;
            	    }          
                } else if (directions[i] == "left") {
                    startNode->column -= 1;
                    grid[startNode->row][startNode->column] = color;
                    printf("left\n");
                    if (solvePuzzle(N, grid, colorfrom, colorto)) {
                        return true;
                    } else {
                        //backtrack...
                        p = Location(N, startNode->row, startNode->column);
                        //s -= pow(10, p-1);
                        s &= ~(1 << p);
                        grid[startNode->row][startNode->column] = 0;
                        startNode->column += 1;
                    }
                } else if (directions[i]=="up") {
                    startNode->row -= 1;
                    grid[startNode->row][startNode->column]=color;
                    printf("up\n");
                    if (solvePuzzle(N, grid, colorfrom, colorto)) {
                        return true;
                    } else {
                        //backtrack...
                        p = Location(N, startNode->row, startNode->column);
                        //s -= pow(10, p-1);
                        s &= ~(1 << p);
                        grid[startNode->row][startNode->column] = 0;
                        startNode->row += 1;
                    }
                } else if (directions[i] == "down") {
                    startNode->row += 1;
                    grid[startNode->row][startNode->column] = color;
                    printf("down\n");
                    if (solvePuzzle(N, grid, colorfrom, colorto)) {
                        return true;
                    } else {
                        //backtrack...
                        p = Location(N, startNode->row, startNode->column);
                        //s -= pow(10, p-1);
                        s &= ~(1 << p);
                        grid[startNode->row][startNode->column] = 0;
                        startNode->row -= 1;
                    }
                }
                //int column = startNode->column;
                //int row = startNode->row;
                //int p = Location(row, column);
		        //printf("current p: %d\n", p);
                //s += pow (10,p);
		        //printf("current s: %d\n", s);
            }
            //As none of the moves lead to a solution... 
            return false;
        }
        else {
            /**TODO**/
            //print the path from vector
            printf("color: %d\n", color);
            //s += pow(10,9);
            //s &= (1 << 9);
            //solution.push_back(s);
            Solutions[color].push_back(s);
            //printf("Solutions\n");
            //printVector(N, Solutions);

            //printf("final s: %d\n", s);
            //initalize s for next color
            //s = 0;
            //printVector(solution);
            /**END**/
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    /*int N = 3;
    int grid[5][5] = {
        {1,1,1,1,1},
        {1,2,4,4,1},
        {1,0,0,3,1},
        {1,2,0,3,1},
        {1,1,1,1,1}
    };*/

    int N = 4;
    int grid[6][6] = {
        {1,1,1,1,1,1},
        {1,4,0,0,4,1},
        {1,0,2,3,0,1},
        {1,0,0,5,0,1},
        {1,2,5,3,0,1},
        {1,1,1,1,1,1}
    };

    /*int N = 5;
    int grid[7][7] = {
        {1,1,1,1,1,1,1},
        {1,4,0,0,0,0,1},
        {1,0,2,3,0,4,1},
        {1,0,0,5,0,0,1},
        {1,0,0,0,0,0,1},
        {1,2,5,6,6,3,1},
        {1,1,1,1,1,1,1}
    };*/

    int **array;
    array = new int *[N + 2];
    result = new int *[N + 2];
    for (int i = 0; i < N + 2; i++) {
        array[i] = new int[N + 2];
        result[i] = new int[N + 2];
    }
    for (int i = 0; i < N + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            array[i][j] = grid[i][j];
            result[i][j] = grid[i][j];
        }
    }

    startNodes = new node[N + 2];
    endNodes = new node[N + 2];

    for (int i = 0; i < N + 2; i++) {
        startNodes[i].row = -1;
        startNodes[i].column = -1;
        endNodes[i].row = -1;
        endNodes[i].column = -1;
    }

    intPuzzle(N, array);
    /*
    for (int i = 0; i < N + 2; i++) {
        printf("%d, %d\n", startNodes[i].row, startNodes[i].column);
        printf("%d, %d\n", endNodes[i].row, endNodes[i].column);
    }
    */
    
    // if (solvePuzzle(3, array, red, red)) {
    // if (solvePuzzle(3, array, yellow, yellow)) {
    // if (solvePuzzle(3, array, blue, blue)) {
    for (int i = red; i <= orange; i++) {
        s = 0;
        if (solvePuzzle(N, array, i, i)) {
            printf("Problem Solved!\n");
            print_grid(N, array);
            /*Solution_array[i].insert(Solution_array[i].end(), solution.begin(), solution.end());
            //Solution_array[i].push_back(solution);
            printf("solution");
            printVector(solution);
            printf("VectorofVector");
            printVectorofVector(Solution_array);*/
        }
        else {
            printf("This problem cannot be solved!\n");
            print_grid(N, array);
        }
        //printf("connect\n");
        /*test
        Solution_array[i].insert(Solution_array[i].end(), solution.begin(), solution.end());
        //Solution_array[i].push_back(solution);
        printf("solution");
        printVector(solution);
        printf("VectorofVector");
        printVectorofVector(Solution_array);
        //test*/
    }
    // check each solution of color
    // int maxcount = c1*c2*c3*...
    int maxcount = 2*2*1;
    if (next(0, 2, 0, 0, N, maxcount, Solutions)) {
        printf("Y\n");
        
    };
    printf("last result\n");
    print_grid(N, result);
    printf("Solutions:\n");
    printVector(N, Solutions);
    
    double END = clock();
    cout << "\n" << "seq N: " << N << endl;
    cout << "RUNTIME: " << (END-START) / CLOCKS_PER_SEC << " s\n" << endl;
   
    for (int i = 0; i < N + 2; i++) {
        delete [] array[i];
        delete [] result[i];
    }

    delete [] array;
    delete [] result;
    delete [] startNodes;
    delete [] endNodes;
    return 0;
}
