#ifndef RVND_H
#define RVND_H

#include "Solution.h"
#include "Infoseq.h"
#include "Utils.h"

using namespace std;

class RVND
{
    public:
        Data *data;
        Solution *sol;
        Infoseq *infoseq;
        RVND(Data *data, Solution *solution, Infoseq *infoseq);
        void run();
        void swap(Solution *s);
        void l_blockInsertion(unsigned int l, Solution *s);
        double compCostSwap(unsigned int i, unsigned int j);
        double compCostBlockInsertion(unsigned int l, unsigned int i, unsigned int j);
        ~RVND();
};



#endif