#include <iostream>
#include <vector>
#include <array>
#include <algorithm> 
#include <fstream>
#include <string>
#include <climits>
#include <stack>
#include <string>

#include "greedy.h"


using namespace std;




bool compareTasks(const array<int,3>& task1, const array<int,3>& task2) {
    // Porównujemy taska na podstawie pierwszego elementu
    return task1[0] > task2[0];
}

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

vector<vector<array<int,2>>> read_orlib(string filename, int* jobs, int* machines){
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Nie można otworzyć pliku!" << endl;
        exit(1);
    }

    inputFile >> *jobs >> *machines;

    vector<vector<array<int, 2>>> data(*jobs, vector<array<int, 2>>(*machines));

    for (int i = 0; i < *jobs; ++i) {
        for (int j = 0; j < *machines; ++j) {
            inputFile >> data[i][j][0] >> data[i][j][1];
        }
    }

    inputFile.close();
    return data;
}

bool TimeLeft(int elem, int currTime, int * job_total){
    if(job_total[elem] == currTime) return true;
    return false;
}

int currTimeJump(int currTime, int* job_total, int machines, bool success){ //jesli success=true, zwroc najmniejsza, ww. drugi najmniejszy
    int smallest;
    int second_smallest;

    smallest = INT_MAX;
    second_smallest = INT_MAX;

    for (int i = 0; i < machines; i++) {
        if (job_total[i] < smallest) {
            second_smallest = smallest;
            smallest = job_total[i];
        } else if (job_total[i] < second_smallest && job_total[i] != smallest) {
            second_smallest = job_total[i];
        }
    }
    if(!success){
        for(int i=0;i<machines;i++){
            if(job_total[i]<second_smallest) job_total[i]=second_smallest; 
        }
        return second_smallest;
    }else{
        return smallest;
    }
    
}

void PutInVec(int index,int machine_n,int duration, int currTime, int* job_total, int* job_free, int* job_index,vector<vector<array<int,2>>> &arr){
    job_free[index]=duration;
    job_total[machine_n] += duration;
    job_index[index]++;
}


void printArray(int* array, int jobs){
    for(int i = 0; i<jobs;i++){
        cout << array[i] << " ";
    }
    cout << endl;
}

void printVector(vector<vector<array<int,2>>> mainVector){
    for (const auto& innerVector : mainVector) {
        for (const auto& task : innerVector) {
            int firstElement, secondElement;
            cout << "(" << task[0] << ", " << task[1] << ") ";
        }
        cout << endl;
    }
    cout << endl;
}

void PrintStack(stack<int> &stk, ofstream &outputFile)
{   
    if (stk.empty())
        return;
    int top = stk.top();
    stk.pop();
    PrintStack(stk, outputFile);
    outputFile << top << " ";
}

void updateJF(int* jobs_free, int GoneCurrTime, int CurrTime,int jobs){
    for(int i=0; i< jobs; i++){
        if(jobs_free[i] + GoneCurrTime - CurrTime > 0){
            jobs_free[i] = jobs_free[i] + GoneCurrTime - CurrTime;
        }
        else jobs_free[i] = 0;
    }
}


bool isReady(int index, int machine_n, int currTime, int* job_total, int* job_free){
    if(job_free[index] == 0 && TimeLeft(machine_n, currTime, job_total)) return true;
    return false;
}

int greedy(string File, const char* type) {
    int jobs, machines;
    int duration,machine_n;
    string inputFileName = File;
    vector<vector<array<int,2>>> mainVector;
    if(atoi(type) == 1){
        mainVector = read_tailard(File, &jobs, &machines);
    }
    else if(atoi(type) == 0){
        mainVector = read_orlib(File, &jobs, &machines);
    }
    else{
        cout << "Bledny pierwszy argument podczas wywolywania!";
        exit(1);
    }

    int control = 0;
    
    int currTime = 0;
    int goneCurrTime = 0;
    int * job_index = new int[jobs];
    fill_n(job_index, jobs, 0);
    int * job_free = new int[jobs];
    fill_n(job_free, jobs, 0);
    int * job_total = new int[machines];
    fill_n(job_total, machines, 0);

    vector<array<int,3>> chosen;
    vector<stack<int>> output;

    for(int i=0;i<jobs;i++){
        stack<int> job;
        output.push_back(job);
    }
    
    bool success;

    while(control<jobs*machines){
        success = false;

        for(int i=0;i<jobs;i++){
            if(job_index[i] < machines){
                duration = mainVector[i][job_index[i]][1];
                machine_n = mainVector[i][job_index[i]][0];

                if(isReady(i, machine_n, currTime, job_total, job_free)){
                    chosen.push_back({duration,machine_n,i});
                }
            }
            
        }

        sort(chosen.begin(), chosen.end(), compareTasks);

        for (int i=0;i<chosen.size();i++){
            if(isReady(chosen[i][2], chosen[i][1], currTime, job_total, job_free)){
                success = true;
                PutInVec(chosen[i][2],chosen[i][1],chosen[i][0],currTime,job_total,job_free,job_index,mainVector);
                output[chosen[i][2]].push(currTime);
                control++;
            }
        }
        chosen.clear();
        goneCurrTime = currTime;
        currTime = currTimeJump(currTime, job_total, machines, success);
        updateJF(job_free, goneCurrTime, currTime, jobs);
    }
    // printArray(job_total, machines);
    // cout<<endl;

    ofstream outFile("output.txt");

    outFile<<*max_element(job_total,job_total+machines)<<endl;

    for(int i=0;i<output.size();i++){
        PrintStack(output[i], outFile);
        outFile<<endl;
    }
    outFile.close();

    delete job_total;
    delete job_free;
    delete job_index;

    return 0;
}


