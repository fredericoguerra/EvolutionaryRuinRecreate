#include "Data.h"
#include "Individual.h"
#include "Population.h"
#include "LocalSearch.h"
#include "Genetic.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <sys/timeb.h>
#include <sys/resource.h>

using namespace std;

double cpuTime()
{
    static struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return ((double)usage.ru_utime.tv_sec) + (((double)usage.ru_utime.tv_usec) / ((double)1000000));
}

int main(int argc, char **argv)
{

    Data *data;
    Population *population;
    

    data = new Data(argc, argv);

    std::string arg1(argv[1]);
    data->read();

    data->setParameters(10, 20, 5, 3, 1000);

    double bestTime = numeric_limits<double>::infinity();
    unsigned bestSeed;
    double sumRunTimes = 0, sumCosts = 0;

    int nExecs = 1;

    if (argc > 2 && argv[2][1] == 'b')
    { // Benchmark mode
        nExecs = 10;
    }

    Individual bestIndividual(data);
    Individual bestIndividualOverall(data);
    bestIndividualOverall.solutionCost = numeric_limits<double>::infinity();

    for (int i = 0; i < nExecs; i++)
    {

        unsigned seed = time(0);
        srand(seed);

        double initialTime = cpuTime();

        population = new Population(data);

        cout << population << endl;

        Genetic solver(data, population);
        solver.evolve(min(data->n * 10, 1000));
        // bestIndividual = population->getBestIndividual();
        bestIndividual = *(population->getBestIndividual());

        double runTime = cpuTime() - initialTime;


        sumRunTimes += runTime;
        sumCosts += bestIndividual.solutionCost;

        /*for (int i = 0; i < bestIndividualOverall.chromosome.size(); i++)
        {
            cout << bestIndividualOverall.chromosome[i] << " ";
        }
        cout << endl;*/

        if (bestIndividual.solutionCost < bestIndividualOverall.solutionCost)
        {
            bestIndividualOverall = bestIndividual;
            bestSeed = seed;
        }
        if (runTime < bestTime)
            bestTime = runTime;

        delete population;
    }

    cout << arg1 << "," << bestIndividualOverall.solutionCost << "," << sumCosts / nExecs << "," << bestTime << "," << sumRunTimes / nExecs << endl;
    cout << "seed: " << bestSeed << endl;
    for (int i = 0; i < bestIndividualOverall.chromosome.size(); i++)
    {
        cout << bestIndividualOverall.chromosome[i] << " ";
    }
    cout << endl;

    delete data;
    /*delete infoseq;
    delete data;
    delete bestIndividualOverall;*/

    /*LocalSearch *ls;

    ls = new LocalSearch(data);
    ls->rvnd(bestIndividual);

    for(int i = 0; i < bestIndividual->chromosome.size(); i++){
        cout << bestIndividual->chromosome[i] << " ";
    }
    cout << endl;

    cost = bestIndividual->calcCost();
    cout << "cost: " << cost << endl;*/

    return 0;
}