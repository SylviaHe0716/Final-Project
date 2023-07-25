#ifndef __PUZZLE_H__   /*Puzzle.h*/
#define __PUZZLE_H__

enum eColor {
    NO_COLOR = 0,
    WALL_COLOR = 1,
    COLOR_START = 2,
    red = 2,
    yellow,
    blue,
    green,
    orange,
    magenta,
    purple,
    brown,
    darkgreen,
    COLOR_ALL
};

enum eDirection {
    DIRECTION_START = 0,
    RIGHT = 0,
    LEFT,
    DOWN,
    UP,
    COINCIDE,
    DIRECTION_ALL = 4
};

typedef struct
{
    int row;
    int column;
} node;

class Puzzle {
public:
    int N;
    int size;
    Puzzle(int N);
    ~Puzzle();
    void setPuzzle(int size, int **array, int N, node *startNodes, node *endNodes);
    void setPuzzle(Puzzle &p);
    int getGridValue(int row, int column);
    void setGridValue(int color, int row, int column);
    node *getStartNode(int color);
    void setStartNode(int color, int row, int column);
    node *getEndNode(int color);
    void setEndNode(int color, int row, int column);
    bool checkGrid();
    bool checksolved();
    void printNodes();
    void printGrid();
private:
    int **grid;
    node *startNodes;
    node *endNodes;
};
#endif