/**
 * @file board_available.cpp
 * @brief 4x4 Sliding Puzzle Available Moves Finder
 * 
* This program analyzes a 4x4 sliding puzzle board configuration and determines
 * all valid moves that can be made from the current state.
 */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Finds and displays all available moves for the current board state
 *
 */
void listAvailable(vector<vector<char>> &board){
  int posx = -1;
  int posy = -1;
  vector<string> moves = {"UP", "DOWN", "LEFT", "RIGHT"};
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      if (board[i][j] == '#'){
        posx = i;
        posy = j; 
        break;
      }
    }
    if (posx > 0 && posy > 0){
      break;
    }
  }
  if (posx == 0){
    moves.erase(moves.begin());
  }
  else if (posx == 3){
    moves.erase(moves.begin() + 1);
  }
  if (posy == 0){
    moves.erase(moves.begin() + 2);
  }
  else if (posy == 3){
    moves.erase(moves.begin() + 3);
  }

  for(int i = 0; i < moves.size(); i++){
    cout << moves[i] << endl;
  }
}
/**
 * @brief Main function - program entry point
 * 
 * Reads the board configuration from standard input and displays all
 * available moves based on the empty space position.
 */
int main(){
      vector<vector<char>> board;
      string in;
      cin >> in;
      for (int r = 0; r < 4; ++r){
            vector<char> row;
            for (int i = 0; i < 4; i++)
            {
                  row.push_back(in[4 * r + i]);
            }
            board.push_back(row);
      }
      listAvailable(board);
      return 0;
}