/**
 * @file bsp_ puzzle_solver.cpp
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
#include <fstream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>
#include <omp.h>

using namespace std;

const string TARGET = "ABCDEFGHIJKLMNO#";

struct AStarState {
    string board;
    int blankPos;
    int cost;
    int heuristic;

    AStarState(string b, int pos, int c, int h)
        : board(b), blankPos(pos), cost(c), heuristic(h) {}

    bool operator>(const AStarState& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
};

const int dRow[] = {-1, 1, 0, 0};
const int dCol[] = {0, 0, -1, 1};

string swapBoardTiles(const string &currentBoard, int position1, int position2) {
    string newBoard = currentBoard;
    swap(newBoard[position1], newBoard[position2]);
    return newBoard;
}

int h1_heuristic(const string& board) {
    int misplaced = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] != '#' && board[i] != TARGET[i]) {
            misplaced++;
        }
    }
    return misplaced;
}

int parallel_aStarSearch(string start) {
    priority_queue<AStarState, vector<AStarState>, greater<AStarState>> pq;
    unordered_set<string> visited;

    int blankPos = -1;
    for (int i = 0; i < 16; i++) {
        if (start[i] == '#') {
            blankPos = i;
            break;
        }
    }

    int initialHeuristic = h1_heuristic(start);
    pq.push(AStarState(start, blankPos, 0, initialHeuristic));
    visited.insert(start);

    bool solution_found = false;
    int solution_cost = -1;

    while (!pq.empty() && !solution_found) {
        vector<AStarState> best_states;
        int batch_size = min((int)pq.size(), omp_get_max_threads() * 2);

        for (int i = 0; i < batch_size && !pq.empty(); i++) {
            best_states.push_back(pq.top());
            pq.pop();
        }

        vector<AStarState> new_states;

        #pragma omp parallel
        {
            vector<AStarState> local_new_states;

            #pragma omp for
            for (int i = 0; i < batch_size; i++) {
                AStarState current = best_states[i];

                if (!solution_found && current.board == TARGET) {
                    #pragma omp critical
                    {
                        if (!solution_found) {
                            solution_found = true;
                            solution_cost = current.cost;
                        }
                    }
                }

                if (solution_found) continue; 

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
                                int newCost = current.cost + 1;
                                int newHeuristic = h1_heuristic(newBoard);
                                local_new_states.push_back(AStarState(newBoard, newPos, newCost, newHeuristic));
                            }
                        }
                    }
                }
            }

            #pragma omp critical
            {
                for (const auto& state : local_new_states) {
                    new_states.push_back(state);
                }
            }
        }

        for (const auto& state : new_states) {
            pq.push(state);
        }
    }

    return solution_cost;
}


int main() {
    ifstream infile("puzzles.txt");
    if (!infile) {
        cerr << "Error: no se pudo abrir puzzles.txt" << endl;
        return 1;
    }

    string start;
    while (infile >> start) {
        cout << "Procesando tablero: " << start << endl;
        double start_time = omp_get_wtime();
        int result = parallel_aStarSearch(start);
        double end_time = omp_get_wtime();
        if (result != -1)
            cout << "Resultado: " << result << endl;
        else
            cout << "No se encontró solución." << endl;
        cout << "Tiempo de ejecución: " << (end_time - start_time) << " segundos." << endl;
        cout << endl;
    }

    infile.close();
    return 0;
}