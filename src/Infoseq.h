#ifndef _INFOSEQ_H_
#define _INFOSEQ_H_

#include <vector>
#include "Data.h"

using namespace std;

struct infoSequence
{
    double duration = 0;       //minimum duration
    double initialTime = 0;    // earliest time
    double setupTime =0;       // setup time 
    double qsum = 0;           // sum of modifications
    double qmin = 0;           // minimum modification
    double qmax = 0;           // max modification
    double lmin = 0;           // min modification allowed to ensure feasibility
    double lmax = 0;           // max modification allowed to ensure feasibility
    unsigned int firstJob = 0; // first job of the subsequence
    unsigned int lastJob = 0;  // last job of the subsequence
};

class Infoseq
{
    public:
        Data *data;
        infoSequence **sequencesMatrix;
        Infoseq(Data *data);
        Infoseq();
        void setSequencesMatrix(vector<unsigned int> s);
        infoSequence concatSequence(infoSequence seq1, infoSequence seq2);
        double evaluateSolution(infoSequence *seq);
        ~Infoseq();
};



#endif