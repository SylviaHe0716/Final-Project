#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* getopt */
#include <thread>
// #include "ThreadPool.h"
#include "Puzzle.h"
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <bitset>
#include <sys/time.h>
#define DN 64
#define Nadd2 10
// should update DN/Nadd2 when puzzle changed

using namespace std;
double START = clock();
Puzzle *result;

node opDirection[DIRECTION_ALL] = {
    [RIGHT] = { .row = 0, .column = 1},
    [LEFT]  = { .row = 0, .column = -1},
    [DOWN]  = { .row = 1, .column = 0},
    [UP]    = { .row = -1, .column = 0}
};

char strDirection[][10] = {
    "RIGHT",
    "LEFT",
    "DOWN",
    "UP",
    "CONNECT"
};

typedef struct
{
    int colorfrom;
    int colorto;
    std::vector<unsigned long long int> *solution;
    unsigned long long int s = 0ull;
    Puzzle *puzzle;
    Puzzle *result;//s_test
} TaskArgs;

void initPuzzle(Puzzle *p) {
    int N = p->N;
    int size = p->size;
    int value;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &value);
            //printf("%d ", value);
            p->setGridValue(value, i, j);
        }
    }

    for (int row = 1; row < 1 + N; row++) {
        for (int column = 1; column < 1 + N; column++) {
            int color = p->getGridValue(row, column);
            if (color != NO_COLOR) {
                node * snode = p->getStartNode(color);
                if (snode->row != -1 && snode->column != -1) {
                    int startrow = snode->row, startcolumn = snode->column;
                    int endrow = row, endcolumn = column;
                    /*
                    int startdir = 0;
                    int enddir = 0;
                    for (int dir = DIRECTION_START; dir < DIRECTION_ALL; dir++) {
                        if (p->getGridValue(startrow + opDirection[dir].row, startcolumn + opDirection[dir].column) == NO_COLOR
                            || p->getGridValue(startrow + opDirection[dir].row, startcolumn + opDirection[dir].column) == color) {
                            startdir++;
                        }
                        if (p->getGridValue(endrow + opDirection[dir].row, endcolumn + opDirection[dir].column) == NO_COLOR
                            || p->getGridValue(endrow + opDirection[dir].row, endcolumn + opDirection[dir].column) == color) {
                            enddir++;
                        }
                    }
                    */
                    //if (enddir > startdir) {
                    srand(time(NULL));
                    int x = rand() % 2;
                    if (x) {
                        p->setStartNode(color, endrow, endcolumn);
                        p->setEndNode(color, startrow, startcolumn);
                    }
                    else {
                        p->setEndNode(color, endrow, endcolumn);
                    }
                }
                else {
                    p->setStartNode(color, row, column);
                }
            }
        }
    }
}

void GenPuzzles(int numThreads, Puzzle *original, std::deque<Puzzle>&Puzzle_array) {
    int N = original->N;
    Puzzle *p;
    for (int i = 0; i < N; i++) {
        p = new Puzzle(original->N);
        p->setPuzzle(*original);
        Puzzle_array.push_back(*p);
    }
    return ;
}

int abs(int a, int b) {
    if (a > b) {
        return a - b;
    }
    else {
        return b - a;
    }
}

unsigned long long int andresult (unsigned long long int a,unsigned long long int b) {
    unsigned long long int r = a & b;
    return r;
}

unsigned long long int orresult (unsigned long long int a,unsigned long long int b) {
    unsigned long long int r = a | b;
    return r;
}

unsigned long long int xorresult (unsigned long long int a,unsigned long long int b) {
    unsigned long long int r = a ^ b;
    return r;
}

int Location(int N, int row, int column) {
    int p = (row-1)*N + column-1;
    return p;
}

void print_grid(int N, int **array) {
    printf("==================\n");
    for (int i = 0; i < N + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            printf("%d", array[i][j]);
        }
        printf("\n");
    }
}

void draw (int N, int color, unsigned long long int x) {
    //get the position
    //printf("draw:");
    for (int i = 0; i < N*N ; i++) {
         if (((1ull<<i) & x) > 0) {
            int a = i/N+1;
            int b = i%N+1; 
            result->setGridValue(color, a, b); //color the position
        }
    }
}

void printVector(int N, vector<unsigned long long int> Solutions[Nadd2])
{
    for (int i = 2; i < N+2 ; ++i) {
        vector<unsigned long long int> :: iterator iter = Solutions[i].begin();
        // cout << "color: " << i << endl;
        for (int j = 0; iter != Solutions[i].end(); ++iter, ++j) {
            // cout << *iter << " = " << bitset<DN>(*iter) << endl;
        }
    }
}

bool next(int count,int i,unsigned long long int x,unsigned long long int temp,int N, int maxcount, vector<unsigned long long int> Solutions[Nadd2]) {
    //cout << "i: " << i << endl;
    if (count == maxcount) { //try all conbinations
        //printf("test_1\n");
        return false;
    } else if (i == N+2 ) { //get the right amount for test
        //if (andresult(x, temp) == 0 && ((orresult(x, temp)+1) >> (N*N)) == 1) {
       // if ( ( temp+1 >> (N*N) ) == 1 ) {
        if ( (((( 1ull << (N*N-1) )-1) <<1) | 1) == temp ) {
                //printf("test_2\n");
            return true;
        } else {
            count ++;
            //printf("test_3\n");
            //cout << "temp_3: " << bitset<DN>(temp) << " = " << temp << endl;
            //cout << "x_3: " << bitset<DN>(x) << " = " << x << endl;
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
        vector<unsigned long long int> :: iterator iter = Solutions[i].begin();
        for (int j = 0; iter != Solutions[i].end(); ++iter, ++j) {
            x = *iter ;
            //cout << "current x: " << bitset<DN>(x) << " = " << x << endl;
            //cout << "current temp: " << bitset<DN>(temp) << " = " << temp << endl;
            //x = Solutions[i][j];
            if (andresult(x, temp) == 0) { //not overlap
                temp |= x;
                //cout << "temp |=: " << bitset<DN>(temp) << " = " << temp << endl;
                //test: if temp update, check the next color
                if (next(count, i+1, x, temp, N, maxcount, Solutions)) {
                    draw (N, i, x);
                    return true;
                } else {
                    //printf("test_7\n");
                    //cout << "temp_7: " << bitset<DN>(temp) << " = " << temp << endl;
                    //cout << "x_7: " << bitset<DN>(x) << " = " << x << endl;
                    temp = (xorresult(x, temp)); //back to previous temp
                }
            } else {
                //printf("test_5\n"); //overlap
                //cout << "temp_5: " << bitset<DN>(temp) << " = " << temp << endl;
                //cout << "x_5: " << bitset<DN>(x) << " = " << x << endl;
            }
        }
        //cout << "x_6: " << bitset<DN>(x) << " = " << x << endl;
        //printf("test_6\n");
        return false;
    }
}

bool solvePuzzle(TaskArgs *t) {
    // printf("from %d\n", t->colorfrom);
    // printf("to %d\n", t->colorto);
    // t->puzzle->printGrid();
    //Applying heuristics to see if the grid can be discarded at this stage, this will save a lot of steps
    if (!t->puzzle->checkGrid()) {
        return false;
    }

    //Is the grid fully solved?
    if (t->puzzle->checksolved()) {
        t->puzzle->printGrid();
        return true;
    }
    //Let's investigate the next move!
    //for color in startNodes:
    for (int color = t->colorfrom; color <= t->colorto && color < COLOR_START + t->puzzle->N; color++) {
        int N = t->puzzle->N;
        node *snode = t->puzzle->getStartNode(color);
        node *enode = t->puzzle->getEndNode(color);

        // printf("%d > %d, %d \n", color, snode->row, snode->column);
        int p = Location(N, snode->row, snode->column);
	    // printf("current p: %d\n", p);
        //s += pow (10,p-1);
        t->s |= (1ull << p);
	    //cout << "current s:" << bitset<DN>(t->s) << endl;
        //Check if the dots from this color are already connected
        //If not let's find out in which direction to progress...
        //empty vector
        std::vector<int> directions;
        int distance = abs(enode->row, snode->row) + abs(enode->column, snode->column);
        if (distance > 0) {

            if (distance == 1) {
                directions.push_back(COINCIDE);
                // printf("direction %s\n", strDirection[COINCIDE]);
            }
            for (int dir = DIRECTION_START; dir < DIRECTION_ALL; dir++) {
                if (t->puzzle->getGridValue(snode->row + opDirection[dir].row, snode->column + opDirection[dir].column) == NO_COLOR) {
                    directions.push_back(dir); //Lower Priority!
                    // printf("direction %s\n", strDirection[dir]);
                }
            }

            if (directions.size() == 0 ) {
                // printf("directions.size = 0\n");
                return false;
            }
        
            //Let's investigate possible moves in different directions...  
            //for direction in directions:
            for (int i = 0; i < directions.size(); i++) {
                if (directions[i] == COINCIDE) {
                    int column = snode->column;
                    int row = snode->row;
                    snode->column = enode->column;
                    snode->row = enode->row;
                    // printf("direction %s\n", strDirection[COINCIDE]);
                    if (solvePuzzle(t)) {
                        return true;
              	    } else {
                        //backtrack...
                        // printf("backtrack %s\n", strDirection[COINCIDE]);

                        p = Location(N, snode->row, snode->column);
                        //s -= pow(10, p-1);
                        t->s &= ~(1ull << p);
                        snode->column = column;
                        snode->row = row;
                    }
                }
                else {
                    for (int dir = DIRECTION_START; dir < DIRECTION_ALL; dir++) {
                        if (directions[i] == dir) {
                            snode->row += opDirection[dir].row;
                            snode->column += opDirection[dir].column;
                            t->puzzle->setGridValue(color, snode->row, snode->column);
                            // printf("direction %s\n", strDirection[dir]);
                            if (solvePuzzle(t)) {
                                return true;
                            } else {
                                //backtrack...
                                // printf("backtrack %s\n", strDirection[dir]);
                                //cout << "backtrack1 s:" << bitset<DN>(t->s) << endl;

                                p = Location(N, snode->row, snode->column);
                                //s -= pow(10, p-1);
                                t->s &= ~(1ull << p);
                                //cout << "backtrack2 s:" << bitset<DN>(t->s) << endl;
                                t->puzzle->setGridValue(NO_COLOR, snode->row, snode->column);
                                snode->row -= opDirection[dir].row;
                                snode->column -= opDirection[dir].column;
                            }
                            break;
                        }
                    }
                }
            }
            //As none of the moves lead to a solution... 
            return false;
        }
        else {
            /**TODO**/
            std::thread::id this_id = std::this_thread::get_id();
            t->solution->push_back(t->s);
            //cout << "this_id : " << this_id << "color : "<< color << "s : "<< bitset<DN>(t->solution->back()) << endl;

            /**END**/
        }
    }
    return false;
}

void thread_function(void *arg) {
    struct timeval start, end;
    long long startusec, endusec;
    TaskArgs *t = (TaskArgs *) arg;
    gettimeofday(&start, NULL);
    printf("========start=========\n");
    //printf("t %d\n", *t);
    solvePuzzle(t);
    printf("========end=========\n");
    gettimeofday(&end, NULL);
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    printf("spend %lld us\n", (endusec - startusec));
    return ;
}

void usage(char *bin)
{
    fprintf(stderr,"Usage: %s [-n number of threads] [< input.file]\n", bin);
    exit(1);
}

int main(int argc, char** argv) {
    int N = 3;
    /*int grid[5][5] = {
        {1,1,1,1,1},
        {1,2,4,4,1},
        {1,0,0,3,1},
        {1,2,0,3,1},
        {1,1,1,1,1}
    };*/
    int i, c;
    int numThreads = 0;

    while ((c = getopt(argc, argv, "n:")) != -1) {
        switch(c) {
        case 'n':
            numThreads = atoi(optarg);
            break;
        case '?':
        default:
            usage(argv[0]);
            return 1;
        }
    }

    scanf("%d", &N);
    // printf("%d\n", N);

    if (N < 3 || N > 8)
        return 1;
    std::deque<Puzzle> Puzzle_array;
    Puzzle *p = new Puzzle(N);
    initPuzzle(p);
    p->printNodes();
    p->printGrid();
    GenPuzzles(numThreads, p, Puzzle_array);

    std::vector<unsigned long long int> Solutions[Nadd2]; //Solutions[N+2]

    numThreads = Puzzle_array.size();
    std::vector<TaskArgs> TaskArgs_array;
    TaskArgs *t;
    for (int i = 0; i < numThreads; i++) {
        t = new TaskArgs;
        t->colorfrom = COLOR_START + i;
        t->colorto = COLOR_START + i;
        t->solution = &Solutions[COLOR_START + i];
        t->s = 0;
        // printf("%d %d\n", t->colorfrom, t->colorto);
        t->puzzle = &Puzzle_array[i];
        TaskArgs_array.push_back(*t);
    }

    std::vector<std::thread> thread_pool;
    //numThreads = 1;
    for (int i = 0; i < numThreads; i++) {
        thread_pool.push_back(std::thread(solvePuzzle, (TaskArgs *)(&TaskArgs_array[i])));
    }

    for (int i = 0; i < numThreads; i++) {
        thread_pool[i].join();
    }

    int maxcount = 1;
    for (int i = 2; i < N+2 ; i++) {
        maxcount *= Solutions[i].size();
    }

    result = new Puzzle(N);
    result->setPuzzle(*p);
    result->printGrid();
    // int **result;
    // initPuzzle(**result);

    if (next(0, 2, 0, 0, N, maxcount, Solutions)) {
        //printf("Y\n");
    };

    /*printf("last result\n");
    result->printGrid();
    printf("Solutions:\n");
    printVector(N, Solutions);*/

    double END = clock();
    cout << "\n" << "pthread N: " << N << endl;
    cout << "RUNTIME: " << (END-START) / CLOCKS_PER_SEC << " s\n" << endl;
   
    while (!TaskArgs_array.empty()) {
        TaskArgs_array.pop_back();
    }
    
    /*printf("=======result=======\n");
    while (!Puzzle_array.empty()) {
        Puzzle_array.front().printGrid();
        Puzzle_array.pop_front();
	}*/
    
    delete p;
    delete result;
    //printf("finish\n");

    return 0;
}
