/**
 * @file bsp_puzzle_solver.cpp
 * @brief 4x4 Sliding Puzzle Solver using A* (Parallel with Manhattan Distance)
 * 
 * This program solves the 4x4 sliding puzzle by finding the minimum number
 * of moves required to reach the goal state from the initial state.
 * 
 * Goal state: "ABCDEFGHIJKLMNO#"
 * Where '#' represents the empty space.
 * 
 * @author JAPeTo
 * @version 1.7
 */

#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>
#include <cmath>
#include <atomic>
#include <omp.h>

using namespace std;

const string TARGET = "ABCDEFGHIJKLMNO#";

struct AStarState {
    string board;
    int blankPos;
    int cost;
    int heuristic;

    AStarState() = default;
    AStarState(const string& b, int pos, int c, int h)
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

int h2_heuristic(const string& board) {
    int totalDistance = 0;

    for (int i = 0; i < 16; i++) {
        char tile = board[i];
        if (tile == '#') continue;

        int targetPos = -1;
        for (int j = 0; j < 16; j++) {
            if (TARGET[j] == tile) {
                targetPos = j;
                break;
            }
        }

        if (targetPos != -1) {
            int currentRow = i / 4;
            int currentCol = i % 4;
            int targetRow = targetPos / 4;
            int targetCol = targetPos % 4;
            totalDistance += abs(currentRow - targetRow) + abs(currentCol - targetCol);
        }
    }

    return totalDistance;
}

int parallel_aStarSearch(const string& start) {
    priority_queue<AStarState, vector<AStarState>, greater<AStarState>> pq;
    unordered_set<string> visited;

    int blankPos = -1;
    for (int i = 0; i < 16; i++) {
        if (start[i] == '#') {
            blankPos = i;
            break;
        }
    }

    int initialHeuristic = h2_heuristic(start);
    pq.push(AStarState(start, blankPos, 0, initialHeuristic));
    visited.insert(start);

    atomic<bool> found(false);
    atomic<int> answer(-1);

    while (!pq.empty() && !found.load()) {
        int max_threads = max(1, omp_get_max_threads());
        int batch_size = min((int)pq.size(), max_threads * 2);

        vector<AStarState> best_states;
        best_states.reserve(batch_size);
        for (int i = 0; i < batch_size && !pq.empty(); ++i) {
            best_states.push_back(pq.top());
            pq.pop();
        }

        if (best_states.empty()) break;

        vector<AStarState> new_states;
        new_states.reserve(best_states.size() * 3);

        #pragma omp parallel
        {
            vector<AStarState> local_new;
            local_new.reserve(32);

            #pragma omp for schedule(dynamic)
            for (int idx = 0; idx < (int)best_states.size(); ++idx) {
                if (found.load(std::memory_order_acquire)) continue;

                AStarState current = best_states[idx];

                if (current.board == TARGET) {
                    if (!found.exchange(true)) {
                        answer.store(current.cost);
                    }
                    continue;
                }

                int row = current.blankPos / 4;
                int col = current.blankPos % 4;

                for (int dir = 0; dir < 4; ++dir) {
                    int newRow = row + dRow[dir];
                    int newCol = col + dCol[dir];

                    if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 4) {
                        int newPos = newRow * 4 + newCol;
                        string newBoard = swapBoardTiles(current.board, current.blankPos, newPos);

                        bool inserted = false;
                        #pragma omp critical (visited_access)
                        {
                            if (visited.find(newBoard) == visited.end()) {
                                visited.insert(newBoard);
                                inserted = true;
                            }
                        }

                        if (inserted) {
                            int newCost = current.cost + 1;
                            int newHeur = h2_heuristic(newBoard);
                            local_new.emplace_back(newBoard, newPos, newCost, newHeur);
                        }
                    }
                }
            }

            if (!local_new.empty()) {
                #pragma omp critical (new_states_push)
                {
                    for (auto &s : local_new) new_states.push_back(std::move(s));
                }
            }
        }

        if (found.load()) break;

        for (const auto &st : new_states) {
            pq.push(st);
        }
    }

    return answer.load();
}

int main() {
    ifstream infile("puzzles.txt");
    if (!infile) {
        cerr << "Error: no se pudo abrir puzzles.txt" << endl;
        return 1;
    }

    string start;
    int puzzleNumber = 1;

    while (infile >> start) {
        cout << "Procesando tablero:" << start << endl;

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
