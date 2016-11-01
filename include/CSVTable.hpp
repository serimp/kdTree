//
//  CSVTable.hpp
//
//  CSVTable class loads data from .csv file and stores into a CSVTable.
//  Each row in CSVTable corresponds to a data point.
//  e.g. CSVTable of size M x K, has M data points belonging to K-dim space.

//  Train data and test data are initalized as CSVTable objects.
//
//
//  Copyright © 2016 Serim Park . All rights reserved.


#ifndef CSVTable_hpp
#define CSVTable_hpp

#include <stdio.h>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

using std::vector;
using std::deque;


template <typename T>
class CSVTable{
    
public:
    
    CSVTable(); // constructor
    CSVTable(const std::string & fileName); // loads data directly from constructor
    ~CSVTable(); // destructor
    
    void loadCSV(const std::string & fileName); // loads data via function call
    
    T get(int ind, int axis) const; // accessor for a single element
    vector<T> get(int ind) const; // accessor for a row
    deque<T> get (const vector<int> &ind, int axis) const; //accessor a column
    
    void printTable() const; // print the table to the console
    int size() const; // returns number of row of the Table
    int dim() const; // returns the number of column of the table
    
    
private:
    deque<vector<T>> csvTable;
    int numCol;
    int numRow;
};

// default constructor
template<typename T>
CSVTable<T>::CSVTable(){
    numCol = 0;
    numRow = 0;
}

// default destructor
template<typename T>
CSVTable<T>::~CSVTable(){
    
}

// constructor: loads CSV
template<typename T>
CSVTable<T>::CSVTable(const std::string & fileName){
    
    std::ifstream fin(fileName.c_str());
    vector<T> values;
    std::string item;
    
    if (fin.fail())
        throw fileName;
    
    for (std::string line; getline(fin, line); )
    {
        std::istringstream in(line);
        
        while (getline(in, item, ','))
        {
            values.push_back(atof(item.c_str()));
        }
        csvTable.push_back(values);
        values.clear();
        
    }
    numRow = static_cast<int>(csvTable.size());
    numCol = static_cast<int>(csvTable[0].size());
    
}

// function that loads CSV
template<typename T>
void CSVTable<T>::loadCSV(const std::string & fileName){
    
    std::ifstream fin(fileName.c_str());
    vector<T> values;
    std::string item;
    
    if (fin.fail())
        throw fileName;
    
    for (std::string line; getline(fin, line); )
    {
        std::istringstream in(line);
        
        while (getline(in, item, ','))
        {
            values.push_back(atof(item.c_str()));
        }
        csvTable.push_back(values);
        values.clear();
        
    }
    numRow = static_cast<int>(csvTable.size());
    numCol = static_cast<int>(csvTable[0].size());
    
}


// accessor for single element of a CSVTable
template<typename T>
T CSVTable<T>::get(int ind, int axis) const{
    return csvTable[ind][axis];
}

// accessor for a row of a CSVTable
template<typename T>
vector<T> CSVTable<T>::get(int ind) const{
    return csvTable[ind];
}

//accessor for a column of a CSVTable
template<typename T>
deque<T> CSVTable<T>::get(const vector<int> &ind, int axis) const{
    deque<T> col(ind.size());
    for (int i=0; i<ind.size(); i++){
        col[i]=(csvTable[ind[i]][axis]);
    }
    return col;
}

template<typename T>
void CSVTable<T>::printTable() const{
    for(int i=0; i < numRow; i++){
        std::cout<<"data"<<i<<": (";
        for(int j=0; j<numCol; j++){
            std::cout << csvTable.at(i).at(j);
            if(j<numCol-1) std::cout <<",";
        }
        std::cout<<")"<<std::endl;
    }
}

// returns the number of columns (number of features)
template<typename T>
int CSVTable<T>::dim() const{
    return numCol;
}

// returns the number of rows (number of samples)
template<typename T>
int CSVTable<T>::size() const{
    return numRow;
}

#endif /* CSVTable_hpp */