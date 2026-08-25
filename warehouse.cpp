#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

class WarehouseRobot {
    vector<string> orig_grid;
    int sx, sy;
    int total_i;

public:
    WarehouseRobot(vector<string> g) : orig_grid(g) {
        total_i = 0;
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                if (g[i][j] == 'S') {
                    sx = i;
                    sy = j;
                }
                if (g[i][j] == 'I') {
                    total_i++;
                }
            }
        }
    }

    void runSimpleReflex() {
        vector<string> grid = orig_grid;
        int x = sx, y = sy;
        int inspected = 0, moves = 0, repeats = 0, actions = 0;
        set<pair<int, int>> visited;

        while (inspected < total_i && actions < 2000) {
            actions++;
            visited.insert({x, y});

            if (grid[x][y] == 'I') {
                grid[x][y] = 'N';
                inspected++;
            } else {
                bool moved = false;
                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i], ny = y + dy[i];
                    if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != 'X') {
                        if (visited.count({nx, ny})) {
                            repeats++;
                        }
                        x = nx;
                        y = ny;
                        moves++;
                        moved = true;
                        break;
                    }
                }
                if (!moved) break;
            }
        }
        cout << "--- Simple Reflex Agent ---\n";
        cout << "Locations Inspected: " << inspected << " / " << total_i << "\n";
        cout << "Total Movements: " << moves << "\n";
        cout << "Repeated Visits: " << repeats << "\n\n";
    }

    void runModelBased() {
        vector<string> grid = orig_grid;
        int x = sx, y = sy;
        int inspected = 0, moves = 0, repeats = 0;
        set<pair<int, int>> visited;
        vector<pair<int, int>> path;

        while (inspected < total_i) {
            visited.insert({x, y});

            if (grid[x][y] == 'I') {
                grid[x][y] = 'N';
                inspected++;
            } else {
                bool moved = false;
                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i], ny = y + dy[i];
                    if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != 'X') {
                        if (!visited.count({nx, ny})) {
                            path.push_back({x, y});
                            x = nx;
                            y = ny;
                            moves++;
                            moved = true;
                            break;
                        }
                    }
                }
                
                if (!moved) {
                    if (!path.empty()) {
                        x = path.back().first;
                        y = path.back().second;
                        path.pop_back();
                        moves++;
                        repeats++;
                    } else {
                        break;
                    }
                }
            }
        }
        cout << "--- Model-Based Reflex Agent ---\n";
        cout << "Locations Inspected: " << inspected << " / " << total_i << "\n";
        cout << "Total Movements: " << moves << "\n";
        cout << "Repeated Visits: " << repeats << "\n\n";
    }
};

int main() {
    vector<string> grid = {
        "SNINX",
        "NXNNI",
        "INNXN",
        "NNINN",
        "XNNNI"
    };

    WarehouseRobot robot(grid);
    robot.runSimpleReflex();
    robot.runModelBased();

    cout << "--- Performance Comparison ---\n";
    cout << "The Model-Based Reflex Agent performs significantly better at avoiding unnecessary repeated movements. By utilizing its internal state (memory of visited cells), it actively prioritizes unvisited locations, whereas the Simple Reflex Agent often gets trapped in endless oscillation loops due to its rigid, memoryless movement priorities.\n";

    return 0;
}