/**
 * @file bsp_puzzle_solver.cpp
 * @brief 4x4 Sliding Puzzle Solver using BFS (Breadth-First Search)
 * 
 * This program solves the 4x4 sliding puzzle by finding the minimum number
 * of moves required to reach the goal state from the initial state.
 * 
 * Goal state: "ABCDEFGHIJKLMNO#"
 * Where '#' represents the empty space.
 * 
 * @author JAPeTo
 * @version 1.6
 */

#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <omp.h>
#include <fstream>

using namespace std;

const int dRow[] = {-1, 1, 0, 0};
const int dCol[] = {0, 0, -1, 1};
const string GOAL = "ABCDEFGHIJKLMNO#";

struct State {
    string board;
    int blankPos;
    int cost;
    State(string b, int pos, int c) : board(b), blankPos(pos), cost(c) {}
};

string swapBoardTiles(const string &currentBoard, int position1, int position2) {
    string newBoard = currentBoard;
    char tmp = currentBoard[position1];
    newBoard[position1] = currentBoard[position2];
    newBoard[position2] = tmp;
    return newBoard;
}

int parallel_bfs(string start) {
    queue<State> q;
    unordered_set<string> visited;

    int blankPos = -1;
    for (int i = 0; i < (int)start.size(); i++) {
        if (start[i] == '#') {
            blankPos = i;
            break;
        }
    }

    q.push(State(start, blankPos, 0));
    visited.insert(start);

    bool found = false;
    int result_cost = -1;

    while (!q.empty() && !found) {
        int level_size = q.size();
        vector<State> current_level;

        for (int i = 0; i < level_size; i++) {
            current_level.push_back(q.front());
            q.pop();
        }

        #pragma omp parallel
        {
            vector<State> local_next_level;

            #pragma omp for
            for (int i = 0; i < level_size; i++) {
                if (found) continue; 

                State current = current_level[i];
                if (current.board == GOAL) {
                    #pragma omp critical
                    {
                        found = true;
                        result_cost = current.cost;
                    }
                    continue;
                }

                int row = current.blankPos / 4;
                int col = current.blankPos % 4;

                for (int dir = 0; dir < 4; dir++) {
                    int newRow = row + dRow[dir];
                    int newCol = col + dCol[dir];

                    if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 4) {
                        int newPos = newRow * 4 + newCol;
                        string newBoard = swapBoardTiles(current.board, current.blankPos, newPos);

                        #pragma omp critical
                        {
                            if (visited.find(newBoard) == visited.end()) {
                                visited.insert(newBoard);
                                local_next_level.push_back(State(newBoard, newPos, current.cost + 1));
                            }
                        }
                    }
                }
            }

            #pragma omp critical
            {
                for (const auto& state : local_next_level) {
                    q.push(state);
                }
            }
        }
    }
    return result_cost;
}

int main() {
    string start;
    ifstream file("puzzles.txt");  

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir puzzles.txt" << endl;
        return 1;
    }

    while (getline(file, start)) {
        if (start.empty()) continue;  

        cout << "Procesando tablero: " << start << endl;

        double start_time = omp_get_wtime();
        int result = parallel_bfs(start);
        double end_time = omp_get_wtime();

        if (result != -1)
            cout << "Resultado: " << result << endl;
        else
            cout << "Sin solución encontrada." << endl;

        cout << "Tiempo de ejecución: " << (end_time - start_time) << " segundos." << endl;
        cout << endl;
    }

    file.close();
    return 0;
}
