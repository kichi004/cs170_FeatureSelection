#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

void printTable(vector<vector<double> > in);
void printFeatures(vector<int>* features);
vector<vector<double> > addDatatoTable();
bool LeftOutNearestNeighbor(int row, vector<vector<double> >* data, vector<int>* features);
double getEuclideanDistance(int row1, int row2, vector<vector<double> >* data, vector<int>* features);
double getLOOAccuracy(vector<vector<double> >* data, vector<int>* features);
void ForwardSelection(vector<vector<double> >* data, vector<int>* features);

int main() 
{
    // initializes a 2D vector and "activeFeatures" vector
    vector<vector<double> > temp = addDatatoTable();
    vector<vector<double> >* table = &temp;
    vector<int> tempFeatures;
    vector<int>* activeFeatures = &tempFeatures;

    // printing table for data input testing
    // printTable(*table);

    // adding activeFeatures for testing
    activeFeatures->push_back(1);
    activeFeatures->push_back(6);
    //activeFeatures->push_back(3);
    printFeatures(activeFeatures);

    // LeftOutNearestNeighbor(1, table, activeFeatures);
    // LeftOutNearestNeighbor(10, table, activeFeatures);
    // LeftOutNearestNeighbor(100, table, activeFeatures);

    cout << to_string(getLOOAccuracy(table, activeFeatures)) << endl;
    //ForwardSelection(table, activeFeatures);
}

void printFeatures(vector<int>* features)
{
    cout << "Current features are ";
    for(int i = 0; i < features->size(); i++)
    {
        cout << features->at(i) << " ";
    }
    cout << endl;
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

bool LeftOutNearestNeighbor(int excludedRow, vector<vector<double> >* data, vector<int>* features)
{
    double minDistance = __DBL_MAX__;
    int minDistRow = 0;
    for (int i = 0; i < data->size(); i++)
    {
        if (i != excludedRow)
        {
            double currDistance = getEuclideanDistance(excludedRow, i, data, features);
            if (currDistance < minDistance)
            {
                minDistance = currDistance;
                minDistRow = i;
            }
        }
    }
    if (data->at(minDistRow)[0] == data->at(excludedRow)[0]) 
    {
        return true;
    }
    return false;
}

double getEuclideanDistance(int rowIndex1, int rowIndex2, vector<vector<double> >* data, vector<int>* features)
{
    double squaredDifference = 0;
    for (int i = 0; i < features->size(); i++)
    {
        double rowValue1 = data->at(rowIndex1)[features->at(i)];
        double rowValue2 = data->at(rowIndex2)[features->at(i)];
        squaredDifference += pow(rowValue1-rowValue2, 2);
    }
    return sqrt(squaredDifference);
}

double getLOOAccuracy(vector<vector<double> >* data, vector<int>* features)
{
    int totalNumber = data->size();
    int correctClassification = 0;
    for (int i = 0; i < totalNumber; i++)
    {
        if (LeftOutNearestNeighbor(i, data, features))
        {
            correctClassification++;
        }
    }
    return double(correctClassification)/double(totalNumber) * 100;
}

void ForwardSelection(vector<vector<double> >* data, vector<int>* features)
{
    features->clear();
    vector<bool> status(data->at(0).size());
    status.at(0) = true;
    bool finished = false;

    vector<bool> bestOverallStatus;
    double bestOverallAccuracy = 0;
    int activeFeatures = 0;

    while (!finished)
    {
        finished = true;
        int bestFeature = 0;
        double bestFeatureAccuracy = 0;
        activeFeatures++;
        features->resize(activeFeatures);
        for (int i = 1; i < status.size(); i++)
        {
            if (!status.at(i))
            {
                finished = false;
                features->at(activeFeatures-1) = i;
                double currAccuracy = getLOOAccuracy(data, features);
                cout << "i is " << i << endl;
                cout << "activefeatures-1 is " << activeFeatures-1 << endl;
                printFeatures(features);
                cout << "Accuracy: " << currAccuracy << endl;
                cout << endl;
                if (currAccuracy > bestFeatureAccuracy) 
                {
                    bestFeatureAccuracy = currAccuracy;
                    bestFeature = i;
                }
            }
        }
        status.at(bestFeature) = true;
        features->at(activeFeatures-1) = bestFeature;
        if (bestFeatureAccuracy > bestOverallAccuracy)
        {
            bestOverallAccuracy = bestFeatureAccuracy;
            bestOverallStatus = status;
        }
    }
    features->clear();
    cout << "The combination with the highest accuracy was features ";
    for(int i = 1; i < bestOverallStatus.size(); i++)
    {
        if (bestOverallStatus.at(i))
        {
            cout << to_string(i) << " ";
            features->push_back(i);
        }
    }
    cout << "for an accuracy of " << bestOverallAccuracy << endl;

}