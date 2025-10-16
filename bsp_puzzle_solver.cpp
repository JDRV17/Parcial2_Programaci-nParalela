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
#include <string>
#include <chrono>
#include <fstream>   
using namespace std::chrono;
using namespace std;

/**
 * @brief Possible movement directions
 * 
 * Up, Down, Left, Right
 */
const int dRow[] = {-1, 1, 0, 0};
const int dCol[] = {0, 0, -1, 1};
const string MOVES[] = {"UP", "DOWN", "LEFT", "RIGHT"};
string goal = "";
int sizeBoard = 0;

struct State{
      string board;
      int blankPos;
      int cost;
      State(string b, int pos, int c) : board(b), blankPos(pos), cost(c) {}
};

string swapBoardTiles(const string &currentBoard, int position1, int position2){
      string newBoard = currentBoard;
      char tmp = currentBoard[position1];
      newBoard[position1] = currentBoard[position2];
      newBoard[position2] = tmp;
      return newBoard;
}

int bfs(string start){
      int expandedNodes = 0;
      queue<State> q;
      unordered_set<string> visited;
      int blankPos = -1;
      for (int i = 0; i < start.size(); i++){
            if (start[i] == '#'){
                  blankPos = i;
            } 
            if (blankPos >= 0)
            {
                  break;
            }
      }
      q.push(State(start, blankPos, 0));
      visited.insert(start);

      while (!q.empty()){
            State current = q.front();
            q.pop();
            expandedNodes++;
            
            if(current.board == goal){
                  cout << "Nodos expandidos: " << expandedNodes << endl;
                  cout << "Longitud de la solución: " << current.cost << endl;
                  return current.cost;
            }

            int row = current.blankPos / sizeBoard;
            int col = current.blankPos % sizeBoard;

            for (int i = 0; i < 4; i++){
                  int newRow = row + dRow[i];
                  int newCol = col + dCol[i];

                  if (newRow >= 0 && newRow < sizeBoard && newCol >= 0 && newCol < sizeBoard){
                        int newPos = newRow * sizeBoard + newCol;

                        string newBoard = swapBoardTiles(current.board, current.blankPos, newPos);

                        if (visited.find(newBoard) == visited.end()){
                              q.push(State(newBoard, newPos, current.cost + 1));
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
      cin >> start;
      cout << "Procesando tablero: " << start << endl;

            auto start_time = high_resolution_clock::now();
            int result = bfs(start);
            auto end_time = high_resolution_clock::now();

            double elapsed = duration<double>(end_time - start_time).count();
            cout << "Resultado: " << result << endl;
            cout << "Tiempo de ejecución: " << elapsed << " segundos" << endl;
            cout << endl;
      while (infile >> start) {
            cout << "Procesando tablero: " << start << endl;

            auto start_time = high_resolution_clock::now();
            int result = bfs(start);
            auto end_time = high_resolution_clock::now();

            double elapsed = duration<double>(end_time - start_time).count();
            cout << "Resultado: " << result << endl;
            cout << "Tiempo de ejecución: " << elapsed << " segundos" << endl;
            cout << endl;
      }

      infile.close();
      return 0;
}
