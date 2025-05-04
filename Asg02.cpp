#include<iostream>

#include<vector>
#include<stack>

#include<omp.h>
#include<chrono>

using namespace std;
using namespace chrono;

class Graph{

    public:
        int V;
        vector<vector<int>> adj;
        vector<bool> vis;

        Graph(int v) {
            V = v;
            adj.resize(V);
            vis.resize(V);

            clearVis();
        }

        void clearVis() {
            for(int i=0; i<V; i++) {
                vis[i] = false;
            }
        }

        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void sequentialDFS(int start=0) {

            auto startTime = high_resolution_clock::now();

            stack<int> s;
            s.push(start);
            vis[start] = true;

            while(!s.empty()) {
                int curr = s.top();
                s.pop();

                cout << curr << " ";

                for(int i=0; i<adj[curr].size(); i++) {
                    int neigh = adj[curr][i];

                    if(!vis[neigh]) {
                        vis[neigh] = true;
                        s.push(neigh);
                    }
                }
            }
            cout << endl;
            clearVis();

            auto endTime = high_resolution_clock::now();

            duration<double, micro> duration = endTime - startTime;

            cout << "Time taken for Sequential DFS : " << duration.count() << endl;
        }

        void parallelDFS(int start=0) {

            auto startTime = high_resolution_clock::now();

            stack<int> s;
            s.push(start);
            vis[start] = true;

            while(!s.empty()) {

                int curr = s.top();
                s.pop();

                cout << curr << " ";

                #pragma omp parallel for
                for(int i=0; i<adj[curr].size(); i++) {
                    int neigh = adj[curr][i];

                    if(!vis[neigh]) {
                        #pragma omp critical
                        {
                            if(!vis[neigh]) {
                                vis[neigh] = true;
                                s.push(neigh);
                            }
                        }
                    }
                }
            }
            cout << endl;

            auto endTime = high_resolution_clock::now();

            duration<double, micro> duration = endTime - startTime;

            cout << "Time taken for Parallel DFS : " << duration.count() << endl;
        }
};

int main() {

    int v, e;

    cout << "Enter number of vertices: ";
    cin >> v;

    Graph * graph = new Graph(v);

    cout << "Enter number of edges: ";
    cin >> e;

    for(int i=1; i<=e; i++) {
        int u, v;
        cout << "Edge " << i << " : (u,v): ";
        cin >> u >> v;
        graph->addEdge(u, v);
    }
    cout << "Graph saved !!\n\n";

    graph->sequentialDFS();
    graph->parallelDFS();
}