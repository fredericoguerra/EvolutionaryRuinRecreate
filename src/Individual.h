#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <list>
#include <iostream>
#include "Data.h"
#include "LocalSearch.h"

using namespace std;


/// Preliminary declaration, because proxData depends on Individual
class Individual;

//class LocalSearch;

/// Structure about the proximity of an Individual with regards to the others in the population
struct ProxData {
    // Individual
    Individual *individual;

    // Its distance to the others
    double dist;
};

class Individual {
    public:
        Data *data; ///< problem parameters
        int age; ///< age of individual
        double fitnessExt; ///< fitness of individual considering its cost and contribution to diversity
        float divRank; ///< rank of individual in terms of diversity
        float fitRank; ///< rank of individual in terms of cost
        double solutionCost; ///< KTNS cost of solution
        vector<unsigned int> chromosome; ///< chromosome of individual stores a permutation of jobs
        vector<int> edgesIndividuals; ///< temporary structure to compute distance
        bool isFitnessComputed; ///< if fitness has been computed
        list<ProxData> closest; ///< Population individuals ranked by proximity
        //LocalSearch *localSearch;



        void copyIndividual(Individual *destination, Individual *source);
        double calcCost();
        unsigned int distance(Individual *indiv);
        void addClose(Individual *indiv);
        void removeClose(Individual *indiv);
        double distToClosests(int n);
        Individual(Data *data);
        ~Individual();
};


#endif