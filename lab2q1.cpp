#include <bits/stdc++.h>
using namespace std;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
class CleaningRobot {
    public:
    vector<string> og;
    int sx, sy;
    int td;
    mt19937 rng;
    
    CleaningRobot(vector<string> grid, int x, int y, int dc) {
        og = grid;
        sx = x;
        sy = y;
        td = dc;
        rng.seed(1337);
    }
    void runSimpleReflex() {
        vector<string> grid = og;
        int actions = 0, movements = 0, cleaned = 0, repeats = 0;
        int x = sx, y = sy;
        while(cleaned < td && actions < 5000) {
            actions++;
            if(grid[x][y] == 'D') {
                cleaned++;
                grid[x][y] = 'C';
            }
            else {
                vector<int> dirs = {0 ,1 ,2 ,3};
                shuffle(dirs.begin(), dirs.end(), rng);
                bool moved = false;
                for(int i: dirs) {
                    int nx = x + dx[i], ny = dy[i];
                    if(nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != 'X') {
                        movements++;
                        repeats++;
                        x = nx;
                        y = ny;
                        moved = true;
                        break;
                    }
                }
                if(!moved) {
                    break;
                }
            }
        }
        cout << "Simple Reflex Agent ---\n";
        cout << "Dirty cells cleaned: " << cleaned << "\nMovements: " << movements 
             << "\nTotal actions: " << actions << "\nRepeated visits: " << repeats << "\n\n";

    }
    
    
    void runModelBased() {
        vector<string> grid = og;
        int actions = 0, movements = 0, cleaned = 0, repeats = 0;
        int x = sx, y = sy;
        set<pair<int, int>> vis;
        vector<pair<int, int>> path;
        while(cleaned < td) {
            actions++;
            vis.insert({x,y});
            if(grid[x][y] == 'D') {
                cleaned++;
                grid[x][y] = 'C';
            }
            else {
                bool moved = false;
                for(int i = 0; i < 4; i++) {
                    int nx = x + dx[i], ny = dy[i];
                    if(nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != 'X') {
                        if(vis.find({nx, ny}) == vis.end()) {
                            path.push_back({x,y});
                            movements++;
                            repeats++;
                            x = nx;
                            y = ny;
                            moved = true;
                            break;
                        }
                    }
                }
                if(!moved) {
                    if(!path.empty()) {
                        x = path.back().first;
                        y = path.back().second;
                        path.pop_back();
                        movements++;
                        repeats++;
                    }
                    else break;
                }
            }
        }
        cout << "Model-Based Reflex Agent ---\n";
        cout << "Dirty cells cleaned: " << cleaned << "\nMovements: " << movements 
             << "\nTotal actions: " << actions << "\nRepeated visits: " << repeats << "\n\n";
    }
    

};
int main() {
	// your code goes here
	vector<string> grid = {
        "SCDCXCCDCC", "CXCCCCXCDC", "DCCXDCCCCC", "CCXCCCDXCC", 
        "CDCCXCCCCD", "CCCDCCXCCC", "XCCCCDCCXC", "CCDXCCCDCC", 
        "CXCCDCCCCC", "DCCCCXCCDC"
    };
    CleaningRobot robot(grid, 0, 0, 8);
    robot.runSimpleReflex();
    robot.runModelBased();
    return 0;

}
