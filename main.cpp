#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <fstream>
#include <array>
#include <map>
#include <string>
#include <time.h>
#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

int jobs, machines;

class WeightedGraph {
public:
    int vertices;
    vector<vector<array<int,3>>> adjacencyList;  // pair<destination, weight>
    vector<int> longestPath;

    WeightedGraph(int V) {
        vertices = V;
        adjacencyList.resize(V);
        for(int i=0;i<V;i++){
            longestPath.push_back(-1);
        }
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

    int topologicalSort() {
        vector<bool> visited(vertices, false);
        stack<int> s;

        for (int i = 0; i < vertices; ++i) { //do usuniecia
            if (!visited[i]) {
                topologicalSortUtil(i, visited, s);
            }
        }

        // Znajdź najdłuższe ścieżki
        //vector<int> longestPath(vertices, -1);
        // map<int,int> longestPath;
        // for(int i=jobs*machines+1;i>=0;i--){
        //     longestPath[i] = -1;
        // }

        // for (int i = 0; i < jobs*machines+2; i++) {
        //     cout<<longestPath[i]<<" ";
        //     if(i%machines==0) cout<<endl;
        // }
        longestPath[s.top()] = 0;

        while (!s.empty()) {
            int u = s.top();
            //cout << u << endl;
            s.pop();

            if (longestPath[u] != -1) {
                for (const auto& neighbor : adjacencyList[u]) {
                    int v = neighbor[0];
                    int weight = neighbor[1];
                    //cout<<u<<"->"<<v<<" : "<<longestPath[v]<<" "<<weight<<" - "<<longestPath[u]<<endl;
                    if (longestPath[v] < longestPath[u] + weight) {
                        longestPath[v] = longestPath[u] + weight;
                        //cout<<u<<"--->"<<v<<" : "<<longestPath[v]<<" "<<weight<<" - "<<longestPath[u]<<endl;
                    }
                }
            }
        }

        return longestPath[jobs*machines+1];
    
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
    //cout<<"Random tasks: "<<randomTask1<<" "<<randomTask2<<endl;

    for(int i=0;i<machine_map[randomMachine].size();i++){
        if(machine_map[randomMachine][i] == randomTask1){
            machine_map[randomMachine][i] = randomTask2;
        }else if(machine_map[randomMachine][i] == randomTask2){
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

void read_tailard(ifstream& inputFile, WeightedGraph &Graph, map<int, vector<int>> &machine_map, vector<int> &durations){
    int temp_time, temp_machine;

    string tmp;
    
    getline(inputFile, tmp);
    getline(inputFile, tmp);

    durations.push_back(0);

    for (int i = 0; i < jobs; ++i) {
        int shiftID = i*(machines);
        Graph.addEdge(0, shiftID+1, 0,0);
        for (int j=shiftID+1;j<machines+shiftID;j++) {
            inputFile >> temp_time;
            durations.push_back(temp_time);
            Graph.addEdge(j, j+1, temp_time,0);
        }
        inputFile >> temp_time;
        Graph.addEdge(machines + shiftID, (jobs)*(machines) + 1, temp_time,0);
        durations.push_back(temp_time);
    }

    durations.push_back(0);

    inputFile>>tmp;
    
    for (int i = 0; i < jobs; ++i) {
        int shiftID = i*(machines);
        for (int j=shiftID+1;j<machines+shiftID;j++) {
            inputFile >> temp_machine;
            machine_map[temp_machine-1].push_back(j);
        }
        inputFile >> temp_machine;
        machine_map[temp_machine-1].push_back(machines+shiftID);
    }

    

    inputFile.close();
}

void read_orlib(ifstream& inputFile, WeightedGraph &Graph, map<int, vector<int>> &machine_map, vector<int> &durations){
    int machine_n, duration;

    durations.push_back(0);
    for (int i = 0; i < jobs; ++i) {
        int shiftID = i*(machines);
        Graph.addEdge(0, shiftID+1, 0,0);
        //cout << "0 ," << shiftID+1 << endl;
        for(int j=shiftID+1;j<machines+shiftID;j++)
        {
            inputFile >> machine_n >> duration;
            //cout<<machine_n <<" "<< duration<<" - "<<j<<"->"<<j + 1<<endl;
            machine_map[machine_n].push_back(j);
            Graph.addEdge(j, j+1, duration,0);
            durations.push_back(duration);
            // cout << j << "," << j+1 << " duration: " << duration << endl;
        }
        inputFile >> machine_n >> duration;
        //cout<<"x:"<<machine_n <<" "<< duration<<" - "<<machines + shiftID<<"->"<<(jobs)*(machines) + 1<<endl;
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

void toFile(WeightedGraph &Graph){
    ofstream outFile("output.txt");
    outFile<<Graph.longestPath[jobs*machines+1]<<endl;

    for(int i=1;i<=jobs*machines;i++){
        outFile<<Graph.longestPath[i]<<" ";
        if(i%machines==0) outFile<<endl;
    }
    outFile.close();
}
    

int main() {
    srand(time(NULL));
    int nr_jobs = -1;
    int bad_neighbors = 0, iterations = 1000;

    ifstream inputFile("swv17.txt");

    if (!inputFile.is_open()) {
        cout << "Nie mozna otworzyc pliku!" << endl;
        exit(1);
    }

    inputFile >> jobs >> machines;

    if(nr_jobs != -1) jobs = nr_jobs;
    
    WeightedGraph baseGraph(jobs*machines+2);
    WeightedGraph mainGraph(jobs*machines+2);
    WeightedGraph neighborGraph(jobs*machines+2);
    
    map<int, vector<int>> machine_map;
    map<int, vector<int>> neighbor_machine_map;

    vector<int> durations; //source and sink
    int mainSolution,tmpSolution;

    read_orlib(inputFile,baseGraph, machine_map, durations);
    //read_tailard(inputFile,baseGraph, machine_map, durations);

    if(jobs==1){
        baseGraph.topologicalSort();
        toFile(baseGraph);
        return 0;
    }

    mainGraph = baseGraph;
    //neighborGraph= baseGraph;
    ///PrintList(mainGraph);
    sort_machine_map(machine_map);
    //czytajMape(machine_map);
    //cout << endl;
    //PrintList(mainGraph);

    //neighbor_machine_map = machine_map;


    initDisjunctiveEgdes(mainGraph,machine_map,durations);

    // czytajMape(machine_map);
    //cout<<endl;


    mainSolution = mainGraph.topologicalSort();
    cout<<mainSolution;
    
    auto start_time = high_resolution_clock::now();
    
    for(int i=0;i<iterations;i++){
        neighborGraph=baseGraph;
        neighbor_machine_map = machine_map;
        //czytajMape(machine_map);
        SwapVertexes(neighbor_machine_map);
        //czytajMape(neighbor_machine_map);
        initDisjunctiveEgdes(neighborGraph,neighbor_machine_map,durations);
        if(!neighborGraph.isCyclic()){
            tmpSolution = neighborGraph.topologicalSort();
            if(mainSolution>tmpSolution){
                mainSolution = tmpSolution;
                mainGraph = neighborGraph;
                machine_map = neighbor_machine_map;
                bad_neighbors = 0;
            }else{
                bad_neighbors++;
            }
        }
        auto current_time = high_resolution_clock::now();
        auto elapsed_time = duration_cast<seconds>(current_time - start_time).count();
        //cout<<elapsed_time<<endl;
        //cout<<bad_neighbors<<" "<<iterations<<endl;
        if(bad_neighbors>=iterations*0.1){
            cout << "Zli sasiedzi"<< endl;
            break;
        }
        if(elapsed_time >= 900) {
            cout << "Przekroczono limit czasu: "<<elapsed_time<< endl;
            break;
        }
    }
    toFile(mainGraph);
    // cout<<mainSolution<<endl;
    // for (int i = 1; i <= jobs*machines; i++) {
    //     cout<<mainGraph.longestPath[i]<<" ";
    //     if(i%machines==0) cout<<endl;
    // }
   
    return 0;
}


//podmiana wierzcholkow - swap w mapie, przerobienie krawędzi dysjunktywnych. Pozostałe stałe ESSA