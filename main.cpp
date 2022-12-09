#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

void printTable(vector<vector<double> > in);
void printFeatures(vector<int>* features);
vector<vector<double> > addDatatoTable(string file);
bool LeftOutNearestNeighbor(int row, vector<vector<double> >* data, vector<int>* features);
double getEuclideanDistance(int row1, int row2, vector<vector<double> >* data, vector<int>* features);
double getLOOAccuracy(vector<vector<double> >* data, vector<int>* features);
void ForwardSelection(vector<vector<double> >* data, vector<int>* features, string file);
void BackwardsElimination(vector<vector<double> >* data, vector<int>* features, string file);

int main() 
{
    // initializes a 2D vector and "activeFeatures" vector
    string datafile = "CS170_Small_Data__89.txt";
    vector<vector<double> > temp = addDatatoTable(datafile);
    vector<vector<double> >* table = &temp;
    vector<int> tempFeatures;
    vector<int>* activeFeatures = &tempFeatures;

    // printing table for data input testing
    // printTable(*table);

    // adding activeFeatures for testing
    // activeFeatures->push_back(21);
    // activeFeatures->push_back(10);

    // LeftOutNearestNeighbor(1, table, activeFeatures);
    // LeftOutNearestNeighbor(10, table, activeFeatures);
    // LeftOutNearestNeighbor(100, table, activeFeatures);

    // getLOOAccuracy(table, activeFeatures);

    ForwardSelection(table, activeFeatures, datafile);
    BackwardsElimination(table, activeFeatures, datafile);
}

void printFeatures(vector<int>* features)
{
    cout << "{ ";
    for(int i = 0; i < features->size(); i++)
    {
        cout << features->at(i) << " ";
    }
    cout << "}";
}

vector<vector<double> > addDatatoTable(string file)
{
    vector<vector<double> > temp;
    ifstream input(file);

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
    double accuracy = double(correctClassification)/double(totalNumber) * 100;

    // cout << "   Using feature(s) ";
    // printFeatures(features);
    // cout << " accuracy is " << accuracy << "%" << endl;

    return accuracy;
}

void ForwardSelection(vector<vector<double> >* data, vector<int>* features, string file)
{
    clock_t time = clock();
    features->clear();
    vector<bool> status(data->at(0).size());
    status.at(0) = true;
    bool finished = false;

    vector<bool> bestOverallStatus;
    double bestOverallAccuracy = 0;
    int activeFeatures = 0;
    cout << "Searching through combinations of up to " << status.size()-1 << " total features in \"" << file << "\"." << endl << endl;

    while (!finished)
    {
        finished = true;
        int bestFeature = 0;
        double bestFeatureAccuracy = 0;
        activeFeatures++;
        features->resize(activeFeatures);
        cout << "Checking for " << activeFeatures << " feature combinations at " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;

        for (int i = 1; i < status.size(); i++)
        {
            if (!status.at(i))
            {
                finished = false;
                features->at(activeFeatures-1) = i;
                double currAccuracy = getLOOAccuracy(data, features);
                if (currAccuracy > bestFeatureAccuracy) 
                {
                    bestFeatureAccuracy = currAccuracy;
                    bestFeature = i;
                }
            }
        }
        status.at(bestFeature) = true;
        features->at(activeFeatures-1) = bestFeature;

        cout << "   Feature set ";
        printFeatures(features);
        cout << " was best in this pool with an accuracy of " << bestFeatureAccuracy << "%" << endl;

        if (bestFeatureAccuracy > bestOverallAccuracy)
        {
            bestOverallAccuracy = bestFeatureAccuracy;
            bestOverallStatus = status;
        }
    }
    features->clear();
    cout << endl << "The combination with the highest accuracy overall was features ";
    for(int i = 1; i < bestOverallStatus.size(); i++)
    {
        if (bestOverallStatus.at(i))
        {
            features->push_back(i);
        }
    }
    printFeatures(features);
    cout << " with an accuracy of " << bestOverallAccuracy << "%." << endl;
    cout << "Total Computing Time: " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;
}

void BackwardsElimination(vector<vector<double> >* data, vector<int>* features, string file)
{
    clock_t time = clock();
    features->clear();
    int size = data->at(0).size()-1;
    features->resize(size);
    for(int i = 1; i < size+1; i++) {
        features->at(i-1) = i;
    }
    bool finished = false;

    vector<int> bestOverallFeatures; //initializes as all false
    double bestOverallAccuracy = 0;
    int activeFeatures = size;
    cout << "Searching through combinations of up to " << size << " total features in \"" << file << "\"." << endl << endl;

    while (activeFeatures != 1)
    {
        finished = true;
        int bestFeaturetoRemove = 0;
        double bestAccuracy = getLOOAccuracy(data, features);
        double worstAccuracy = bestAccuracy;
        cout << " " << activeFeatures-1 << " feature combinations being tested at " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;

        int firstFeature = features->at(0);
        for(int i = 0; i < activeFeatures; i++)
        {
            int currFeature = features->at(0);
            features->erase(features->begin());

            double currAccuracy = getLOOAccuracy(data, features);
            if (currAccuracy < worstAccuracy) 
            {
                worstAccuracy = currAccuracy;
            }
            if (currAccuracy > bestAccuracy)
            {
                bestAccuracy = currAccuracy;
                bestFeaturetoRemove = i;
            }

            features->push_back(currFeature);
        }
        // cout << "   Removing feature " << features->at(bestFeaturetoRemove) << endl;
        features->erase(features->begin()+bestFeaturetoRemove);
        activeFeatures--;

        // cout << "Feature set ";
        // printFeatures(features);
        // cout << " was best in this pool with an accuracy of " << bestAccuracy << "%" << endl;

        if (bestAccuracy > bestOverallAccuracy)
        {
            bestOverallAccuracy = bestAccuracy;
            bestOverallFeatures.clear();
            for (int i = 0; i < features->size(); i++)
            {
                bestOverallFeatures.push_back(features->at(i));
            }
        }
        // break;
    }
    features->clear();
    cout << endl << "The combination with the highest accuracy overall was features ";
    for(int i = 0; i < bestOverallFeatures.size(); i++)
    {
        features->push_back(bestOverallFeatures.at(i));
    }
    printFeatures(features);
    cout << " with an accuracy of " << bestOverallAccuracy << "%." << endl;
    cout << "Total Computing Time: " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;
}