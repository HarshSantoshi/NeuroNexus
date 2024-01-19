#include <iostream>
#include <vector>

using namespace std;

const int BOARD_SIZE = 3;

void printBoard(const vector<vector<string>>& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << "\t \t \t \t \t \t";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j] << "  ";
        }
        cout << endl << endl;
    }
}

bool isValidMove(const vector<vector<string>>& board, int row, int col) {
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == "_");
}

bool winCondition(const vector<vector<string>>& board, const string& currPlayer, int row, int col) {
    bool isWinner = true;
    for (int j = 0; j < 3; j++) {
        if (board[row][j] != currPlayer) {
            isWinner = false;
            break;
        }
    }
    if (isWinner) return true;

    isWinner = true;
    for (int i = 0; i < 3; i++) {
        if (board[i][col] != currPlayer) {
            isWinner = false;
            break;
        }
    }
    if (isWinner) return true;

    if (row == col) {
        isWinner = true;
        for (int i = 0; i < 3; i++) {
            if (board[i][i] != currPlayer) {
                isWinner = false;
                break;
            }
        }
    }
    if (isWinner) return true;

    if (row + col == 2) {
        isWinner = true;
        for (int i = 0; i < 3; i++) {
            if (board[i][2 - i] != currPlayer) {
                isWinner = false;
                break;
            }
        }
    }
    return isWinner;
}

bool isBoardFull(const vector<vector<string>>& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == "_") {
                return false; 
            }
        }
    }
    return true;  
}

void updateBoard(vector<vector<string>>& board, int row, int col, const string& currPlayer) {
    board[row][col] = currPlayer;
}

string start(vector<vector<string>>& board) {
    string player1 = "X";
    string player2 = "O";
    string currPlayer = player1;
    int steps = BOARD_SIZE * BOARD_SIZE;

    while (steps--) {
        cout << "Player " << currPlayer << "'s turn" << endl;
        cout << "Enter row and column" << endl;
        int row, column;
        cin >> row >> column;

        if (isValidMove(board, row - 1, column - 1)) {
            updateBoard(board, row - 1, column - 1, currPlayer);
            printBoard(board);

            if (winCondition(board, currPlayer, row - 1, column - 1)) {
                return currPlayer;
            }

            currPlayer = (currPlayer == player1) ? player2 : player1;
        } else {
            cout << "Invalid move. Try again." << endl;
            cout<<endl;
            steps++;
        }
    }
    return "";
}

int main() {
    cout << "\t \t \t \tWelcome to Tic Tac Toe Game :" << endl;
    vector<vector<string>> board(BOARD_SIZE, vector<string>(BOARD_SIZE, "_"));
    printBoard(board);

    string winner = start(board);

    if (!winner.empty()) {
        cout << "Player " << winner << " won!" << endl;
    } else {
        if (isBoardFull(board)) {
            cout << "It's a draw!" << endl;
        } else {
            cout << "Game ended unexpectedly!" << endl;
        }
    }

    return 0;
}
