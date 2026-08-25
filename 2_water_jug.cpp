
#include <bits/stdc++.h>
using namespace std;

const int CAP_A = 4; // capacity of jug A (4-litre)
const int CAP_B = 3; // capacity of jug B (3-litre)
const int GOAL = 2;  // target amount of water in Jug A

struct State
{
    int a, b;
    bool operator==(const State &other) const
    {
        return a == other.a && b == other.b;
    }
};

// Hash function so State can be used in unordered_map/unordered_set
struct StateHash
{
    size_t operator()(const State &s) const
    {
        return hash<int>()(s.a) * 31 + hash<int>()(s.b);
    }
};

int main()
{
    State start = {0, 0};

    // BFS setup
    queue<State> q;
    unordered_set<State, StateHash> visited;
    unordered_map<State, pair<State, string>, StateHash> parent; // child -> (parent, action)

    q.push(start);
    visited.insert(start);

    State goalState = {-1, -1};
    bool found = false;

    while (!q.empty() && !found)
    {
        State cur = q.front();
        q.pop();

        if (cur.a == GOAL)
        {
            goalState = cur;
            found = true;
            break;
        }

        // Generate all possible next states with action labels
        vector<pair<State, string>> nextStates;

        // 1. Fill Jug A
        nextStates.push_back({{CAP_A, cur.b}, "Fill Jug A (4L)"});
        // 2. Fill Jug B
        nextStates.push_back({{cur.a, CAP_B}, "Fill Jug B (3L)"});
        // 3. Empty Jug A
        nextStates.push_back({{0, cur.b}, "Empty Jug A"});
        // 4. Empty Jug B
        nextStates.push_back({{cur.a, 0}, "Empty Jug B"});
        // 5. Pour A -> B
        {
            int pour = min(cur.a, CAP_B - cur.b);
            nextStates.push_back({{cur.a - pour, cur.b + pour}, "Pour Jug A -> Jug B"});
        }
        // 6. Pour B -> A
        {
            int pour = min(cur.b, CAP_A - cur.a);
            nextStates.push_back({{cur.a + pour, cur.b - pour}, "Pour Jug B -> Jug A"});
        }

        for (auto &[nxt, action] : nextStates)
        {
            if (visited.find(nxt) == visited.end())
            {
                visited.insert(nxt);
                parent[nxt] = {cur, action};
                q.push(nxt);
            }
        }
    }

    if (!found)
    {
        cout << "No solution found.\n";
        return 0;
    }

    // Reconstruct path from goalState back to start
    vector<pair<State, string>> path;
    State cur = goalState;
    while (!(cur == start))
    {
        auto &[par, action] = parent[cur];
        path.push_back({cur, action});
        cur = par;
    }
    path.push_back({start, "Start"});
    reverse(path.begin(), path.end());

    cout << "Water Jug Problem: Get exactly " << GOAL << " litres in the "
         << CAP_A << "-litre jug.\n\n";
    cout << "Solution steps (BFS shortest path):\n";
    cout << "-------------------------------------\n";
    for (size_t i = 0; i < path.size(); i++)
    {
        cout << i << ". " << left << setw(22) << path[i].second
             << " -> State (JugA=" << path[i].first.a
             << "L, JugB=" << path[i].first.b << "L)\n";
    }

    cout << "\nGoal reached: Jug A contains exactly " << GOAL << " litres.\n";
    cout << "Total number of moves: " << (path.size() - 1) << "\n";

    return 0;
}
