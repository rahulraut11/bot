/*
 * Q1. Cost-Aware Delivery Robot using Uniform Cost Search (UCS)
 * ----------------------------------------------------------------
 * UCS = Dijkstra's algorithm on a grid. Each cell is a node; moving into
 * a neighbouring cell costs whatever that cell's terrain cost is.
 * UCS always expands the q node with smallest accumulated cost
 * g(n), using a min-heap (priority_queue).
 */

#include <bits/stdc++.h>
using namespace std;

const int ROWS = 10, COLS = 10;

// terrain -> cost to ENTER that cell
int costOf(char c) {
    switch (c) {
        case 'S': return 0;
        case 'G': return 1;
        case 'R': return 1;
        case 'M': return 3;
        case 'T': return 5;
        default:  return -1; // 'X' or invalid, never used
    }
}

// moves: up, down, left, right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

pair<int,int> findSymbol(vector<string> &grid, char sym) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (grid[r][c] == sym) return {r, c};
    return {-1, -1};
}

int main() {
    vector<string> grid = {
        "SRRTTRRRRR",
        "XXRTXRXXXR",
        "RRRTRRRRXR",
        "RXXTRXXRXR",
        "RRRRRRXRRR",
        "RXXXXRXXXR",
        "RRRRXRRRRR",
        "XXXRXXXXXR",
        "RRRRRRRRRR",
        "RXXXXXXXXG",
    };
    auto start = findSymbol(grid, 'S');
    auto goal  = findSymbol(grid, 'G');

    cout << "--- Uniform Cost Search ---\n";
    cout << "Start: (" << start.first << "," << start.second << ")\n";
    cout << "Goal: (" << goal.first << "," << goal.second << ")\n\n";

    if (start.first == -1 || goal.first == -1) {
        cout << "Start or Goal not found in grid.\n";
        return 0;
    }

    // g(n): best known accumulated cost to reach a state
    vector<vector<int>> g(ROWS, vector<int>(COLS, INT_MAX));
    // parent pointers to reconstruct path
    vector<vector<pair<int,int>>> parent(ROWS, vector<pair<int,int>>(COLS, {-1,-1}));
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    // min-heap of (cost, {row, col}) - smallest cost on top
    priority_queue<
        pair<int, pair<int,int>>,
        vector<pair<int, pair<int,int>>>,
        greater<>
    > q;

    g[start.first][start.second] = 0;
    q.push({0, start});

    vector<pair<int,int>> expansionOrder;
    bool goalReached = false;

    while (!q.empty()) {
        auto [cost, node] = q.top();
        q.pop();
        int r = node.first, c = node.second;

        if (visited[r][c]) continue; // stale entry, skip
        visited[r][c] = true;
        expansionOrder.push_back(node);

        if (node == goal) { goalReached = true; break; }

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) continue;
            if (grid[nr][nc] == 'X') continue;
            if (visited[nr][nc]) continue;

            int stepCost = costOf(grid[nr][nc]);
            int newCost = cost + stepCost;

            if (newCost < g[nr][nc]) {
                g[nr][nc] = newCost;
                parent[nr][nc] = {r, c};
                q.push({newCost, {nr, nc}});
            }
        }
    }

    cout << "Order of node expansion:\n";
    for (auto &p : expansionOrder) cout << "(" << p.first << "," << p.second << ") ";
    cout << "\n\n";

    if (!goalReached) {
        cout << "No path exists\n";
        return 0;
    }

    // reconstruct path
    vector<pair<int,int>> path;
    pair<int,int> node = goal;
    while (node.first != -1) {
        path.push_back(node);
        node = parent[node.first][node.second];
    }
    reverse(path.begin(), path.end());

    cout << "Minimum-cost route:\n";
    for (size_t i = 0; i < path.size(); i++) {
        cout << "(" << path[i].first << "," << path[i].second << ")";
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n\n";
    cout << "Total movements: " << path.size() - 1 << "\n";
    cout << "Total path cost: " << g[goal.first][goal.second] << "\n";

    return 0;
}
