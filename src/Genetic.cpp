#include "Genetic.h"
#include <ctime>
#include<limits>

using namespace std;

Genetic::Genetic(Data *data, Population *population) : data(data), population(population){

}

Genetic::~Genetic(){
    delete offspring;
    delete localSearch;
    delete trainer;
}

void Genetic::ruinRecreate(Individual *parent1, int ruinRate){
    int best_i = 0;
    offspring->chromosome.clear();
    /*cout << endl << endl << endl << "parent1:" << endl;
    for(int i = 0; i < parent1->chromosome.size(); i++){
        cout << parent1->chromosome[i] << " ";
    }
    cout << endl;*/

    vector<unsigned int> ruinedJobs;

    // Calculate the number of jobs to ruin
    //int ruinSize = int (parent1->chromosome.size() * ruinRate/ 100.0);
    
    int ruinSize = ruinRate;
    /*cout <<"ruinSize: " << ruinSize << endl;*/


    for (unsigned int i = 0; i < data->n; i++) {
        data->positionsOffspring[i] = false;
    }

    // Destroy a subset of the solution
    while(ruinedJobs.size()<ruinSize){
        int index = rand() % data->n;
        // Add the removed jobs into a temporary list
        if(!data->positionsOffspring[parent1->chromosome[index] - 1]){
            ruinedJobs.push_back(parent1->chromosome[index]);
            data->positionsOffspring[parent1->chromosome[index] - 1] = true;
            }
        }
    
    for(int i = 0; i < data->n; i++){
        if(!data->positionsOffspring[parent1->chromosome[i] - 1]){
            offspring->chromosome.push_back(parent1->chromosome[i]);
        }
    }
    /*
    cout << endl;
    cout << "ruined jobs:" << endl;

    for(int i = 0; i < ruinedJobs.size(); i++){
        cout << " " << ruinedJobs[i];
    }
    cout << endl;*/
    while(ruinedJobs.size() > 0){
        best_i = 0;
        int selectedJobIndex = rand() % ruinedJobs.size();
        double tempCost = numeric_limits<double>::infinity();

        for(int i = 0; i < offspring->chromosome.size(); i++)
        {
            offspring->chromosome.insert(offspring->chromosome.begin() + i,ruinedJobs[selectedJobIndex]);
            if(offspring->calcCost() < tempCost){
                best_i = i;
                tempCost = offspring->calcCost();
            }
            offspring->chromosome.erase(offspring->chromosome.begin() + i);
        }
        

        /*cout << endl << "offspring before insertion:" << endl;
        for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
        }*/
        offspring->chromosome.insert(offspring->chromosome.begin() + best_i, ruinedJobs[selectedJobIndex]);

        ruinedJobs.erase(ruinedJobs.begin() + selectedJobIndex);
        
        /*cout << endl << "offspring after insertion:" << endl;*/
        
        /*for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
        }*/
        /*cout << endl <<  "ruined jobs after insertion: " << endl;*/
        
        /*for(int i=0; i<ruinedJobs.size();i++){
            cout << " " << ruinedJobs[i]; 
        }*/

    }

    offspring->solutionCost = offspring->calcCost();

    /*cout << "offspring:" << endl;
    for(int i = 0; i < offspring->chromosome.size(); i++){
        cout << offspring->chromosome[i] << " ";
    }*/
    /*cout << endl;
    cout << "best i: " << best_i << endl;
    cout << "parent cost: " << parent1->solutionCost << endl;
    cout << "offspring cost: " << offspring->solutionCost << endl;*/
}

void Genetic::crossoverOX(Individual *parent1, Individual *parent2) {

    // Beginning and end of the crossover zone
    // offspring = new Individual(data);

    unsigned int begin = rand() % data->n;
    unsigned int end = rand() % data->n;
    while (end == begin && data->n > 1)
        end = rand() % data->n;

    if (begin > end) {
        unsigned int temp = begin;
        begin = end;
        end = temp;
    }
    /*cout << "begin: " << begin << endl;
    cout << "end: " << end <<endl;*/

    // Copy part of parent1 to child
    offspring->chromosome = parent1->chromosome;
    for (unsigned int i = 0; i < data->positionsOffspring.size(); i++) {
        data->positionsOffspring[i] = false;
    }
    for (unsigned int i = begin; i <= end; i++) {
        data->positionsOffspring[parent1->chromosome[i] - 1] = true;
    }

    /* for (unsigned int i = 0; i < data->positionsOffspring.size(); i++) {
        cout << data->positionsOffspring[i] << " ";
    }
    cout << endl;*/
    // Copy unused values of parent2 to child sequentially
    unsigned int pos = end + 1, i = end + 1;
    while (pos < data->positionsOffspring.size()) {
        if (!data->positionsOffspring[parent2->chromosome[i] - 1]) {
            offspring->chromosome[pos] = parent2->chromosome[i];
            pos++;
        }
        i++;
        if (i == data->positionsOffspring.size()) {
            i = 0;
        }
    }
    if (i == data->positionsOffspring.size()) {
        i = 0;
    }
    pos = 0;
    while (pos < begin) {
        if (!data->positionsOffspring[parent2->chromosome[i] - 1]) {
            offspring->chromosome[pos] = parent2->chromosome[i];
            pos++;
        }
        i++;
        if (i == data->positionsOffspring.size()) {
            i = 0;
        }
    }
    offspring->solutionCost = offspring->calcCost();
}

void Genetic::evolve(int maxTimeWithoutImprov, int ruinRate) {
    
    //Creates two parentals
    Individual *parent1;
    Individual *parent2;

    double nbTime = 0;
    nbTimeWithoutImprov = 1;
    int nbTimeWithoutImprovDiv = 1;
    nbIter = 1;
    int place;

    offspring = new Individual(data);
    trainer = new Individual(data);

    localSearch = new LocalSearch(data);

    std::clock_t c_start = std::clock();

    while (nbTimeWithoutImprov < maxTimeWithoutImprov) {
        // CROSSOVER
       // cout << "\nIteration: " << nbTimeWithoutImprov << endl;
        parent1 = population->getIndividualBinT(); // Pick individual by binary tournament
        parent2 = population->getIndividualBinT(); // Pick individual by binary tournament
        /*cout << endl << endl << "parents : "<< endl;
        for(int i = 0; i < parent1->chromosome.size(); i++){
            cout << parent1->chromosome[i] << " ";
        }
        cout << endl;
         for(int i = 0; i < parent2->chromosome.size(); i++){
            cout << parent2->chromosome[i] << " ";
        }
        cout << endl;
    
        
        cout << "offspring before: " << endl;
        for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
        }*/

        //crossoverOX(parent1, parent2); // OX crossover
        
        ruinRecreate(parent1, ruinRate);

        /*
        cout << endl << "offspring: " << endl;
        for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
        }*/
        // Calculates second objective
        //offspring->solutionCost.zeroBlocks = offspring->calcZeroBlocks();

        // LOCAL SEARCH
       /* cout << "cost before ls: " << offspring->solutionCost << endl;
        cout << "real cost before ls: " << offspring->calcCost() << endl;
        for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
        }
        cout << endl;*/
        //cout << "offspring:" << offspring->age <<endl;
        /*for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
            //cout << data->positionsOffspring[i] << " ";
        }
        cout << endl;*/
        

        trainer->copyIndividual(trainer, offspring);
        //cout << "trainer:" << trainer->age << endl;
        /*for(int i = 0; i < trainer->chromosome.size(); i++){
            cout << trainer->chromosome[i] << " ";
            //cout << data->positionsOffspring[i] << " ";
        }
        cout << endl;*/
        //cout <<"initial: " << trainer->calcCost() << endl;
        //trainer->localSearch = new LocalSearch(data, trainer);
        localSearch->rvnd(trainer);
        //cout << "final: " << trainer->calcCost() << endl;
        /*cout << "cost after ls: " << trainer->solutionCost << endl;
        cout << "real cost after ls: " << trainer->calcCost() << endl;
         for(int i = 0; i < trainer->chromosome.size(); i++){
            cout << trainer->chromosome[i] << " ";
        }*/
        offspring->copyIndividual(offspring, trainer);
        //cout << offspring->calcCost() << " " << offspring->solutionCost << endl;

        //delete trainer->localSearch;
        //delete localSearch;

        // Tries to add child to population
        place = population->addIndividual(offspring);

        //cout << place << endl;
        if (place == -2) {
            return;
        }

        if (place == 0) { // A new best solution has been found
            nbTimeWithoutImprov = 1;
            nbTimeWithoutImprovDiv = 1;
        }
        else
            nbTimeWithoutImprov++;
        

        nbTimeWithoutImprovDiv++;
        nbIter++;

        // DIVERSIFICATION
        // Max iterations without improvement resulting in diversification reached
        if (nbTimeWithoutImprovDiv == data->maxDiversify) {
            population->diversify();
            if (data->terminate) {
                return;
            }
            nbTimeWithoutImprovDiv = 1;
        }
        //cout << "cost = " << population->getBestIndividual()->solutionCost << endl;
        //cout << "real = " << population->getBestIndividual()->calcCost() << endl;

        std::clock_t c_end = std::clock();
        nbTime = 1000.0 * (c_end - c_start)/ CLOCKS_PER_SEC;
        //cout << nbTime << endl;
    }
    data->nbIter = (unsigned int) nbIter;


}