/**
 * @file bsp_puzzle_solver.cpp
 * @brief 4x4 Sliding Puzzle Solver using A* (A-Star Search)
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
#include <functional>
#include <chrono>
#include <fstream>  

using namespace std;
using namespace std::chrono;

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
      char tmp = currentBoard[position1];
      newBoard[position1] = currentBoard[position2];
      newBoard[position2] = tmp;
      return newBoard;
}

int h1_heuristic(const string& board) {
    int misplaced = 0;
    for (int i = 0; i < sizeBoard * sizeBoard; i++) {
        if (board[i] != '#' && board[i] != goal[i]) {
            misplaced++;
        }
    }
    return misplaced;
}

int aStarSearch(string start){
      priority_queue<AStarState, vector<AStarState>, greater<AStarState>> pq;
      unordered_set<string> visited;
      int expandedNodes = 0;
      
      int blankPos = -1;
      for (int i = 0; i < sizeBoard * sizeBoard; i++) {
            if (start[i] == '#') {
                  blankPos = i;
                  break;
            }
      }

      int initialHeuristic = h1_heuristic(start);
    
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
                              int newHeuristic = h1_heuristic(newBoard);
                              
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
            cerr << "Uso: " << argv[0] << " <tamaño_tablero>" << endl;
            return 1;
      }

      sizeBoard = stoi(argv[1]);
      if(sizeBoard == 4){
            goal = "ABCDEFGHIJKLMNO#";
            cout << "Goal size: " << goal.size() << endl;
      }
      else if(sizeBoard == 8){
            goal = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJK#";
            cout << "Goal size: " << goal.size() << endl;
      }
      else if(sizeBoard == 16){
            goal = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTU#";
            cout << "Goal size: " << goal.size() << endl;
      }
      else if(sizeBoard == 32){
            goal = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHI#";            
            cout << "Goal size: " << goal.size() << endl;
      }

      ifstream infile("puzzles.txt");
      if (!infile.is_open()) {
            cerr << "Error: no se pudo abrir puzzles.txt" << endl;
            return 1;
      }

      string start;
      while (infile >> start) {
            cout << "Procesando tablero: " << start << endl;

            auto start_time = high_resolution_clock::now();
            int result = aStarSearch(start);
            auto end_time = high_resolution_clock::now();

            double elapsed = duration<double>(end_time - start_time).count();
            cout << "Resultado: " << result << endl;
            cout << "Tiempo de ejecución: " << elapsed << " segundos" << endl;
            cout <<  endl;
      }

      infile.close();
      return 0;
}
