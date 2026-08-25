/*
 * Q2. Rescue Robot - Depth Limited Search (DLS)
 * DLS = standard grid DFS + a depth cap.
 * Standard CP grid-DFS template: vis[][] array, dx[]/dy[] direction
 * arrays, dfs(x, y, depth) recursive function, backtrack by unmarking
 * vis on the way out.
 */

#include <bits/stdc++.h>
using namespace std;

#define pii pair<int,int>
#define fi first
#define se second

const int N = 10;

int n = N, m = N;
vector<string> grid;
bool vis[N][N];
int limitL;
bool found_;
bool cutoff_;
int maxDepth;
vector<pii> path_, ans, order_;

// Right, Down, Left, Up
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

pii startPos, goalPos;

bool isValid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m && grid[x][y] != 'X';
}

void dfs(int x, int y, int depth) {
    order_.push_back({x, y});
    maxDepth = max(maxDepth, depth);

    if (x == goalPos.fi && y == goalPos.se) {
        found_ = true;
        ans = path_;
        return;
    }

    if (depth == limitL) {
        cutoff_ = true;
        return; // depth cutoff, backtrack
    }

    vis[x][y] = true;

    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (isValid(nx, ny) && !vis[nx][ny]) {
            path_.push_back({nx, ny});
            dfs(nx, ny, depth + 1);
            if (found_) {
                vis[x][y] = false;
                return;
            }
            path_.pop_back(); // backtrack: undo the move
        }
    }

    vis[x][y] = false; // backtrack: unmark so other branches can use this cell
}

void solve(int limit) {
    memset(vis, false, sizeof(vis));
    path_.clear();
    order_.clear();
    ans.clear();
    found_ = false;
    cutoff_ = false;
    maxDepth = 0;
    limitL = limit;

    path_.push_back(startPos);
    dfs(startPos.fi, startPos.se, 0);

    cout << "--- DLS with limit = " << limit << " ---\n";
    cout << "Expanded nodes: ";
    for (auto &p : order_) cout << "(" << p.fi << "," << p.se << ") ";
    cout << "\n";
    cout << "Max depth reached: " << maxDepth << "\n";

    if (found_) {
        cout << "Result: SUCCESS\n";
        cout << "Path: ";
        for (int i = 0; i < (int)ans.size(); i++) {
            cout << "(" << ans[i].fi << "," << ans[i].se << ")";
            if (i + 1 < (int)ans.size()) cout << " -> ";
        }
        cout << "\nDepth of solution: " << ans.size() - 1 << "\n";
    } else if (cutoff_) {
        cout << "Result: CUTOFF\n";
    } else {
        cout << "Result: FAILURE\n";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    grid = {
        "SRRXRRRRRR",
        "XXRXRXXXXR",
        "RRRXRRRRXR",
        "RXXXXXXRXR",
        "RRRRRRXRRR",
        "XXXXXRXXXR",
        "RRRRXRRRRR",
        "RXXRXXXXXR",
        "RRRRRRRRRR",
        "XXXXXXXXXG",
    };

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'S') startPos = {i, j};
            if (grid[i][j] == 'G') goalPos = {i, j};
        }

    solve(18);
    solve(25);

    return 0;
}