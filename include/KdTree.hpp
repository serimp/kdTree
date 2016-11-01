//
//  KdTree.hpp
//
//  KdTree class for Knn Search Algorithm.
//
//  KdTree is built by calling the constructor with the trainData as the input e.g. KdTree(trainData).
//      - The default bound for the distance between the query point and the splitting hyperplane is set to 0.1.
//      - or the bound can be specified explicitely via e.g KdTree(trainData, bound);
//
//  When a query is given, the nearest point to the query point can be found using traverseTree(...);.
//      - At the root, the left and right child node is traversed.
//      - If the distance to the splitting hyperplane from the query point is smaller than the bound, the both child nodes are traversed.
//      - If not, only one child node is traversed (by comparing to the node value).
//
//
//  KdTree can be writed to CSV file and loaded from CSV file.
//      - The tree is saved and loaded via pre-order traversing.
//      - For each node, four values are saved:
//          - splitting Axis
//          - the indice of the datapoint in CSVTable (instead of storing the datapoint itself)
//          - whether the node has a left child node
//          - whether the node has a right child node
//
//
//  Copyright © 2016 Serim Park . All rights reserved.
//


#ifndef KdTree_h
#define KdTree_h

#include "KdNode.hpp"
#include "statHelper.hpp"
#include "debug.hpp"
#include <unistd.h>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
using std::string;
using std::to_string;
using std::cout;


template <typename T, class CSVTable>
class KdTree{
public:
    
    KdTree();
    KdTree(const CSVTable* trainData);
    KdTree(const CSVTable* trainData, T up, int rule=0);
    ~KdTree();
    
    // traverse the Tree until the nearest point is found.
    void traverseTree(std::shared_ptr<KdNode<T, CSVTable>> p, const vector<T>& testPoint, const CSVTable* trainData, vector<T> &ind_dist) const;
    void printTree(std::shared_ptr<KdNode<T, CSVTable>> p, const CSVTable* trainData, int indent) const; // print
    void write2CSV(std::shared_ptr<KdNode<T, CSVTable>> p, std::ofstream &fout); // write
    void loadCSV(std::shared_ptr<KdNode<T, CSVTable>> p, std::ifstream &fin); // read
    
    std::shared_ptr<KdNode<T, CSVTable>> getRoot() const; // accessor
    T getBound() const; // accessor
    void setBound(T up); // mutator
    
private:
    T findDistance(const vector<T>& testPoint, const vector<T>& nodePoint) const;
    T findDistanceToHyperplane(const vector<T>& testPoint, const vector<T>& nodePoint, int ax) const;
    
    std::shared_ptr<KdNode<T, CSVTable>> root;
    T bound = 0.1; // bound for the distance to the hyperplane. Default to 0.1.
    
};

// default constructor.
template<typename T, class CSVTable>
KdTree<T, CSVTable>::KdTree(){
    root = std::shared_ptr<KdNode<T, CSVTable>>(new KdNode<T, CSVTable>());
}

// destructor
template<typename T, class CSVTable>
KdTree<T, CSVTable>::~KdTree(){
    
}

// constructor with trainData input. Calls the Kdnode constructor for the root.
template<typename T, class CSVTable>
KdTree<T, CSVTable>::KdTree(const CSVTable* trainData): root(new KdNode<T, CSVTable>(trainData)){}


// constructor with trainData and Bound input. Calls the Kdnode constructor for the root.
template<typename T, class CSVTable>
KdTree<T, CSVTable>::KdTree(const CSVTable* trainData, T up, int rule): root(new KdNode<T, CSVTable>(trainData, rule)), bound(up){}

//template<typename T, class CSVTable>
//KdTree<T, CSVTable>::KdTree(const CSVTable* trainData, T up): root(new KdNode<T, CSVTable>(trainData)), bound(up){}


// Save the Tree to csv file
// The tree is saved via pre-order traversing.
// For each node, four values are stored:
//      - splitting Axis
//      - the indice of the datapoint in CSVTable (instead of storing the datapoint itself)
//      - whether the node has a left child node
//      - whether the node has a right child node
template <typename T, class CSVTable>
void KdTree<T, CSVTable>::write2CSV(std::shared_ptr<KdNode<T, CSVTable>> p, std::ofstream &fout){
    
    if(p!=nullptr){
        int medInd = p->getMedianInd();
        int splitAxis = p->getSplitAxis();
        fout<<medInd<<","<<splitAxis<<","<<bool(p->left)<<","<<bool(p->right)<<endl;
        fout.flush();
        if(p->left) write2CSV(p->left, fout);
        if(p->right) write2CSV(p->right, fout);
    }
}

// Load the Tree from csv file
// The tree is loaded via pre-order traversing.
// For each node, four values are read:
//      - splitting Axis
//      - the indice of the datapoint in CSVTable (instead of storing the datapoint itself)
//      - whether the node has a left child node
//      - whether the node has a right child node
// If the node has either left or right child node, than tree is traversed to that node.
template <typename T, class CSVTable>
void KdTree<T, CSVTable>::loadCSV(std::shared_ptr<KdNode<T, CSVTable>> p, std::ifstream &fin){
    
    if(p!=nullptr){
        
        int ind, ax;
        bool hasRight, hasLeft;
        std::string line;
        
        if (getline(fin, line)){
            
            std::istringstream in(line);
            std::string item1, item2, item3, item4;
            getline(in, item1, ',');
            getline(in, item2, ',');
            getline(in, item3, ',');
            getline(in, item4, ',');
            
            ind = atof(item1.c_str());
            ax = atof(item2.c_str());
            hasLeft = atof(item3.c_str());
            hasRight = atof(item4.c_str());
            
            p->setMedianInd(ind);
            p->setSplitAxis(ax);
            
            fin.clear();
            usleep(500);
            if (hasLeft && hasRight){
                p->left = std::shared_ptr<KdNode<T, CSVTable>>(new KdNode<T, CSVTable>());
                loadCSV(p->left, fin);
                p->right = std::shared_ptr<KdNode<T, CSVTable>>(new KdNode<T, CSVTable>());
                loadCSV(p->right, fin);
            }
            else if (hasLeft){
                p->left = std::shared_ptr<KdNode<T, CSVTable>>(new KdNode<T, CSVTable>());
                loadCSV(p->left, fin);
                p->right = nullptr;
            }
            else if (hasRight){
                p->right = std::shared_ptr<KdNode<T, CSVTable>>(new KdNode<T, CSVTable>());
                loadCSV(p->right, fin);
                p->left = nullptr;
            }
            else {
                p->left = nullptr;
                p->right = nullptr;
            }
        }
    }
}

//
// traverseTree finds the nearest point to the query (testPoint) and the distance between the two.
// Note that the nearest point is represented using its indice in CSVtable, than its values.
//
// At the root node, the left and right child node is traversed.
// At the other nodes:
//  - If the distance to the splitting hyperplane from the query point is smaller than the bound, the both child nodes are traversed.
//  - If not, only one child node is traversed.
//
// At each node, the new distance is computed, and if it is smaller than the previously computed distance, it is updated.
//
template<typename T, class CSVTable>
void KdTree<T, CSVTable>::traverseTree(std::shared_ptr<KdNode<T, CSVTable>> p, const vector<T> & testPoint, const CSVTable * trainData, vector<T> & ind_dist) const{
    
    int ax = p->getSplitAxis();
    int ind = p->getMedianInd();
    const vector<T> nodePoint = trainData->get(ind);
    
    T dist_new = findDistance(testPoint, nodePoint); // distance to the node point
    T dist_hyperplane = findDistanceToHyperplane(testPoint, nodePoint, ax);
    
    DEBUG_MSG(cout, "Traversing down:" + to_string_with_precision(ind, 3)+ ": "+returnStringVector(nodePoint));
    bool isRoot = ind_dist.empty();
    
    if (isRoot){ // at root go to both direction & update the ind_dist at the root
        ind_dist.push_back(ind);
        ind_dist.push_back(dist_new);
        
        if(p->left)
            traverseTree(p->left, testPoint, trainData, ind_dist);
        if(p->right)
            traverseTree(p->right, testPoint, trainData, ind_dist);
    }
    else{
        if (dist_new < ind_dist[1]){
            ind_dist[0] = ind;
            ind_dist[1] = dist_new;
            DEBUG_MSG(cout, " Distanced updated:" + to_string_with_precision(ind_dist[1],2)+" to " + to_string_with_precision(dist_new,2));
        }
        if(dist_hyperplane < bound){ // if the distance to the hyperplane is too small
            if(p->left)
                traverseTree(p->left, testPoint, trainData, ind_dist);
            if(p->right)
                traverseTree(p->right, testPoint, trainData, ind_dist);
        }
        
        else if (nodePoint[ax] < testPoint[ax]){
            if(p->right)
                traverseTree(p->right, testPoint, trainData, ind_dist);
        }
        else if (nodePoint[ax] > testPoint[ax]){
            if(p->left)
                traverseTree(p->left, testPoint, trainData, ind_dist);
        }
        else
            ; // do nothing
    }
}

// Finds the distance between the query point (testPoint) and the splitting hyperplane.
template<typename T, class CSVTable>
T KdTree<T, CSVTable>::findDistanceToHyperplane(const vector<T>& testPoint, const vector<T>& nodePoint, int ax) const{
    return std::abs(testPoint[ax] - nodePoint[ax]);
}

// Finds the distance between the query point (testPoint) and the node point. 
template<typename T, class CSVTable>
T KdTree<T, CSVTable>::findDistance(const vector<T>& testPoint, const vector<T>& nodePoint) const{
    T dist = 0;
    for (int i=0; i<testPoint.size(); i++){
        dist += pow(testPoint[i] - nodePoint[i],2);
    }
    dist = sqrt(dist);
    return dist;
}


// print the tree to the console
template<typename T, class CSVTable>
void KdTree<T, CSVTable>::printTree(std::shared_ptr<KdNode<T, CSVTable>> p, const CSVTable* trainData, int indent)const {
    
    if(p!=nullptr){
        if(indent){
            std::cout<<std::setw(indent)<<' ';
        }
        int medInd = p->getMedianInd();
        auto element = trainData->get(medInd);
        
        std::cout<<medInd<<":";
        printVector<T>(element);
        std::cout<<" ax:"<<p->getSplitAxis();
        std::cout<<std::endl;
        
        if(p->left) printTree(p->left, trainData, indent+4);
        if(p->right) printTree(p->right, trainData, indent+4);
        
    }
}

// mutator
template <typename T, class CSVTable>
void KdTree<T, CSVTable>::setBound(T up){
    return bound = up;
}

// accessor
template <typename T, class CSVTable>
T KdTree<T, CSVTable>::getBound() const{
    return bound;
}

// accessor
template <typename T, class CSVTable>
std::shared_ptr<KdNode<T, CSVTable>> KdTree<T, CSVTable>::getRoot() const{
    return root;
}



#endif /* KdTree_h */
