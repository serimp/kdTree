//
//  statHelper.hpp
//
//  Implementation for functions that return statistics of a vector,
//  such as median, mean, variance, skew, and kurtosis.
//
//
//  Copyright © 2016 Serim Park . All rights reserved.
//

#ifndef statHelper_h
#define statHelper_h

#include <cmath>
#include <deque>
#include <vector>
#include <iostream>
#include <string>
#include "debug.hpp"

using std::string;
using std::to_string;
using std::vector;
using std::deque;

namespace statHelper{
    
// Finds median.
template<typename T, class CSVTable>
T findMedian(const CSVTable* trainData, int axis, const vector <int> & ind){
    
    deque<T> col = trainData->get(ind, axis);
    int n;
    if (col.size()%2 == 0)
        n = static_cast<int>(col.size()/2);
    else
        n = static_cast<int>((col.size()-1)/2);
    std::nth_element(col.begin(), col.begin() + n, col.end());
    return col[n];
    
}

// Finds the indices of the vector which is the median.
template<typename T, class CSVTable>
int findMedianPos(const CSVTable* trainData, int axis, const vector <int> & ind, T median){
    
    deque<T> col = trainData->get(ind, axis);
    auto iterator = std::find(col.begin(), col.end(), median);
    int medianInd = static_cast<int>(iterator - col.begin());
    return medianInd;
}


// Finds mean.
template<typename T, class CSVTable>
T findMean(const CSVTable* trainData, int axis, const vector <int> & ind){
    T mean = 0;
    for(int i=0; i<ind.size(); i++){
        mean += trainData->get(i,axis);
    }
    return mean / ind.size();
    
}

// Finds standard deviation.
template<typename T, class CSVTable>
T findStd(const CSVTable* trainData, int axis, const vector <int> & ind, T mean){
    T std = 0;
    for(int i=0; i<ind.size(); i++){
        std += pow(trainData->get(i,axis) - mean,2);
    }
    return sqrt(std/ind.size());
}

// Sample Fisher-Pearson coefficient of skewness. Should be approx 0 when normally distributed.
template<typename T, class CSVTable>
T findSkew(const CSVTable* trainData, int axis, const vector <int> & ind, T mean, T std){
    
    T skew = 0;
    int N = static_cast<int>(ind.size());
    for(int i=0; i<N; i++){
        skew += pow(trainData->get(i,axis) - mean,3);
    }
    
    return skew/pow(std,3);
}

// Sample Kurtosis. Small Kurtosis = light tail and heavy peak.
template<typename T, class CSVTable>
T findKurtosis(const CSVTable* trainData, int axis, const vector <int> & ind, T mean, T std){
    
    T kurt = 0;
    int N = static_cast<int>(ind.size());
    for(int i=0; i<N; i++){
        kurt += pow(trainData->get(i,axis) - mean,4);
    }
    
    return kurt/pow(std,4);
}

}

#endif /* statHelper_h */
