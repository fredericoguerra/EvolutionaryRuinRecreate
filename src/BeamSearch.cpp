#include "BeamSearch.h"

//sobral?
bool compMakespan(tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> a, tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> b);
bool compBSLCT(tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> a, tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> b);
bool compBSLWT(tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> a, tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> b);

BeamSearch::BeamSearch(Data *data, int totalBranchesPerNode, int totalBranchesPerLevel, double alfa, double beta) : data(data)
{
    //ponteiro para passar os valores de input do BeamSearch para cada parâmetro 
    this->totalBranchesPerNode = totalBranchesPerNode;
    this->totalBranchesPerLevel = totalBranchesPerLevel;
    this->alfa = alfa;
    this->beta = beta;
}

//sobral?
BeamSearch::~BeamSearch()
{
}

void BeamSearch::runBS(Solution *solution)
{
    int removePosition = 0;

    //preenche vetor de nós a serem adicionados
    vector<unsigned int> listOfNodesRemaining;
    for (unsigned int i = 1; i <= (unsigned int)data->n; i++)
    {
        listOfNodesRemaining.push_back(i);
    }
    //idle time, lbmakespan, sequence, unscheduled, c_time

    //idle time, lbmakespan, sequence, unscheduled, c_time, inventory
    //sobral? - o que eh listOfSols, nodeListOfSols, levelListOfSols, emptSols?
    vector<tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>>> listOfSols;
    vector<tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>>> nodeListOfSols;
    vector<tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>>> levelListOfSols;
    vector<unsigned int> empty;

    vector<tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>>> emptSols;
    tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> sol({0, 0}, empty, listOfNodesRemaining, {0, data->initialInventory});
    listOfSols.push_back(sol);


    for (int i = 0; i < data->n - 1; i++)
    {
       // cout << "Nivel: " << i << endl;

        //Limpa a lista de nós por nível para nova análise
        levelListOfSols.clear();

        for (int j = 0; j < listOfSols.size(); j++)
        {
            //cout << listOfSols.size() << endl;
            //Limpa a lista de nós para uma nova análise
            nodeListOfSols.clear();

            //Obtem todas as possíveis expações a partir do nó J
            int nodeExpansions = get<2>(listOfSols[j]).size();
            int nodeListSize = -1;
            //int k = 0;
            for (int k = 0; k < nodeExpansions; k++)
            {
                    // if next modification is feasible, expand node
                    nodeListOfSols.push_back(listOfSols[j]); // CHECK EMPTY LIST
                    nodeListSize++;
                    get<3>(nodeListOfSols[nodeListSize]).second = get<3>(listOfSols[j]).second + data->jobs[get<2>(listOfSols[j])[k] - 1].inv_mod; // att inventory

                   /* if(get<3>(nodeListOfSols[nodeListSize]).second < 0 || get<3>(nodeListOfSols[nodeListSize]).second > data->maxCapacity){
                        nodeListOfSols.pop_back();
                        nodeListSize--;
                        continue;
                    }*/
                    get<0>(nodeListOfSols[nodeListSize]).first = idleTime(get<1>(listOfSols[j]), get<3>(listOfSols[j]).first,
                                                               get<2>(listOfSols[j])[k]);
                    get<3>(nodeListOfSols[nodeListSize]).first += get<0>(nodeListOfSols[nodeListSize]).first + data->jobs[get<2>(listOfSols[j])[k] - 1].p_j; // att c_time
                    get<1>(nodeListOfSols[nodeListSize]).push_back(get<2>(listOfSols[j])[k]);
                    removePosition = binarySearch(get<2>(nodeListOfSols[nodeListSize]), 0, get<2>(nodeListOfSols[nodeListSize]).size() - 1,
                                                  get<1>(nodeListOfSols[nodeListSize])[get<1>(nodeListOfSols[nodeListSize]).size() - 1]);
                    get<2>(nodeListOfSols[nodeListSize]).erase(get<2>(nodeListOfSols[nodeListSize]).begin() + removePosition);
                //}
                /*else{
                    if(!nodeListOfSols.empty())
                        nodeListOfSols.erase(nodeListOfSols.begin() + nodeListOfSols.size());
                }*/

            }

            if(nodeListOfSols.empty())
                continue;

            //Ordena pelo Waiting Time a lista de nós obtidos

            sort(nodeListOfSols.begin(), nodeListOfSols.end(), compBSLWT);

            //Escolhe os W melhores nós para expadir
            int sizeNode = min((int)nodeListOfSols.size(), totalBranchesPerNode);
            for (int k = 0; k < sizeNode; k++)
            {
                int sel = genRandomInteger(0, min((int)nodeListOfSols.size() - 1, (int)floor(alfa * nodeListOfSols.size()) + totalBranchesPerNode - k - 1));
                levelListOfSols.push_back(nodeListOfSols[sel]);
                nodeListOfSols.erase(nodeListOfSols.begin() + sel);
            }
        }

        //Caso a quantidade de nós para expansão ultrapasse o limite por nível, ordena pelo LBMakespan
        if (levelListOfSols.size() > totalBranchesPerLevel)
        {
            for (int k = 0; k < levelListOfSols.size(); k++)
            {
                get<0>(levelListOfSols[k]).second = totalInventoryMod(get<2>(levelListOfSols[k]));
            }
            sort(levelListOfSols.begin(), levelListOfSols.end(), compBSLCT);
        }

        //Limpa a lista de nós para expanção para nova análise
        listOfSols.clear();

        //Expande os N melhores nós do nível
        int sizeNextLevel = min(totalBranchesPerLevel, (int)levelListOfSols.size());
        for (int k = 0; k < sizeNextLevel; k++)
        {
            int sel = genRandomInteger(0, min((int)levelListOfSols.size() - 1, (int)floor(beta * levelListOfSols.size()) + totalBranchesPerLevel - k - 1));
            listOfSols.push_back(levelListOfSols[sel]);
            levelListOfSols.erase(levelListOfSols.begin() + sel);
        }

        //cout << "Best: " << get<0>(listOfSols[0]).second << endl << endl;
        //cout << "C_time: " << get<3>(listOfSols[0]) << endl;
    }

    //Obtem melhor solução ao final do algorítimo
    for (int j = 0; j < listOfSols.size(); j++)
    {
        get<0>(listOfSols[j]).first = idleTime(get<1>(listOfSols[j]), get<3>(listOfSols[j]).first,
                                               get<2>(listOfSols[j])[0]);
        get<3>(listOfSols[j]).first += get<0>(listOfSols[j]).first + data->jobs[get<2>(listOfSols[j])[0]-1].p_j;
        get<1>(listOfSols[j]).push_back(get<2>(listOfSols[j])[0]);
    }

    sort(listOfSols.begin(), listOfSols.end(), compMakespan);
    solution->sequence = get<1>(listOfSols[0]);
    solution->makespan = get<3>(listOfSols[0]).first;
}

int BeamSearch::binarySearch(vector<unsigned int> &vec, int posInit, int posEnd, int compare)
{
    if (posEnd >= posInit)
    {
        int midlePos = posInit + (posEnd - posInit) / 2;

        if (vec[midlePos] == compare)
            return midlePos;

        if (vec[midlePos] > compare)
            return binarySearch(vec, posInit, midlePos - 1, compare);

        return binarySearch(vec, midlePos + 1, posEnd, compare);
    }

    return -1;
}

//Funções de ordenação
bool compBSLWT(tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> a, tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> b)
{
    //Ordena pelo Waiting Time

    //Esta comparação apresenta melhoras em algumas instâncias
    //return get<0>(a).first*get<0>(a).second < get<0>(b).first*get<0>(b).second;
    //Comparação do artigo
    return get<0>(a).first < get<0>(b).first;
}

bool compBSLCT(tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> a, tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> b)
{
    //Ordena pelo LB Completion Time
    return get<0>(a).second < get<0>(b).second;
}

//Funçoes de cálculos
double BeamSearch::idleTime(vector<unsigned int> s, double cTime, unsigned int node)
{
    double idleTime;
    int lastNode = (s.size() > 0) ? s[s.size() - 1] : 0;

    if (data->jobs[node - 1].r_j <= cTime)
        idleTime = 0;
    else
        idleTime = data->jobs[node - 1].r_j - cTime;

    return idleTime;
}

double BeamSearch::totalInventoryMod(vector<unsigned int> remainingVertices)
{
    double remaining_inv_mod = 0;

    for (int i = 0; i < remainingVertices.size(); i++)
    {
        remaining_inv_mod += data->jobs[remainingVertices[i] - 1].inv_mod;
    }

    return abs(remaining_inv_mod); // return absolute inventory modification
}

bool compMakespan(tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> a, tuple<pair<double, double>, vector<unsigned int>, vector<unsigned int>, pair<double, double>> b)
{
    //Ordena pelo LB Completion Time
    return get<3>(a).first < get<3>(b).first;
}

int BeamSearch::genRandomInteger(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

/*double BeamSearch::currentCapacity(vector<unsigned int> s, double current_capacity, unsigned int job)
{
}*/