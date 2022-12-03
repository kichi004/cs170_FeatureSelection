#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

void printTable(vector<vector<double> > in);
vector<vector<double> > addDatatoTable();
bool LeftOutNearestNeighbor(int row, vector<vector<double> >* data, vector<int> features);
double getEuclideanDistance(int row1, int row2, vector<vector<double> >* data, vector<int> features);

int main() 
{
    // initializes a 2D vector and "activeFeatures" vector
    vector<vector<double> > temp = addDatatoTable();
    vector<vector<double> >* table = &temp;
    vector<int> activeFeatures;

    // printing table for data input testing
    // printTable(*table);

    // adding activeFeatures for testing
    activeFeatures.push_back(1);
    activeFeatures.push_back(3);
    activeFeatures.push_back(6);

    LeftOutNearestNeighbor(1, table, activeFeatures);
    LeftOutNearestNeighbor(10, table, activeFeatures);
    LeftOutNearestNeighbor(100, table, activeFeatures);

}

vector<vector<double> > addDatatoTable()
{
    vector<vector<double> > temp;
    ifstream input("CS170_Small_Data__96.txt");

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

bool LeftOutNearestNeighbor(int excludedRow, vector<vector<double> >* data, vector<int> features)
{
    double minDistance = __DBL_MAX__;
    int minDistRow = 0;
    for (int i = 0; i < data->size(); i++)
    {
        if (i != excludedRow)
        {
            double currDistance = getEuclideanDistance(excludedRow, i, data, features);
            cout << "Distance for " + to_string(i) + " is " + to_string(currDistance) << endl;
            if (currDistance < minDistance)
            {
                minDistance = currDistance;
                minDistRow = i;
            }
        }
    }
    if (data->at(minDistRow)[0] == data->at(excludedRow)[0]) 
    {
        cout << "The instance at the " + to_string(excludedRow) + " row was correctly determined to be class " + to_string(data->at(minDistRow)[0]) << endl;
        return true;
    }
    cout << "Closest row was " + to_string(minDistRow) << endl;
    return false;
}

double getEuclideanDistance(int rowIndex1, int rowIndex2, vector<vector<double> >* data, vector<int> features)
{
    double squaredDifference = 0;
    for (int i = 0; i < features.size(); i++)
    {
        double rowValue1 = data->at(rowIndex1)[features.at(i)];
        double rowValue2 = data->at(rowIndex2)[features.at(i)];
        squaredDifference += pow(rowValue1-rowValue2, 2);
    }
    return sqrt(squaredDifference);
}
