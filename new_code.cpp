#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <fstream>
#include <array>
#include <map>

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
        //DFS - algorytm Tarjana
        visited[v] = true;

        for (const auto& neighbor : adjacencyList[v]) {
            int nextVertex = neighbor.first;
            cout << "next element: " << nextVertex << endl;
            if (!visited[nextVertex]) {
                topologicalSortUtil(nextVertex, visited, stack);
            }
        }
        
        stack.push(v);
        //cout << stack.top() << endl;
    }

    void topologicalSort() {
        vector<bool> visited(vertices, false);
        stack<int> s;

        for (int i = 0; i < vertices; ++i) { //do usuniecia
            if (!visited[i]) {
                topologicalSortUtil(i, visited, s);
            }
        }

        // Znajdź najdłuższe ścieżki
        vector<int> longestPath(vertices, -1);
        longestPath[s.top()] = 0;

        while (!s.empty()) {
            int u = s.top();
            //cout << u << endl;
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

vector<vector<array<int,2>>> read_tailard(string filename, int* jobs, int* machines){
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Nie można otworzyć pliku!" << endl;
        exit(1);
    }

    string tmp;

    inputFile >> *jobs >> *machines;
    
    getline(inputFile, tmp);
    getline(inputFile, tmp);

    vector<vector<array<int, 2>>> data(*jobs, vector<array<int, 2>>(*machines));

    for (int i = 0; i < *jobs; ++i) {
        for (int j = 0; j < *machines; ++j) {
            inputFile >> data[i][j][1];
        }
    }
    inputFile>>tmp;
    for (int i = 0; i < *jobs; ++i) {
        for (int j = 0; j < *machines; ++j) {
            inputFile >> tmp;
            data[i][j][0] = stoi(tmp)-1;
        }
    }

    inputFile.close();
    return data;
}

void read_orlib(string filename, int* jobs, int* machines, WeightedGraph &Graph, map<int, vector<int>> &machine_map){
    ifstream inputFile(filename);
    int machine_n, duration;
    if (!inputFile.is_open()) {
        cout << "Nie można otworzyć pliku!" << endl;
        exit(1);
    }

    inputFile >> *jobs >> *machines;

    
    for (int i = 0; i < *jobs; ++i) {
        int shiftID = i*(*machines);
        Graph.addEdge(0, shiftID+1, 0);
        cout << "0 ," << shiftID+1 << endl;
        for(int j=shiftID+1;j<*machines+shiftID;j++)
        {
            inputFile >> machine_n >> duration;
            machine_map[machine_n].push_back(j);
            Graph.addEdge(j, j+1, duration);
            cout << j << "," << j+1 << " duration: " << duration << endl;
        }
        inputFile >> machine_n >> duration;
        machine_map[machine_n].push_back(*machines+shiftID);
        Graph.addEdge(*machines + shiftID, (*jobs)*(*machines) + 1, duration);
        cout << *machines + shiftID << ", " << (*jobs)*(*machines) + 1 << " duration: " << duration << endl;
    }

    inputFile.close();

}



void makeBaseGraph(WeightedGraph &Graph, int machines, int jobs)
{
    //int i*machines = 0;
    int tasks = machines*jobs;
    for(int i=0;i<jobs;i++)
    {
        //Graph.addEdge(0, i*machines+1, 0);
        cout << "0 ," << i*machines+1 << endl;
        for(int j=i*machines+1;j<machines+i*machines;j++)
        {
            cout << j << "," << j+1 << endl;
            //Graph.addEdge(j, j+1, 2);   // pobieranie dlugosci ze struktury
        }
        cout << machines + i*machines << ", " << tasks + 1<< endl;
        //Graph.addEdge(machines + i*machines, tasks + 1, 4);
        //i*machines += machines;
    }
    
}

void czytajMape(const map<int, vector<int>>& mapa) {
    // Iteracja przez elementy mapy
    for (const auto &entry : mapa) {
        cout << "Klucz: " << entry.first << ", Wartości: ";
        
        // Iteracja przez elementy wektora
        for (const auto &value : entry.second) {
            cout << value << " ";
        }
        
        cout << endl;
    }
}

int main() {
    int jobs, machines;
    WeightedGraph weightedGraph(11);
    map<int, vector<int>> machine_map;
    read_orlib("fs1.txt", &jobs, &machines, weightedGraph, machine_map);
    czytajMape(machine_map);
    // weightedGraph.addEdge(0, 1, 0);
    // weightedGraph.addEdge(0, 7, 0);
    // weightedGraph.addEdge(0, 4, 0);
    // weightedGraph.addEdge(1, 2, 5);
    // weightedGraph.addEdge(1, 7, 5);
    // weightedGraph.addEdge(7, 8, 3);
    // weightedGraph.addEdge(7, 5, 3);
    // weightedGraph.addEdge(8, 2, 6);
    // weightedGraph.addEdge(8, 9, 6);
    
    // weightedGraph.addEdge(2, 3, 4);
    // weightedGraph.addEdge(2, 6, 4);
    // weightedGraph.addEdge(5, 6, 3);
    // weightedGraph.addEdge(4, 9, 2);
    // weightedGraph.addEdge(4, 5, 2);
    // weightedGraph.addEdge(3, 10, 2);
    
    // weightedGraph.addEdge(9, 10, 1);
    // weightedGraph.addEdge(6, 10, 7);
    // weightedGraph.addEdge(9, 3, 1);
    // M0 : 1,5,7 M1 : 2,6,8 M3 : 3,4,9

    //makeBaseGraph(weightedGraph, 3, 3);  //potestowac przez zmienianie rozmiaru jobow i maszyn
    // Wykonaj sortowanie topologiczne i znajdź najdłuższe ścieżki

    //weightedGraph.topologicalSort();

    return 0;
}
