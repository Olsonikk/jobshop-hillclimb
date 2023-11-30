#include <iostream>
#include <cstring>
using namespace std;

int jobs, machines,tasks;

void printMatrix(int** matrix){
    for (int i = 0; i < tasks+2; i++)
    {
        for (int j = 0; j < tasks+2; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    
}

void addElement(int i, int j, int **matrix)
{
    if(matrix[i][tasks+1] == 0)
    {
        matrix[i][tasks+1] = j;
    }
    else
    {

    }
}

void removeElement()
{

}

int main()
{
    jobs = 5;
    machines = 5;
    tasks = jobs*machines;
    int** arr = new int*[tasks+2];
    for (int i = 0; i < tasks+2; i++)
    {
        arr[i] = new int[tasks+2];
    }

    for (int i = 0; i < tasks+2; i++) {
        for (int j = 0; j < tasks+2; j++) {  //to musi być
            arr[i][j] = 0;
        }
    }
 
    printMatrix(arr);
    cout << "Jebac walkusia " << arr[1][2] << endl;

    return 0;
}

//QUESTION: what is k value in addElement that is shown on pdf