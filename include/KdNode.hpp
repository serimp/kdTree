//
//  KdNode.hpp
//
//  KdNode class for the nodes in KdTree.
//
//  Given a set of points, KdNode splits the points into left and right child nodes.
//  The median point is stored as the current node's point.
//  Rather than storing the point, the indices for that point (in the corresponding CSVTable) is kept.
//
//  Thus, a KdNode can be fully represented by two variables:
//          - the node's splitting axis: splitAxis.
//          - the corresponding indices in CSVTable: medianInd.
//
//
//  KdNode splits the points into left and right child nodes via:
//          - findIndicesLeftRight(...);
//  KdNodes finds the splitting axis via findSplitAxis(...) according to 3 criteria:
//          - maximizing variance
//          - mininmizing skew
//          - minimizing kurtosis
//
//  Copyright © 2016 Serim Park . All rights reserved.
//

#ifndef KdNode_h
#define KdNode_h


#include <memory>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <string>

#include "statHelper.hpp"
#include "KdTree.hpp"
using std::deque;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;


template <typename T, class CSVTable>
class KdNode{
    template <typename T2, class CSVTable2> friend class KdTree;
    
public:
    
    KdNode(); // default constructor
    KdNode(const CSVTable* trainData, int rule=0); // constructor for the root node
    KdNode(const CSVTable* trainData, const vector<int> & ind, int dp, int rule=0); // constructor for the other nodes
    ~KdNode(); // default destructor
    
    int getMedianInd(); // accessor
    int getSplitAxis(); // accessor
    void setMedianInd(int ind); // mutator
    void setSplitAxis(int ax); // mutator
    
    int depth;  // at which depth the node belongs to. Kept for debugging purpose.
    
private:
    
    int splitAxis; // the splitting axis
    int medianInd; // the indices in CSVTable that represents this node.
    std::shared_ptr <KdNode> left;
    std::shared_ptr <KdNode> right;
    
    std::shared_ptr<vector<vector<int>>> findIndicesLeftRight(const CSVTable* trainData, int axis, const vector<int> & ind, T median, int medianInd);
    int findSplitAxis(const CSVTable* trainData, const vector<int> & ind, int rule);
    
};

// Constructor for the root node of the tree.
// Entire trainData is given, which is accessed via its indices.
// The splitting axis is found which yields the maximum variance along that axis.
// The splitting value is found, which is the median value of the data along that splitting axis.
// The data that yields the median will be the representative of this node.
// The remaining trainData is splitted into left and right child node.
template<typename T, class CSVTable>
KdNode<T, CSVTable>::KdNode(const CSVTable* trainData, int rule){
    
    depth = 1; // root node has depth 1
    
    // indices to access trainData. At the root, all data points are accessed.
    vector <int> ind (trainData->size());
    for(int i=0; i<trainData->size(); i++){
        ind[i] = i;
    }
    
    DEBUG_MSG(cout, "Depth " + to_string(depth) + " Indices:" + returnStringVector(ind,0));
    
    if (ind.size()>1){
        
        splitAxis = findSplitAxis(trainData, ind, rule); //find the splitting axis.
        T median = statHelper::findMedian<T, CSVTable>(trainData, splitAxis, ind); //find the value to split against.
        medianInd = ind[statHelper::findMedianPos<T, CSVTable>(trainData, splitAxis, ind, median)]; // data indice that yields the median value
        
        // The remaining trainData is splitted into left and right child node.
        std::shared_ptr<vector<vector<int>>> childInds = std::move(findIndicesLeftRight(trainData, splitAxis, ind, median, medianInd));
        
        DEBUG_MSG(cout,"Left Child Indices:" + returnStringVector((*childInds)[0],0));
        DEBUG_MSG(cout,"Right Child Indices:"+ returnStringVector((*childInds)[1],0));
        DEBUG_MSG(cout,"Node Indice:" + to_string(medianInd));
        DEBUG_MSG(cout, "---------------------");
        
        if ((*childInds)[0].size() != 0)
            left = std::shared_ptr<KdNode>(new KdNode<T, CSVTable>(trainData, (*childInds)[0], depth+1));
        else
            left = nullptr;
        
        if ((*childInds)[1].size() != 0)
            right = std::shared_ptr<KdNode>(new KdNode<T, CSVTable>(trainData, (*childInds)[1], depth+1));
        else
            right = nullptr;
    }
    
}

// Constructor for other nodes of the tree.
// Partial trainData[ind] is given, which is accessed via its indices.
// The splitting axis is found which yields the maximum variance along that axis.
// The splitting value is found, which is the median value of the data along that splitting axis.
// The data that yields the median will be the representative of this node.
// The remaining trainData is splitted into left and right child node.

template<typename T, class CSVTable>
KdNode<T, CSVTable>::KdNode(const CSVTable* trainData, const vector <int> & ind, int dp, int rule){
    
    depth = dp;
    if(ind.size()>1){
        splitAxis = findSplitAxis(trainData, ind, rule); //find the splitting axis.
        T median = statHelper::findMedian<T, CSVTable>(trainData, splitAxis, ind); //find the value to split against
        medianInd = ind[statHelper::findMedianPos<T, CSVTable>(trainData, splitAxis, ind, median)];// data indice that yields the median value
        
        DEBUG_MSG(cout, "Depth " + to_string(depth) + " Indices:" + returnStringVector(ind,0));
        std::shared_ptr<vector<vector<int>>> childInds = std::move(findIndicesLeftRight(trainData, splitAxis, ind, median, medianInd));
        DEBUG_MSG(cout,"Left Child Indices:" + returnStringVector((*childInds)[0],0));
        DEBUG_MSG(cout,"Right Child Indices:"+ returnStringVector((*childInds)[1],0));
        DEBUG_MSG(cout,"Node Indice:" + to_string(medianInd));
        DEBUG_MSG(cout, "---------------------");
        
        if ((*childInds)[0].size() != 0)
            left = std::shared_ptr<KdNode>(new KdNode<T, CSVTable>(trainData, (*childInds)[0], depth+1));
        if ((*childInds)[1].size() != 0)
            right = std::shared_ptr<KdNode>(new KdNode<T, CSVTable>(trainData, (*childInds)[1], depth+1));
    }
    
    else{ // the leaf
        left = nullptr;
        right = nullptr;
        splitAxis = -1;
        medianInd = ind[0];
    }
}

// findIndicesLeftRight(...) splits the given sets of data to the right and left nodes
// by comparing it to the medain value of the node.
//
// The node is represented by trainData[medianInd] (a single point).
// The trainData[ind] is splitted into left and right child node by comparing to the median value: trainData[medianInd].
//      e.g. if (trainData[ind] < trainData[medianInd])
//              trainData[ind] belongs to the left node.
//           if (trainData[ind] > trainData[medianInd])
//              trainData[ind] belongs to the right node.
//
// leftChild: indices of trainData that belongs to the left child node
// rightChild: indices of trainData that belongs to the right child node
// childInds: [leftChild; rightChild].
template<typename T, class CSVTable>
std::shared_ptr<vector<vector<int>>> KdNode<T, CSVTable>::findIndicesLeftRight(const CSVTable* trainData, int axis, const vector<int> & ind, T median, int medianInd){
    
    std::shared_ptr<vector<vector <int>>> childInds (new vector<vector<int>>);
    vector <int> leftChild;
    vector <int> rightChild;
    deque<T> col = trainData->get(ind, axis);
    for(int i=0; i<col.size() ; i++){
        if (col[i] <= median && ind[i]!=medianInd)
            leftChild.push_back(ind[i]);
        else if (col[i]>median)
            rightChild.push_back(ind[i]);
    }
    childInds->push_back(leftChild);
    childInds->push_back(rightChild);
    return childInds;
}


// findSplitAxis(...) finds the splitting axis
// which maximize the variance of the data points along that axis.
template<typename T, class CSVTable>
int KdNode<T, CSVTable>::findSplitAxis(const CSVTable* trainData, const vector<int> & ind, int rule){
    
    vector<T> splitCriteria(trainData->dim() );
    T maxVal = 0;
    int splitAxis = 0;
    for(int axis=0; axis<trainData->dim() ; axis++){
        
        T mean = statHelper::findMean<T, CSVTable>(trainData, axis, ind);
        T stdv = statHelper::findStd<T, CSVTable>(trainData, axis, ind, mean);
        T kurt = statHelper::findKurtosis<T, CSVTable>(trainData, axis, ind, mean, stdv);
        T skew = statHelper::findSkew<T, CSVTable>(trainData, axis, ind, mean, stdv);
        
        if (rule == 0) // use only std
            splitCriteria[axis] = (stdv);
        else if ( rule ==1) // use skew
            splitCriteria[axis] = - std::abs(skew);
        else if (rule ==2)
            splitCriteria[axis] = - std::abs(kurt);
        else
            splitCriteria[axis] = (stdv);
            
        if (axis==0)
            maxVal = splitCriteria[axis];
        else if (splitCriteria[axis] > maxVal) {
            maxVal = splitCriteria[axis];
            splitAxis = axis;
        }
    }
    return splitAxis;
}


//default constructor
template<typename T, class CSVTable>
KdNode<T, CSVTable>::KdNode(){
    left = nullptr;
    right = nullptr;
    
}

//default destructor
template<typename T, class CSVTable>
KdNode<T, CSVTable>::~KdNode(){
}


// mutator
template<typename T, class CSVTable>
void KdNode<T, CSVTable>::setMedianInd(int ind){
    medianInd = ind;
    
}

// mutator
template<typename T, class CSVTable>
void KdNode<T, CSVTable>::setSplitAxis(int ax){
    splitAxis = ax;
    
}

// accessor
template<typename T, class CSVTable>
int KdNode<T, CSVTable>::getMedianInd(){
    return medianInd;
    
}

// accessor
template<typename T, class CSVTable>
int KdNode<T, CSVTable>::getSplitAxis(){
    return splitAxis;
    
}
#endif /* KdNode_h */
