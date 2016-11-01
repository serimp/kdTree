//
//  debug.hpp
//
//  Functions for debugging purposes.
//
//
//  Copyright © 2016 Serim Park . All rights reserved.
//


#ifndef debug
#define debug

#define DEBUG_MSG( os, var )\
{}
//(os) << var << std::endl

//comment out the following to print out debug messages
/*
#define DEBUG_MSG( os, var )\
(os) << var << std::endl
*/

#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
using std::string;
using std::to_string;
using std::vector;

template <typename T>
std::string to_string_with_precision(T a_value, const int n = 2)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

template<typename T>
string returnStringVector(vector<T> a, int precision=2){
    string tmp = "(";
    for (int i=0; i< a.size(); i++){
        tmp = tmp + to_string_with_precision(a[i], precision);
        if (i<a.size()-1) tmp = tmp + ",";
    }
    tmp = tmp+")";
    return tmp;
}

template<typename T>
void printVector(vector<T> a){
    std::cout<<"(";
    for (int i=0; i< a.size(); i++){
        std::cout<<to_string_with_precision(a[i],2);
        if (i<a.size()-1) std::cout<<",";
    }
    std::cout<<")";
}

template<typename T>
void printDebugVector(vector<T> a){
    string tmp = "(";
    for (int i=0; i< a.size(); i++){
        tmp = tmp + to_string(a[i]);
        if (i<a.size()-1) tmp = tmp + ",";
    }
    tmp = tmp+")";
    DEBUG_MSG(std::cout, tmp);
}

#endif