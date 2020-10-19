#include "proj1.h"

//Constructor
Predictor::Predictor(vector<long> hAddress, vector<String> Ppath) : hAddress(hAddress), bPrediction(bPrediction){
    this->bPredition = Ppath;
    this->hAddress = hAddress;
    if(hAddress.size() == bPrediction.size()){
    }
}

//Case 1 (Always Taken)
pair<long, unsigned long> Predictor::alwaysTaken(){
    long accum = 0;
    for(int i = 0; i < bPrediction.size(); i++){
        if(bPrediction.at(i) == "T"){ accum += 1;}
    }
    return pair<long, unsigned long>(accum, bPrediction.size());
}
//Case 2 (Never Taken)
pair<long, unsigned long> Predictor::neverTaken(){
    long accum = 0;
    for(int i = 0; i < bPrediction.size(); i++){
        if(bPrediction.at(i) == "NT"){ accum += 1;}
    }
    return pair<long, unsigned long>(accum, bPrediction.size());
}
//Case 3 (Bimodal Single Bit)
pair<long, unsigned long> Predictor::ONEbit(int size){
    long accum = 0;
    vector<int> tbl(size, 1);
    for(int i = 0; i < bPrediction.size(); i++){
        int index = hAddress.at(i) % size;
        int pred = tbl.at(index);
        
        String myPred = bPrediction.at(i);

        if(pred == 1){
            if(myPred == "T"){ accum++; }
            else{
                tbl.at(index) = 0;
            }
        }
        else if(pred == 0){
            if(myPred == "NT") { accum++;}
            else{
                tbl.at(index) = 1;
            }
        }
    }
    return pair<long, unsigned long>(accum, bPrediction.size());
}
//Case 4 (Bimodal Two-Bit)
pair<long, unsigned long> Predictor::TWObit(int size){
    long accum;
    vector<int> PTable(size, 3);

    for(int i = 0; i < bPrediction.size(); i++){
        int location = hAddress.at(i) % size;
        int prediction = PTable.at(location);
        String myPred = bPrediction.at(i);

        if(prediction >= 2){
            if(myPred == "T"){
                if(prediction == 2){ PTable.at(location) = 3;}
                accum++;
            }else{ PTable.at(location) = (prediction - 1);}
        }
        else if(prediction >= 0){
            if(myPred == "NT"){
                if(prediction == 1){ PTable.at(location) = 0;}
                accum++; 
            }else{ PTable.at(location) = (prediction + 1);}
        }
    }
    return pair<long, unsigned long>(accum, bPrediction.size());
}
//Case 5 (gshare)
pair<long, unsigned long> Predictor::predictGSHARE(int hist){
    long accum;
    int TABLE_SIZE = 2048;
    vector<int> PTable(TABLE_SIZE, 3);
    int GHR = 0;

    for(int i = 0; i < bPrediction.size(); i++){
        int Tsize = hAddress.at(i) % TABLE_SIZE;
        int location = Tsize ^ GHR;
        int prediction = PTable.at(location); 
        String myPred = bPrediction.at(i);

        if(prediction >= 2){
            if(myPred == "T"){
                if(prediction == 2){ PTable.at(location) = 3;}
                accum++;
            }else{ PTable.at(location) = (prediction - 1);}
        }
        else if(prediction >= 0){
            if(myPred == "NT"){
                if(prediction == 1){ PTable.at(location) = 0;}
                accum++; 
            }else{ PTable.at(location) = (prediction + 1);}
        }
        reset(&GHR, myPred, hist);
    }
    return pair<long, unsigned long>(accum, bPrediction.size());
}
//Case 6 (Tournament Predictor)
pair<long, unsigned long> Predictor::tournament(){
    long accum = 0;
    int TABLE_SIZE = 2048;

    vector<int> tTable(TABLE_SIZE, 0);
    vector<int> gTable(TABLE_SIZE, 3);
    vector<int> bTable(TABLE_SIZE, 3);

    int GHR = 0;

    for(int i = 0; i < bPrediction.size(); i++){
        long index = hAddress.at(i) % TABLE_SIZE;
        int pred = tTable.at(index);
        bool gResult = gshare(i, &gTable, &GHR, TABLE_SIZE);
        bool bResult = bimodal(i, &bTable, TABLE_SIZE);

        if(pred >= 2){
            if(bResult){
                if(!gResult && pred == 2){
                    tTable.at(index) = 3;
                }
                accum++;
            }else{
                if(gResult){ tTable.at(index) = (pred - 1);}
            }
        }
        else if(pred >= 0){
            if(gResult){
                if(!bResult && pred == 1){
                    tTable.at(index) = 0;
                }
                accum++;
            }else{
                if(bResult){ tTable.at(index) = (pred + 1);}
            }
        }
    }
    return pair<long, unsigned long>(accum, bPrediction.size());
}



//SET UP PREDICTORS

bool Predictor::bimodal(int location, vector<int> *Tlocation, int Tsize){
    bool check;
    long index = hAddress.at(location) % Tsize;
    long pred = (*Tlocation).at(index);
    String myPred = bPrediction.at(location);

    if(pred >= 2){
        if(myPred == "T"){
            if(pred == 2){ (*Tlocation).at(index) = (pred - 1);}
            check = true;
        }else{
            (*Tlocation).at(index) = (pred - 1);
            check = false;
        }
    }
    else if(pred >= 0){
        if(myPred == "NT"){
            if(pred == 1){ (*Tlocation).at(index) = 0; }
            check = true;
        }else{
            (*Tlocation).at(index) = (pred + 1);
            check = false;
        }
    }
    return check;
}

bool Predictor::gshare(int index, vector<int> *PTable, int *hist, int Tsize){
    bool check;
    long plsEnd = hAddress.at(index) % Tsize;
    long Tindex = plsEnd ^ *hist;
    int pred = (*PTable).at(Tindex);
    String myPred = bPrediction.at(index);

    if(pred >= 2){
        if(myPred == "T"){
            if(pred == 2){ (*PTable).at(Tindex) = 3;}
            check = true;
        }else{
            (*PTable).at(Tindex) = (pred - 1);
            check = false;
        }
    }
    else if(pred >= 0){
        if(myPred == "NT"){
            if(pred == 1){ (*PTable).at(Tindex) = 0; }
            check = true;
        }else{
            (*PTable).at(Tindex) = (pred + 1);
            check = false;
        }
    }
    reset(hist, myPred, 11);
    return check;
}

void Predictor::reset(int *hist, const String myPred, int length){
    int tHist = *hist;
    int bit = (myPred == "T") ? 1 : 0;
    tHist = (int)(tHist << 1);
    tHist = (int)(tHist | bit);
    tHist = (int)(tHist & (int)(pow(2, length) - 1));
    *hist = tHist;
}
