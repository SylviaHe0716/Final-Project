#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* getopt */
#include "ThreadPool.h"
#include "Puzzle.h"
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>

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
    Puzzle *puzzle;
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
                    
                    if (enddir > startdir) {
                    // srand(time(NULL));
                    // int x = rand() % 2;
                    // if (x) {
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
    Puzzle *p = new Puzzle(original->N);
    p->setPuzzle(*original);
    Puzzle_array.push_back(*p);
    int numThr = Puzzle_array.size();
    for (int color = COLOR_START; color < COLOR_START + N && numThr > 0; color++) {
        node *snode = original->getStartNode(color);
        int row = snode->row;
        int column = snode->column;
        int numDir = 0;
        numThr = Puzzle_array.size();
        //printf("numThr %d\n", numThr);
        for (int dir = DIRECTION_START; dir < DIRECTION_ALL; dir++) {
            if (original->getGridValue(row + opDirection[dir].row, column + opDirection[dir].column) == NO_COLOR
               || original->getGridValue(row + opDirection[dir].row, column + opDirection[dir].column) == color) {
               numDir++;
            }
        }
        //printf("numDir %d\n", numDir);
        if (numDir > 1 && numThr * numDir <= numThreads) {
            for (int i = 0; i < numThr; i++) {
                for (int dir = DIRECTION_START; dir < DIRECTION_ALL; dir++) {
                    p = new Puzzle(Puzzle_array.front().N);
                    p->setPuzzle(Puzzle_array.front());
                    //p->printGrid();
                    if (p->getGridValue(row + opDirection[dir].row, column + opDirection[dir].column) == NO_COLOR
                        || p->getGridValue(row + opDirection[dir].row, column + opDirection[dir].column) == color) {
                        p->setStartNode(color, row + opDirection[dir].row, column + opDirection[dir].column);
                        p->setGridValue(color, row + opDirection[dir].row, column + opDirection[dir].column);
                        //printf("get new puzzle\n");
                        //p->printGrid();
                        Puzzle_array.push_back(*p);
                        //printf("%d %d\n", color, &(*p));
                    }
                    else {
                        //printf("no match\n");
                        delete p;
                    }
                }
                Puzzle_array.pop_front();
            }
        }
        else if (numDir == 0) {
            printf("%d\n", color);
            while (!Puzzle_array.empty()) {
                Puzzle_array.pop_front();
	        }
        }
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
        //t->puzzle->printGrid();
        return true;
    }
    //Let's investigate the next move!
    //for color in startNodes:
    for (int color = t->colorfrom; color <= t->colorto && color < COLOR_START + t->puzzle->N; color++) {
        node *snode = t->puzzle->getStartNode(color);
        node *enode = t->puzzle->getEndNode(color);

        // printf("%d > %d, %d \n", color, snode->row, snode->column);
        //Check if the dots from this color are already connected
        //If not let's find out in which direction to progress...
        //empty vector
        std::vector<int> directions;
        int distance = abs(enode->row, snode->row) + abs(enode->column, snode->column);
        if (distance > 0) {

            if (distance == 1) {
                directions.push_back(COINCIDE);
                //printf("direction %s\n", strDirection[COINCIDE]);
            }
            for (int dir = DIRECTION_START; dir < DIRECTION_ALL; dir++) {
                if (t->puzzle->getGridValue(snode->row + opDirection[dir].row, snode->column + opDirection[dir].column) == NO_COLOR) {
                    directions.push_back(dir); //Lower Priority!
                    //printf("direction %s\n", strDirection[dir]);
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
            // printf("color: %d\n", color);
            // t->puzzle->printGrid();
            /**END**/
        }
    }
    return false;
}

struct timeval master_start;

void thread_function(void *arg) {
    struct timeval start, end;
    long long master_startusec, startusec, endusec;
    TaskArgs *t = (TaskArgs *) arg;
    gettimeofday(&start, NULL);
    if (solvePuzzle(t)) {
        //printf("========start=========\n");
        gettimeofday(&end, NULL);
        master_startusec = master_start.tv_sec * 1000000 + master_start.tv_usec;
        startusec = start.tv_sec * 1000000 + start.tv_usec;
        endusec = end.tv_sec * 1000000 + end.tv_usec;
        // printf("spend %lld us\n", (endusec - startusec));
        //printf("thread spend %.6f s\n", (endusec - startusec) / 1000000.0);
        //printf("all spend %.6f s\n", (endusec - master_startusec) / 1000000.0);
        printf("| %.6f\n", (endusec - master_startusec) / 1000000.0);
        //printf("========end=========\n");
    }
    return ;
}

void usage(char *bin)
{
    fprintf(stderr,"Usage: %s [-n number of threads] [< input.file]\n", bin);
    exit(1);
}

int main(int argc, char** argv) {
    int N = 3;
    int grid[5][5] = {
        {1,1,1,1,1},
        {1,2,4,4,1},
        {1,0,0,3,1},
        {1,2,0,3,1},
        {1,1,1,1,1}
    };
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
    //printf("%d\n", N);

    if (N < 3 || N > 15)
        return 1;

    gettimeofday(&master_start, NULL);

    std::deque<Puzzle> Puzzle_array;
    Puzzle *p = new Puzzle(N);
    initPuzzle(p);
    // p->printNodes();
    // p->printGrid();
    GenPuzzles(numThreads, p, Puzzle_array);

    numThreads = Puzzle_array.size();
    std::vector<TaskArgs> TaskArgs_array;
    TaskArgs *t;
    for (int i = 0; i < numThreads; i++) {
        t = new TaskArgs;
        t->colorfrom = COLOR_START;
        t->colorto = COLOR_START + Puzzle_array[i].N;
        // printf("%d %d\n", t->colorfrom, t->colorto);
        t->puzzle = &Puzzle_array[i];
        TaskArgs_array.push_back(*t);
    }
    printf("Task num : %d\n", numThreads);

    ThreadPool thread_pool(numThreads);
    for (int i = 0; i < numThreads; i++) {
        thread_pool.addTask(thread_function, (void *)(&TaskArgs_array[i]));
        //std::thread(solvePuzzle, (TaskArgs *)(&TaskArgs_array[i]));
    }




    while (!TaskArgs_array.empty()) {
        TaskArgs_array.pop_back();
    }
    
    /*printf("=======result=======\n");
    while (!Puzzle_array.empty()) {
        Puzzle_array.front().printGrid();
        Puzzle_array.pop_front();
	}*/
    
    delete p;
    //printf("finish\n");

    return 0;
}
