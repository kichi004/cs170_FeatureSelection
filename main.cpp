#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void printTable(vector<vector<double> > in);
vector<vector<double> > addDatatoTable();

int main() 
{
    // initializes a 2D vector and "activeFeatures" vector
    vector<vector<double> > table = addDatatoTable();
    vector<int> activeFeatures;

    // printing table for data input testing
    printTable(table);
}

vector<vector<double> > addDatatoTable()
{
    vector<vector<double> > temp;
    ifstream input("CS170_Small_Data__69.txt");

    if (!input.is_open()) 
    {
        cout << "Failed to Open" << endl;
        return temp;
    }
    string line;
    int currRow = 0;
    while (getline(input, line))
    {
        temp.resize(currRow+1);
        istringstream sstream(line);
        string strValue;
        while (sstream >> strValue)
        {
            temp[currRow].push_back(stod(strValue));
        }
        currRow++;
    }
    return temp;
}

void printTable(vector<vector<double> > in)
{
    for(int i = 0; i < in.size(); i++) 
    {
        for(int k = 0; k < in[i].size(); k++)
        {
            cout << in[i][k];
            cout << " ";
        }
        cout << endl;
    }
}


