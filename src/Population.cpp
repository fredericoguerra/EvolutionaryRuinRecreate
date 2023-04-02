
#include "Population.h"

Population::Population(Data *data) : data(data){
    Individual *randomIndiv;
    subPopulation = new SubPopulation();
    subPopulation->numberIndividuals = 0;

    trainer = new Individual(data);
    localSearch = new LocalSearch(data);

    for (unsigned int i = 0; i < data->populationSize; i++) {
        randomIndiv = new Individual(data);
        education(randomIndiv); // busca local na população inicial
        addIndividual(randomIndiv);
        delete randomIndiv;
    }
}

Population::~Population(){

    delete localSearch;
    
    int size;
    if (subPopulation != nullptr) {
        size = (int)subPopulation->individuals.size();
        for (int i = 0; i < size; i++)
            delete subPopulation->individuals[i];
        delete subPopulation;
    }
    //delete trainer->localSearch;
    delete trainer;
    
}

int Population::placeIndividual(SubPopulation *subPop, Individual *indiv){
    Individual *myIndiv = new Individual(data);
    myIndiv->copyIndividual(myIndiv, indiv);

    int i = subPop->individuals.size() - 1;
    subPop->individuals.push_back(myIndiv);
    while (i >= 0) {
        if (subPop->individuals[i]->solutionCost > indiv->solutionCost) {
            subPop->individuals[i + 1] = subPop->individuals[i];
            i--;
        }
        else {
            subPop->individuals[i + 1] = myIndiv;
            subPop->numberIndividuals++;
            updateProximity(subPop, subPop->individuals[i + 1]);
            return i + 1; // success
        }
    }
    subPop->individuals[0] = myIndiv;
    subPop->numberIndividuals++;
    updateProximity(subPop, subPop->individuals[0]);
    //    if (subPop == )
    //        timeBest = clock();
    return 0; // success
}

void Population::removeIndividual(SubPopulation *subPop, int p) {
    Individual *remIndiv = subPop->individuals[p];

    // Place individual at the end
    for (int i = p + 1; i < subPop->individuals.size(); i++)
        subPop->individuals[i - 1] = subPop->individuals[i];

    // Remove it from the population
    subPop->individuals.pop_back();
    subPop->numberIndividuals--;

    // Remove it from the proximity structures
    for (int i = 0; i < subPop->numberIndividuals; i++)
        subPop->individuals[i]->removeClose(remIndiv);

    delete remIndiv;
}

int Population::addIndividual(Individual *indiv) {
    SubPopulation *subPop;
    int k, result;
    bool firstIt = true;

    subPop = subPopulation;
    result = placeIndividual(subPop, indiv);

   //calcJobsDistance(subPop, data);

    // Keep only the survivors if the maximum size of the population has been reached
    if (result != -1 && subPop->numberIndividuals > data->populationSize + data->maxPopulationSize) {
        while (subPop->numberIndividuals > data->populationSize) {
            k = selectToRemove(subPop);
            removeIndividual(subPop, k);
            if (firstIt) {
                firstIt = false;
            }
        }
    }
    return result;
}

void Population::updateProximity(SubPopulation *subPop, Individual *indiv) {
    for (int k = 0; k < subPop->numberIndividuals; k++) {
        if (subPop->individuals[k] != indiv) {
            subPop->individuals[k]->addClose(indiv);
            indiv->addClose(subPop->individuals[k]);
        }
    }
}

bool Population::fitExist(SubPopulation *subPop, Individual *indiv) {
    unsigned int count = 0;
    unsigned int distance = indiv->solutionCost;
    for (int i = 0; i < subPop->numberIndividuals; i++) {
        if (subPop->individuals[i]->solutionCost == distance) {
                count++;
        }
    }
    if (count <= 1)
        return false;
    else
        return true;
}

void Population::evalExtFit(SubPopulation *subPop) {
    int temp;
    vector<int> position;
    vector<double> distances;

    for (int i = 0; i < subPop->numberIndividuals; i++) {
        position.push_back(i);
        distances.push_back(subPop->individuals[i]->distToClosests(data->numberCloseIndividuals));
    }

    // Rank individuals in terms of contribution to diversity
    for (int n = 0; n < subPop->numberIndividuals; n++) {
        for (int i = 0; i < subPop->numberIndividuals - n - 1; i++) {
            if (distances[position[i]] < distances[position[i + 1]] - 0.000001) {
                temp = position[i + 1];
                position[i + 1] = position[i];
                position[i] = temp;
            }
        }
    }

    // Compute the biased fitness
    for (int i = 0; i < subPop->numberIndividuals; i++) {
        subPop->individuals[position[i]]->divRank = (float)i / (float)(subPop->numberIndividuals - 1);
        subPop->individuals[position[i]]->fitRank = (float)position[i] / (float)(subPop->numberIndividuals - 1);
        subPop->individuals[position[i]]->fitnessExt = subPop->individuals[position[i]]->fitRank + ((float) 1.0 - (float)data->numberElite / (float)subPop->numberIndividuals) * subPop->individuals[position[i]]->divRank;
    }
}




void Population::updateAge() {
    for (int i = 0; i < subPopulation->numberIndividuals; i++)
        subPopulation->individuals[i]->age++;
}

void Population::diversify() {
    Individual *randomIndiv;

    // Remove 70% of the population
    while (subPopulation->numberIndividuals > (int)(0.3 * (double)data->populationSize)) {
        delete subPopulation->individuals[subPopulation->numberIndividuals - 1];
        subPopulation->individuals.pop_back();
        subPopulation->numberIndividuals--;
    }

    // Create new individuals until minimum population size is reached
    for (unsigned int i = 0; i < data->populationSize; i++) {
        randomIndiv = new Individual(data);
        education(randomIndiv);
        addIndividual(randomIndiv);
        delete randomIndiv;
    }
}


Individual *Population::getIndividualBinT() {
    Individual *individual1;
    Individual *individual2;
    int place1, place2;

    // Pick the first individual
    place1 = rand() % (subPopulation->numberIndividuals);
    individual1 = subPopulation->individuals[place1];

    // Pick the second individual
    place2 = rand() % (subPopulation->numberIndividuals);
    individual2 = subPopulation->individuals[place2];

    evalExtFit(subPopulation);

    // Keep the best one
    if (individual1->fitnessExt < individual2->fitnessExt)
        return individual1;
    else
        return individual2;
}


Individual *Population::getBestIndividual() {
    if (subPopulation->numberIndividuals != 0)
        return subPopulation->individuals[0];
    else
        return nullptr;
}

int Population::selectToRemove(SubPopulation *subPop) {
    // Select one individual to be eliminated from the population
    vector<int> position;
    int temp, toRemove;

    updateAge();
    evalExtFit(subPop);

    for (int i = 0; i < subPop->numberIndividuals; i++)
        position.push_back(i);

    // Add a penalty in case of clone
    for (int i = 1; i < subPop->numberIndividuals; i++) {
        if (subPop->individuals[i]->distToClosests(1) <= 0.001) { // in solution space
            subPop->individuals[i]->fitnessExt += 5;
        }
        if (fitExist(subPop, subPop->individuals[i])) { // in objective space
            subPop->individuals[i]->fitnessExt += 5;
        }
    }

    // Rank elements by extended fitness and select out the worst
    for (int n = 0; n < subPop->numberIndividuals; n++) {
        for (int i = 0; i < subPop->numberIndividuals - n - 1; i++) {
            if (subPop->individuals[position[i]]->fitnessExt > subPop->individuals[position[i + 1]]->fitnessExt) {
                temp = position[i + 1];
                position[i + 1] = position[i];
                position[i] = temp;
            }
        }
    }
    toRemove = position[subPop->numberIndividuals - 1];
    return toRemove;
}

void Population::education(Individual *indiv) {
    //trainer = new Individual(data);
    trainer->copyIndividual(trainer, indiv);
    localSearch->rvnd(trainer);
    indiv->copyIndividual(indiv, trainer);
}

