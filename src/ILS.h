#ifndef ILS_H
#define ILS_H

#include "BeamSearch.h"
#include "Infoseq.h"
#include "RVND.h"
#include <limits>

using namespace std;

//define os parâmetros do ils
struct Params
{
    int i_max;
    int i_ils;
    int nodesPerLevel;
    int branchesPerNode;
    double alfa, beta;
};

class ILS
{

public:
    Data *data;
    Params parameters;
    //RVND *rvnd;

    ILS(Data *data, Params parameters);
    Solution runILS();
    Solution perturb(Solution *solution, Infoseq *infoseq);
    ~ILS();
};

ILS::ILS(Data *data, Params parameters):data(data), parameters(parameters)
{
}

ILS::~ILS()
{
}

Solution ILS::runILS(){
    //double ff = numeric_limits<double>::infinity(); // custo final
    //cria as solucoes s, s', s*
    Solution s, s1, sf;                             // s, s', s*
    //inicia o custo makespan como infinito
    sf.makespan = numeric_limits<double>::infinity();
    //executa o BeamSearch para criar uma solução inicial
    BeamSearch bs(data, parameters.branchesPerNode, parameters.nodesPerLevel, parameters.alfa, parameters.beta);
    
    Infoseq infoseq(data);
    //cout << parameters.i_max << endl;
    //RVND aaa(s, infoseq);
    for (int i = 0; i < parameters.i_max; i++)
    {
        //cout << "ITER_ILS: " << i << endl;
        //double alfa = (double)rand() / RAND_MAX; // gera aleatorio entre 0 e 1
        //s = construction(n, mJobs, mSetupTimes, alfa, fs); // constroi solucao inicial
        bs.runBS(&s);
        //cout << s.makespan << endl;
        //cout << s.sequence.size() << endl;
        infoseq.setSequencesMatrix(s.sequence);
        s.makespan = infoseq.evaluateSolution(&infoseq.sequencesMatrix[0][data->n-1]);
        s1 = s;
        int iter_ILS = 0;
        while (iter_ILS < parameters.i_ils)
        {
            //cout << "in_rvnd" << endl;
            RVND rvnd(data, &s, &infoseq);
            rvnd.run();
            rvnd.~RVND();
            //cout << "out_rvnd" << endl;
            if (s.makespan < s1.makespan)
            {
                s1 = s;
                iter_ILS = 0;
            }
            s = perturb(&s1, &infoseq); // perturba a solução evitando resultado ótimo local
            iter_ILS++;
        }
        if (s1.makespan < sf.makespan)
        {
            sf = s1;
            //cout << "\n" << ff << " " << " na " << i+1 <<  " iter " << melhorasptb << " melhoras" << " Oropt-" << melhorasReinsert[0] << " Oropt2-" << melhorasReinsert[1] << " Oropt3-" << melhorasReinsert[2] << " Oropt4-" << melhorasReinsert[3] << " Oropt5-" << melhorasReinsert[4] << " Oropt6-" << melhorasReinsert[5] << " Oropt7-" << melhorasReinsert[6] << " Oropt8-" << melhorasReinsert[7] << " Oropt9-" << melhorasReinsert[8] << " Oropt10-" << melhorasReinsert[9]<< " Oropt11-" << melhorasReinsert[10]<< " Oropt12-" << melhorasReinsert[11]<< " Oropt13-" << melhorasReinsert[12]<< " 2opt-" << melhoras2opt << " troca-" << melhorasSwap << endl;
        }
        /*if(ff < fconst)
            cout << "melhorou" << endl;*/
    }
    return sf; // retorna a melhor solucao
}


Solution ILS::perturb(Solution *solution, Infoseq *infoseq)
{
    vector<unsigned int> s = solution->sequence;
    int tam_max;

    if(data->n > 10)
        tam_max = ceil(((double)data->n) / 10);
    else
        tam_max = 2;
    //int tam_max = 15; // tamanho maximo das subsequencias
    int inicio1, fim1, inicio2, fim2;

    inicio1 = (rand() % ((data->n - (2 * tam_max)) - 1 + 1));                                // posicao minima = 0, posicao maxima = final - 2*tmax
    fim1 = (rand() % ((inicio1 + (tam_max - 1)) - (inicio1 + 1) + 1)) + (inicio1 + 1); // minima = inicio+1, maxima = inicio1 + tmax - 1;
    inicio2 = (rand() % ((data->n - tam_max) - (fim1 + 1) + 1) + (fim1 + 1));                // minima = fim1 + 1, maxima = final - tmax;
    fim2 = (rand() % ((inicio2 + (tam_max - 1)) - (inicio2 + 1) + 1) + (inicio2 + 1)); // minima = inicio2 + 1, maxima = inicio2 + tmax - 1;

    int d1 = fim1 - inicio1; // tamanho da primeira subsequencia, usado para corrigir erros na inserção
    int d2 = fim2 - inicio2; // tamanho da segunda subsequencia, usado pra corrigir erros na inserção

    //cout << "inicio1: " << inicio1 <<  " fim2: " << fim2 << endl;

    s.erase(s.begin() + inicio2, s.begin() + fim2 + 1);                                      // apaga primeira subsequencia
    s.erase(s.begin() + inicio1, s.begin() + fim1 + 1);                                      // apaga segunda subsequencia
    s.insert(s.begin() + inicio1, &solution->sequence[inicio2], &solution->sequence[fim2] + 1);                    // inclui a segunda subsequencia na posicao da primeira
    s.insert(s.begin() + inicio2 + (-1 * (d1 - d2)), &solution->sequence[inicio1], &solution->sequence[fim1] + 1); // inclui a segunda subsequencia na posicao da segunda

    //compCompletionTime(s,compTimes);
    infoseq->setSequencesMatrix(s);

    Solution s1;
    s1.makespan = infoseq->evaluateSolution(&infoseq->sequencesMatrix[0][data->n-1]);
    s1.sequence = s;

    return s1;

}

#endif