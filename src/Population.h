#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "Individual.h"
#include "LocalSearch.h"

using namespace std;

struct SubPopulation {

    vector<Individual*> individuals; ///< individuals in the population

    int numberIndividuals; ///< number of individuals in the population
};

class Population {
    private:
        void education(Individual *indiv);
        int placeIndividual(SubPopulation *subPop, Individual *individual);

    public:
        //vector<Individual*> individuals;
        //int numIndividuals = 0;
        SubPopulation *subPopulation;
        Data *data;
        Individual *trainer; // structure to instantiate only one individual for local search procedures
        LocalSearch *localSearch;
        
        
        void removeIndividual(SubPopulation *subPop, int pos);
        int addIndividual(Individual *indiv);
        void updateProximity(SubPopulation *subPop, Individual *indiv);
        bool fitExist(SubPopulation *subPop, Individual *indiv);
        void evalExtFit(SubPopulation *subPop);
        void updateAge();
        void diversify();
        Individual *getIndividualBinT();
        Individual *getBestIndividual();
        int selectToRemove(SubPopulation *subPop);
        Population(Data *data);
        ~Population();

    };




#endif