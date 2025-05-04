#include<iostream>

#include<vector>
#include<queue>

#include<chrono>
#include<omp.h>

using namespace std;
using namespace chrono;

class Graph {
    public:
        int V;
        vector<vector<int>> adj;
        vector<bool> vis;

        Graph(int v) {
            V = v;
            adj.resize(V);
            vis.resize(V);

            clearVisited();
        }

        void clearVisited() {
            for(int i=0; i<V; i++) {
                vis[i] = false;
            }
        }

        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void sequentialBFS(int start) {

            queue<int> q;
            q.push(start);
            vis[start] = true;

            while(!q.empty()) {
                int curr = q.front();
                q.pop();

                cout << curr << " ";

                for(int i=0; i<adj[curr].size(); i++) {
                    int neigh = adj[curr][i];

                    if(!vis[neigh]) {
                        q.push(neigh);
                        vis[neigh] = true;
                    }
                }
            }
            cout << endl;
            clearVisited();
        }

        void parallelBFS(int start) {

            queue<int> q;
            q.push(start);
            vis[start] = true;

            while(!q.empty()) {
                int curr = q.front();
                q.pop();

                cout << curr << " ";

                #pragma omp parallel for
                for(int i=0; i<adj[curr].size(); i++) {
                    int neigh = adj[curr][i];

                    if(!vis[neigh]) {
                        
                        #pragma omp critical
                        {
                            if(!vis[neigh]) {
                                q.push(neigh);
                                vis[neigh] = true;
                            }
                        }
                    }
                }
            }
            cout << endl;
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
        cout << "Edge " << i << " (u v) : ";
        cin >> u >> v;
        graph->addEdge(u, v);
    }

    cout << "Graph with " << v << " vertices & " << e << " edges has been created.\n";

    cout << "--- SEQUENTIAL BFS ---\n";
    auto seqStart = high_resolution_clock::now();
    graph->sequentialBFS(0);
    auto seqEnd = high_resolution_clock::now();
    duration<double, milli> timeSeq = seqEnd-seqStart;
    cout << "Time taken: " << timeSeq.count() << "ms\n";
    auto parStart = high_resolution_clock::now();
    graph->parallelBFS(0);
    auto parEnd = high_resolution_clock::now();
    duration<double, milli> timePar = parEnd-parStart;
    cout << "Time taken: " << timePar.count() << "ms\n";
}