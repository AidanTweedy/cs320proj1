#include "proj1.h"

//Driver Code to perform all tests

void start(Predictor path, String file_output){
    vector<int> TABLE_SIZE = {16,32,128,256,512,1024,2048};
    pair<long, unsigned long> myPred;

    ofstream file(file_output);

    //1 + 2

    myPred = path.alwaysTaken();
    file << myPred.first << "," << myPred.second << ";\n";
    myPred = path.neverTaken();
    file << myPred.first << "," <<  myPred.second << ";\n";

    //3

    for(int i = 0; i < TABLE_SIZE.size(); i++){
        myPred = path.ONEbit(TABLE_SIZE.at(i));
        file << myPred.first << "," << myPred.second;
        file << ((i == TABLE_SIZE.size() - 1) ? ";" : "; ");
    }
    file << "\n";

    //4

    for(int i = 0; i < TABLE_SIZE.size(); i++){
        myPred = path.TWObit(TABLE_SIZE.at(i));
        file << myPred.first << "," << myPred.second;
        file << ((i == TABLE_SIZE.size() - 1) ? ";" : "; ");
    }
    file << "\n";

    //5

    for(int i = 3; i <= 11; i++){
        myPred = path.predictGSHARE(i);
        file << myPred.first << "," << myPred.second;
        file << ((i == 11) ? ";" : "; ");
    }
    file << "\n";

    //6

    myPred = path.tournament();
    file << myPred.first << "," << myPred.second << ";\n";
    file.close();
}

//Driver Code

int main(int argc, char *arr[], char *envp[]){
    vector<long> input;
    vector<String> pred;

    ifstream file(arr[1]);
    String curr_line;

    if(file.is_open()){
        while(getline(file, curr_line)){
            String curr_addy = curr_line.substr(0, curr_line.find(" "));
            String curr_pred = curr_line.substr(curr_line.find(" ") + 1, curr_addy.length());
            unsigned long curr_hAddy = stoul(curr_addy, nullptr, 16);


            input.push_back(curr_hAddy);
            pred.push_back(curr_pred);

 
        }

        file.close();
    }
    Predictor testPred(input, pred);
    start(testPred, arr[2]);

    return 0;  
}