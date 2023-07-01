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
#include <string>
#include <sstream>

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

    data->setParameters(5, 10, 5, 3, 1000);
    
    int ruinRate = 5;

    //data->printInstance();

    double bestTime = numeric_limits<double>::infinity();
    unsigned bestSeed;
    double sumRunTimes = 0, sumCosts = 0;

    int nExecs = 1;

    if (argc > 2 && argv[2][1] == 'b')
    { // Benchmark mode
        nExecs = 10;
        //ruinRate = stoi(argv[3]);
        ruinRate = 3;
    }

    string instance_name = argv[1];

    string delimiter_f = "/";
    size_t pos_f = 0;
    string token_f;
    while ((pos_f = instance_name.find(delimiter_f)) != string::npos) {
                    token_f = instance_name .substr(0, pos_f);
                    instance_name.erase(0, pos_f + delimiter_f.length());
                }
    /*
    //get ILS time
    ifstream file("./ILS_results.txt");
    string search = instance_name;
    string delimiter = ",";
    size_t pos = 0;
    string token;
    double ilsTime;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.find(search) != string::npos) {
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos);
                    line.erase(0, pos + delimiter.length());
                }

                stringstream(line) >> ilsTime;
                break;
            }
        }
        file.close();
    }
    else {
        cout << "Unable to open file." << endl;
    }*/

    Individual bestIndividual(data);
    Individual bestIndividualOverall(data);
    bestIndividualOverall.solutionCost = numeric_limits<double>::infinity();

    for (int i = 0; i < nExecs; i++)
    {

        unsigned seed = time(0);
        srand(seed);
        //int seed = 1683593333;

        double initialTime = cpuTime();
        //initialize population
        population = new Population(data);


        Genetic solver(data, population);
        //solver.evolve(ilsTime * 1000.0);
        solver.evolve(min(data->n * 10, 1000), ruinRate);
        
        //solver.evolve(1);
        // bestIndividual = population->getBestIndividual();
        bestIndividual = *(population->getBestIndividual());

        double runTime = cpuTime() - initialTime;

        //cout << runTime << endl;


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

    cout << arg1 << "," << bestIndividualOverall.solutionCost << "," << sumCosts / nExecs << "," << bestTime << "," << sumRunTimes / nExecs << "," << ruinRate << endl;
    cout << "seed: " << bestSeed << endl;
    cout << "best sol: ";
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