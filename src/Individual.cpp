#include "Individual.h"

Individual::Individual(Data *data) : data(data)
{
    age = 0;
    isFitnessComputed = false;

    for (unsigned int i = 1; i <= data->n; i++)
    { // initiate a permutation of jobs
        chromosome.push_back(i);
        edgesIndividuals.push_back(0);
    }

    unsigned int jj, temp;
    for (unsigned int i = 0; i <= (unsigned int)chromosome.size() - 1; i++)
    { // shuffles the sequence of jobs
        jj = i + rand() % ((unsigned int)chromosome.size() - i);
        temp = chromosome[i];
        chromosome[i] = chromosome[jj];
        chromosome[jj] = temp;
    }

    solutionCost = calcCost(); // compute cost of individual
}

Individual::~Individual()
{
}

void Individual::copyIndividual(Individual *destination, Individual *source)
{
    destination->chromosome = source->chromosome;
    destination->solutionCost = source->solutionCost;
    destination->isFitnessComputed = source->isFitnessComputed;
    destination->closest = source->closest;
    destination->age = 0;
}

double Individual::calcCost()
{
    double c_time = 0;
    double inventory = data->initialInventory;
    bool penalize = false;

    for (int i = 0; i < data->n; i++)
    {
        if (c_time >= data->jobs[chromosome[i] - 1].r_j)
            c_time += data->jobs[chromosome[i] - 1].p_j;
        else
            c_time = data->jobs[chromosome[i] - 1].r_j + data->jobs[chromosome[i] - 1].p_j;

        inventory += data->jobs[chromosome[i] - 1].inv_mod;
        // cout << inventory << " ";
        if (inventory < 0 || inventory > data->maxCapacity)
            penalize = true;
    }
    // cout << endl;
    c_time = (penalize) ? c_time * 10000 : c_time;

    return c_time;
}

unsigned int Individual::distance(Individual *indiv)
{
    // This method computed the edges distance between two solutions
    // If an element of solution 1 has a different neighbor (behind
    // or ahead) from the same element of solution 2, then the distance
    // is incremented by one

    unsigned int dist = 0;
    for (unsigned int i = 0; i < chromosome.size() - 2; i++)
    {
        edgesIndividuals[chromosome[i]] = (int)chromosome[i + 1];
    }
    for (unsigned int i = 0; i < indiv->chromosome.size() - 2; i++)
    {
        if (edgesIndividuals[indiv->chromosome[i]] != indiv->chromosome[i + 1] && edgesIndividuals[indiv->chromosome[i + 1]] != indiv->chromosome[i])
        {
            dist++;
        }
    }
    return dist;
}

void Individual::addClose(Individual *indiv)
{
    // Add an individual in the structure of proximity
    list<ProxData>::iterator it;
    ProxData proxdata;
    proxdata.individual = indiv;
    proxdata.dist = distance(indiv);

    if (closest.empty())
        closest.push_back(proxdata);
    else
    {
        it = closest.begin();
        while (it != closest.end() && it->dist < proxdata.dist)
            ++it;
        closest.insert(it, proxdata);
    }
}

void Individual::removeClose(Individual *indiv)
{
    // Remove an individual in the structure of proximity
    list<ProxData>::iterator last = closest.end();
    for (list<ProxData>::iterator first = closest.begin(); first != last;)
        if (first->individual == indiv)
            first = closest.erase(first);
        else
            ++first;
}

double Individual::distToClosests(int n)
{
    // Compute the average distance with the n close elements
    double result = 0;
    double compte = 0;
    list<ProxData>::iterator it = closest.begin();

    for (int i = 0; i < n && it != closest.end(); i++)
    {
        result += it->dist;
        compte += 1.0;
        ++it;
    }
    return result / compte;
}