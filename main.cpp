#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main() 
{
    // initializes a 2D vector and "activeFeatures" vector
    vector<vector<double> > table;
    vector<int> activeFeatures;

    // adds dataset values to the vector
    ifstream input("CS170_Small_Data_69.txt");

    if (!input.is_open()) 
    {
        return -1;
    }
    string line;
    int currRow = 0;
    while (getline(input, line))
    {
        istringstream sstream(line);
        string strValue;
        while (sstream >> strValue)
        {
            table[currRow].push_back(stod(strValue));
        }
        currRow++;
    }

    printTable(table);
    cout << "hello" << endl;
}

void printTable(vector<vector<double> > in)
{
    for(int i = 0; i < in.size(); i++) 
    {
        for(int k = 0; k < in[i].size(); k++)
        {
            cout << in[i][k] << endl;
        }
    }
}


