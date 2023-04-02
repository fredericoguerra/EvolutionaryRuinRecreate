#include "Infoseq.h"

Infoseq::Infoseq(Data *data) : data(data)
{
    //this->data = data;

    sequencesMatrix = new infoSequence *[data->n + 1];
    for (int i = 0; i <= data->n; ++i)
    {
        sequencesMatrix[i] = new infoSequence[data->n + 1];
    }
}

Infoseq::Infoseq(){
    
}

Infoseq::~Infoseq()
{
    for (int i = 0; i <= data->n; ++i)
        delete[] sequencesMatrix[i];
    delete[] sequencesMatrix;
}

infoSequence Infoseq::concatSequence(infoSequence seq1, infoSequence seq2)
{
    infoSequence res_seq; // resulting sequence
    double idleTime = 0;

    /*if (seq1.initialTime + seq1.duration < data->jobs[seq2.firstJob - 1].r_j)
        idleTime = data->jobs[seq2.firstJob - 1].r_j - (seq1.initialTime + seq1.duration);*/

    if(seq1.initialTime + seq1.duration >= seq2.initialTime)
        idleTime = 0;
    else{
        idleTime = 0;
        if(seq1.initialTime + seq1.duration < seq2.initialTime)
            idleTime += seq2.initialTime - (seq1.initialTime + seq1.duration);
    }

    res_seq.firstJob = seq1.firstJob;
    res_seq.lastJob = seq2.lastJob;
    res_seq.duration = seq1.duration + seq2.duration;
    res_seq.initialTime = seq1.initialTime + idleTime;
    res_seq.qsum = seq1.qsum + seq2.qsum;
    res_seq.qmin = min(seq1.qmin, seq1.qsum + seq2.qmin);
    res_seq.qmax = max(seq1.qmax, seq1.qsum + seq2.qmax);
    res_seq.lmin = -res_seq.qmin;
    res_seq.lmax = data->maxCapacity - res_seq.qmax;

    return res_seq;
}


double Infoseq::evaluateSolution(infoSequence *seq){
    infoSequence res_seq;
    res_seq.firstJob = seq->firstJob;
    res_seq.lastJob = seq->lastJob;
    res_seq.initialTime = seq->initialTime;
    res_seq.duration = seq->duration;
    res_seq.qsum = seq->qsum + data->initialInventory;
    res_seq.qmin = min(0.0, data->initialInventory + seq->qmin);
    res_seq.qmax = max(max(0.0, data->initialInventory+0.0), data->initialInventory + seq->qmax);
    res_seq.lmin = -res_seq.qmin;
    res_seq.lmax = data->maxCapacity - res_seq.qmax;

    //infoSequence res = concatSequence(res_seq, sequencesMatrix[0][data->n-1]);
    //infoSequence res = sequencesMatrix[0][data->n-1];

    double cost = res_seq.initialTime + res_seq.duration + 10000 *(max(0.0, -res_seq.qmin) + max(0.0, res_seq.qmax - data->maxCapacity));
    //cout <<"t1: " << max(0.0, -res_seq.qmin) << endl;
    //cout <<"t2: " << max(0.0, res_seq.qmax - data->maxCapacity) << endl;
    //cout <<"qmax: " << res_seq.qmax << endl;
    //cout <<"qmin: " << res_seq.qmin << endl;

    return cost;
}

void Infoseq::setSequencesMatrix(vector<unsigned int> s)
{
    for (unsigned int i = 0; i < data->n; i++)
    {
        sequencesMatrix[i][i].firstJob = s[i];
        sequencesMatrix[i][i].lastJob = s[i];
        sequencesMatrix[i][i].initialTime = data->jobs[s[i] - 1].r_j;
        sequencesMatrix[i][i].duration = data->jobs[s[i] - 1].p_j;
        sequencesMatrix[i][i].qsum = data->jobs[s[i] - 1].inv_mod;
        sequencesMatrix[i][i].qmin = min(0, data->jobs[s[i] - 1].inv_mod);
        sequencesMatrix[i][i].qmax = max(0, data->jobs[s[i] - 1].inv_mod);
        sequencesMatrix[i][i].lmin = -sequencesMatrix[i][i].qmin;
        sequencesMatrix[i][i].lmax = data->maxCapacity - sequencesMatrix[i][i].qmax;
    }

    for (int i = 0; i < data->n; i++)
    { // original path
        for (int j = i + 1; j < data->n; j++)
        {
            sequencesMatrix[i][j] = concatSequence(sequencesMatrix[i][j - 1], sequencesMatrix[j][j]);
        }
    }

    for (int i = data->n - 1; i >= 1; i--)
    { // inverted subsequences
        for (int j = i - 1; j >= 0; j--)
        {
            sequencesMatrix[i][j] = concatSequence(sequencesMatrix[i][j + 1], sequencesMatrix[j][j]);
        }
    }
}