#include <iostream>
#include <vector>
#include <utility>
#include <stack>

using namespace std;

class WeightedGraph {
public:
    int vertices;
    vector<vector<pair<int, int>>> adjacencyList;  // pair<destination, weight>

    WeightedGraph(int V) {
        vertices = V;
        adjacencyList.resize(V);
    }

    void addEdge(int src, int dest, int weight) {
        adjacencyList[src].push_back(make_pair(dest, weight));
    }

    void topologicalSortUtil(int v, vector<bool>& visited, stack<int>& stack) {
        visited[v] = true;

        for (const auto& neighbor : adjacencyList[v]) {
            int nextVertex = neighbor.first;
            if (!visited[nextVertex]) {
                topologicalSortUtil(nextVertex, visited, stack);
            }
        }

        stack.push(v);
    }

    void topologicalSort() {
        vector<bool> visited(vertices, false);
        stack<int> s;

        for (int i = 0; i < vertices; ++i) {
            if (!visited[i]) {
                topologicalSortUtil(i, visited, s);
            }
        }

        // Znajdź najdłuższe ścieżki
        vector<int> longestPath(vertices, -1);
        longestPath[s.top()] = 0;

        while (!s.empty()) {
            int u = s.top();
            cout << u << endl;
            s.pop();

            if (longestPath[u] != -1) {
                for (const auto& neighbor : adjacencyList[u]) {
                    int v = neighbor.first;
                    int weight = neighbor.second;

                    if (longestPath[v] < longestPath[u] + weight) {
                        longestPath[v] = longestPath[u] + weight;
                    }
                }
            }
        }

        // Wydrukuj najdłuższe ścieżki
        cout << "Longest paths from source vertex:\n";
        for (int i = 0; i < vertices; ++i) {
            cout << "Vertex " << i << ": " << longestPath[i] << endl;
        }
    }
};

void makeBaseGraph(WeightedGraph &Graph, int machines, int jobs)
{
    int shift = 0;
    int tasks = machines*jobs;
    for(int i=0;i<jobs;i++)
    {
        Graph.addEdge(0, shift+1, 0);
        //cout << "0 ," << shift+1 << endl;
        for(int j=shift+1;j<machines+shift;j++)
        {
            //cout << j << "," << j+1 << endl;
            Graph.addEdge(j, j+1, 2);   // pobieranie dlugosci ze struktury
        }
        //cout << machines + shift << ", " << tasks + 1<< endl;
        Graph.addEdge(machines + shift, tasks + 1, 4);
        shift += machines;
    }
    
}

int main() {
    WeightedGraph weightedGraph(11);

    // weightedGraph.addEdge(0, 1, 0);
    // weightedGraph.addEdge(0, 7, 0);
    // weightedGraph.addEdge(0, 4, 0);
    // weightedGraph.addEdge(1, 2, 5);
    // weightedGraph.addEdge(1, 7, 5);
    // weightedGraph.addEdge(7, 8, 3);
    // weightedGraph.addEdge(7, 5, 3);
    // weightedGraph.addEdge(8, 2, 6);
    // weightedGraph.addEdge(8, 9, 6);
    // weightedGraph.addEdge(2, 6, 4);
    // weightedGraph.addEdge(2, 3, 4);
    // weightedGraph.addEdge(5, 6, 3);
    // weightedGraph.addEdge(4, 9, 2);
    // weightedGraph.addEdge(4, 5, 2);
    // weightedGraph.addEdge(3, 10, 2);
    // weightedGraph.addEdge(6, 10, 7);
    // weightedGraph.addEdge(9, 10, 1);
    // weightedGraph.addEdge(9, 3, 1);
    // M0 : 1,5,7 M1 : 2,6,8 M3 : 3,4,9

    makeBaseGraph(weightedGraph, 3, 3);  //potestowac przez zmienianie rozmiaru jobow i maszyn
    // Wykonaj sortowanie topologiczne i znajdź najdłuższe ścieżki
    weightedGraph.topologicalSort();

    return 0;
}
