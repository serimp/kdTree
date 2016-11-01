//  This is the main function for build_kdtree
//
//  This function gets two arguments from the console:
//      (1) The absolute path to the train data (.csv)
//      (2) The absolute path to save the model (.csv)
//
//  Alternatively, by typing '1' at the prompt, the sample_data.csv can be loaded to train the model.
//
//  Copyright © 2016 Serim. All rights reserved.
//
//
#include "KdTree.hpp"
#include "KdNode.hpp"
#include "CSVTable.hpp"
#include "QueryTable.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;
using std::cin;

int main(int argc, const char * argv[]) {
    
    const char* fileName;
    const char* modelFileName;
    int input;
    float bound;
    int rule;
    
    if (argc < 3){
        
        cout<< "---------------- Arguments are missing  --------------------" <<endl;
        cout<< "Please provide 2 arguments: "<<endl;
        cout<< "(1) The absolute path to the train data" <<endl;
        cout<< "(2) The absolute path to save the model." <<endl;
        cout<< "---- If you wish to use the sample_data.csv, press 1. ------"<<endl;
        
        cin >> input;
        if(input ==1){
            cout<<"------------------------------------------------------------"<<endl;
            cout<< "The default data will be used. "<<endl;
            cout <<"The train data is loaded from ../../examples/sample_data.csv" << endl;
            cout<< "The model will be saved at ../../examples/precomputed_model.csv"<<endl;
            fileName = "../../examples/sample_data.csv";
            modelFileName = "../../examples/model.csv";
         
        }
        else{
            return 1;
        }
    }
    else if(argc==3){
        fileName = argv[1];
        modelFileName = argv[2];
        cout<<"------------------------------------------------------------"<<endl;
        cout<< "The train data is loaded from: " << fileName << endl;
        cout<< "The model will be saved at: "<< modelFileName << endl;
    }
    
    
    // Load Train data
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"... Loading the train data ..."<<endl;
    CSVTable <float> trainTable(fileName);
    
    // Build KdTree
    cout<<"------------------------------------------------------------"<<endl;
    
    
    cout<<"The bound for the least distance between the query point and the splitting hyperplane"<<endl;
    cout<<"is currenlty set to 0.1. Do you wish to change? (Yes: press 1. No: press any other keys.)"<<endl;
    cin>>input;
    if(input==1){
        cout<<"Enter the new bound:";
        cin>>bound;
    }
    else
        bound = 0.1;
    cout<<"------------------------------------------------------------"<<endl;
    
    cout<<"Choose the rule to determine the splitting axis." <<endl;
    cout<<"0: Maximizing Standard Deviation. "<< endl;
    cout<<"1: Minimizing Skew."<<endl;
    cout<<"2: Minimizing Kurtosis."<<endl;
    
    cin>>rule;
    if(rule ==1 | rule ==2)
        cout<< rule << " is selected. "<<endl;
    else
        cout << "0 is selected."<<endl;
    
    
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"... Building K-d Tree ..." <<endl;
    KdTree <float, CSVTable<float>> trainTree(&trainTable, bound, rule);
    cout<<"... Finished building K-d Tree ..."<<endl;
    cout<<"... To print the tree, press 1. Otherwise, press any keys ..."<<endl;
    cin >> input;
    if(input ==1) trainTree.printTree(trainTree.getRoot(), &trainTable, 0);
        
    // Store KdTree
    cout<<"------------------------------------------------------------"<<endl;
    cout << "... Saving the K-d Tree ..."<< endl;
    std::ofstream fout;
    fout.open(modelFileName, std::fstream::out |  std::fstream::binary);
    fout.close();
    fout.open(modelFileName, std::fstream::out | std::fstream::app | std::fstream::binary);
    if (fout.is_open()){
        trainTree.write2CSV(trainTree.getRoot(), fout);
    }
    else{
        throw std::runtime_error("Couldn't open CSV file to write.");
    }
    fout.close();
    
    cout << "... Done ... " << endl;
    
    
    return 0;
}
