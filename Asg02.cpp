#include<iostream>

#include<stack>
#include<vector>

#include<omp.h>
#include<chrono>

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

    void sequentialBFS(int st=0) {
        cout << "   - - - SEQUENTIAL BFS - - -\n";
        cout << "DFS : ";
        auto start = high_resolution_clock::now();
        
        stack<int>s;
        s.push(st);
        vis[st] = true;
        
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
        auto end = high_resolution_clock::now();
        duration<double, nano> duration = end - start;
        cout << "Time taken by sequential DFS is " << duration.count() << " nanoseconds\n";
        clearVis();
    }

    void parallelBFS(int st=0) {
        cout << "   - - - PARALLEL BFS - - -\n";
        cout << "DFS : ";
        auto start = high_resolution_clock::now();
        
        stack<int>s;
        s.push(st);
        vis[st] = true;
        
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
        auto end = high_resolution_clock::now();
        duration<double, nano> duration = end - start;
        cout << "Time taken by parallel DFS is " << duration.count() << " nanoseconds\n";
        clearVis();
    }
};

int main() {

    ifstream inputFile("graph.txt");

    int V;
    int u, v;

    inputFile >> V;
    Graph*graph = new Graph(V);

    while(inputFile >> u >> v)
        graph->addEdge(u, v);

    cout << "Graph fetched successfully!\n\n";

    graph->sequentialBFS();
    graph->parallelBFS();

    delete graph;

    return 0;
}