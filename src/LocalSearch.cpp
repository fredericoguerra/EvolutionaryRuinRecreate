#include "LocalSearch.h"

LocalSearch::LocalSearch()
{
}

LocalSearch::LocalSearch(Data *data) : data(data)
{
    tempIndiv = new Individual(data);
    //tempIndiv->copyIndividual(tempIndiv, individual); // that's ittttt 
    infoseq = new Infoseq(data);

    //rvnd();
}

LocalSearch::~LocalSearch()
{
    delete infoseq;
    delete tempIndiv;
}

void LocalSearch::swap(vector<unsigned int> &solution, double &cost)
{
    // double inicioSwap = cpuTime();
    vector<unsigned int> sol = solution;
    double delta, d2;
    double menor = cost;
    int pos_i = -1, pos_j = -1; // guarda as posições para realizar a operação
    for (int i = 0; i < data->n - 1; i++)
    { // exclui da operação a primeira e a ultima posição do vetor
        for (int j = i + 1; j < data->n; j++)
        {
            // cout << "i: " << i << " j: " << j << endl;
            //sol = solution;
            //std::swap(sol[i], sol[j]);
            delta = compCostSwap(i, j);
            /*d2 = compCost(sol);
            if (d2 != delta && delta < 10000)
                cout << "swap i j: " << i << " " << j <<" delta: " << delta << " d2: " << d2 << endl;*/
            if (delta < menor)
            {
                menor = delta;
                pos_i = i;
                pos_j = j;
            }
        }
    }

    if (pos_i >= 0)
    { // realiza a operação
        // melhoras++;
        std::swap(solution[pos_i], solution[pos_j]);
        cost = menor;
        // setinfoseq->sequencesMatrix(infoseq->sequencesMatrix,solution,n,mJobs,mSetupTimes);
        // melhorasSwap++;

        std::swap(infoseq->sequencesMatrix[pos_i][pos_i], infoseq->sequencesMatrix[pos_j][pos_j]);

        for (int i = 0; i < pos_i; i++)
        {
            for (int j = pos_i; j < data->n; j++)
            {
                infoseq->sequencesMatrix[i][j] = infoseq->concatSequence(infoseq->sequencesMatrix[i][j - 1], infoseq->sequencesMatrix[j][j]);
                infoseq->sequencesMatrix[j][i] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][i]);
            }
        }

        for (int j = pos_i + 1; j < data->n; j++)
        {
            infoseq->sequencesMatrix[pos_i][j] = infoseq->concatSequence(infoseq->sequencesMatrix[pos_i][j - 1], infoseq->sequencesMatrix[j][j]);
            infoseq->sequencesMatrix[j][pos_i] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][pos_i]);
        }

        for (int i = pos_i + 1; i < pos_j; i++)
        {
            for (int j = pos_j; j < data->n; j++)
            {
                infoseq->sequencesMatrix[i][j] = infoseq->concatSequence(infoseq->sequencesMatrix[i][j - 1], infoseq->sequencesMatrix[j][j]);
                infoseq->sequencesMatrix[j][i] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][i]);
            }
        }

        for (int j = pos_j + 1; j < data->n; j++)
        {
            infoseq->sequencesMatrix[pos_j][j] = infoseq->concatSequence(infoseq->sequencesMatrix[pos_j][j - 1], infoseq->sequencesMatrix[j][j]);
            infoseq->sequencesMatrix[j][pos_j] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][pos_j]);
        }
        // infoseq->setSequencesMatrix(solution);
    }
}

double LocalSearch::compCostSwap(int i, int j)
{
    double cost;
    infoSequence newseq;

    if (i == 0)
    {
        if (j == 1)
        {  
            //cout << infoseq->sequencesMatrix[j][i].firstJob << ">>" << infoseq->sequencesMatrix[j][i].lastJob << endl;
            //cout << infoseq->sequencesMatrix[j + 1][data->n - 1].firstJob << ">>" << infoseq->sequencesMatrix[j + 1][data->n - 1].lastJob << endl;
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[i][j], infoseq->sequencesMatrix[j + 1][data->n - 1]);
        }
        else
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[1][j - 1]);
            newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i][i]);
            if (j != data->n - 1)
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + 1][data->n - 1]);
            // cout << "eqo" << endl;
            // cout << infoseq->sequencesMatrix[j+1][data->n - 1].firstJob << endl;
            // cout << infoseq->sequencesMatrix[j+1][data->n-1].lastJob << endl;
            // cost = newseq.initialTime + newseq.duration;
        }
    }
    else
    {
        if (i == data->n - 2)
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[j][i]);
            // cost = newseq.initialTime + newseq.duration;
        }
        else
        {
            if (j == i + 1)
            {
                newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[j][i]);
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + 1][data->n - 1]);
                // cost = newseq.initialTime + newseq.duration;
            }
            else
            {
                newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[j][j]);
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i + 1][j - 1]);
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i][i]);
                if (j != data->n - 1)
                    newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + 1][data->n - 1]);
                // cost = newseq.initialTime + newseq.duration;
            }
        }
    }

    return infoseq->evaluateSolution(&newseq);
}

void LocalSearch::l_InsertBlock(int l, vector<unsigned int> &solution, double &cost)
{ // reinsere um nó em posição diferente
    // double inicioreinsertion = cpuTime();
    double menor = cost;
     vector<unsigned int> sol = solution;
    double delta, delta2;
    int pos_i = -1, pos_j = -1;
    for (unsigned int i = 0; i < data->n - l; i++)
    { // varre a solução exceto o 0 e o final
        for (unsigned int j = 0; j < data->n - l; j++)
        {
             sol = solution;
            if (i != j)
            { // exclui a posição inicial do nó

                /*vector<unsigned int> subseq(sol.begin() + i, sol.begin() + i + l);
                sol.erase(sol.begin() + i, sol.begin() + i + l);
                sol.insert(sol.begin() + j, subseq.begin(), subseq.end());*/
                delta = compCostBlockInsertion(l, i, j);
                //delta2 = compCost(sol);

                /*if (delta2 != delta && delta < 1000)
                    cout << "insert i j: " << i << " " << j << " delta: " << delta << " d2: " << delta2 << endl;*/

                if (delta < menor)
                {
                    menor = delta;
                    pos_i = i;
                    pos_j = j;
                }
            }
        }
    }
    if (pos_i != -1)
    {
        // cout << "i: " << pos_i << " j: " << pos_j << endl;
        vector<unsigned int> subsequence(solution.begin() + pos_i, solution.begin() + pos_i + l);
        solution.erase(solution.begin() + pos_i, solution.begin() + pos_i + l);
        solution.insert(solution.begin() + pos_j, subsequence.begin(), subsequence.end());
        cost = menor;
        // setSequencesMatrix(sequencesMatrix,solucao,n,mJobs,mSetupTimes);
        // melhoras++;
        // melhorasReinsert[l-1]++;

        // ATUALIZAÇÃO DA MATRIZ DE SUBSEQUENCIAS
        if (pos_i > pos_j)
        {
            std::swap(pos_i, pos_j);
        }

        for (int i = pos_i; i < pos_j + l; i++)
        {
            infoseq->sequencesMatrix[i][i].firstJob = solution[i];
            infoseq->sequencesMatrix[i][i].lastJob = solution[i];
            infoseq->sequencesMatrix[i][i].duration = data->jobs[solution[i] - 1].p_j;
            infoseq->sequencesMatrix[i][i].initialTime = data->jobs[solution[i] - 1].r_j;
            infoseq->sequencesMatrix[i][i].qsum = data->jobs[solution[i] - 1].inv_mod;
            infoseq->sequencesMatrix[i][i].qmin = min(0, data->jobs[solution[i] - 1].inv_mod);
            infoseq->sequencesMatrix[i][i].qmax = max(0, data->jobs[solution[i] - 1].inv_mod);
            infoseq->sequencesMatrix[i][i].lmin = -infoseq->sequencesMatrix[i][i].qmin;
            infoseq->sequencesMatrix[i][i].lmax = data->maxCapacity - infoseq->sequencesMatrix[i][i].qmax;
        }

        for (int i = 0; i < pos_i; i++)
        {
            for (int j = pos_i; j < data->n; j++)
            {
                infoseq->sequencesMatrix[i][j] = infoseq->concatSequence(infoseq->sequencesMatrix[i][j - 1], infoseq->sequencesMatrix[j][j]);
                infoseq->sequencesMatrix[j][i] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][i]);
            }
        }
        for (int i = pos_i; i < pos_j; i++)
        {
            for (int j = i + 1; j < data->n; j++)
            {
                infoseq->sequencesMatrix[i][j] = infoseq->concatSequence(infoseq->sequencesMatrix[i][j - 1], infoseq->sequencesMatrix[j][j]);
                infoseq->sequencesMatrix[j][i] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][i]);
            }
        }
        for (int i = pos_j; i < pos_j + l; i++)
        {
            for (int j = i + 1; j < data->n; j++)
            {
                infoseq->sequencesMatrix[i][j] = infoseq->concatSequence(infoseq->sequencesMatrix[i][j - 1], infoseq->sequencesMatrix[j][j]);
                infoseq->sequencesMatrix[j][i] = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[j - 1][i]);
            }
        }
        // infoseq->setSequencesMatrix(solution); // erro na att das subseq
    }
    // double fimReinsertion =  cpuTime();
    // tempo_reinsertion += (fimReinsertion - inicioreinsertion);
}

double LocalSearch::compCostBlockInsertion(int l, int i, int j)
{ // concatenação de subsequencias para avaliação do l-blockinsertion
    infoSequence newseq;

    if (i < j)
    {
        if (i == 0)
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[i + l][j + l - 1], infoseq->sequencesMatrix[i][i + l - 1]);
            if (j != data->n - l)
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + l][data->n - 1]);
        }
        else
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[i + l][j + l - 1]);
            newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i][i + l - 1]);
            if (j != data->n - l)
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + l][data->n - 1]);
        }
    }
    else
    {
        if (j == 0)
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[i][i + l - 1], infoseq->sequencesMatrix[0][i - 1]);
            if (i != data->n - l)
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i + l][data->n - 1]);
        }
        else
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][j - 1], infoseq->sequencesMatrix[i][i - 1 + l]);
            newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j][i - 1]);
            if (i != data->n - l)
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i + l][data->n - 1]);
        }
    }

    return infoseq->evaluateSolution(&newseq);
}

void LocalSearch::rvnd(Individual *individual)
{
      //cout << "recebido: " << endl;*/
    //cout << endl;
    tempIndiv->copyIndividual(tempIndiv, individual);
    infoseq->setSequencesMatrix(individual->chromosome); // build the subsequence structures to perform local search
    //vector<unsigned int> nLista = {0, 1, 2, 3, 4, 5, 6};
    vector<unsigned int> nLista = {0};
    int swap_c = 0, ib1 = 0, ib2 = 0, ib3 = 0, ib4 = 0, ib5 = 0, ib6 = 0;
    int sel, pos;

    while (!nLista.empty())
    { // roda enquanto existirem estruturas de vizinhança na lista
        
        int k = rand() % nLista.size();
    
        switch (nLista[k])
        {
        case 0:
            swap_c += 1;
            swap(tempIndiv->chromosome, tempIndiv->solutionCost);
            break;

        case 1:
            ib1 += 1;
            l_InsertBlock(1, tempIndiv->chromosome, tempIndiv->solutionCost);
            break;

        case 2:
            ib2 += 1;
            l_InsertBlock(2, tempIndiv->chromosome, tempIndiv->solutionCost);
            break;

        case 3:
            ib3 += 1;
            l_InsertBlock(3, tempIndiv->chromosome, tempIndiv->solutionCost);
            break;

        case 4:
            ib4 += 1;
            l_InsertBlock(4, tempIndiv->chromosome, tempIndiv->solutionCost);
            break;

        case 5:
            ib5 += 1;
            l_InsertBlock(5, tempIndiv->chromosome, tempIndiv->solutionCost);
            break;

        case 6:
            ib6 += 1;
            l_InsertBlock(6, tempIndiv->chromosome, tempIndiv->solutionCost);
            break;
        }

        if (individual->solutionCost > tempIndiv->solutionCost)
        { // movimento melhorou o custo
            //cout << "improved " << individual->solutionCost << " to "  << tempIndiv->solutionCost << endl;
            individual->solutionCost = tempIndiv->solutionCost;
            individual->chromosome = tempIndiv->chromosome;
            nLista = {0, 1, 2, 3, 4, 5, 6};
        }
        else
        { // nao melhorou, exclui o movimento da lista
            tempIndiv->chromosome = individual->chromosome;
            tempIndiv->solutionCost = individual->solutionCost;
            nLista.erase(nLista.begin() + k);
        }
    }
}

double LocalSearch::compCost(vector<unsigned int> s){
   int s_size = s.size();
   double c_time = 0;
   double inventory = data->initialInventory;
   bool penalize = false;

   for(int i = 0; i < s_size; i++){
      if(c_time >= data->jobs[s[i]-1].r_j)
         c_time += data->jobs[s[i]-1].p_j;
      else
         c_time = data->jobs[s[i]-1].r_j + data->jobs[s[i]-1].p_j;

      inventory += data->jobs[s[i]-1].inv_mod;
      //cout << inventory << " ";
      if(inventory < 0 || inventory > data->maxCapacity)
         penalize = true;
   }
   //cout << endl;
   c_time = (penalize) ? c_time * 10000 : c_time;

   return c_time;
}