#include <time.h>
#include <stdio.h>
#include "Puzzle.h"

Puzzle::Puzzle(int N) {
    // printf("new puzzle\n");
    this->N = N;
    this->size = N + 2;
    this->grid = new int *[this->size];
    for (int i = 0; i < this->size; i++) {
        this->grid[i] = new int[this->size];
    }
    this->startNodes = new node[N];
    this->endNodes = new node[N];
    for (int i = 0; i < N; i++) {
        this->startNodes[i].row = -1;
        this->startNodes[i].column = -1;
        this->endNodes[i].row = -1;
        this->endNodes[i].column = -1;
    }
}

Puzzle::~Puzzle() {
    // printf("delete puzzle\n");
    for (int i = 0; i < this->size; i++) {
        delete [] this->grid[i];
    }
    delete [] this->grid;
    delete [] this->startNodes;
    delete [] this->endNodes;
}

void Puzzle::setPuzzle(int size, int **array, int N, node startNodes[], node endNodes[]) {
    if (this->size == size && this->N == N) {
        for (int i = 0; i < this->size; i++) {
            for (int j = 0; j < this->size; j++) {
                this->grid[i][j] = array[i][j];
            }
        }
        for (int i = 0; i < this->N; i++) {
            this->startNodes[i].row = startNodes[i].row;
            this->startNodes[i].column = startNodes[i].column;
            this->endNodes[i].row = endNodes[i].row;
            this->endNodes[i].column = endNodes[i].column;
        }
    }
}

void Puzzle::setPuzzle(Puzzle &p) {
    setPuzzle(p.size, p.grid, p.N, p.startNodes, p.endNodes);
}

int Puzzle::getGridValue(int row, int column) {
    if (row >= 0 && row < this->size && column >= 0 && column < this->size) {
        return this->grid[row][column];
    }
    else {
        return -1;
    }
}

void Puzzle::setGridValue(int color, int row, int column) {
    if (row >= 0 && row < this->size && column >= 0 && column < this->size) {
        if (color >= 0 && color < COLOR_START + this->N) {
            this->grid[row][column] = color;
        }
    }
}

node *Puzzle::getStartNode(int color) {
    if (color >= COLOR_START && color < COLOR_START + this->N) {
        return &(this->startNodes[color - COLOR_START]);
    }
    else {
        return NULL;
    }
}

void Puzzle::setStartNode(int color, int row, int column) {
    if (color >= COLOR_START && color < COLOR_START + this->N) {
        if (row > 0 && row < this->size && column > 0 && column < this->size) {
            this->startNodes[color - COLOR_START].row = row;
            this->startNodes[color - COLOR_START].column = column;
        }
    }
}

node *Puzzle::getEndNode(int color) {
    if (color >= COLOR_START && color < COLOR_START + this->N) {
        return &(this->endNodes[color - COLOR_START]);
    }
    else {
        return NULL;
    }
}

void Puzzle::setEndNode(int color, int row, int column) {
    if (color >= COLOR_START && color < COLOR_START + this->N) {
        if (row > 0 && row < this->size && column > 0 && column < this->size) {
            this->endNodes[color - COLOR_START].row = row;
            this->endNodes[color - COLOR_START].column = column;
        }
    }
}

bool Puzzle::checkGrid() {
    //this->printGrid();
    for (int row = 1; row < this->N + 1; row++) {
        for (int column = 1; column < this->N + 1; column++) {
            if (this->grid[row][column] != NO_COLOR) {
                int color = this->grid[row][column];
                // A grid that contains a cell of colour surrounded by other colours is invalid... 
                if (this->grid[row + 1][column] != NO_COLOR && this->grid[row + 1][column] != color)
                    if (this->grid[row - 1][column] != NO_COLOR && this->grid[row - 1][column] != color)
                        if (this->grid[row][column + 1] != NO_COLOR && this->grid[row][column + 1] != color)
                            if (this->grid[row][column - 1] != NO_COLOR && this->grid[row][column - 1] != color) {
                                // printf("surrounded by others\n");
                                return false;
                            }
                // A connection line that crosses with itself is invalid
                /*if (this->grid[row + 1][column] == color && this->grid[row - 1][column] == color) {
                    if (this->grid[row][column + 1] == color || this->grid[row][column - 1] == color) {
                        printf("row cross\n");
                        return false;
                    }
                }
                else if (this->grid[row][column + 1] == color && this->grid[row][column - 1] == color) {
                    if (this->grid[row + 1][column] == color || this->grid[row - 1][column] == color) {
                        printf("column cross\n");

                        return false;
                    }
                }*/
            }
        }
    }
    return true;
}

bool Puzzle::checksolved() {
    for (int color = COLOR_START; color < COLOR_START + this->N; color++) {
        node *snode = this->getStartNode(color);
        node *enode = this->getEndNode(color);
        if (snode->column != enode->column || snode->row != enode->row)
            return false;
    }
    for (int row = 1; row < this->N + 1; row++) {
        for (int column = 1; column < this->N + 1; column++) {
            if (this->grid[row][column] == NO_COLOR)
                return false;
        }
    }
    return true;
}

void Puzzle::printNodes() {
    printf("========Nodes==========\n");
    for (int i = 0; i < this->N; i++) {
        printf("%d, %d\n", this->startNodes[i].row, this->startNodes[i].column);
        printf("%d, %d\n", this->endNodes[i].row, this->endNodes[i].column);
    }
}
void Puzzle::printGrid() {
    printf("========Grid==========\n");
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            printf("%2d", this->grid[i][j]);
        }
        printf("\n");
    }
}
