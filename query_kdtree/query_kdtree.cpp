//  This is the main function for build_kdtree
//
//  This function gets two arguments from the console:
//      (1) The absolute path to the train data (.csv)
//      (2) The absolute path to the kdtree model (.csv)
//      (3) The absolute path to the test data (.csv)
//      (4) The absolute path to the knn search result to be saved (.csv)
//
//  Alternatively, by typing '1' at the prompt,
//  sample_data.csv, precomputed_model.csv, query_data.csv can be loaded and saved as query_result.csv
//
//  Copyright © 2016 Serim. All rights reserved.
//
//

#include "KdTree.hpp"
#include "KdNode.hpp"
#include "CSVTable.hpp"
#include "QueryTable.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>

using std::vector;
using std::cout;
using std::endl;
using std::cin;


int main(int argc, const char * argv[]) {
    
    const char* fileName;
    const char* modelFileName;
    const char* testFileName;
    const char* queryResultFileName;
    
    if (argc < 5){
        
        cout<< "---------------- Arguments are missing  --------------------" <<endl;
        cout<< "Please provide 4 arguments: "<<endl;
        cout<< "(1) The absolute path to the train data." <<endl;
        cout<< "(2) The absolute path of the kdtree model." <<endl;
        cout<< "(3) The absolute path of the query data."<<endl;
        cout<< "(4) The absolute path to save the query result."<<endl;
        cout<< "---------- To test sample_data.csv, press 1 ----------"<<endl;
        
        int input;
        cin >> input;
        if(input ==1){
            cout<<"------------------------------------------------------------"<<endl;
            cout<< "The default data will be used. "<<endl;
            cout <<"The train data is loaded from: ../../examples/sample_data.csv" << endl;
            cout<< "The kdtree model is loaded from: ../../examples/precomputed_model.csv"<<endl;
            cout<< "The query data is loaded from: ../../examples/query_data.csv"<<endl;
            cout<< "The query result will be saved at: ../../examples/query_result.csv"<<endl;
            fileName = "../../examples/sample_data.csv";
            modelFileName = "../../examples/precomputed_model.csv";
            testFileName = "../../examples/query_data.csv";
            queryResultFileName = "../../examples/query_result.csv";
        }
        else{
            return 1;
        }
    }
    
    else if(argc == 5){
        fileName = argv[1];
        modelFileName = argv[2];
        testFileName = argv[3];
        queryResultFileName = argv[4];
        cout<<"------------------------------------------------------------"<<endl;
        cout<< "The train data is loaded from: " << fileName << endl;
        cout<< "The kdtree model is loaded from: "<< modelFileName << endl;
        cout<< "The query data is loaded from: " << testFileName <<endl;
        cout<< "The query result will be saved at:" <<queryResultFileName<<endl;
    }
    
    // Load Train and Test data
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"... Loading the train data ..."<< endl;
    CSVTable <float> trainTable(fileName);
    cout<<"... Loading the test data ..."<< endl;
    CSVTable <float> testTable(testFileName);
    
    // Load The Tree
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"... Loading the tree ..."<< endl;
    KdTree <float, CSVTable<float>> newTree;
    std::ifstream fin;
    fin.open(modelFileName, std::fstream::in | std::fstream::binary);
    if (fin.is_open()){
        newTree.loadCSV(newTree.getRoot(), fin);
    }
    else{
        throw std::runtime_error("Couldn't open CSV file to load.");
    }
    fin.close();
    
    
    // Knnsearch
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"... Querying for the closest points ...."<<endl;
    QueryTable <float> queryTable(testTable, newTree, &trainTable);
    
    // Saving the result
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"... Saving the query results ..."<<endl;
    std::ofstream fout;

    fout.open(queryResultFileName, std::fstream::out |  std::fstream::binary);
    fout.close();
    fout.open(queryResultFileName, std::fstream::out | std::fstream::app | std::fstream::binary);
    if (fout.is_open()){
        queryTable.write2CSV(fout);
    }
    else{
        throw std::runtime_error("Couldn't open CSV file to write.");
    }
    fout.close();
    
    cout <<"... Done. ... "<<endl;
    return 0;
}
