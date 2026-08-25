/*
 * Problem 3: Smart Parking System using BFS
 * ------------------------------------------
 * Grid symbols:
 *   E -> Entrance / starting position
 *   A -> Available parking space
 *   O -> Occupied parking space
 *   R -> Road (vehicle can move through)
 *   X -> Blocked area / wall
 *
 * The vehicle moves one cell at a time in 4 directions (up, down, left,
 * right) through R and A cells only (O and X cannot be crossed).
 * BFS is used since all moves have equal cost -> BFS guarantees the
 * first available space found is the nearest one.
 *
 * The program accepts a grid of size at least 10 x 10, provided as
 * input by the user (row by row, space separated symbols).
 */

#include <bits/stdc++.h>
using namespace std;

int rows, cols;
vector<vector<char>> grid;

struct Point {
    int r, c;
};

bool isValid(int r, int c) {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

// Returns true if BFS finds a reachable available space; fills
// nearestSpot, moveCount, and route (entrance -> ... -> spot).
bool bfsFindNearestParking(Point entrance, Point &nearestSpot,
                            int &moveCount, vector<Point> &route) {
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1, -1}));
    vector<vector<int>> dist(rows, vector<int>(cols, -1));

    queue<Point> q;
    q.push(entrance);
    visited[entrance.r][entrance.c] = true;
    dist[entrance.r][entrance.c] = 0;

    // 4 directions: up, down, left, right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    Point found = {-1, -1};
    bool goalReached = false;

    while (!q.empty() && !goalReached) {
        Point cur = q.front();
        q.pop();

        // If this cell itself is an available space (and not the entrance
        // cell trivially, though entrance won't be 'A'), we could stop.
        if (grid[cur.r][cur.c] == 'A') {
            found = cur;
            goalReached = true;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = cur.r + dr[d];
            int nc = cur.c + dc[d];

            if (!isValid(nr, nc)) continue;
            if (visited[nr][nc]) continue;

            char cell = grid[nr][nc];
            // Vehicle can only move through Road or Available cells
            if (cell != 'R' && cell != 'A') continue;

            visited[nr][nc] = true;
            parent[nr][nc] = cur;
            dist[nr][nc] = dist[cur.r][cur.c] + 1;
            q.push({nr, nc});
        }
    }

    if (!goalReached) return false;

    nearestSpot = found;
    moveCount = dist[found.r][found.c];

    // Reconstruct path from found back to entrance
    vector<Point> path;
    Point cur = found;
    while (!(cur.r == entrance.r && cur.c == entrance.c)) {
        path.push_back(cur);
        cur = parent[cur.r][cur.c];
    }
    path.push_back(entrance);
    reverse(path.begin(), path.end());

    route = path;
    return true;
}

int main() {
    cout << "===== Smart Parking System (BFS) =====\n\n";
    cout << "Enter grid dimensions (rows and columns, minimum 10 x 10):\n";
    cout << "Rows: ";
    cin >> rows;
    cout << "Columns: ";
    cin >> cols;

    if (rows < 10 || cols < 10) {
        cout << "\nNote: Assignment requires a grid of size at least 10 x 10. "
             << "Proceeding with entered size anyway.\n\n";
    }

    grid.assign(rows, vector<char>(cols));

    cout << "\nEnter the grid row by row, space-separated symbols.\n";
    cout << "Symbols: E=Entrance, A=Available, O=Occupied, R=Road, X=Blocked\n\n";

    Point entrance = {-1, -1};
    int entranceCount = 0;

    for (int i = 0; i < rows; i++) {
        cout << "Row " << i << ": ";
        for (int j = 0; j < cols; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 'E') {
                entrance = {i, j};
                entranceCount++;
            }
        }
    }

    if (entranceCount == 0) {
        cout << "\nError: No entrance 'E' found in the grid.\n";
        return 1;
    }
    if (entranceCount > 1) {
        cout << "\nWarning: Multiple entrances found; using the last one encountered.\n";
    }

    cout << "\nGrid entered:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) cout << grid[i][j] << ' ';
        cout << "\n";
    }

    Point nearestSpot;
    int moveCount;
    vector<Point> route;

    bool result = bfsFindNearestParking(entrance, nearestSpot, moveCount, route);

    cout << "\n===== Result =====\n";
    if (!result) {
        cout << "No reachable available parking space found from the entrance.\n";
    } else {
        cout << "Nearest available parking space: (" << nearestSpot.r << ", "
             << nearestSpot.c << ")\n";
        cout << "Number of movements required: " << moveCount << "\n";
        cout << "Route: ";
        for (size_t i = 0; i < route.size(); i++) {
            cout << "(" << route[i].r << ", " << route[i].c << ")";
            if (i != route.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }

    return 0;
}
