#ifndef GENETIC_H
#define GENETIC_H

#include "Population.h"
#include "Data.h"
#include "Individual.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <math.h>

using namespace std;

class Genetic {
    private:
        double nbTimeWithoutImprov; ///< number of iterations without improvement
        int nbIter; ///< number of iterations
    
    public:
        Population *population; ///< population structure
        Individual *offspring; ///< temporary structure for crossover childs
        Individual *trainer; ///< temporary structure for local search
        Data *data; ///< problem parameters
        LocalSearch *localSearch;

        void crossoverOX(Individual *parent1, Individual *parent2);
        void ruinRecreate(Individual *parent1, int ruinRate);
        void evolve(int maxTimeWithoutImprov, int ruinRate);
        Genetic(Data *data, Population *population);
        ~Genetic();
};


#endif
