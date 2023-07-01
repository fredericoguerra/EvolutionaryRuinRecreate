#include "Data.h"


Data::Data(int argc, char** argv)
{
    
    if (argc < 2) {
        cout << "\nFaltando parametros\n";
        cout << " ./exec [Instancia] "<< endl;
        exit(1);
    }

    if (argc > 4) { 
        cout << "\nMuitos parametros\n";
        cout << " ./exec [Instancia] " << endl;
        exit(1);
    }

    instance = argv[1];
}

Data::~Data()
{
}

void Data::read(){
    
   ifstream in(instance, ios::in);

   if (!in.is_open()) {
       cout << "Error: Instance not found" << endl;
        exit(1);
   }
   in >> n;
   in >> initialInventory;
   in >> maxCapacity;

   for(unsigned int i = 0; i < n; i++){

       bool isLoading; //0 - loading, 1- unloading
       in >> isLoading;
       //cout << loading << endl;

       Job tempJob;
       in >> tempJob.p_j;
       in.ignore(2, EOF);
       in >> tempJob.r_j;
       in >> tempJob.inv_mod;
       tempJob.id_job = i+1;

       if(!isLoading) // if loading is true then the inventory modification will be negative
            tempJob.inv_mod *= -1; 

        //cout << tempJob.r_j << endl;
        //cout << tempJob.inv_mod << endl;

        jobs.push_back(tempJob);
       
        /*
       jobs.push_back(Job())
       for(int j = 0; j < 5; j++){


       }*/
    }
}

void Data::setParameters(unsigned int populationSize, unsigned int maxPopulationSize, unsigned int numberElite, unsigned int numberCloseIndividuals, unsigned int maxDiversify){
    vector<bool> positionsOffspring(n);
    this->populationSize = populationSize;
    this->maxPopulationSize = maxPopulationSize;
    this->numberElite = numberElite;
    this->numberCloseIndividuals = numberCloseIndividuals;
    this->maxDiversify = maxDiversify;
    this->positionsOffspring = positionsOffspring;
    terminate = false;
}

void Data::printInstance(){
    cout << "N° jobs: " << n << endl;
    cout << "Initial Inventory: " << initialInventory << " " << "Max Capacity: " << maxCapacity << endl;

    for(int i = 0; i < n; i++){
        cout << i+1 << " " << jobs[i].p_j << " " << jobs[i].r_j << " " << jobs[i].inv_mod << endl; 
    }

}