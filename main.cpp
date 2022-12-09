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
    // User inputs file name
    string input;
    std::cout << "This is Keita Ichii's Feature Selection Algorithm." << endl; 
    std::cout << "Type in the name of the file to test: ";
    cin >> input;
    string datafile = input;

    // initializes a 2D vector and "activeFeatures" vector
    vector<vector<double> > temp = addDatatoTable(datafile);
    vector<vector<double> >* table = &temp;
    vector<int> tempFeatures;
    vector<int>* activeFeatures = &tempFeatures;

    // User inputs desired algorithm
    int inputNum;
    std::cout << "\nType the number of the desired algorithm.\n1) Forward Selection\n2) Backward Elimination\n3) Both" << endl;
    cin >> inputNum;
    if (inputNum == 1 || inputNum == 3) {ForwardSelection(table, activeFeatures, datafile);}
    if (inputNum == 2 || inputNum == 3) {BackwardsElimination(table, activeFeatures, datafile);}
    if (inputNum == 4)
    {
        // Testing Space
    }
}

void printFeatures(vector<int>* features)
{
    std::cout << "{ ";
    for(int i = 0; i < features->size(); i++)
    {
        std::cout << features->at(i) << " ";
    }
    std::cout << "}";
}

vector<vector<double> > addDatatoTable(string file)
{
    vector<vector<double> > temp;
    ifstream input(file);

    if (!input.is_open()) 
    {
        std::cout << "Failed to Open" << endl;
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
            std::cout << in[i][k];
            std::cout << " ";
        }
        std::cout << endl;
    }
}

// finds the nearest neighbor to the excluded row, then returns a boolean based on whether or not the neighbor 
// and the excluded row share the same class.
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

// calls the Nearest Neighbor function on every row and then counts to get accuracy percentage.
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

    std::cout << "   Using feature(s) ";
    printFeatures(features);
    std::cout << " accuracy is " << accuracy << "%" << endl;

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
    std::cout << "\nApplying Forward Selection to search through combinations of up to " << status.size()-1 << " total features in \"" << file << "\" of " << data->size()+0 << " instances." << endl << endl;

    while (!finished)
    {
        finished = true;
        int bestFeature = 0;
        double bestFeatureAccuracy = 0;
        activeFeatures++;
        features->resize(activeFeatures);
        std::cout << "Checking for " << activeFeatures << " feature combinations at " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;

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

        std::cout << "\n   Feature set ";
        printFeatures(features);
        std::cout << " was best in this pool with an accuracy of " << bestFeatureAccuracy << "%\n" << endl;

        if (bestFeatureAccuracy > bestOverallAccuracy)
        {
            bestOverallAccuracy = bestFeatureAccuracy;
            bestOverallStatus = status;
        }
    }
    features->clear();
    std::cout << endl << "The combination with the highest accuracy overall was features ";
    for(int i = 1; i < bestOverallStatus.size(); i++)
    {
        if (bestOverallStatus.at(i))
        {
            features->push_back(i);
        }
    }
    printFeatures(features);
    std::cout << " with an accuracy of " << bestOverallAccuracy << "%." << endl;
    std::cout << "Total Computing Time: " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;
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
    std::cout << "\nApplying Backward Elimination to search through combinations of up to " << size << " total features in \"" << file << "\" of " << data->size()+0 << " instances." << endl << endl;

    getLOOAccuracy(data, features);
    std::cout << endl;

    while (activeFeatures != 1)
    {
        finished = true;
        int bestFeaturetoRemove = 0;
        double bestAccuracy = 0;
        double worstAccuracy = 100;
        std::cout << " " << activeFeatures-1 << " feature combinations being tested at " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;

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
        // std::cout << "   Removing feature " << features->at(bestFeaturetoRemove) << endl;
        features->erase(features->begin()+bestFeaturetoRemove);
        activeFeatures--;

        std::cout << "\n   Feature set ";
        printFeatures(features);
        std::cout << " was best in this pool with an accuracy of " << bestAccuracy << "%\n" << endl;

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
    std::cout << endl << "The combination with the highest accuracy overall was features ";
    for(int i = 0; i < bestOverallFeatures.size(); i++)
    {
        features->push_back(bestOverallFeatures.at(i));
    }
    printFeatures(features);
    std::cout << " with an accuracy of " << bestOverallAccuracy << "%." << endl;
    std::cout << "Total Computing Time: " << fixed << setprecision(2) << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << endl;
}