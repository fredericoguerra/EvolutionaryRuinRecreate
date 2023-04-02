#include "RVND.h"

RVND::RVND(Data *data, Solution *sol, Infoseq *infoseq) : data(data), sol(sol), infoseq(infoseq)
{
    
}

RVND::~RVND()
{
}

void RVND::run()
{
    vector<unsigned int> neighborhood_list = {0, 1, 2, 3, 4, 5, 6};
    Solution s = *sol;

    unsigned int sel, pos;

    while (!neighborhood_list.empty())
    { // roda enquanto existirem estruturas de vizinhança na lista

        int k = rand() % neighborhood_list.size();

        switch (neighborhood_list[k])
        {
        case 0:
            swap(&s);
            break;

        case 1:
            l_blockInsertion(1, &s);
            break;

        case 2:
            l_blockInsertion(2, &s);
            break;

        case 3:
            l_blockInsertion(3, &s);
            break;

        case 4:
            l_blockInsertion(4, &s);
            break;

        case 5:
            l_blockInsertion(5, &s);
            break;

        case 6:
            l_blockInsertion(6, &s);
            break;
        }
        if (sol->makespan > s.makespan)
        { // movimento melhorou o custo
            *sol = s;
            neighborhood_list = {0, 1, 2, 3, 4, 5, 6};
        }
        else
        { // nao melhorou, exclui o movimento da lista
            s = *sol;
            neighborhood_list.erase(neighborhood_list.begin() + k);
        }
    }
}

void RVND::swap(Solution *s)
{
    //double inicioSwap = cpuTime();
    //vector<unsigned int> sol = s->sequence;
    double delta, d2;
    double menor = s->makespan;
    int pos_i = -1, pos_j = -1; // guarda as posições para realizar a operação
    for (int i = 0; i < data->n - 1; i++)
    { // exclui da operação a primeira e a ultima posição do vetor
        for (int j = i + 1; j < data->n; j++)
        {
            //cout << "i: " << i << " j: " << j << endl;
            /*sol = s->sequence;
            std::swap(sol[i], sol[j]);*/
            delta = compCostSwap(i, j);
            /*d2 = compCost(sol, data);
            if (d2 != delta && delta < 10000)
                cout << "swap i j: " << i << " " << " delta: " << delta << " d2: " << d2 << endl;*/
                
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
        //melhoras++;
        std::swap(s->sequence[pos_i], s->sequence[pos_j]);
        s->makespan = menor;
        //setinfoseq->sequencesMatrix(infoseq->sequencesMatrix,solution,n,mJobs,mSetupTimes);
        //melhorasSwap++;

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
        //infoseq->setSequencesMatrix(s->sequence);
    }
}

double RVND::compCostSwap(unsigned int i, unsigned int j)
{
    double cost;
    infoSequence newseq;

    if (i == 0)
    {
        if (j == 1)
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[j][i], infoseq->sequencesMatrix[j + 1][data->n - 1]);
        }
        else
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[j][j], infoseq->sequencesMatrix[1][j - 1]);
            newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i][i]);
            if (j != data->n - 1)
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + 1][data->n - 1]);
            //cout << "eqo" << endl;
            //cout << infoseq->sequencesMatrix[j+1][data->n - 1].firstJob << endl;
            //cout << infoseq->sequencesMatrix[j+1][data->n-1].lastJob << endl;
            //cost = newseq.initialTime + newseq.duration;
        }
    }
    else
    {
        if (i == data->n - 2)
        {
            newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[j][i]);
            //cost = newseq.initialTime + newseq.duration;
        }
        else
        {
            if (j == i + 1)
            {
                newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[j][i]);
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + 1][data->n - 1]);
                //cost = newseq.initialTime + newseq.duration;
            }
            else
            {
                newseq = infoseq->concatSequence(infoseq->sequencesMatrix[0][i - 1], infoseq->sequencesMatrix[j][j]);
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i + 1][j - 1]);
                newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[i][i]);
                if (j != data->n - 1)
                    newseq = infoseq->concatSequence(newseq, infoseq->sequencesMatrix[j + 1][data->n - 1]);
                //cost = newseq.initialTime + newseq.duration;
            }
        }
    }

    return infoseq->evaluateSolution(&newseq);
}

void RVND::l_blockInsertion(unsigned int l, Solution *s)
{ // reinsere um nó em posição diferente
    //double inicioreinsertion = cpuTime();
    double menor = s->makespan;
    //vector<unsigned int> sol = s->sequence;
    double delta, delta2;
    int pos_i = -1, pos_j = -1;
    for (unsigned int i = 0; i < data->n - l; i++)
    { // varre a solução exceto o 0 e o final
        for (unsigned int j = 0; j < data->n - l; j++)
        {
            //sol = s->sequence;
            if (i != j)
            { // exclui a posição inicial do nó

                /*vector<unsigned int> subseq(sol.begin() + i, sol.begin() + i + l);
                sol.erase(sol.begin() + i, sol.begin() + i + l);
                sol.insert(sol.begin() + j, subseq.begin(), subseq.end());*/
                delta = compCostBlockInsertion(l, i, j);
                //delta2 = compCost(sol, data);

                /*if (delta2 != delta && delta < 10000)
                    cout << "insert i j: " << i << " " << " delta: " << delta << " d2: " << delta2 << endl;*/

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
        //cout << "i: " << pos_i << " j: " << pos_j << endl;
        vector<unsigned int> subsequence(s->sequence.begin() + pos_i, s->sequence.begin() + pos_i + l);
        s->sequence.erase(s->sequence.begin() + pos_i, s->sequence.begin() + pos_i + l);
        s->sequence.insert(s->sequence.begin() + pos_j, subsequence.begin(), subsequence.end());
        s->makespan = menor;
        //setSequencesMatrix(sequencesMatrix,solucao,n,mJobs,mSetupTimes);
        //melhoras++;
        //melhorasReinsert[l-1]++;

        // ATUALIZAÇÃO DA MATRIZ DE SUBSEQUENCIAS
        if (pos_i > pos_j)
        {
            std::swap(pos_i, pos_j);
        }

        for (int i = pos_i; i < pos_j + l; i++)
        {
            infoseq->sequencesMatrix[i][i].firstJob = s->sequence[i];
            infoseq->sequencesMatrix[i][i].lastJob = s->sequence[i];
            infoseq->sequencesMatrix[i][i].duration = data->jobs[s->sequence[i] - 1].p_j;
            infoseq->sequencesMatrix[i][i].initialTime = data->jobs[s->sequence[i] - 1].r_j;
            infoseq->sequencesMatrix[i][i].qsum = data->jobs[s->sequence[i] - 1].inv_mod;
            infoseq->sequencesMatrix[i][i].qmin = min(0, data->jobs[s->sequence[i] - 1].inv_mod);
            infoseq->sequencesMatrix[i][i].qmax = max(0, data->jobs[s->sequence[i] - 1].inv_mod);
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
        //infoseq->setSequencesMatrix(s->sequence); // erro na att das subseq
    }
    //double fimReinsertion =  cpuTime();
    //tempo_reinsertion += (fimReinsertion - inicioreinsertion);
}

double RVND::compCostBlockInsertion(unsigned int l, unsigned int i, unsigned int j)
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