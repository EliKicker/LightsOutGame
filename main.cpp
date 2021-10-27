#include <cmath>
#include <iostream>
#include <random>
#include "matrix.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
typedef matrix<int> tensor;

tensor solve(tensor& start_grid, tensor& end_grid);
tensor gauss_jordan(tensor& mov_mat, tensor& move);
void makeMove(tensor& a, int x, int y);
bool isSame(const tensor& a, const tensor& b);
tensor genGrid(int rows, int cols, int count_ones);
void printTensor(const tensor& grid);
void printGame(const tensor& start_grid, const tensor& end_grid);

int main() {
    int rows = 6;
    int cols = 6;
    int count_ones = 3;

    tensor start_grid = genGrid(rows, cols, count_ones);
    tensor end_grid = genGrid(rows, cols, count_ones);
    tensor sol_grid = solve(start_grid, end_grid);
    cout << "solution: " << endl;
    printTensor(sol_grid);
    printGame(start_grid, end_grid);

    bool isRunning = true;
    int x, y;
    cout << "x:";
    cin >> x;
    cout << "y:";
    cin >> y;
    while (isRunning) {
        makeMove(start_grid, x, y);
        printGame(start_grid, end_grid);
        if (isSame(start_grid, end_grid)) {
            isRunning = false;
        } else {
            cout << "x:";
            cin >> x;
            cout << "y:";
            cin >> y;
        }
    }

    return 0;
}

tensor solve(tensor& start_grid, tensor& end_grid) {
    tensor sol;
    int start[start_grid.rows * start_grid.columns];
    int end[end_grid.rows * end_grid.columns];
    for (int i = 0; i < start_grid.rows; i++) {
        for (int j = 0; j < start_grid.columns; j++) {
            start[i * start_grid.columns + j] = start_grid(i, j);
            end[i * end_grid.columns + j] = end_grid(i, j);
        }
    }

    //Gen Move Matrix
    tensor mov_mat = tensor(start_grid.rows * start_grid.columns, start_grid.rows * start_grid.columns);
    for (int j = 0; j < start_grid.rows; j++) {
        for (int k = 0; k < start_grid.columns; k++) {
            tensor move = tensor(start_grid.rows, start_grid.columns);
            makeMove(move, k, j);
            for (int m = 0; m < move.rows; m++) {
                for (int n = 0; n < move.columns; n++) {
                    mov_mat(m * move.columns + n, j * start_grid.columns + k) = move(m, n);
                }
            }
        }
    }

    //Gen Move Combination Matrix
    tensor move(start_grid.rows, start_grid.columns);
    for (int i = 0; i < start_grid.rows; i++) {
        for (int j = 0; j < start_grid.columns; j++) {
            move(i, j) = (start_grid(i, j) + end_grid(i, j)) & 1;
        }
    }

    sol = gauss_jordan(mov_mat, move);
    return sol;
}

tensor gauss_jordan(tensor& mov_mat, tensor& move) {
    tensor field = tensor(mov_mat.rows, mov_mat.columns + 1);

    //Initialise field
    for (int i = 0; i < mov_mat.rows; i++) {
        for (int j = 0; j < mov_mat.columns; j++) {
            field(i, j) = mov_mat(i, j);
        }
    }
    for (int i = 0; i < move.rows; i++) {
        for (int j = 0; j < move.columns; j++) {
            field(i * move.columns + j, mov_mat.columns) = move(i, j);
        }
    }

    //Gauss Jordan Algorithm
    for (int i = 0; i < mov_mat.columns; i++) {
        //Main diagonal entries to 1
        if (field(i, i) != 1) {
            for (int j = 0; j < field.rows; j++) {
                if (field(j, i) == 1 && j > i) {
                    for (int k = 0; k < field.columns; k++) {
                        field(i, k) = (field(i, k) + field(j, k)) & 1;
                    }
                    break;
                }
            }
        }

        //Elimination
        for (int j = 0; j < mov_mat.rows; j++) {
            if (i != j && field(j, i) != 0) {
                for (int k = 0; k < field.columns; k++) {
                    field(j, k) = (field(j, k) + field(i, k)) & 1;
                }
            }
        }
    }

    //Get Solution Move Matrix
    tensor sol = tensor(move.rows, move.columns);
    for (int i = 0; i < move.rows; i++) {
        for (int j = 0; j < move.columns; j++) {
            sol(i, j) = field(i * move.columns + j, mov_mat.columns);
        }
    }

    return sol;
}

void makeMove(tensor& a, int x, int y) {
    if (0 <= x && x < a.rows && 0 <= y && y < a.columns) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (0 <= y + i && y + i < a.rows && 0 <= x + j && x + j < a.columns) (a(y + i, x + j) == 0) ? a(y + i, x + j) = 1 : a(y + i,x + j) = 0;
            }
        }
    }
}

bool isSame(const tensor& a, const tensor& b) {
    if (a.rows != b.rows || a.columns != b.columns) return false;
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.columns; j++) {
            if (a(i, j) != b(i, j)) return false;
        }
    }
    return true;
}

tensor genGrid(int rows, int cols, int count_ones) { //fix sis
    tensor grid = tensor(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int b = rand() & 1;
            count_ones -= b;
            (count_ones < 0) ? grid(i, j) = 0 : grid(i, j) = b;
        }
    }
    return grid;
}

void printGame(const tensor& start_grid, const tensor& end_grid) {
    printTensor(start_grid);
    cout << endl;
    printTensor(end_grid);
}

void printTensor(const tensor& grid) {
    cout << "x ";
    for (int i = 0; i < grid.columns; i++) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < grid.columns; i++) {
        cout << "--";
    }
    cout << "--- y" << endl;

    for (int i = 0; i < grid.rows; i++) {
        cout << "- ";
        for (int j = 0; j < grid.columns; j++) {
            cout << grid(i, j) << " ";
        }
        cout << "- " << i << endl;
    }

    for (int i = 0; i < grid.columns; i++) {
        cout << "--";
    }
    cout << "---" << endl;
}