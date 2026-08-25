#include <bits/stdc++.h>
using namespace std;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
struct Node {
    int x, y, dist;
    double  cost;
};
struct CompareCost {
    bool operator()(const Node& a, const Node& b) {
        return a.cost > b.cost;
    }
};
class DeliveryRobot {
    vector<string> grid;
    int sx, sy, px, py, gx, gy;
    int bfs(int s_x, int s_y, int t_x, int t_y) {
        queue<Node> q;
        vector<vector<bool>> vis(grid.size(), vector<bool>(grid[0].size(), false));
        q.push({s_x, s_y, 0, 0.0});
        vis[s_x][s_y] = true;
        while(!q.empty()) {
            Node cur = q.front();
            q.pop();
            if(cur.x == t_x && cur.y == t_y) return cur.dist; // Cor
            for(int i = 0; i < 4; i++) {
                int nx = cur.x + dx[i], ny = cur.y + dy[i];
                if(nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != 'X' && !vis[nx][ny]) {
                    vis[nx][ny] = true;
                    q.push({nx, ny, cur.dist+1, 0.0});
                }
            }
        }
        return -1;
    }
    double ucs(int s_x, int s_y, int t_x, int t_y, int &movements) {
        priority_queue<Node, vector<Node> , CompareCost> q;
        vector<vector<double>> min_cost(grid.size(), vector<double>(grid[0].size(), 1e9));
        q.push({s_x, s_y, 0, 0.0});
        min_cost[s_x][s_y] = 0;
        while(!q.empty()) {
            Node cur = q.top();
            q.pop();
            if(cur.x == t_x && cur.y == t_y) {
                movements = cur.dist;
                return cur.cost;
            }
            for(int i = 0; i < 4; i++) {
                int nx = cur.x + dx[i], ny = cur.y + dy[i];
                if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != 'X') {
                    double cell_cost = (0.4 * 1) + (0.3 * 2) + (0.2 * 1) + (0.1 * 1);
                    double new_cost = cell_cost+cur.cost;
                    if(new_cost < min_cost[nx][ny]) {
                        min_cost[nx][ny] = new_cost;
                        q.push({nx, ny, cur.dist+1, new_cost});
                    }
                }
            }
        }
        return -1.0; 
    }
public:
    DeliveryRobot(vector<string> g) : grid(g) {
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == 'S') { sx = i; sy = j; }
                if (grid[i][j] == 'P') { px = i; py = j; }
                if (grid[i][j] == 'G') { gx = i; gy = j; }
            }
        }
    }

    void runGoalBased() {
        int m1 = bfs(sx, sy, px, py);
        int m2 = bfs(px, py, gx, gy);
        cout << "Goal-Based Agent ---\n";
        cout << "Movements to Package: " << m1 << "\n";
        cout << "Movements to Destination: " << m2 << "\n";
        cout << "Total Movements: " << (m1 + m2) << "\n\n";
    }

    void runUtilityBased() {
        int m1 = 0, m2 = 0;
        double c1 = ucs(sx, sy, px, py, m1);
        double c2 = ucs(px, py, gx, gy, m2);
        cout << "Utility-Based Agent ---\n";
        cout << "Total Movements: " << (m1 + m2) << "\n";
        cout << "Total Cost: " << (c1 + c2) << "\n";
    }
};

int main() {
    vector<string> grid = {
        "SRRXRRRRRR", "XXRRRXRXRR", "RRRXRRRPRR", "RXRRRXRRRX", 
        "RRRXRRXRRR", "XRRRRRRRXR", "RRXRXRRRRR", "RRRRRXRXRR", 
        "RXRRRRRRRR", "RRRXRRRRXG"
    };
    DeliveryRobot robot(grid);
    robot.runGoalBased();
    robot.runUtilityBased();
    return 0;
}

