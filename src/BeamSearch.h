#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include <vector>
#include <list>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Data.h"
#include "Solution.h"


using namespace std;

class BeamSearch
{
    public:
        Data *data; ///< problem parameters
        int totalBranchesPerNode;
        int totalBranchesPerLevel;
        double alfa; 
        double beta;
        //vector<unsigned int> solution;
        //double solutionCost;

        //Solution *solution;


        BeamSearch(Data *data, int totalBranchesPerNode, int totalBranchesPerLevel, double alfa, double beta);
        void runBS(Solution *solution);
        int binarySearch(vector<unsigned int> &vec, int posInit, int posEnd, int compare);
        //bool compBSLWT(tuple <pair<double, double>, vector<int>, vector<int>, double > a, tuple <pair<double, double>, vector<int>, vector<int>, double> b);
        //bool compBSLCT(tuple <pair<double, double>, vector<int>, vector<int>, double > a, tuple <pair<double, double>, vector<int>, vector<int>, double > b);
        double idleTime(vector<unsigned int> s, double cTime, unsigned int node);
        double currentCapacity(vector<unsigned int> s, double cTime, unsigned int job);
        bool checkViability();
        double totalInventoryMod(vector<unsigned int> remainingVertices);
        int genRandomInteger(int min, int max);
        //bool compMakespan(tuple <pair<double, double>, vector<int>, vector<int>, double > a, tuple <pair<double, double>, vector<int>, vector<int>, double > b);
        
        //sobral?
        ~BeamSearch();
};

#endif

