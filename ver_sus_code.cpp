// #include <iostream>
// #include <vector>
// #include <utility>
// #include <stack>
// #include <fstream>
// #include <array>
// #include <map>
// #include <string>
// #include <time.h>
#include <bits/stdc++.h>

using namespace std;

int jobs, machines;

class WeightedGraph {
public:
    int vertices;
    vector<vector<array<int,3>>> adjacencyList;  // pair<destination, weight>

    WeightedGraph(int V) {
        vertices = V;
        adjacencyList.resize(V);
    }

    void addEdge(int src, int dest, int weight,int type) {
        adjacencyList[src].push_back({dest, weight,type});
    }

    void topologicalSortUtil(int v, vector<bool>& visited, stack<int>& stack) {
        //DFS - algorytm Tarjana
        visited[v] = true;

        for (const auto& neighbor : adjacencyList[v]) {
            int nextVertex = neighbor[0];
            // cout << "next element: " << nextVertex << endl;
            if (!visited[nextVertex]) {
                //cout << "next element: " << nextVertex << endl;
                topologicalSortUtil(nextVertex, visited, stack);
            }
        }
        
        stack.push(v);
        //cout << stack.top() << endl;
    }

    void topologicalSort(int makespan) {
        vector<bool> visited(vertices, false);
        stack<int> s;

        for (int i = 0; i < vertices; ++i) { //do usuniecia
            if (!visited[i]) {
                topologicalSortUtil(i, visited, s);
            }
        }

        // Znajdź najdłuższe ścieżki
        map<int,int> longestPath;
        for(int i=jobs*machines+1;i>=0;i--){
            longestPath[i] = -1;
        }

        longestPath[s.top()] = 0;

        while (!s.empty()) {
            int u = s.top();
            cout << u << endl;
            s.pop();

            if (longestPath[u] != -1) {
                for (const auto& neighbor : adjacencyList[u]) {
                    int v = neighbor[0];
                    int weight = neighbor[1];
                    cout<<u<<"->"<<v<<" : "<<longestPath[v]<<" "<<weight<<" - "<<longestPath[u]<<endl;
                    if (longestPath[v] < longestPath[u] + weight) {
                        longestPath[v] = longestPath[u] + weight;
                        cout<<u<<"--->"<<v<<" : "<<longestPath[v]<<" "<<weight<<" - "<<longestPath[u]<<endl;
                    }
                }
            }
        }

        //Wydrukuj najdłuższe ścieżki
        cout << "Longest paths from source vertex:\n";
        makespan = longestPath[jobs*machines+1];
        cout<<makespan<<endl;
        for (int i = 1; i <= jobs*machines; i++) {
            cout<<longestPath[i]<<" ";
            if(i%machines==0) cout<<endl;
        }
        //cout<<endl;
        // cout << "Longest paths from source vertex:\n";
        // for (int i = 0; i < vertices; ++i) {
        //     cout << "Vertex " << i << ": " << longestPath[i] << endl;
        // }
    
    }
        bool isCyclicUtil(int v, vector<bool>& visited, unordered_set<int>& recursionStack) {
            visited[v] = true;
            recursionStack.insert(v);
            for (const auto& adj_neighbor : adjacencyList[v]) {
                int neighbor = adj_neighbor[0];
                if (!visited[neighbor]) {
                    if (isCyclicUtil(neighbor, visited, recursionStack)) {
                        return true;
                    }
                } else if (recursionStack.find(neighbor) != recursionStack.end()) {
                    // The neighbor is already in the recursion stack, indicating a cycle
                    return true;
                }
            }

            recursionStack.erase(v);
            return false;
        }

    bool isCyclic() {
        vector<bool> visited(vertices, false);
        unordered_set<int> recursionStack;
        for (int i = 0; i < vertices; ++i) {
            if (!visited[i]) {
                if (isCyclicUtil(i, visited, recursionStack)) {
                    return true;
                }
            }
        }
        return false;
    }

};

void SwapVertexes(map<int, vector<int>> &machine_map){
    vector<int> tmpDisjunVertex;
    int randomMachine = rand() % (machines);
    tmpDisjunVertex = machine_map[randomMachine];

    int index1 = rand() % tmpDisjunVertex.size();
    int randomTask1 = tmpDisjunVertex[index1];
    tmpDisjunVertex.erase(tmpDisjunVertex.begin() + index1);

    int index2 = rand() % tmpDisjunVertex.size();
    int randomTask2 = tmpDisjunVertex[index2];
    tmpDisjunVertex.erase(tmpDisjunVertex.begin() + index2);
    
    tmpDisjunVertex.clear();
    cout<<"Random tasks: "<<randomTask1<<" "<<randomTask2<<endl;

    for(int i=0;i<machine_map[randomMachine].size();i++){
        if(machine_map[randomMachine][i] == randomTask1){
            machine_map[randomMachine][i] = randomTask2;
        }
        else if(machine_map[randomMachine][i] == randomTask2){
            machine_map[randomMachine][i] = randomTask1;
        }
    }
}

void PrintList(WeightedGraph &Graph){
    for(int vertex=0;vertex<machines*jobs+2;vertex++){
        cout<<vertex<<"."<<endl;
        for(int edge=0;edge<Graph.adjacencyList[vertex].size();edge++){
            cout<<Graph.adjacencyList[vertex][edge][0]<<" "<<Graph.adjacencyList[vertex][edge][1]<<" "<<Graph.adjacencyList[vertex][edge][2]<<endl;
        }
    }

}

void initDisjunctiveEgdes(WeightedGraph &Graph, map<int, vector<int>> &machine_map, vector<int> durations){
    for(int i=0;i<machines;i++){
        for(int j=0;j<jobs-1;j++){
            Graph.addEdge(machine_map[i][j],machine_map[i][j+1],durations[machine_map[i][j]],1);

        }
    }
}

// vector<vector<array<int,2>>> read_tailard(string filename, int* jobs, int* machines){
//     ifstream inputFile(filename);

//     if (!inputFile.is_open()) {
//         cout << "Nie mozna otworzyc pliku!" << endl;
//         exit(1);
//     }

//     string tmp;

//     inputFile >> jobs >> machines;
    
//     getline(inputFile, tmp);
//     getline(inputFile, tmp);

//     vector<vector<array<int, 2>>> data(jobs, vector<array<int, 2>>(machines));

//     for (int i = 0; i < jobs; ++i) {
//         for (int j = 0; j < machines; ++j) {
//             inputFile >> data[i][j][1];
//         }
//     }
//     inputFile>>tmp;
//     for (int i = 0; i < jobs; ++i) {
//         for (int j = 0; j < machines; ++j) {
//             inputFile >> tmp;
//             data[i][j][0] = stoi(tmp)-1;
//         }
//     }

//     inputFile.close();
//     return data;
// }
void read_orlib(ifstream& inputFile, int jobs, int machines, WeightedGraph &Graph, map<int, vector<int>> &machine_map, vector<int> &durations){
    int machine_n, duration;
    if (!inputFile.is_open()) {
        cout << "Nie mozna otworzyc pliku!" << endl;
        exit(1);
    }

    durations.push_back(0);
    for (int i = 0; i < jobs; ++i) {
        int shiftID = i*(machines);
        Graph.addEdge(0, shiftID+1, 0,0);
        cout << "0 ," << shiftID+1 << endl;
        for(int j=shiftID+1;j<machines+shiftID;j++)
        {
            inputFile >> machine_n >> duration;
            cout<<machine_n <<" "<< duration<<" - "<<j<<"->"<<j + 1<<endl;
            //cout << "chuj " << duration << endl;
            machine_map[machine_n].push_back(j);
            Graph.addEdge(j, j+1, duration,0);
            durations.push_back(duration);
            // cout << j << "," << j+1 << " duration: " << duration << endl;
        }
        inputFile >> machine_n >> duration;
        cout<<"x:"<<machine_n <<" "<< duration<<" - "<<machines + shiftID<<"->"<<(jobs)*(machines) + 1<<endl;
        // cout << "kurwa " << duration << endl;
        machine_map[machine_n].push_back(machines+shiftID);
        Graph.addEdge(machines + shiftID, (jobs)*(machines) + 1, duration,0);
        durations.push_back(duration);
        // cout << machines + shiftID << ", " << (jobs)*(machines) + 1 << " duration: " << duration << endl;
    }
    
    durations.push_back(0);
    inputFile.close();

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

bool compareTasks(int task1, int task2) {
    return (task1-1) % machines < (task2-1) % machines;
}

void sort_machine_map(map<int, vector<int>> &mapa){
    for(int i=0;i<machines;i++)
    {
        sort(mapa[i].begin(), mapa[i].end(), compareTasks);
    }
    
}

int main() {
    srand(time(NULL));
    int makespan;
    ifstream inputFile("fs1.txt");
    //wszytsko chuj
    inputFile >> jobs >> machines;
    
    WeightedGraph baseGraph(jobs*machines+2);
    WeightedGraph mainGraph(jobs*machines+2);
    WeightedGraph neighborGraph(jobs*machines+2);
    
    map<int, vector<int>> machine_map;
    map<int, vector<int>> neighbor_machine_map;

    vector<int> durations; //source and sink



    read_orlib(inputFile, jobs, machines, baseGraph, machine_map, durations);
    mainGraph = baseGraph;
    neighborGraph= baseGraph;

    sort_machine_map(machine_map);
    czytajMape(machine_map);

    neighbor_machine_map = machine_map;

    initDisjunctiveEgdes(mainGraph,machine_map,durations);

    cout<<endl;
    SwapVertexes(neighbor_machine_map);
    initDisjunctiveEgdes(neighborGraph,neighbor_machine_map,durations);

    mainGraph.topologicalSort(makespan); //nie wiem czy nie *makespan
    cout<<endl;
    
    neighborGraph.topologicalSort(makespan); //nie wiem czy nie *makespan
    
    if(neighborGraph.isCyclic()) {
        cout << "Cykl w grafie neighbor!";
    }
    if(mainGraph.isCyclic()) {
        cout << "Cykl w grafie main!";
    }

///////////////////////////////////////////
    //mainGraph = neighborGraph;

    //neighborGraph = baseGraph;

    

    //initDisjunctiveEgdes(mainGraph,neighbor_machine_map,durations);

    // neighbor_machine_map = machine_map;

    // if(neighborGraph.isCyclic()) {
    //     cout << "Cykl w grafie neighbor!";
    // }
    // if(mainGraph.isCyclic()) {
    //     cout << "Cykl w grafie main!";
    // }

    return 0;
}
