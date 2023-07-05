#ifndef _DATA_H_
#define _DATA_H_

#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

struct Job
{
    unsigned int id_job;
    int r_j; // release date
    int p_j; // processing time
    int s_j; // setup time
    int inv_mod; // inventory modification
};


class Data{
    public:
        char* instance;
        int n; // number of jobs
        int initialInventory, maxCapacity;
        vector<Job> jobs; // vector containing release dates and processing times
        double** mSetupTimes; // machine setup times before process each job 

        //parameters
        unsigned int populationSize;
        unsigned int maxPopulationSize;
        unsigned int numberElite;
        unsigned int numberCloseIndividuals;
        unsigned int maxDiversify;
        unsigned int nbIter;
        bool terminate;

        vector<bool> positionsOffspring; ///< temporary structure for crossover operation

        
        void read();
        void printInstance();
        void setParameters(unsigned int populationSize, unsigned int maxPopulationSize, unsigned int numberElite, unsigned int numberCloseIndividuals, unsigned int maxDiversify);

        Data(int argc, char** argv);
        ~Data();
};



#endif