/*
 * Problem 1: Tic-Tac-Toe Solver (Traditional Programming)
 * -------------------------------------------------------
 * Reads a 3x3 board (matrix) as input and determines the result:
 * 'X' wins, 'O' wins, Draw, or game still in progress.
 *
 * Time Complexity Analysis:
 * - The board is fixed size 3x3 (n = 3), so all checks are O(1) in the
 *   general "board size" sense, but if we generalize to an n x n board:
 *     - Checking all rows        -> O(n^2)
 *     - Checking all columns     -> O(n^2)
 *     - Checking both diagonals  -> O(n)
 *     - Checking for a draw      -> O(n^2)
 *   Overall Time Complexity: O(n^2)  (O(9) = O(1) for the classic 3x3 case)
 *   Space Complexity: O(n^2) for storing the board.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3;

// Checks rows, columns and diagonals for a winner.
// Returns 'X' or 'O' if there's a winner, or '\0' if none.
char checkWinner(vector<vector<char>> &board) {
    // Check rows
    for (int i = 0; i < N; i++) {
        if (board[i][0] != '_' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
    }
    // Check columns
    for (int j = 0; j < N; j++) {
        if (board[0][j] != '_' && board[0][j] == board[1][j] && board[1][j] == board[2][j])
            return board[0][j];
    }
    // Check main diagonal
    if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    // Check anti-diagonal
    if (board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return '\0'; // no winner yet
}

bool isBoardFull(vector<vector<char>> &board) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == '_')
                return false;
    return true;
}

void printBoard(vector<vector<char>> &board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << board[i][j];
            if (j < N - 1) cout << " | ";
        }
        cout << "\n";
        if (i < N - 1) cout << "---------\n";
    }
}

int main() {
    vector<vector<char>> board(N, vector<char>(N));

    cout << "Enter the 3x3 Tic-Tac-Toe board.\n";
    cout << "Use 'X' for player X, 'O' for player O, and '_' for empty cell.\n";
    cout << "Enter row by row (space separated), e.g.:  X O _\n\n";

    for (int i = 0; i < N; i++) {
        cout << "Row " << (i + 1) << ": ";
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
        }
    }

    cout << "\nInput Board:\n";
    printBoard(board);

    char winner = checkWinner(board);

    cout << "\nResult: ";
    if (winner == 'X' || winner == 'O') {
        cout << "Player '" << winner << "' wins!\n";
    } else if (isBoardFull(board)) {
        cout << "The game is a Draw.\n";
    } else {
        cout << "Game is still in progress (no winner yet).\n";
    }

    cout << "\nTime Complexity: O(n^2) for an n x n board (O(1) for classic 3x3).\n";
    cout << "Space Complexity: O(n^2) for storing the board.\n";

    return 0;
}
