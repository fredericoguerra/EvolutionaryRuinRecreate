#include "Genetic.h"

Genetic::Genetic(Data *data, Population *population) : data(data), population(population){

}

Genetic::~Genetic(){
    delete offspring;
    delete localSearch;
    delete trainer;
}


void Genetic::crossoverOX(Individual *parent1, Individual *parent2) {

    // Beginning and end of the crossover zone
    //offspring = new Individual(data);

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


void Genetic::evolve(int maxIterWithoutImprov) {
    Individual *parent1;
    Individual *parent2;

    nbIterWithoutImprov = 1;
    int nbIterWithoutImprovDiv = 1;
    nbIter = 1;
    int place;

    offspring = new Individual(data);
    trainer = new Individual(data);
    localSearch = new LocalSearch(data);

     while (nbIterWithoutImprov < maxIterWithoutImprov) {
        // CROSSOVER
       // cout << "\nIteration: " << nbIterWithoutImprov << endl;
        parent1 = population->getIndividualBinT(); // Pick individual by binary tournament
        parent2 = population->getIndividualBinT(); // Pick individual by binary tournament
       /*  for(int i = 0; i < parent1->chromosome.size(); i++){
            cout << parent1->chromosome[i] << " ";
        }
        cout << endl;
         for(int i = 0; i < parent2->chromosome.size(); i++){
            cout << parent2->chromosome[i] << " ";
        }
        cout << endl;*/
    

        crossoverOX(parent1, parent2); // OX crossover

        // Calculates second objective
        //offspring->solutionCost.zeroBlocks = offspring->calcZeroBlocks();

        // LOCAL SEARCH
       /* cout << "cost before ls: " << offspring->solutionCost << endl;
        cout << "real cost before ls: " << offspring->calcCost() << endl;
        for(int i = 0; i < offspring->chromosome.size(); i++){
            cout << offspring->chromosome[i] << " ";
        }
        cout << endl;*/
        /*for(int i = 0; i < offspring->chromosome.size(); i++){
            //cout << offspring->chromosome[i] << " ";
            cout << data->positionsOffspring[i] << " ";
        }
        cout << endl;*/
        

        trainer->copyIndividual(trainer, offspring);
       // cout <<"initial: " << trainer->calcCost() << endl;
        //trainer->localSearch = new LocalSearch(data, trainer);
        localSearch->rvnd(trainer);

        // cout << "final: " << trainer->calcCost() << endl;
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
            nbIterWithoutImprov = 1;
            nbIterWithoutImprovDiv = 1;
        }
        else
            nbIterWithoutImprov++;

        nbIterWithoutImprovDiv++;
        nbIter++;

        // DIVERSIFICATION
        // Max iterations without improvement resulting in diversification reached
        if (nbIterWithoutImprovDiv == data->maxDiversify) {
            population->diversify();
            if (data->terminate) {
                return;
            }
            nbIterWithoutImprovDiv = 1;
        }
        //cout << "cost = " << population->getBestIndividual()->solutionCost << endl;
        //cout << "real = " << population->getBestIndividual()->calcCost() << endl;
    }
    data->nbIter = (unsigned int) nbIter;


}