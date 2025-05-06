#include<iostream>

#include<vector>
#include<queue>

#include<chrono>
#include<omp.h>

#include<fstream>

using namespace std;
using namespace chrono;

class Graph{
    int V;
    vector<vector<int>> adj;
    vector<bool> vis;

    public:

    Graph(int v) {
        V = v;
        adj.resize(V);
        vis.resize(V);
        clearVis();
    }

    void clearVis() {
        for(int i=0; i<V; i++)
            vis[i] = false;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void sequentialBFS(int st = 0) {
        cout << "   - - - SEQUENTIAL BFS - - -\n";
        cout << "BFS : ";

        auto start = high_resolution_clock::now();

        queue<int> q;
        q.push(st);
        vis[st] = true;

        while(!q.empty()) {
            int curr = q.front();
            q.pop();

            cout << curr << " ";

            for(int i=0; i<adj[curr].size(); i++) {
                int neigh = adj[curr][i];
                if(!vis[neigh]) {
                    vis[neigh] = true;
                    q.push(neigh);
                }
            }
        }
        cout << endl;

        auto end = high_resolution_clock::now();

        duration<double, micro> duration = end - start;
        cout << "Time taken by sequential BFS is " << duration.count() << " microseconds\n";

        clearVis();
    }

    void parallelBFS(int st = 0) {
        cout << "   - - - PARALLEL BFS - - -\n";
        cout << "BFS : ";

        auto start = high_resolution_clock::now();

        queue<int> q;
        q.push(st);
        vis[st] = true;

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
                            vis[neigh] = true;
                            q.push(neigh);
                        }
                    }
                }
            }

        }
        cout << endl;

        auto end = high_resolution_clock::now();

        duration<double, micro> duration = end - start;
        cout << "Time taken by sequential BFS is " << duration.count() << " microseconds\n";
    }
};

int main() {

    ifstream inputFile("graph.txt");

    if(!inputFile.is_open()){
        cout << "Failed to open file.\n";
        return 1;
    }

    int V;

    inputFile >> V;

    Graph * graph = new Graph(V);

    int u, v;
    while(inputFile >> u >> v)
        graph->addEdge(u, v);

    inputFile.close();

    graph->sequentialBFS();
    graph->parallelBFS();

    return 0;
}