#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

#define String std::string

using namespace std;

class Predictor{
    private:

        vector<String> bPrediction;
        vector<long> hAddress;

        bool gshare(int index, vector<int> *PTable, int *hist, int Tsize);
        bool bimodal(int location, vector<int> *Tlocation, int Tsize);

        void reset(int *hist, const String myPred, int length);
    
    public:

        void start(Predictor path, String file_output);

        Predictor(vector<long> hAddress, vector<String> Ppath);

        //CASES

        pair<long, unsigned long> alwaysTaken();
        pair<long, unsigned long> neverTaken();
        pair<long, unsigned long> ONEbit(int size);
        pair<long, unsigned long> TWObit(int size);
        pair<long, unsigned long> predictGSHARE(int hist);
        pair<long, unsigned long> tournament();
};

#endif