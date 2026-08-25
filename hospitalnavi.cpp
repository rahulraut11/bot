#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Directions: Up, Down, Left, Right
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

class HospitalNavigation {
    vector<string> grid;
    int start_x, start_y;
    int rows, cols;

public:
    HospitalNavigation(vector<string> g) : grid(g) {
        rows = grid.size();
        cols = grid[0].size();
        
        // Find the starting position 'S'
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 'S') {
                    start_x = i;
                    start_y = j;
                }
            }
        }
    }

    void findNearestEmergencyRoom() {
        // Using std::pair to represent (x, y) coordinates
        queue<pair<int, int>> q;
        
        // Grids to keep track of visited cells, path distances, and parents for route tracing
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
        vector<vector<int>> dist(rows, vector<int>(cols, 0));

        // Start BFS from 'S'
        q.push({start_x, start_y});
        visited[start_x][start_y] = true;

        int target_x = -1, target_y = -1;

        while (!q.empty()) {
            auto curr = q.front();
            q.pop();
            int cx = curr.first;
            int cy = curr.second;

            // If we found an available emergency room, stop immediately (BFS guarantees it's the closest)
            if (grid[cx][cy] == 'E') {
                target_x = cx;
                target_y = cy;
                break; 
            }

            // Check all 4 neighboring cells
            for (int i = 0; i < 4; i++) {
                int nx = cx + dx[i], ny = cy + dy[i];

                // Rule: Must be within bounds, not visited, not a blocked area ('X'), and not an occupied room ('O')
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
                    !visited[nx][ny] && grid[nx][ny] != 'X' && grid[nx][ny] != 'O') {
                    
                    visited[nx][ny] = true;                 // Mark as visited
                    parent[nx][ny] = {cx, cy};              // Remember where we came from
                    dist[nx][ny] = dist[cx][cy] + 1;        // Increment distance
                    q.push({nx, ny});                       // Add to queue for exploration
                }
            }
        }

        // Print final outputs
        if (target_x != -1) {
            cout << "Nearest available emergency room: (" << target_x << ", " << target_y << ")\n";
            cout << "Minimum number of movements: " << dist[target_x][target_y] << "\n";

            // Reconstruct the route by stepping backward through the parent map
            vector<pair<int, int>> route;
            int cx = target_x, cy = target_y;
            while (cx != -1 && cy != -1) {
                route.push_back({cx, cy});
                pair<int, int> p = parent[cx][cy];
                cx = p.first;
                cy = p.second;
            }
            // Reverse it so it prints from Start to End
            reverse(route.begin(), route.end());

            cout << "Route: ";
            for (size_t i = 0; i < route.size(); i++) {
                cout << "(" << route[i].first << "," << route[i].second << ")";
                if (i != route.size() - 1) cout << " -> ";
            }
            cout << "\n\n";
            
            // Print Complexities
            cout << "--- Complexity Analysis ---\n";
            cout << "Time Complexity: O(N * M)\n";
            cout << "Space Complexity: O(N * M)\n";
            cout << "(Where N is the number of rows and M is the number of columns in the grid.)\n";
            
        } else {
            cout << "No reachable emergency rooms available.\n";
        }
    }
};

int main() {
    // 10x10 Hospital grid (C = Corridor, S = Start, E = Emergency, X = Blocked, O = Occupied)
    vector<string> grid = {
        "S C C X C C C C E C",
        "X X C C C O X C C C",
        "E C C O X C C C X C",
        "C C C O C X X C O C",
        "C X C C C C X C C C",
        "C C C X X C E C C C",
        "X X C C C C C X X X",
        "C O C X E C C C C C",
        "C C C C C X O C C C",
        "X X C C E C C C X X"
    };

    HospitalNavigation hospital(grid);
    hospital.findNearestEmergencyRoom();

    return 0;
}