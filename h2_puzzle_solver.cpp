/**
 * @file bsp_puzzle_solver.cpp
 * @brief Sliding Puzzle Solver using A* Search
 * 
 * Reads multiple puzzles from "puzzles.txt" and solves each one.
 * Measures execution time using <chrono>.
 */

#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
#include <chrono>
using namespace std;

string goal = "";
int sizeBoard = 0;

struct AStarState {
    string board;
    int blankPos;
    int cost;  
    int heuristic; 
    
    AStarState(string b, int pos, int c, int h) : board(b), blankPos(pos), cost(c), heuristic(h) {}
    
    bool operator>(const AStarState& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
};

const int dRow[] = {-1, 1, 0, 0};
const int dCol[] = {0, 0, -1, 1};

string swapBoardTiles(const string &currentBoard, int position1, int position2){
    string newBoard = currentBoard;
    swap(newBoard[position1], newBoard[position2]);
    return newBoard;
}

int h2_heuristic(const string& board) {
    int totalDistance = 0;
    for (int i = 0; i < sizeBoard * sizeBoard; i++) {
        char tile = board[i];
        if (tile == '#') continue;
        
        int targetPos = -1;
        for (int j = 0; j < sizeBoard * sizeBoard; j++) {
            if (goal[j] == tile) {
                targetPos = j;
                break;
            }
        }
        if (targetPos != -1) {
            int currentRow = i / sizeBoard;
            int currentCol = i % sizeBoard;
            int targetRow = targetPos / sizeBoard;
            int targetCol = targetPos % sizeBoard;
            totalDistance += abs(currentRow - targetRow) + abs(currentCol - targetCol);
        }
    }
    return totalDistance;
}

int aStarSearch(string start){
    priority_queue<AStarState, vector<AStarState>, greater<AStarState>> pq;
    unordered_set<string> visited;
    int expandedNodes = 0;
    
    int blankPos = start.find('#');
    int initialHeuristic = h2_heuristic(start);
    
    pq.push(AStarState(start, blankPos, 0, initialHeuristic));
    visited.insert(start);
    
    while (!pq.empty()) {
        AStarState current = pq.top();
        pq.pop();
        expandedNodes++;
        
        if (current.board == goal) {
            cout << "Nodos expandidos: " << expandedNodes << endl;
            cout << "Longitud de la solución: " << current.cost << endl;
            return current.cost;
        }
        int row = current.blankPos / sizeBoard;
        int col = current.blankPos % sizeBoard;
        
        for (int i = 0; i < 4; i++) {
            int newRow = row + dRow[i];
            int newCol = col + dCol[i];
            
            if (newRow >= 0 && newRow < sizeBoard && newCol >= 0 && newCol < sizeBoard) {
                int newPos = newRow * sizeBoard + newCol;
                string newBoard = swapBoardTiles(current.board, current.blankPos, newPos);
                
                if (visited.find(newBoard) == visited.end()) {
                    int newCost = current.cost + 1;
                    int newHeuristic = h2_heuristic(newBoard);
                    pq.push(AStarState(newBoard, newPos, newCost, newHeuristic));
                    visited.insert(newBoard);
                }
            }
        }
    }
    cout << "Nodos expandidos: " << expandedNodes << endl;
    return -1; 
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        cerr << "Uso: ./solver <tamaño_tablero>\n";
        return 1;
    }

    sizeBoard = stoi(argv[1]);

    if (sizeBoard == 4) goal = "ABCDEFGHIJKLMNO#";
    else if (sizeBoard == 8) goal = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJK#";
    else if (sizeBoard == 16) goal = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTU#";
    else if (sizeBoard == 32) goal = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHI#";
    else {
        cerr << "Tamaño no soportado.\n";
        return 1;
    }

    ifstream infile("puzzles.txt");
    if (!infile) {
        cerr << "Error: no se pudo abrir puzzles.txt\n";
        return 1;
    }

    string start;
    int caseNum = 1;
    while (infile >> start) {
        cout << "Procesando tablero: " << start << endl;

        auto start_time = chrono::high_resolution_clock::now();
        int result = aStarSearch(start);
        auto end_time = chrono::high_resolution_clock::now();

        double elapsed = chrono::duration<double>(end_time - start_time).count();
        cout << "Resultado: " << (result != -1 ? to_string(result) : "No resuelto") << endl;
        cout << "Tiempo de ejecución: " << elapsed << " segundos\n";
        cout << endl;
    }

    infile.close();
    return 0;
}
