//
//  QueryTable.hpp
//
//  
//  Copyright © 2016 Serim Park . All rights reserved.
//

#ifndef QueryTable_h
#define QueryTable_h

#include "CSVTable.hpp"
#include "KdTree.hpp"
#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
template <typename T>
class QueryTable{//: public CSVTable<T>{
    
public:
    
    QueryTable(const CSVTable<T>& testTable, const KdTree<T, CSVTable<T>>& trainTree, const CSVTable<T> * trainData);
    QueryTable();
    ~QueryTable();
    
    void write2CSV(std::ofstream &fout);
    
private:
    
    deque<vector<T>> queryTable;
    int numCol;
    int numRow;
};


template <typename T>
QueryTable<T>::QueryTable(){
}

template <typename T>
QueryTable<T>::~QueryTable(){
}

template <typename T>
QueryTable<T>::QueryTable(const CSVTable<T>& testTable, const KdTree<T, CSVTable<T>>& trainTree, const CSVTable<T> * trainData){
    
    for(int i=0; i<testTable.size(); i++){
        vector<T> ind_dist;
        const vector<T> testPoint = testTable.get(i);
        trainTree.traverseTree(trainTree.getRoot(), testPoint, trainData, ind_dist);
        queryTable.push_back(ind_dist);
        
        DEBUG_MSG(cout, "Query: " + to_string_with_precision(i,0)+ returnStringVector((testPoint)));
        DEBUG_MSG(cout, "Closest to " + to_string_with_precision(ind_dist[0],2)+". Dist:" + to_string(ind_dist[1]));
    }
    numRow = static_cast<int>(queryTable.size());
    numCol = static_cast<int>(queryTable[0].size());
}


template <typename T>
void QueryTable<T>::write2CSV(std::ofstream &fout){
    for (int i=0; i<numRow; i++){
        for (int j=0; j<numCol; j++){
            fout<< queryTable[i][j];
            if(j< numCol-1) fout<<",";
        }
        fout<<std::endl;
    }
    fout.flush();
}



#endif /* QueryTable_h */
