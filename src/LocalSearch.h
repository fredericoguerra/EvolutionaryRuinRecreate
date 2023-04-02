#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <math.h>
#include <algorithm>
#include "Data.h"
#include "Individual.h"
#include "Infoseq.h"

class Individual;


class LocalSearch{
    public:
        unsigned int numNeighboors;
        Data *data; ///< problem data
        //Individual *individual; ///< individual to perform local search
        Individual *tempIndiv; ///< temporary structure for local search

        Infoseq *infoseq;

        void swap(vector<unsigned int> &solution, double &cost);
        void l_InsertBlock(int l, vector<unsigned int> &solucao, double &custo);
        double compCostSwap(int i, int j);
        double compCostBlockInsertion(int l, int i, int j);
        void rvnd(Individual *id);
        double compCost(vector<unsigned int> s);
        LocalSearch();
        LocalSearch(Data *data);
        ~LocalSearch();
    };



#endif