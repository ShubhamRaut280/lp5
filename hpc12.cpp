#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void dfs(int node) {
    stack<int> s;
    s.push(node);

    while (!s.empty()) {
        int curr_node = s.top();
        s.pop();

        if (!visited[curr_node]) {
            visited[curr_node] = true;
            cout << curr_node << " ";

            // This loop is not truly parallel-safe because of stack operations,
            // but parallelizing for demonstration
            #pragma omp parallel for
            for (int i = 0; i < graph[curr_node].size(); i++) {
                int adj_node = graph[curr_node][i];

                // Use critical section to avoid race condition on the stack
                if (!visited[adj_node]) {
                    #pragma omp critical
                    {
                        s.push(adj_node);
                    }
                }
            }
        }
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter No of Nodes, Edges, and Start Node: ";
    cin >> n >> m >> start_node;

    cout << "Enter Pair of Edges:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    dfs(start_node);
    cout<<"\n";

    // Optional: print all visited nodes
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            cout << i << " ";
        }
    }

    return 0;
}
